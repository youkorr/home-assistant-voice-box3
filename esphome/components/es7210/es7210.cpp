#include "es7210.h"

void ES7210::setup() {
  ESP_LOGI("ES7210", "Initialisation du microphone ES7210...");
  // Configuration de base
  write_register(0x00, 0x41); // Exemple de configuration
}

void ES7210::configure(uint8_t sample_rate, uint8_t gain) {
  ESP_LOGI("ES7210", "Configuration du microphone...");
  write_register(0x01, sample_rate); // Régler le taux d'échantillonnage
  write_register(0x02, gain);        // Régler le gain
}

void ES7210::start_recording() {
  ESP_LOGI("ES7210", "Démarrage de l'enregistrement...");
  write_register(0x03, 0x01); // Activer l'enregistrement
  is_recording = true;
}

void ES7210::stop_recording() {
  ESP_LOGI("ES7210", "Arrêt de l'enregistrement...");
  write_register(0x03, 0x00); // Désactiver l'enregistrement
  is_recording = false;
}

void ES7210::write_register(uint8_t reg, uint8_t value) {
  i2c_cmd_handle_t cmd = i2c_cmd_link_create();
  i2c_master_start(cmd);
  i2c_master_write_byte(cmd, (address << 1) | I2C_MASTER_WRITE, true);
  i2c_master_write_byte(cmd, reg, true);
  i2c_master_write_byte(cmd, value, true);
  i2c_master_stop(cmd);
  esp_err_t err = i2c_master_cmd_begin(I2C_NUM_0, cmd, 1000 / portTICK_PERIOD_MS);
  i2c_cmd_link_delete(cmd);
  if (err != ESP_OK) {
    ESP_LOGE("ES7210", "Erreur d'écriture I2C: %s", esp_err_to_name(err));
  }
}

uint8_t ES7210::read_register(uint8_t reg) {
  uint8_t value = 0;
  i2c_cmd_handle_t cmd = i2c_cmd_link_create();
  i2c_master_start(cmd);
  i2c_master_write_byte(cmd, (address << 1) | I2C_MASTER_WRITE, true);
  i2c_master_write_byte(cmd, reg, true);
  i2c_master_start(cmd);
  i2c_master_write_byte(cmd, (address << 1) | I2C_MASTER_READ, true);
  i2c_master_read_byte(cmd, &value, I2C_MASTER_NACK);
  i2c_master_stop(cmd);
  esp_err_t err = i2c_master_cmd_begin(I2C_NUM_0, cmd, 1000 / portTICK_PERIOD_MS);
  i2c_cmd_link_delete(cmd);
  if (err != ESP_OK) {
    ESP_LOGE("ES7210", "Erreur de lecture I2C: %s", esp_err_to_name(err));
  }
  return value;
}
