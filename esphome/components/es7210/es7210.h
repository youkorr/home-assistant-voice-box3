#pragma once
#include "esphome.h"

namespace esphome {
namespace es7210 {

class ES7210 : public Component, public i2c::I2CDevice {
public:
    void setup() override;
    void dump_config() override;
};

}  // namespace es7210
}  // namespace esphome
