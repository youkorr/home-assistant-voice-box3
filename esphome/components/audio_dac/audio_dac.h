#pragma once

#include "esphome/core/defines.h"
#include "esphome/core/hal.h"

namespace esphome {
namespace audio_dac {

class AudioDac {
 public:
  // Méthodes virtuelles pures pour contrôler le DAC audio
  virtual bool set_mute_off() = 0;  // Désactiver le mute
  virtual bool set_mute_on() = 0;   // Activer le mute
  virtual bool set_volume(float volume) = 0;  // Régler le volume (0.0 à 1.0)

  // Méthodes virtuelles pures pour obtenir l'état actuel
  virtual bool is_muted() = 0;  // Vérifier si le mute est activé
  virtual float volume() = 0;   // Obtenir le volume actuel

  // Destructeur virtuel pour assurer une destruction correcte des classes dérivées
  virtual ~AudioDac() = default;

 protected:
  bool is_muted_{false};  // État du mute (par défaut : désactivé)
};

}  // namespace audio_dac
}  // namespace esphome
