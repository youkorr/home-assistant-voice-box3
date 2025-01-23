#pragma once

#ifdef USE_ESP32

#include "../i2s_audio.h"

#include "driver/i2s.h"
#include "esp_log.h"

#include "esphome/components/media_player/media_player.h"
#include "esphome/core/component.h"
#include "esphome/core/gpio.h"
#include "esphome/core/helpers.h"

namespace esphome {
namespace i2s_audio {

enum I2SState : uint8_t {
  I2S_STATE_STOPPED = 0,
  I2S_STATE_STARTING,
  I2S_STATE_RUNNING,
  I2S_STATE_STOPPING,
};

class I2SAudioMediaPlayer : public Component, public Parented<I2SAudioComponent>, public media_player::MediaPlayer {
 public:
  void setup() override;
  float get_setup_priority() const override { return esphome::setup_priority::LATE; }

  void loop() override;

  void dump_config() override;

  void set_dout_pin(uint8_t pin) { this->dout_pin_ = pin; }
  void set_mute_pin(GPIOPin *mute_pin) { this->mute_pin_ = mute_pin; }
  void set_external_dac_channels(uint8_t channels) { this->external_dac_channels_ = channels; }

  void set_i2s_comm_fmt_lsb(bool lsb) { this->i2s_comm_fmt_lsb_ = lsb; }

  media_player::MediaPlayerTraits get_traits() override;

  bool is_muted() const override { return this->muted_; }

  void start();
  void stop();

 protected:
  void control(const media_player::MediaPlayerCall &call) override;

  void mute_();
  void unmute_();
  void set_volume_(float volume, bool publish = true);

  void start_();
  void stop_();
  void play_();

  I2SState i2s_state_{I2S_STATE_STOPPED};

  uint8_t dout_pin_{0};

  GPIOPin *mute_pin_{nullptr};
  bool muted_{false};
  float unmuted_volume_{0};

  uint8_t external_dac_channels_;
  bool i2s_comm_fmt_lsb_;

  HighFrequencyLoopRequester high_freq_;

  optional<std::string> current_url_{};
  bool is_announcement_{false};
};

}  // namespace i2s_audio
}  // namespace esphome

#endif  // USE_ESP32
