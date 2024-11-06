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

#include "logging.h"

#include "colorinjector.h"

class SettingsWidget : public QWidget
{
    Q_OBJECT
public:
    SettingsWidget(ResourceManager* mgr, QWidget *parent = nullptr);

    ~SettingsWidget();

    void initUI();

    void fetchDevices();

    static void onDevicesUpdated(void* wid);

protected slots:
    void onWidgetUpdate();

signals:
    void settingsUpdated();

protected:
    ResourceManager* res_mgr;

    ColorInjector inj;

    QHBoxLayout* zone_lyt;
    std::vector<QCheckBox*> zone_boxes;
    QSlider* blue;
    QSlider* chan;
};

#endif // SETTINGSWIDGET_H
