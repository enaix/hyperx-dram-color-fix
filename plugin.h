#ifndef OPENRGBPLUGIN_H
#define OPENRGBPLUGIN_H

#include <QObject>

#include "OpenRGBPluginInterface.h"

#include "settingswidget.h"

class OpenRGBPlugin : public QObject, OpenRGBPluginInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID OpenRGBPluginInterface_IID)
    Q_INTERFACES(OpenRGBPluginInterface)

public:
    virtual ~OpenRGBPlugin() {};

    virtual OpenRGBPluginInfo GetPluginInfo() override;
    virtual unsigned int GetPluginAPIVersion() override;

    virtual void Load(bool dark_theme, ResourceManager* resource_manager_ptr) override;
    virtual QWidget* GetWidget() override;
    virtual QMenu* GetTrayMenu() override;
    virtual void Unload() override;

protected:
    ResourceManager* resource_manager;
};
#endif // OPENRGBPLUGIN_H
