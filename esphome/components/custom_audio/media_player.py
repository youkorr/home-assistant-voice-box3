import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import media_player, i2s_audio
from esphome.const import CONF_ID, CONF_I2S_AUDIO_ID

DEPENDENCIES = ['i2s_audio']
AUTO_LOAD = ['media_player']

# Namespace DOIT correspondre au nom du composant
custom_audio_ns = cg.esphome_ns.namespace('custom_audio')

CustomAudioMediaPlayer = custom_audio_ns.class_(
    'CustomAudioMediaPlayer',
    media_player.MediaPlayer,
    cg.Component,
    i2s_audio.I2SAudioOutput
)

CONFIG_SCHEMA = cv.platform_schema(
    media_player.MEDIA_PLAYER_PLATFORM_SCHEMA.extend({
        cv.GenerateID(): cv.declare_id(CustomAudioMediaPlayer),
        cv.Required(CONF_I2S_AUDIO_ID): cv.use_id(i2s_audio.I2SAudioOutput)
    }).extend(cv.COMPONENT_SCHEMA)
)

async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
    await media_player.register_media_player(var, config)
    
    i2s = await cg.get_variable(config[CONF_I2S_AUDIO_ID])
    cg.add(var.set_i2s_output(i2s))
