import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.const import CONF_ID, CONF_NAME
from esphome.components import i2s_audio
from esphome.core import coroutine_with_priority

CODEOWNERS = ["@youkorr"]

IS_PLATFORM_COMPONENT = True

media_player_ns = cg.esphome_ns.namespace("media_player")
MediaPlayer = media_player_ns.class_("MediaPlayer", cg.Component)

# Configuration schema
CONFIG_SCHEMA = cv.Schema({
    cv.GenerateID(): cv.declare_id(MediaPlayer),
    cv.Required(CONF_NAME): cv.string,
    cv.Required("i2s_audio_id"): cv.use_id(i2s_audio.I2SAudio),
}).extend(cv.COMPONENT_SCHEMA)

# Function to generate the code
@coroutine_with_priority(100.0)
async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
    cg.add(var.set_name(config[CONF_NAME]))
    
    # Bind i2s_audio to media_player
    i2s_audio_var = await cg.get_variable(config["i2s_audio_id"])
    cg.add(var.set_i2s_audio(i2s_audio_var))
