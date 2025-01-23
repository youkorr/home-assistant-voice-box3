#include "i2s_audio_media_player.h"

#ifdef USE_ESP32

#include "esphome/core/log.h"

namespace esphome {
namespace i2s_audio {

static const char *const TAG = "audio";

void I2SAudioMediaPlayer::control(const media_player::MediaPlayerCall &call) {
  media_player::MediaPlayerState play_state = media_player::MEDIA_PLAYER_STATE_PLAYING;
  if (call.get_announcement().has_value()) {
    play_state = call.get_announcement().value() ? media_player::MEDIA_PLAYER_STATE_ANNOUNCING
                                                 : media_player::MEDIA_PLAYER_STATE_PLAYING;
  }
  if (call.get_media_url().has_value()) {
    this->current_url_ = call.get_media_url();
    if (this->i2s_state_ != I2S_STATE_STOPPED) {
      this->stop();
    }
    this->start();
  }

  if (play_state == media_player::MEDIA_PLAYER_STATE_ANNOUNCING) {
    this->is_announcement_ = true;
  }

  if (call.get_volume().has_value()) {
    this->volume = call.get_volume().value();
    this->set_volume_(volume);
    this->unmute_();
  }
  if (call.get_command().has_value()) {
    switch (call.get_command().value()) {
      case media_player::MEDIA_PLAYER_COMMAND_MUTE:
        this->mute_();
        break;
      case media_player::MEDIA_PLAYER_COMMAND_UNMUTE:
        this->unmute_();
        break;
      case media_player::MEDIA_PLAYER_COMMAND_VOLUME_UP: {
        float new_volume = this->volume + 0.1f;
        if (new_volume > 1.0f)
          new_volume = 1.0f;
        this->set_volume_(new_volume);
        this->unmute_();
        break;
      }
      case media_player::MEDIA_PLAYER_COMMAND_VOLUME_DOWN: {
        float new_volume = this->volume - 0.1f;
        if (new_volume < 0.0f)
          new_volume = 0.0f;
        this->set_volume_(new_volume);
        this->unmute_();
        break;
      }
      default:
        break;
    }
  }
  this->publish_state();
}

void I2SAudioMediaPlayer::mute_() {
  if (this->mute_pin_ != nullptr) {
    this->mute_pin_->digital_write(true);
  } else {
    this->set_volume_(0.0f, false);
  }
  this->muted_ = true;
}
void I2SAudioMediaPlayer::unmute_() {
  if (this->mute_pin_ != nullptr) {
    this->mute_pin_->digital_write(false);
  } else {
    this->set_volume_(this->volume, false);
  }
  this->muted_ = false;
}
void I2SAudioMediaPlayer::set_volume_(float volume, bool publish) {
  // Implémentez la logique pour régler le volume ici
  if (publish)
    this->volume = volume;
}

void I2SAudioMediaPlayer::setup() {
  ESP_LOGCONFIG(TAG, "Setting up Audio...");
  this->state = media_player::MEDIA_PLAYER_STATE_IDLE;
}

void I2SAudioMediaPlayer::loop() {
  switch (this->i2s_state_) {
    case I2S_STATE_STARTING:
      this->start_();
      break;
    case I2S_STATE_RUNNING:
      this->play_();
      break;
    case I2S_STATE_STOPPING:
      this->stop_();
      break;
    case I2S_STATE_STOPPED:
      break;
  }
}

void I2SAudioMediaPlayer::play_() {
  // Implémentez la logique pour lire l'audio ici
  if ((this->state == media_player::MEDIA_PLAYER_STATE_PLAYING ||
       this->state == media_player::MEDIA_PLAYER_STATE_ANNOUNCING)) {
    this->stop();
  }
}

void I2SAudioMediaPlayer::start() { this->i2s_state_ = I2S_STATE_STARTING; }
void I2SAudioMediaPlayer::start_() {
  if (!this->parent_->try_lock()) {
    return;  // Waiting for another i2s to return lock
  }

  i2s_pin_config_t pin_config = this->parent_->get_pin_config();
  pin_config.data_out_num = this->dout_pin_;
  i2s_set_pin(this->parent_->get_port(), &pin_config);

  if (this->mute_pin_ != nullptr) {
    this->mute_pin_->setup();
    this->mute_pin_->digital_write(false);
  }

  this->i2s_state_ = I2S_STATE_RUNNING;
  this->high_freq_.start();
  this->set_volume_(this->volume, false);

  if (this->current_url_.has_value()) {
    // Implémentez la logique pour lire l'URL audio ici
    this->state = media_player::MEDIA_PLAYER_STATE_PLAYING;
    if (this->is_announcement_) {
      this->state = media_player::MEDIA_PLAYER_STATE_ANNOUNCING;
    }
    this->publish_state();
  }
}
void I2SAudioMediaPlayer::stop() {
  if (this->i2s_state_ == I2S_STATE_STOPPED) {
    return;
  }
  if (this->i2s_state_ == I2S_STATE_STARTING) {
    this->i2s_state_ = I2S_STATE_STOPPED;
    return;
  }
  this->i2s_state_ = I2S_STATE_STOPPING;
}
void I2SAudioMediaPlayer::stop_() {
  this->current_url_ = {};
  this->parent_->unlock();
  this->i2s_state_ = I2S_STATE_STOPPED;

  this->high_freq_.stop();
  this->state = media_player::MEDIA_PLAYER_STATE_IDLE;
  this->publish_state();
  this->is_announcement_ = false;
}

media_player::MediaPlayerTraits I2SAudioMediaPlayer::get_traits() {
  auto traits = media_player::MediaPlayerTraits();
  traits.set_supports_pause(true);
  return traits;
};

void I2SAudioMediaPlayer::dump_config() {
  ESP_LOGCONFIG(TAG, "Audio:");
  if (this->is_failed()) {
    ESP_LOGCONFIG(TAG, "Audio failed to initialize!");
    return;
  }
  ESP_LOGCONFIG(TAG, "  External DAC channels: %d", this->external_dac_channels_);
  ESP_LOGCONFIG(TAG, "  I2S DOUT Pin: %d", this->dout_pin_);
  LOG_PIN("  Mute Pin: ", this->mute_pin_);
}

}  // namespace i2s_audio
}  // namespace esphome

#endif  // USE_ESP32
