#pragma once

#include "esphome/core/entity_base.h"
#include "esphome/core/helpers.h"
#include "esphome/components/i2s_audio/i2s_audio.h"

namespace esphome {
namespace media_player {

// États du lecteur média
enum MediaPlayerState : uint8_t {
  MEDIA_PLAYER_STATE_NONE = 0,
  MEDIA_PLAYER_STATE_IDLE = 1,
  MEDIA_PLAYER_STATE_PLAYING = 2,
  MEDIA_PLAYER_STATE_PAUSED = 3,
  MEDIA_PLAYER_STATE_ANNOUNCING = 4
};

const char *media_player_state_to_string(MediaPlayerState state);

// Commandes du lecteur média
enum MediaPlayerCommand : uint8_t {
  MEDIA_PLAYER_COMMAND_PLAY = 0,
  MEDIA_PLAYER_COMMAND_PAUSE = 1,
  MEDIA_PLAYER_COMMAND_STOP = 2,
  MEDIA_PLAYER_COMMAND_MUTE = 3,
  MEDIA_PLAYER_COMMAND_UNMUTE = 4,
  MEDIA_PLAYER_COMMAND_TOGGLE = 5,
  MEDIA_PLAYER_COMMAND_VOLUME_UP = 6,
  MEDIA_PLAYER_COMMAND_VOLUME_DOWN = 7,
};

const char *media_player_command_to_string(MediaPlayerCommand command);

// Déclaration anticipée
class MediaPlayerDevice;
class MediaPlayerCall;

// Structure pour les traits du lecteur média
struct MediaPlayerTraits {
  bool supports_pause{false};
  bool supports_volume{false};
  bool supports_mute{false};
};

// Classe principale du lecteur média
class MediaPlayerDevice : public EntityBase {
 public:
  MediaPlayerState state{MEDIA_PLAYER_STATE_NONE};
  float volume{1.0f};

  MediaPlayerCall make_call();
  void publish_state();
  void add_on_state_callback(std::function<void()> &&callback);
  
  virtual bool is_muted() const { return false; }
  virtual MediaPlayerTraits get_traits() = 0;

 protected:
  friend class MediaPlayerCall;
  virtual void control(const MediaPlayerCall &call) = 0;
  CallbackManager<void()> state_callback_{};
};

// Classe pour gérer les commandes du lecteur média
class MediaPlayerCall {
 public:
  explicit MediaPlayerCall(MediaPlayerDevice *parent) : parent_(parent) {}

  MediaPlayerCall &set_command(MediaPlayerCommand command);
  MediaPlayerCall &set_command(optional<MediaPlayerCommand> command);
  MediaPlayerCall &set_command(const std::string &command);
  
  MediaPlayerCall &set_media_url(const std::string &url);
  MediaPlayerCall &set_volume(float volume);
  MediaPlayerCall &set_announcement(bool announce);
  
  void perform();

  const optional<MediaPlayerCommand> &get_command() const { return command_; }
  const optional<std::string> &get_media_url() const { return media_url_; }
  const optional<float> &get_volume() const { return volume_; }
  const optional<bool> &get_announcement() const { return announcement_; }

 protected:
  void validate_();
  MediaPlayerDevice *const parent_;
  optional<MediaPlayerCommand> command_;
  optional<std::string> media_url_;
  optional<float> volume_;
  optional<bool> announcement_;
};

}  // namespace media_player
}  // namespace esphome
