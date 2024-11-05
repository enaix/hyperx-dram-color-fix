#ifndef COLORINJECTOR_H
#define COLORINJECTOR_H

#include <QObject>
#include <vector>

#include "RGBController/RGBController.h"
#include "ResourceManager.h"

#include "colorcorrection.h"

class ColorInjector : public QObject
{
    Q_OBJECT
public:
    ColorCorrection cc;

    ColorInjector(ResourceManager* manager, QObject* parent = nullptr) : QObject(parent), mgr(manager), ctrl_id(SIZE_T_MAX), cc(this) {}

    void RegisterCallbacks();

    void setRGBController(size_t rgb_ctrl_id);

    static void callback(void* arg);

    RGBController* get_ctrl();

protected:
    ResourceManager* mgr;
    size_t ctrl_id;
    std::vector<RGBColor> _colormap;
};

#endif // COLORINJECTOR_H