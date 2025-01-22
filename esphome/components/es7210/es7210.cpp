#include "es7210.h"
#include "es7210_idf.h"  // Inclure le code ESP-IDF

void ES7210::setup() {
    ESP_LOGD("ES7210", "Initializing ES7210...");
    es7210_init(I2C_NUM_0);  // Utiliser la fonction ESP-IDF
    ESP_LOGD("ES7210", "I2C Address: 0x%02X", i2c_address);
    ESP_LOGD("ES7210", "Sample Rate: %d Hz", sample_rate);
    ESP_LOGD("ES7210", "Bits per Sample: %d", bits_per_sample);
}

void ES7210::loop() {
    // Logique principale du ES7210 (optionnel)
}

void ES7210::set_i2c_address(uint8_t address) {
    this->i2c_address = address;
}

void ES7210::set_sample_rate(uint32_t sample_rate) {
    this->sample_rate = sample_rate;
}

void ES7210::set_bits_per_sample(uint8_t bits_per_sample) {
    this->bits_per_sample = bits_per_sample;
}
