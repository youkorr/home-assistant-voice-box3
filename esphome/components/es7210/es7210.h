#pragma once

#include "esphome/core/component.h"
#include "esphome/components/i2c/i2c.h"

namespace esphome {
namespace es7210 {

class ES7210 : public i2c::I2CDevice, public Component {
 public:
  void setup() override;
  void loop() override;
  // Ajoutez d'autres méthodes et attributs nécessaires
};

}  // namespace es7210
}  // namespace esphome

