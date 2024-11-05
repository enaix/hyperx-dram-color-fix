#include "plugin.h"



OpenRGBPluginInfo OpenRGBPlugin::GetPluginInfo()
{
    OpenRGBPluginInfo info;
    info.Name = "HyperX DRAM Color Correction Plugin";
    info.Description = "Plugin that fixes blue channel bug on mixed HyperX DRAM";
    info.Version = "0.0.1";
    info.Commit = "";
    info.URL = "";
    info.Label = "HyperX DRAM CC";
    // TODO fill missing info
    return info;
}

unsigned int OpenRGBPlugin::GetPluginAPIVersion()
{
    return(OPENRGB_PLUGIN_API_VERSION);
}

void OpenRGBPlugin::Load(bool dark_theme, ResourceManager* resource_manager_ptr)
{
    resource_manager = resource_manager_ptr;
}

QWidget* OpenRGBPlugin::GetWidget()
{
    QWidget* wid = new SettingsWidget(resource_manager);
    return wid;
}

QMenu* OpenRGBPlugin::GetTrayMenu()
{
    return nullptr;
}

void OpenRGBPlugin::Unload()
{

}
