# hyperx-dram-color-fix

Fix for dim blue channel bug for mixed HyperX DRAM models

## Description

On some mixed HyperX DRAM models (Predator and Fury) RGB controller doesn't work correctly, so why blue channel on 2 of the ram sticks is more dim. Provided bash script and OpenRGB module apply color correciton to minimize color distortion

**Article link:** [https://enaix.github.io/2024/11/08/hyperx-color-fix.html](https://enaix.github.io/2024/11/08/hyperx-color-fix.html)

## Bash script

Requirements: `i2c-tools` package

Disable the `ee1004` module (`modprobe -r ee1004`), since it doesn't allow to access ram i2c device

#### Configuration

Locate the i2c device either through `i2cdetect -l` or device information tab in OpenRGB and then set the `I2C_DEV` variable in the script

Set the dim and bright zones in `RAM_ZONES_BRIGHT` and `RAM_ZONES_DIM` - you may try swapping the variables if variables do not match

`BLUE_CORR_MAX` - blue channel color correction (1-255), lower - dimmer. Tune on 0000ff color

`COLOR_CORR_MAX` - R, G channels color correction (1-255), lower - dimmer, colder; higher - brighter, warmer. Tune on ffffff color

#### Running

Run `./hyperx_dram_set_rgb.sh rrggbb`, where rrggbb is hex color without the `#` symbol - it acts as a bash comment

## OpenRGB module

It's still in WIP stage
