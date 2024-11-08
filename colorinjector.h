#ifndef COLORINJECTOR_H
#define COLORINJECTOR_H

#include <QObject>
#include <vector>

#include "RGBController/RGBController.h"
#include "Controllers/HyperXDRAMController/RGBController_HyperXDRAM.h" // Load constants
#include "ResourceManager.h"

#include "logging.h"

#include "colorcorrection.h"

class ColorInjector : public QObject
{
    Q_OBJECT
public:
    ColorCorrection cc;

    ColorInjector(ResourceManager* manager, QObject* parent = nullptr) : QObject(parent), cc(this), mgr(manager), ctrl_id(SIZE_MAX) {}

    void RegisterCallbacks();

    void setRGBController(size_t rgb_ctrl_id);

    static void callback(void* arg);

    RGBController* get_ctrl();

public slots:
    void onSettingsUpdate();

    void setTmpColor(RGBColor c);
    void unsetTmpColor();

protected:
    ResourceManager* mgr;
    size_t ctrl_id;
    std::vector<RGBColor> _colormap;

    void resetColormap();
    void apply();
};

#endif // COLORINJECTOR_H
