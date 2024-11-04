#include "settingswidget.h"

SettingsWidget::SettingsWidget(ResourceManager* mgr, QWidget *parent)
    : QWidget(parent), res_mgr(mgr), inj(res_mgr, this)
{
    res_mgr->RegisterDeviceListChangeCallback(&SettingsWidget::onDevicesUpdated, this);
    res_mgr->RegisterDetectionProgressCallback(&SettingsWidget::onDevicesUpdated, this);
}

void SettingsWidget::initUI()
{
    auto* lyt = new QFormLayout();

    zone_lyt = new QHBoxLayout();
    auto* label = new QLabel("Dim slots");
    lyt->addRow(label, zone_lyt);

    auto* blue = new QSlider();
    connect(blue, &QSlider::sliderReleased, this, &SettingsWidget::settingsUpdated);
    blue->setMinimum(1);
    blue->setMaximum(255);
    auto* blue_label = new QLabel("Blue channel dimming");
    lyt->addRow(blue_label, blue);

    auto* chan = new QSlider();
    connect(chan, &QSlider::sliderReleased, this, &SettingsWidget::settingsUpdated);
    chan->setMinimum(1);
    chan->setMaximum(255);
    auto* chan_label = new QLabel("R,G channels dimming");
    lyt->addRow(chan_label, chan);

    setLayout(lyt);
}

void SettingsWidget::fetchDevices()
{
    auto& controllers = res_mgr->GetRGBControllers();
    for (size_t i = 0; i < controllers.size(); i++)
    {
        if (controllers[i].name == "HyperX DRAM") {
            // Found controller
            inj.setRGBController(i);

            // Destroy all old checkboxes
            for (auto& box : zone_boxes) {
                box->deleteLater();
                delete box;
            }
            zone_boxes.clear();

            for (size_t j = 0; j < controllers[i].zones.size(); j++)
            {
                auto* box = new QCheckBox(tr("Zone ") + QString::number(j));
                connect(box, &QCheckBox::clicked, this, &SettingsWidget::settingsUpdated);
                zone_lyt->addWidget(box);
            }
        }
    }
}

void SettingsWidget::onDevicesUpdated(void* wid)
{
    auto widget = reinterpret_cast<SettingsWidget*>(wid);
    if (widget->res_mgr->GetDetectionPercent() == 100)
    {
        // List updated, get new settings
    } else {
        // Disable elements
    }
}
