#include "es8311.h"
#include "esphome/core/log.h"

namespace esphome {
namespace es8311 {

static const char *const TAG = "es8311";

void ES8311Component::write_register(uint8_t reg, uint8_t value) {
  if (!this->get_device()->write_byte(reg, value)) { // Utilisation de get_device()
    ESP_LOGE(TAG, "Erreur écriture registre 0x%02X", reg);
  }
}

void ES8311Component::setup() {
  ESP_LOGCONFIG(TAG, "Configuration ES8311...");
  if (!this->get_device()->probe()) { // Utilisation de get_device()
    ESP_LOGE(TAG, "ES8311 non trouvé sur le bus I2C !");
    this->mark_failed();
    return;
  }
  // ... (votre code d'initialisation)
}

// ... (autres méthodes)

}  // namespace es8311
}  // namespace esphome
