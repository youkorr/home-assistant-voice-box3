#include "es7210.h"
#include "esphome/core/log.h"

namespace esphome {
namespace es7210 {

void ES7210::setup() {
  ESP_LOGCONFIG(TAG, "Setting up ES7210...");
  write_register(0x00, 0x41);
}

void ES7210::write_register(uint8_t reg, uint8_t value) {
  if (!this->write_byte(reg, value)) {
    ESP_LOGE(TAG, "Error writing register 0x%02X", reg);
  }
}

uint8_t ES7210::read_register(uint8_t reg) {
  uint8_t value;
  if (!this->read_byte(reg, &value)) {
    ESP_LOGE(TAG, "Error reading register 0x%02X", reg);
    return 0;
  }
  return value;
}

// ... reste des méthodes avec les mêmes corrections ...

}  // namespace es7210
}  // namespace esphome
  }
  return value;
}
