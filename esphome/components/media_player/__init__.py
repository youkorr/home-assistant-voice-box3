import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.const import CONF_ID, CONF_NAME
from esphome.components import i2s_audio

CODEOWNERS = ["@youkorr"]
DEPENDENCIES = ["i2s_audio"]
AUTO_LOAD = ["i2s_audio"]

# Définir le namespace
i2s_audio_ns = cg.esphome_ns.namespace("i2s_audio")
media_player_ns = cg.esphome_ns.namespace('media_player')

# Créer la classe de base MediaPlayerDevice
MediaPlayerDevice = media_player_ns.class_('MediaPlayerDevice', cg.Component)

# Définir la classe I2SAudioMediaPlayer
I2SAudioMediaPlayer = i2s_audio_ns.class_(
    "I2SAudioMediaPlayer", MediaPlayerDevice
)

# Schéma de configuration
CONFIG_SCHEMA = cv.All(
    cv.Schema({
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
    
    cg.add(var.set_dac_type(config["dac_type"]))
    cg.add(var.set_sample_rate(config["sample_rate"]))
    cg.add(var.set_mode(0 if config["mode"] == "mono" else 1))
    
    # Configuration I2S
    if config["dac_type"] == "external":
        cg.add_library("esphome/ESP32-audioI2S", "2.0.7")
        cg.add_build_flag("-DUSE_I2S_EXTERNAL_DAC")
