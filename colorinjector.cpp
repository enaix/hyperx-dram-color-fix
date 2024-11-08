#include "colorinjector.h"

#include "Controllers/HyperXDRAMController/RGBController_HyperXDRAM.h"

void ColorInjector::RegisterCallbacks()
{
    get_ctrl()->RegisterUpdateCallback(&ColorInjector::callback, this);
}

void ColorInjector::setRGBController(size_t rgb_ctrl_id)
{
    ctrl_id = rgb_ctrl_id;
}

void ColorInjector::callback(void *arg)
{
    PLUGIN_DEBUG("ColorInjector::callback() : begin");
    // Callback mutex is called, it's safe to access RGBController
    auto* inj = reinterpret_cast<ColorInjector*>(arg);

    // Color or mode has been updated, we need to set new color

    auto* ctrl = inj->get_ctrl();
    // Works only in Direct mode
    if (ctrl->active_mode != HYPERX_MODE_DIRECT) return; // TODO apply old colormap

    // Store a copy of original colormap
    inj->_colormap = ctrl->colors; // TODO check if we need to apply old one

    for (size_t i = 0; i < ctrl->zones.size(); i++)
    {
        inj->cc.correct_color(ctrl, i);
    }

    // TODO call DeviceUpdateLEDs

    PLUGIN_DEBUG("ColorInjector::callback() : finished");
}

RGBController *ColorInjector::get_ctrl()
{
    // Fetch current rgb controller
    // TODO add likely/unlikely
    return mgr->GetRGBControllers()[ctrl_id];
}

void ColorInjector::onSettingsUpdate()
{
    auto* ctrl = get_ctrl();
    for (size_t i = 0; i < ctrl->zones.size(); i++)
    {
        cc.correct_color(ctrl, i);
    }
    // TODO call DeviceUpdateLEDs
}

void ColorInjector::setTmpColor(RGBColor c)
{
    auto* ctrl = get_ctrl();
    _colormap = ctrl->colors;
    for (size_t i = 0; i < _colormap.size(); i++)
    {
        ctrl->colors[i] = c;
    }

    // TODO call DeviceUpdateLEDs
}

void ColorInjector::unsetTmpColor()
{
    resetColormap();
}

void ColorInjector::resetColormap()
{
    auto* ctrl = get_ctrl();
    for (size_t i = 0; i < _colormap.size(); i++)
    {
        ctrl->colors[i] = _colormap[i];
    }

    // TODO call DeviceUpdateLEDs
}
