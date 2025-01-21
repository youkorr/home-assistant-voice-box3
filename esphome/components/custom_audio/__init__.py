import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import media_player
from esphome.const import CONF_ID

DEPENDENCIES = []
AUTO_LOAD = ["media_player"]

custom_audio_ns = cg.esphome_ns.namespace("custom_audio")
CustomAudioMediaPlayer = custom_audio_ns.class_("CustomAudioMediaPlayer", media_player.MediaPlayer, cg.Component)

CONFIG_SCHEMA = media_player.MEDIA_PLAYER_SCHEMA.extend({
    cv.GenerateID(): cv.declare_id(CustomAudioMediaPlayer),
}).extend(cv.COMPONENT_SCHEMA)

def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    yield cg.register_component(var, config)
    yield media_player.register_media_player(var, config)
