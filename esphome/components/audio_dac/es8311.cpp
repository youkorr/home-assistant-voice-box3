#pragma once
#include "esphome.h"
#include "esphome/components/audio_dac/audio_dac.h"
#include "esphome/components/i2c/i2c.h"

namespace esphome {
namespace es8311 {

class ES8311Component : public audio_dac::AudioDAC, public i2c::I2CDevice, public Component {
 public:
  void setup() override;
  void start() override;
  void stop() override;
  void write(const uint8_t *data, size_t len) override;
  void set_sample_rate(uint32_t sample_rate);

 protected:
  void write_register(uint8_t reg, uint8_t value);
};

}  // namespace es8311
}  // namespace esphome
