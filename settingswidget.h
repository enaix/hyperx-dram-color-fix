#ifndef SETTINGSWIDGET_H
#define SETTINGSWIDGET_H

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QPushButton>
#include <QCheckBox>
#include <QLabel>
#include <QSlider>

#include <vector>

#include "ResourceManager.h"

#include "colorinjector.h"

class SettingsWidget : public QWidget
{
    Q_OBJECT
public:
    explicit SettingsWidget(ResourceManager* mgr, QWidget *parent = nullptr);

    void initUI();

    void fetchDevices();

    static void onDevicesUpdated(void* wid);
signals:
    void settingsUpdated();

protected:
    ResourceManager* res_mgr;

    ColorInjector inj;

    QHBoxLayout* zone_lyt;
    std::vector<QCheckBox*> zone_boxes;
};

#endif // SETTINGSWIDGET_H
