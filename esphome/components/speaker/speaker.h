#pragma once

#include <cstddef>
#include <cstdint>
#include <vector>

#ifdef USE_ESP32
#include <freertos/FreeRTOS.h>
#endif

#include "esphome/core/defines.h"
#include "esphome/components/audio/audio.h"

#ifdef USE_AUDIO_DAC
#include "esphome/components/audio_dac/audio_dac.h"  // Assurez-vous que ce fichier existe et est correct
#endif

namespace esphome {
namespace speaker {

enum State : uint8_t {
  STATE_STOPPED = 0,
  STATE_STARTING,
  STATE_RUNNING,
  STATE_STOPPING,
};

class Speaker {
 public:
#ifdef USE_ESP32
  /// @brief Plays the provided audio data.
  virtual size_t play(const uint8_t *data, size_t length, TickType_t ticks_to_wait) {
    return this->play(data, length);
  }
#endif

  /// @brief Plays the provided audio data.
  virtual size_t play(const uint8_t *data, size_t length) = 0;

  size_t play(const std::vector<uint8_t> &data) { return this->play(data.data(), data.size()); }

  virtual void start() = 0;
  virtual void stop() = 0;
  virtual void finish() { this->stop(); }

  virtual bool has_buffered_data() const = 0;

  bool is_running() const { return this->state_ == STATE_RUNNING; }
  bool is_stopped() const { return this->state_ == STATE_STOPPED; }

  // Volume control
  virtual void set_volume(float volume) {
    this->volume_ = volume;
#ifdef USE_AUDIO_DAC
    if (this->audio_dac_ != nullptr) {
      this->audio_dac_->set_volume(volume);
    }
#endif
  }

  float get_volume() { return this->volume_; }

  // Mute control
  virtual void set_mute_state(bool mute_state) {
    this->mute_state_ = mute_state;
#ifdef USE_AUDIO_DAC
    if (this->audio_dac_) {
      if (mute_state) {
        this->audio_dac_->set_mute_on();
      } else {
        this->audio_dac_->set_mute_off();
      }
    }
#endif
  }

  bool get_mute_state() { return this->mute_state_; }

#ifdef USE_AUDIO_DAC
  void set_audio_dac(audio_dac::AudioDAC *audio_dac) { this->audio_dac_ = audio_dac; }
#endif

  void set_audio_stream_info(const audio::AudioStreamInfo &audio_stream_info) {
    this->audio_stream_info_ = audio_stream_info;
  }

 protected:
  State state_{STATE_STOPPED};
  audio::AudioStreamInfo audio_stream_info_;
  float volume_{1.0f};
  bool mute_state_{false};

#ifdef USE_AUDIO_DAC
  audio_dac::AudioDAC *audio_dac_{nullptr};  // Déclaration conditionnelle de audio_dac_
#endif
};

}  // namespace speaker
}  // namespace esphome
