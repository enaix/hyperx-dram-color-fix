#include "colorcorrection.h"

void ColorCorrection::correct_color(RGBController *ctrl, size_t zone_id)
{
    for (size_t led_id = 0; led_id < ctrl->zones[zone_id].leds_count; led_id++)
    {
        RGBColor old_color = ctrl->zones[zone_id].colors[led_id];
        // Check zone map and apply the corresponding color

        uint8_t b_chan = old_color & 0xFF0000;

        // Limit blue between 0 and blue_chan_max for bright channels
        uint8_t b_chan_corr = (dim_zones.find(zone_id) == dim_zones.cend() ?
                                                   b_chan * blue_chan_max / 0xFF : b_chan);

        uint16_t rg_chan = old_color & 0xFFFF * (blue_chan_max + color_chan_max_dim - b_chan_corr);
        // TODO check if we need to clamp

        ctrl->zones[zone_id].colors[led_id] = rg_chan | b_chan_corr;
    }
}

void ColorCorrection::set_color_no_cc(RGBController *ctrl, size_t zone_id, size_t led_id, RGBColor c)
{
    // Set plain color without color correction
    ctrl->zones[zone_id].colors[led_id] = c;
}
