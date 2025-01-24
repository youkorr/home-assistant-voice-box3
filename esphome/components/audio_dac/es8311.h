#include "esphome.h"
#include "esphome/components/i2c/i2c.h"

namespace esphome {
namespace es8311 {

class ES8311Component : public Component {
 public:
  // Constructeur pour initialiser l'appareil I2C
  explicit ES8311Component(i2c::I2CDevice *device) : device_(*device) {}

  void setup() override;
  void write_register(uint8_t reg, uint8_t value);

  // Méthode pour accéder à l'appareil I2C
  i2c::I2CDevice *get_device() { return &this->device_; }

 protected:
  i2c::I2CDevice device_; // Stocke l'appareil I2C
};

}  // namespace es8311
}  // namespace esphome
