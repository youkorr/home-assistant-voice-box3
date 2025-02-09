import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.const import CONF_ID, CONF_NAME
from esphome.components import i2s_audio
from esphome.components.media_player import MediaPlayerDevice  # Change this line

CODEOWNERS = ["@youkorr"]
DEPENDENCIES = ["i2s_audio"]
AUTO_LOAD = ["i2s_audio"]

i2s_audio_ns = cg.esphome_ns.namespace("i2s_audio")
I2SAudioMediaPlayer = i2s_audio_ns.class_(
    "I2SAudioMediaPlayer", MediaPlayerDevice, cg.Component  # Changed this line
)

CONFIG_SCHEMA = cv.All(
    MediaPlayerDevice.schema().extend({  # Changed this line
        cv.GenerateID(): cv.declare_id(I2SAudioMediaPlayer),
        cv.Required("dac_type"): cv.one_of("external", "internal", lower=True),
        cv.Optional("sample_rate", default=44100): cv.int_range(min=8000, max=192000),
        cv.Optional("mode", default="mono"): cv.one_of("mono", "stereo", lower=True),
    }).extend(cv.COMPONENT_SCHEMA),
    cv.only_with_arduino,
)

async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
    await MediaPlayerDevice.register_media_player(var, config)  # Changed this line
    cg.add(var.set_dac_type(config["dac_type"]))
    cg.add(var.set_sample_rate(config["sample_rate"]))
    cg.add(var.set_mode(0 if config["mode"] == "mono" else 1))
    
    # Configuration I2S
    if config["dac_type"] == "external":
        cg.add_library("esphome/ESP32-audioI2S", "2.0.7")
        cg.add_build_flag("-DUSE_I2S_EXTERNAL_DAC")
