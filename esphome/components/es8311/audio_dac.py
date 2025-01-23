from esphome.components import i2c
from esphome.components.audio_dac import AudioDac
import esphome.config_validation as cv
from esphome.const import CONF_BITS_PER_SAMPLE, CONF_ID, CONF_MIC_GAIN, CONF_SAMPLE_RATE

CODEOWNERS = ["@kyoukorr]
DEPENDENCIES = ["i2c"]

es8311_ns = cg.esphome_ns.namespace("es8311")
ES8311 = es8311_ns.class_("ES8311", AudioDac, cg.Component, i2c.I2CDevice)

CONF_USE_MCLK = "use_mclk"
CONF_USE_MICROPHONE = "use_microphone"
