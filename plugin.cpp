#include "plugin.h"



OpenRGBPluginInfo OpenRGBPlugin::GetPluginInfo()
{
    OpenRGBPluginInfo info;
    // TODO fill info
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

}

QMenu* OpenRGBPlugin::GetTrayMenu()
{
    return nullptr;
}

void OpenRGBPlugin::Unload()
{

}
