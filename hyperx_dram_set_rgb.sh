#!/bin/bash
set -e

# Device options
# ======
I2C_DEV=9 # I2C device (check i2cdetect -l)
RAM_ZONES_BRIGHT=(2 3) # Zones with bright blue channel
RAM_ZONES_DIM=(1 0)    # Zones with dim blue channel

# Color correction options
# ======
COLOR_CORR_MAX=45    # Color to set for R,G channels at B=0xFF - white balance/brightness tradeoff. Should be tuned on #FFFFFF color
BLUE_CORR_MAX=45   # Max blue channel brightness. Should be tuned on #0000FF color

ADDR=0x27 # Should be the same for all HyperX ram models

get_rgb_index() {
	# Get hex address of each led from zone 0..3, led 0..4 and channel 0..2
	IND="0x$(printf '%X' $((0x11+$1*0x30+$2*3+$3)))"
	# echo "zone $1 led $2 ch $3 : $IND"
}

get_bri_index() {
	# Get hex address of brightness for led from zone 0..3 and led 0..4
	IND="0x$(printf '%X' $((0x21+0x30*$1+$2*3)))"
	# echo "zone $1 led $2 bri  : $IND"
}

color_corr() {
	# Color correction for R and G channels, arguments: color, blue_corr
	CORR_INT=$(( (($BLUE_CORR_MAX - $2 + $COLOR_CORR_MAX) * $1) / 0xFF ))
	# (b_min*(c_max-c)+b_max(c-c_min))/(c_max-c_min)
	if [[ $CORR_INT -lt 0 ]]; then COL="0x00"
	else
		if [[ $CORR_INT -gt 255 ]]; then COL="0xFF"
		else COL=0x"$(printf '%X' $CORR_INT)"
		fi
	fi
}

if [[ $# -lt 1 ]]; then
	echo "usage : $0 RRGGBB Bri"
	echo "Brightness (optional) : 0x00 - 0x64"
	exit
fi

# 0xABCDFEF -> 0xAB 0xCD 0xEF
HEX_IN="0x$1"
R_IN="0x$(printf '%X' $(($HEX_IN/0x10000)))"
G_IN="0x$(printf '%X' $((($HEX_IN/0x100) % 0x100)))"
B_IN="0x$(printf '%X' $((($HEX_IN) % 0x100)))"

if [[ $# -eq 1 ]]; then
	BRI_IN="0xFF"
else
	BRI_IN=$2
fi

# Mode to static with: {mode (with color and brightness): &0xe4, static: 0x09}
i2cset -y $I2C_DEV $ADDR 0xe1 0x01 # Start of message

# i2cset -y $I2C_DEV $ADDR 0xe4 0x09 # Mode Control 2 (speed, brightness, color )
# Selected mode : Static
i2cset -y $I2C_DEV $ADDR 0xe5 0x21 # Mode Control 3, selected mode : Direct

i2cset -y $I2C_DEV $ADDR 0xe1 0x02 # End of message
i2cset -y $I2C_DEV $ADDR 0xe1 0x03 # Apply


# Color: {red (&0xEC): 0xFF (255), green (&0xED): 0xFF (255), blue (&0xEE): 0xFF (255), brightness (&0xDD): 0x64 (100)}
i2cset -y $I2C_DEV $ADDR 0xe1 0x01 # Start

BLUE_CORR="0x$(printf '%X' $(($B_IN*$BLUE_CORR_MAX/0xFF)))"
# Blue corrected for the bright channel

for zone in "${RAM_ZONES_BRIGHT[@]}"; do
	for led in {0..4}; do
		get_rgb_index $zone $led 0
		color_corr $R_IN $BLUE_CORR
		CORR_R=$COL
		i2cset -y $I2C_DEV $ADDR $IND $COL # R

		get_rgb_index $zone $led 1
		color_corr $G_IN $BLUE_CORR
		CORR_G=$COL
		i2cset -y $I2C_DEV $ADDR $IND $COL # G

		get_rgb_index $zone $led 2
		i2cset -y $I2C_DEV $ADDR $IND $BLUE_CORR # B
		
		get_bri_index $zone $led
		i2cset -y $I2C_DEV $ADDR $IND $BRI_IN # Bri
	done
done

for zone in "${RAM_ZONES_DIM[@]}"; do
	for led in {0..4}; do
		get_rgb_index $zone $led 0
		color_corr $R_IN $BLUE_CORR
		i2cset -y $I2C_DEV $ADDR $IND $COL # R
		
		get_rgb_index $zone $led 1
		color_corr $G_IN $BLUE_CORR
		i2cset -y $I2C_DEV $ADDR $IND $COL # G

		get_rgb_index $zone $led 2
		i2cset -y $I2C_DEV $ADDR $IND $B_IN # B

		get_bri_index $zone $led
		i2cset -y $I2C_DEV $ADDR $IND $BRI_IN # Bri
	done
done

# i2cset -y $I2C_DEV $ADDR 0xEC $1 # R
# i2cset -y $I2C_DEV $ADDR 0xED $2 # G
# i2cset -y $I2C_DEV $ADDR 0xEE $3 # B
# i2cset -y $I2C_DEV $ADDR 0xDD $4 # Bri

i2cset -y $I2C_DEV $ADDR 0xe1 0x02 # End
i2cset -y $I2C_DEV $ADDR 0xe1 0x03 # Apply

echo "color correction bri : $1 -> $CORR_R $CORR_G $BLUE_CORR"
echo "color correction dimm: $1 -> $CORR_R $CORR_G $B_IN"
