#ifndef COLORCORRECTION_H
#define COLORCORRECTION_H

#include <QObject>
#include "RGBController/RGBController.h"
#include <cstdint>
#include <unordered_set>


class ColorCorrection : public QObject
{
    Q_OBJECT
public:
    uint8_t blue_chan_max;

    uint8_t color_chan_max_dim;

    std::unordered_set<size_t> dim_zones;

    explicit ColorCorrection(QObject* parent = nullptr) : QObject(parent) {}

    inline void correct_color(RGBController* ctrl, size_t zone_id);
};

#endif // COLORCORRECTION_H
