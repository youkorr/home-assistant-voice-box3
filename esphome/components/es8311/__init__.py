import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import i2c, audio_dac
from esphome.const import CONF_ID

DEPENDENCIES = ["i2c"]
AUTO_LOAD = ["audio_dac"]

es8311_ns = cg.esphome_ns.namespace("es8311")
ES8311 = es8311_ns.class_("ES8311Component", audio_dac.AudioDAC, i2c.I2CDevice, cg.Component)

CONFIG_SCHEMA = audio_dac.AUDIO_DAC_SCHEMA.extend({
    cv.GenerateID(): cv.declare_id(ES8311),
}).extend(i2c.i2c_device_schema(0x18)).extend(cv.COMPONENT_SCHEMA)

def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    yield cg.register_component(var, config)
    yield i2c.register_i2c_device(var, config)
