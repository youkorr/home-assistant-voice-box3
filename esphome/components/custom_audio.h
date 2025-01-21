#pragma once
#include "esphome/components/media_player/media_player.h"

namespace esphome {
namespace custom_audio {

class CustomAudioMediaPlayer : public media_player::MediaPlayer {
 public:
  void setup() override;
  void control(const media_player::MediaPlayerCall &call) override;
  media_player::MediaPlayerTraits traits() override;

 protected:
  void play();
  void pause();
  void stop();
  void set_volume(float volume);
};

}  // namespace custom_audio
}  // namespace esphome
