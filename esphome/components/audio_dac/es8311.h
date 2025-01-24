#pragma once

#include "esphome/components/audio_dac/audio_dac.h" // Assurez-vous que ce fichier existe
#include "esphome/components/i2c/i2c.h"
#include "esphome/core/component.h"

namespace esphome {
namespace es8311 {

enum ES8311MicGain {
  ES8311_MIC_GAIN_MIN = -1,
  ES8311_MIC_GAIN_0DB,
  ES8311_MIC_GAIN_6DB,
  ES8311_MIC_GAIN_12DB,
  ES8311_MIC_GAIN_18DB,
  ES8311_MIC_GAIN_24DB,
  ES8311_MIC_GAIN_30DB,
  ES8311_MIC_GAIN_36DB,
  ES8311_MIC_GAIN_42DB,
  ES8311_MIC_GAIN_MAX
};

enum ES8311Resolution : uint8_t {
  ES8311_RESOLUTION_16 = 16,
  ES8311_RESOLUTION_18 = 18,
  ES8311_RESOLUTION_20 = 20,
  ES8311_RESOLUTION_24 = 24,
  ES8311_RESOLUTION_32 = 32
};

struct ES8311Coefficient {
  uint32_t mclk;     // fréquence mclk
  uint32_t rate;     // fréquence d'échantillonnage
  uint8_t pre_div;   // le pré-diviseur avec une plage de 1 à 8
  uint8_t pre_mult;  // le pré-multiplicateur avec sélection x1, x2, x4 et x8
  uint8_t adc_div;   // diviseur adcclk
  uint8_t dac_div;   // diviseur dacclk
  uint8_t fs_mode;   // vitesse simple (0) ou double vitesse (1)
  uint8_t lrck_h;    // diviseur lrck adc et diviseur lrck dac
  uint8_t lrck_l;    //
  uint8_t bclk_div;  // diviseur sclk
  uint8_t adc_osr;   // osr adc
  uint8_t dac_osr;   // osr dac
};

class ES8311 : public audio_dac::AudioDac, public Component, public i2c::I2CDevice {
 public:
  /////////////////////////
  // Overrides de Component //
  /////////////////////////

  void setup() override;
  float get_setup_priority() const override { return setup_priority::DATA; }
  void dump_config() override;

  ////////////////////////
  // Overrides de AudioDac //
  ////////////////////////

  // Vérifiez que ces méthodes existent dans audio_dac::AudioDac
  bool set_volume(float volume) override; // Assurez-vous que cette méthode existe dans la classe de base
  float volume() override; // Assurez-vous que cette méthode existe dans la classe de base
  bool set_mute_off() override { return this->set_mute_state_(false); }
  bool set_mute_on() override { return this->set_mute_state_(true); }
  bool is_muted() override { return this->is_muted_; }

  //////////////////////////////////
  // Setters de configuration ES8311 //
  //////////////////////////////////

  void set_use_mclk(bool use_mclk) { this->use_mclk_ = use_mclk; }
  void set_bits_per_sample(ES8311Resolution resolution) {
    this->resolution_in_ = resolution;
    this->resolution_out_ = resolution;
  }
  void set_sample_frequency(uint32_t sample_frequency) { this->sample_frequency_ = sample_frequency; }
  void set_use_mic(bool use_mic) { this->use_mic_ = use_mic; }
  void set_mic_gain(ES8311MicGain mic_gain) { this->mic_gain_ = mic_gain; }

 protected:
  bool is_muted_ = false; // Ajout de la variable membre

  /// @brief Calcule la valeur du registre pour la résolution configurée (bits par échantillon)
  static uint8_t calculate_resolution_value(ES8311Resolution resolution);

  /// @brief Récupère les valeurs de registre appropriées pour le mclk et le taux configurés
  static const ES8311Coefficient *get_coefficient(uint32_t mclk, uint32_t rate);

  /// @brief Configure les registres ES8311 pour le taux d'échantillonnage choisi
  bool configure_clock_();
