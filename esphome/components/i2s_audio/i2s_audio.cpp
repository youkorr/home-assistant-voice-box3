#include "i2s_audio.h"

#ifdef USE_ESP32

#include "esphome/core/log.h"
#include "driver/i2s.h"  // Inclure l'API I2S d'ESP-IDF

namespace esphome {
namespace i2s_audio {

static const char *const TAG = "i2s_audio";

#if defined(USE_ESP_IDF) && (ESP_IDF_VERSION_MAJOR >= 5)
static const uint8_t I2S_NUM_MAX = SOC_I2S_NUM;  // because IDF 5+ took this away :(
#endif

void I2SAudioComponent::setup() {
  static i2s_port_t next_port_num = I2S_NUM_0;

  if (next_port_num >= I2S_NUM_MAX) {
    ESP_LOGE(TAG, "Too many I2S Audio components!");
    this->mark_failed();
    return;
  }

  this->port_ = next_port_num;
  next_port_num = (i2s_port_t) (next_port_num + 1);

  ESP_LOGCONFIG(TAG, "Setting up I2S Audio...");

  // Configuration I2S pour la sortie audio
  i2s_config_t i2s_config = {
      .mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_TX | I2S_MODE_RX),  // Mode duplex
      .sample_rate = 16000,                                               // Fréquence d'échantillonnage
      .bits_per_sample = I2S_BITS_PER_SAMPLE_16BIT,                       // Résolution audio
      .channel_format = I2S_CHANNEL_FMT_RIGHT_LEFT,                       // Format de canal
      .communication_format = I2S_COMM_FORMAT_STAND_I2S,                  // Format I2S standard
      .intr_alloc_flags = ESP_INTR_FLAG_LEVEL1,                           // Priorité d'interruption
      .dma_buf_count = 8,                                                 // Nombre de buffers DMA
      .dma_buf_len = 64,                                                  // Taille de chaque buffer DMA
      .use_apll = false,                                                  // Pas de PLL audio
  };

  // Configuration des broches I2S
  i2s_pin_config_t pin_config = {
      .bck_io_num = this->bclk_pin_,    // Broche BCK (Bit Clock)
      .ws_io_num = this->lrclk_pin_,    // Broche WS (Word Select)
      .data_out_num = this->dout_pin_,  // Broche DOUT (Data Out)
      .data_in_num = this->din_pin_,    // Broche DIN (Data In)
      .mck_io_num = this->mclk_pin_,    // Broche MCLK (Master Clock)
  };

  // Installer le driver I2S
  esp_err_t err = i2s_driver_install(this->port_, &i2s_config, 0, nullptr);
  if (err != ESP_OK) {
    ESP_LOGE(TAG, "Failed to install I2S driver: %s", esp_err_to_name(err));
    this->mark_failed();
    return;
  }

  // Configurer les broches I2S
  err = i2s_set_pin(this->port_, &pin_config);
  if (err != ESP_OK) {
    ESP_LOGE(TAG, "Failed to set I2S pins: %s", esp_err_to_name(err));
    this->mark_failed();
    return;
  }

  ESP_LOGI(TAG, "I2S Audio initialized successfully on port %d", this->port_);
}

void I2SAudioComponent::write(const uint8_t *data, size_t len) {
  size_t bytes_written = 0;
  esp_err_t err = i2s_write(this->port_, data, len, &bytes_written, portMAX_DELAY);
  if (err != ESP_OK) {
    ESP_LOGE(TAG, "Failed to write audio data: %s", esp_err_to_name(err));
  }
}

void I2SAudioComponent::read(uint8_t *data, size_t len) {
  size_t bytes_read = 0;
  esp_err_t err = i2s_read(this->port_, data, len, &bytes_read, portMAX_DELAY);
  if (err != ESP_OK) {
    ESP_LOGE(TAG, "Failed to read audio data: %s", esp_err_to_name(err));
  }
}

}  // namespace i2s_audio
}  // namespace esphome

#endif  // USE_ESP32
