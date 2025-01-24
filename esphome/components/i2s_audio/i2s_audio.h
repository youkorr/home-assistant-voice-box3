#pragma once

#ifdef USE_ESP32

#include <driver/i2s.h>
#include "esphome/core/component.h"
#include "esphome/core/helpers.h"

namespace esphome {
namespace i2s_audio {

class I2SAudioComponent;

class I2SAudioBase : public Parented<I2SAudioComponent> {
 public:
  void set_i2s_mode(i2s_mode_t mode) { this->i2s_mode_ = mode; }
  void set_channel(i2s_channel_fmt_t channel) { this->channel_ = channel; }
  void set_sample_rate(uint32_t sample_rate) { this->sample_rate_ = sample_rate; }
  void set_bits_per_sample(i2s_bits_per_sample_t bits_per_sample) { this->bits_per_sample_ = bits_per_sample; }
  void set_bits_per_channel(i2s_bits_per_chan_t bits_per_channel) { this->bits_per_channel_ = bits_per_channel; }
  void set_use_apll(uint32_t use_apll) { this->use_apll_ = use_apll; }

 protected:
  i2s_mode_t i2s_mode_{};                       // Mode I2S
  i2s_channel_fmt_t channel_;                   // Format du canal
  uint32_t sample_rate_;                        // Fréquence d'échantillonnage
  i2s_bits_per_sample_t bits_per_sample_;       // Bits par échantillon
  i2s_bits_per_chan_t bits_per_channel_;        // Bits par canal
  bool use_apll_;                               // Utilisation de l'APLL (Audio PLL)
};

class I2SAudioIn : public I2SAudioBase {};       // Classe pour les entrées audio
class I2SAudioOut : public I2SAudioBase {};      // Classe pour les sorties audio

class I2SAudioComponent : public Component {
 public:
  void setup() override;
  void write(const uint8_t *data, size_t len);   // Écrire des données audio
  void read(uint8_t *data, size_t len);         // Lire des données audio

  // Récupérer la configuration des broches I2S
  i2s_pin_config_t get_pin_config() const {
    return {
        .mck_io_num = this->mclk_pin_,
        .bck_io_num = this->bclk_pin_,
        .ws_io_num = this->lrclk_pin_,
        .data_out_num = this->dout_pin_,
        .data_in_num = this->din_pin_,
    };
  }

  // Setters pour configurer les broches
  void set_mclk_pin(int pin) { this->mclk_pin_ = pin; }
  void set_bclk_pin(int pin) { this->bclk_pin_ = pin; }
  void set_lrclk_pin(int pin) { this->lrclk_pin_ = pin; }
  void set_dout_pin(int pin) { this->dout_pin_ = pin; }
  void set_din_pin(int pin) { this->din_pin_ = pin; }

  // Gestion des verrous
  void lock() { this->lock_.lock(); }
  bool try_lock() { return this->lock_.try_lock(); }
  void unlock() { this->lock_.unlock(); }

  // Récupérer le port I2S assigné
  i2s_port_t get_port() const { return this->port_; }

  // Vérification de la configuration des broches
  bool are_pins_valid() const {
    return (this->bclk_pin_ != I2S_PIN_NO_CHANGE && 
            this->lrclk_pin_ != I2S_PIN_NO_CHANGE &&
            (this->dout_pin_ != I2S_PIN_NO_CHANGE || this->din_pin_ != I2S_PIN_NO_CHANGE));
  }

 protected:
  Mutex lock_;                 // Verrou pour accès concurrentiel

  I2SAudioIn *audio_in_{nullptr};   // Instance pour l'entrée audio
  I2SAudioOut *audio_out_{nullptr}; // Instance pour la sortie audio

  int mclk_pin_{I2S_PIN_NO_CHANGE}; // Broche MCLK (Master Clock)
  int bclk_pin_{I2S_PIN_NO_CHANGE}; // Broche BCLK (Bit Clock)
  int lrclk_pin_;                   // Broche LRCLK (Word Select)
  int dout_pin_{I2S_PIN_NO_CHANGE}; // Broche DOUT (Data Out)
  int din_pin_{I2S_PIN_NO_CHANGE};  // Broche DIN (Data In)
  i2s_port_t port_{};               // Port I2S utilisé
};

}  // namespace i2s_audio
}  // namespace esphome

#endif  // USE_ESP32

