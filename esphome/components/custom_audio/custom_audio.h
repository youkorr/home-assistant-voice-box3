#pragma once
#include "esphome.h"
#include <Audio.h>

namespace esphome {
namespace custom_audio {

class CustomAudioMediaPlayer : public Component, 
                             public media_player::MediaPlayer,
                             public i2s_audio::I2SAudioOutput {
 public:
  void set_i2s_output(i2s_audio::I2SAudioOutput *output) { 
    this->output_ = output; 
  }
  
  void setup() override;
  void loop() override;
  
  // Implémenter les méthodes MediaPlayer
  media_player::MediaPlayerTraits get_traits() override;
  void control(const media_player::MediaPlayerCall &call) override;

 protected:
  i2s_audio::I2SAudioOutput *output_;
};

}  // namespace custom_audio
}  // namespace esphome
