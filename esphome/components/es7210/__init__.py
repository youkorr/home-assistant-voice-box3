import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import i2c
from esphome.const import CONF_ID

DEPENDENCIES = ["i2c"]

es7210_ns = cg.esphome_ns.namespace("es7210")
ES7210 = es7210_ns.class_("ES7210", i2c.I2CDevice, cg.Component)

CONFIG_SCHEMA = cv.Schema({
    cv.GenerateID(): cv.declare_id(ES7210),
}).extend(i2c.i2c_device_schema(0x40)).extend(cv.COMPONENT_SCHEMA)

def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    
    # Ajouter les fichiers nécessaires
    cg.add_library("es7210", None)  # Facultatif si vous avez une bibliothèque externe
    cg.add_include("es7210/es7210_idf.h")
    cg.add_include("es7210/es7210.h")
    cg.add_source("es7210/es7210_idf.cpp")
    cg.add_source("es7210/es7210.cpp")

    # Enregistrer le composant et le périphérique I2C
    yield cg.register_component(var, config)
    yield i2c.register_i2c_device(var, config)

