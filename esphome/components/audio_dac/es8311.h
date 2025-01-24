#include "esphome/components/i2c/i2c.h"
#include "esphome/core/component.h"

namespace esphome {
namespace es8311 {

class ES8311Component : public Component, public i2c::I2CDevice {
 public:
  void setup() override;
  void start();
  void stop();
  void write(const uint8_t *data, size_t len);

  void write_register(uint8_t reg, uint8_t value);

 protected:
  // Add any necessary protected members
};

}  // namespace es8311
}  // namespace esphome
