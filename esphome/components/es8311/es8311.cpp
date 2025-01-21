#include "es8311.h"

namespace esphome {
namespace es8311 {

void ES8311Component::setup() {
  ESP_LOGI("ES8311", "Initialisation...");
  write_register(0x00, 0x80);  // Config de base (selon datasheet)
}

void ES8311Component::start() {
  ESP_LOGI("ES8311", "Démarrage");
  write_register(0x03, 0x01);  // Activer DAC (registre 0x03)
}

void ES8311Component::stop() {
  ESP_LOGI("ES8311", "Arrêt");
  write_register(0x03, 0x00);  // Désactiver DAC
}

void ES8311Component::write(const uint8_t *data, size_t len) {
  this->write_bytes(0x04, data, len);  // Envoyer données audio (registre 0x04)
}

void ES8311Component::set_sample_rate(uint32_t sample_rate) {
  write_register(0x01, (sample_rate >> 8) & 0xFF);  // MSB
  write_register(0x02, sample_rate & 0xFF);         // LSB
}

}  // namespace es8311
}  // namespace esphome
