import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import i2c
from esphome.const import CONF_ID

DEPENDENCIES = ['i2c']
AUTO_LOAD = ['i2c']

es7210_ns = cg.esphome_ns.namespace('es7210')
ES7210 = es7210_ns.class_('ES7210', i2c.I2CDevice)

CONFIG_SCHEMA = cv.Schema({
    cv.GenerateID(): cv.declare_id(ES7210),
}).extend(i2c.i2c_device_schema(0x40))  # Ajustez l'adresse I2C selon votre ES7210

def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    yield i2c.register_i2c_device(var, config)
