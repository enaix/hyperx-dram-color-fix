#include "settingswidget.h"

SettingsWidget::SettingsWidget(ResourceManager* mgr, QWidget *parent)
    : QWidget(parent), res_mgr(mgr), inj(res_mgr, this)
{
    res_mgr->RegisterDeviceListChangeCallback(&SettingsCallback::callback, &sc);
    res_mgr->RegisterDetectionProgressCallback(&SettingsCallback::callback, &sc);

    connect(this, &SettingsWidget::settingsUpdated, &inj, &ColorInjector::onSettingsUpdate);
    connect(this, &SettingsWidget::calibrate, &inj, &ColorInjector::setTmpColor);
    connect(this, &SettingsWidget::stopCalibration, &inj, &ColorInjector::unsetTmpColor);

    connect(&sc, &SettingsCallback::devicesUpdated, this, &SettingsWidget::onDevicesUpdated);

    initUI();
}

SettingsWidget::~SettingsWidget()
{
    res_mgr->UnregisterDeviceListChangeCallback(&SettingsCallback::callback, &sc);
    res_mgr->UnregisterDetectionProgressCallback(&SettingsCallback::callback, &sc);
}

void SettingsWidget::initUI()
{
    PLUGIN_DEBUG("SettingsWidget::initUI() : begin");
    auto* lyt = new QFormLayout(static_cast<QWidget*>(this));

    zone_lyt = new QHBoxLayout();
    auto* label = new QLabel("Dim slots");
    lyt->addRow(label, zone_lyt);

    // Blue channel
    auto* blue_lyt = new QHBoxLayout();
    blue = new QSlider(Qt::Orientation::Horizontal);
    connect(blue, &QSlider::sliderMoved, this, &SettingsWidget::onSliderMoved);
    connect(blue, &QSlider::sliderReleased, this, &SettingsWidget::onWidgetUpdate);
    blue->setMinimum(1);
    blue->setMaximum(255);
    auto* blue_label = new QLabel("Blue channel dimming");
    blue_lyt->addWidget(blue);

    blue_value = new QLabel("0");
    connect(blue, &QSlider::valueChanged, [=](int value){
        blue_value->setText(QString::number(value));
    });
    blue_lyt->addWidget(blue_value);
    blue_calib = new QPushButton("Calibrate");
    connect(blue_calib, &QPushButton::clicked, this, &SettingsWidget::calibrateBlue);
    blue_lyt->addWidget(blue_calib);
    lyt->addRow(blue_label, blue_lyt);
    // ---

    // R,G channels
    auto* chan_lyt = new QHBoxLayout();
    chan = new QSlider(Qt::Orientation::Horizontal);
    connect(chan, &QSlider::sliderMoved, this, &SettingsWidget::onSliderMoved);
    connect(chan, &QSlider::sliderReleased, this, &SettingsWidget::onWidgetUpdate);
    chan->setMinimum(1);
    chan->setMaximum(255);
    auto* chan_label = new QLabel("R,G channels dimming");
    chan_lyt->addWidget(chan);

    chan_value = new QLabel("0");
    connect(chan, &QSlider::valueChanged, [=](int value){
        chan_value->setText(QString::number(value));
    });
    chan_lyt->addWidget(chan_value);
    white_calib = new QPushButton("Calibrate");
    connect(white_calib, &QPushButton::clicked, this, &SettingsWidget::calibrateWhite);
    chan_lyt->addWidget(white_calib);
    lyt->addRow(chan_label, chan_lyt);
    // ---

    PLUGIN_DEBUG("SettingsWidget::initUI() : finished");
    //setLayout(lyt);
}

