#pragma once
#include "esphome.h"
#include "esphome/components/media_player/media_player.h"
#include "esphome/components/i2s_audio/i2s_audio.h"

namespace esphome {
namespace custom_audio {

class MediaPlayer : public Component,
                             public media_player::MediaPlayer,
                             public i2s_audio::I2SAudioOutput {
 public:
  void set_i2s_output(i2s_audio::I2SAudioOutput *output) { 
    this->i2s_output_ = output; 
  }

  void setup() override;
  void loop() override;
  media_player::MediaPlayerTraits get_traits() override;
  void control(const media_player::MediaPlayerCall &call) override;

 protected:
  i2s_audio::I2SAudioOutput *i2s_output_{nullptr};
};

} // namespace custom_audio
} // namespace esphome
