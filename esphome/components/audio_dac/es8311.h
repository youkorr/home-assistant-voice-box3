#pragma once
#include "esphome/components/i2c/i2c.h"
#include "esphome/core/component.h"

namespace esphome {
namespace es8311 {

class ES8311Component : public Component, public i2c::I2CDevice {
 public:
  void setup() override;
  void write_register(uint8_t reg, uint8_t value);
  // ... (autres méthodes)
};

}  // namespace es8311
}  // namespace esphome
