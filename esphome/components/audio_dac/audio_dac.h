#pragma once
#include "esphome/core/defines.h"
#include "esphome/core/hal.h"

namespace esphome {
namespace audio_dac {

class AudioDAC {
 public:
  virtual void start() = 0;  // Démarrer le DAC audio
  virtual void stop() = 0;   // Arrêter le DAC audio
  virtual void write(const uint8_t *data, size_t len) = 0;  // Écrire des données audio

  virtual bool set_mute_off() = 0;  // Désactiver la mise en sourdine
  virtual bool set_mute_on() = 0;   // Activer la mise en sourdine
  virtual bool set_volume(float volume) = 0;  // Régler le volume
  virtual bool is_muted() = 0;  // Vérifier si le son est en sourdine
  virtual float volume() = 0;   // Obtenir le volume actuel

 protected:
  bool is_muted_{false};  // État de la mise en sourdine
};

}  // namespace audio_dac
}  // namespace esphome
