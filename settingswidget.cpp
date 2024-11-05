#include "settingswidget.h"

SettingsWidget::SettingsWidget(ResourceManager* mgr, QWidget *parent)
    : QWidget(parent), res_mgr(mgr), inj(res_mgr, this)
{
    res_mgr->RegisterDeviceListChangeCallback(&SettingsWidget::onDevicesUpdated, this);
    res_mgr->RegisterDetectionProgressCallback(&SettingsWidget::onDevicesUpdated, this);
}

SettingsWidget::~SettingsWidget()
{
    res_mgr->UnregisterDeviceListChangeCallback(&SettingsWidget::onDevicesUpdated, this);
    res_mgr->UnregisterDetectionProgressCallback(&SettingsWidget::onDevicesUpdated, this);
}

void SettingsWidget::initUI()
{
    auto* lyt = new QFormLayout(static_cast<QWidget*>(this));

    zone_lyt = new QHBoxLayout();
    auto* label = new QLabel("Dim slots");
    lyt->addRow(label, zone_lyt);

    blue = new QSlider();
    connect(blue, &QSlider::sliderReleased, this, &SettingsWidget::onWidgetUpdate);
    blue->setMinimum(1);
    blue->setMaximum(255);
    auto* blue_label = new QLabel("Blue channel dimming");
    lyt->addRow(blue_label, blue);

    chan = new QSlider();
    connect(chan, &QSlider::sliderReleased, this, &SettingsWidget::onWidgetUpdate);
    chan->setMinimum(1);
    chan->setMaximum(255);
    auto* chan_label = new QLabel("R,G channels dimming");
    lyt->addRow(chan_label, chan);

    //setLayout(lyt);
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
                connect(box, &QCheckBox::clicked, this, &SettingsWidget::onWidgetUpdate);
                zone_lyt->addWidget(box);
                zone_boxes.push_back(box);
            }
        }
    }
}

void SettingsWidget::onDevicesUpdated(void* wid)
{
    auto* widget = reinterpret_cast<SettingsWidget*>(wid);
    if (widget->res_mgr->GetDetectionPercent() == 100)
    {
        // List updated, get new settings
        widget->fetchDevices();
    } else {
        // Disable elements
        for (auto* box : widget->zone_boxes)
            box->setDisabled(true);
    }
}

void SettingsWidget::onWidgetUpdate()
{
    for (size_t i = 0; i < zone_boxes.size(); i++)
    {
        if (zone_boxes[i].isChecked())
            inj.cc.dim_zones.insert()
    }

    inj.cc.blue_chan_max = (uint8_t)blue->value();
    inj.cc.color_chan_max_dim = (uint8_t)chan->value();
    emit settingsUpdated();
}
