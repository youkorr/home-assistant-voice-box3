#pragma once

#include "esphome.h"
#include "esphome/core/component.h"
#include "esphome/components/i2c/i2c.h"

namespace esphome {
namespace es7210 {

class ES7210 : public i2c::I2CDevice, public Component {
 public:
  void setup() override;
  void configure(uint8_t sample_rate, uint8_t gain);
  void start_recording();
  void stop_recording();

 private:
  bool is_recording = false;
  void write_register(uint8_t reg, uint8_t value);
  uint8_t read_register(uint8_t reg);
};

}  // namespace es7210
}  // namespace esphome
