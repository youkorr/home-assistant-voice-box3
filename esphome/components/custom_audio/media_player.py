import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import media_player, i2s_audio
from esphome.const import CONF_ID, CONF_I2S_AUDIO_ID

DEPENDENCIES = ["i2s_audio"]  # Ajout de la dépendance I2S
AUTO_LOAD = ["media_player"]

custom_audio_ns = cg.esphome_ns.namespace("custom_audio")
CustomAudioMediaPlayer = custom_audio_ns.class_(
    "CustomAudioMediaPlayer", 
    media_player.MediaPlayer, 
    cg.Component,
    i2s_audio.I2SAudioOutput
)

CONFIG_SCHEMA = media_player.MEDIA_PLAYER_SCHEMA.extend({
    cv.GenerateID(): cv.declare_id(CustomAudioMediaPlayer),
    cv.Required(CONF_I2S_AUDIO_ID): cv.use_id(i2s_audio.I2SAudioOutput),
}).extend(cv.COMPONENT_SCHEMA)

def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    yield cg.register_component(var, config)
    yield media_player.register_media_player(var, config)
    
    # Lier la configuration I2S
    i2s = yield cg.get_variable(config[CONF_I2S_AUDIO_ID])
    cg.add(var.set_i2s_output(i2s))
