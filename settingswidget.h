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


class SettingsCallback : public QObject
{
    Q_OBJECT
public:
    static void callback(void* wid)
    {
        auto* widget = reinterpret_cast<SettingsCallback*>(wid);
        emit widget->devicesUpdated();
    };

signals:
    void devicesUpdated();
};


class SettingsWidget : public QWidget
{
    Q_OBJECT
public:
    SettingsWidget(ResourceManager* mgr, QWidget *parent = nullptr);

    ~SettingsWidget();

    void initUI();

    void fetchDevices();

public slots:
    void onDevicesUpdated();

protected slots:
    void onWidgetUpdate();

signals:
    void settingsUpdated();

protected:
    ResourceManager* res_mgr;

    ColorInjector inj;
    SettingsCallback sc;

    QHBoxLayout* zone_lyt;
    std::vector<QCheckBox*> zone_boxes;
    QSlider* blue;
    QSlider* chan;
};

#endif // SETTINGSWIDGET_H
