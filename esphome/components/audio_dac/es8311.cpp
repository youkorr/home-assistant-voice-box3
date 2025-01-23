static const char *const TAG = "es8311";

void ES8311Component::write_register(uint8_t reg, uint8_t value) {
  if (!this->write_byte(reg, value)) {
    ESP_LOGE(TAG, "Error writing register 0x%02X", reg);
  }
}

void ES8311Component::setup() {
  ESP_LOGCONFIG(TAG, "Setting up ES8311...");
  write_register(0x00, 0x80);
}

void ES8311Component::start() {
  ESP_LOGD(TAG, "Starting ES8311");
  write_register(0x03, 0x01);
}

void ES8311Component::stop() {
  ESP_LOGD(TAG, "Stopping ES8311");
  write_register(0x03, 0x00);
}
