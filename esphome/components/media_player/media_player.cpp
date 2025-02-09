#include "media_player.h"
#include "esphome/core/log.h"
#include "esphome/components/media_player/media_player_device.h"  // Ajout de l'en-tête

namespace esphome {
namespace media_player {

static const char *const TAG = "media_player";

// Utilisation de MediaPlayerDevice au lieu de MediaPlayer
const char *media_player_state_to_string(MediaPlayerState state) {
    // ... reste du code inchangé ...
}

const char *media_player_command_to_string(MediaPlayerCommand command) {
    // ... reste du code inchangé ...
}

// Modification de la classe MediaPlayerCall pour utiliser MediaPlayerDevice
void MediaPlayerCall::validate_() {
    if (this->media_url_.has_value()) {
        if (this->command_.has_value()) {
            ESP_LOGW(TAG, "MediaPlayerCall: Setting both command and media_url is not needed.");
            this->command_.reset();
        }
    }
    if (this->volume_.has_value()) {
        if (this->volume_.value() < 0.0f || this->volume_.value() > 1.0f) {
            ESP_LOGW(TAG, "MediaPlayerCall: Volume must be between 0.0 and 1.0.");
            this->volume_.reset();
        }
    }
}

void MediaPlayerCall::perform() {
    ESP_LOGD(TAG, "'%s' - Setting", this->parent_->get_name().c_str());
    this->validate_();
    // ... reste du code inchangé ...
    this->parent_->control(*this);
}

// Les méthodes de MediaPlayerCall restent inchangées
MediaPlayerCall &MediaPlayerCall::set_command(MediaPlayerCommand command) {
    // ... reste du code inchangé ...
}

// Modification de la classe principale pour hériter de MediaPlayerDevice
class MediaPlayerDevice {
public:
    void add_on_state_callback(std::function<void()> &&callback) {
        this->state_callback_.add(std::move(callback));
    }

    void publish_state() { 
        this->state_callback_.call(); 
    }

protected:
    CallbackManager<void()> state_callback_;
};

}  // namespace media_player
}  // namespace esphome
