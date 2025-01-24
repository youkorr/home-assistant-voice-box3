#pragma once

#include "esphome/components/audio_dac/audio_dac.h"
#include "esphome/core/component.h"
#include "esphome/core/hal.h"

namespace esphome {
namespace es8311 {

class ES8311Component : public audio_dac::AudioDAC, public Component {
 public:
  void setup() override;
  void start() override;
  void stop() override;
  void write(const uint8_t *data, size_t len) override;

 protected:
  void write_register(uint8_t reg, uint8_t value);
};

}  // namespace es8311
}  // namespace esphome