void SettingsWidget::fetchDevices()
{
    PLUGIN_DEBUG("SettingsWidget::fetchDevices() : parsing controllers");
    auto& controllers = res_mgr->GetRGBControllers();
    for (size_t i = 0; i < controllers.size(); i++)
    {
        if (controllers[i]->name == "HyperX DRAM") {
            // Found controller
            PLUGIN_DEBUG("SettingsWidget::fetchDevices() : found HyperX DRAM");
            inj.setRGBController(i);

            // Destroy all old checkboxes
            for (auto& box : zone_boxes) {
                box->deleteLater();
                delete box;
            }
            zone_boxes.clear();

            PLUGIN_DEBUG("SettingsWidget::fetchDevices() : found " << controllers[i]->zones.size() << " zones");

            for (size_t j = 0; j < controllers[i]->zones.size(); j++)
            {
                auto* box = new QCheckBox(tr("Zone ") + QString::number(j));
                connect(box, &QCheckBox::clicked, this, &SettingsWidget::onWidgetUpdate);
                zone_lyt->addWidget(box);
                zone_boxes.push_back(box);
            }

            PLUGIN_DEBUG("SettingsWidget::fetchDevices() : finished");
            return;
        }
    }
    PLUGIN_DEBUG("SettingsWidget::fetchDevices() : no controllers found");
}

void SettingsWidget::loadSettings()
{
    std::filesystem::create_directories(res_mgr->GetConfigurationDirectory() / "plugins" / "settings"); // If the directory doesn't exist yet
    settings.LoadSettings(res_mgr->GetConfigurationDirectory() / "plugins" / "settings" / "HyperXDRAMcc.json");
    auto b_chan = settings.GetSettings("blue_chan_max"),
        rg_chan = settings.GetSettings("color_chan_max"),
        boxes = settings.GetSettings("zones");

    if (!b_chan.empty())
    {
        blue->setValue((int)b_chan);
        inj.cc.blue_chan_max = (uint8_t)b_chan;
    }

    if (!rg_chan.empty())
    {
        chan->setValue((int)rg_chan);
        inj.cc.color_chan_max_dim = (uint8_t)rg_chan;
    }

    if (!boxes.empty())
    {
        for (size_t i : boxes)
        {
            if (i >= zone_boxes.size())
            {
                // Zones are changed, so the user should update them manually
                break;
            }

            zone_boxes[i]->setChecked(true);
            inj.cc.dim_zones.insert(i);
        }
    }

    emit settingsUpdated();
}

void SettingsWidget::saveSettings()
{
    settings.SetSettings("blue_chan_max", inj.cc.blue_chan_max);
    settings.SetSettings("color_chan_max", inj.cc.color_chan_max_dim);
    std::vector<int> boxes(inj.cc.dim_zones.begin(), inj.cc.dim_zones.end());
    settings.SetSettings("zones", boxes);
    settings.SaveSettings();
}

void SettingsWidget::onDevicesUpdated()
{
    if (res_mgr->GetDetectionPercent() == 100)
    {
        PLUGIN_DEBUG("SettingsWidget::onDevicesUpdated() : 100%");
        fetchDevices();
        loadSettings();
    } else {
        // Disable elements
        for (auto* box : zone_boxes)
            box->setDisabled(true);
    }
}

void SettingsWidget::onWidgetUpdate()
{
    PLUGIN_DEBUG("SettingsWidget::onWidgetUpdate() : fetching widgets info");
    for (size_t i = 0; i < zone_boxes.size(); i++)
    {
        if (zone_boxes[i]->isChecked())
            inj.cc.dim_zones.insert(i);
    }
    // Slider info is already updated
    emit settingsUpdated();
    saveSettings();
}

void SettingsWidget::onSliderMoved()
{
    inj.cc.blue_chan_max = (uint8_t)blue->value();
    inj.cc.color_chan_max_dim = (uint8_t)chan->value();
    emit settingsUpdated();
}

void SettingsWidget::calibrateBlue()
{
    if (blue_calib->text() == "Calibrate")
    {
        emit calibrate(ToRGBColor(0, 0, 255));
        blue_calib->setText("Stop");
    } else {
        emit stopCalibration();
        blue_calib->setText("Calibrate");
    }
}

void SettingsWidget::calibrateWhite()
{
    if (white_calib->text() == "Calibrate")
    {
        emit calibrate(ToRGBColor(255, 255, 255));
        white_calib->setText("Stop");
    } else {
        emit stopCalibration();
        white_calib->setText("Calibrate");
    }
}
