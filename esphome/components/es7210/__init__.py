import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import i2c, microphone
from esphome.const import CONF_ID

DEPENDENCIES = ["i2c"]
AUTO_LOAD = ["microphone"]

# Déclarer les dépendances ESP-IDF si nécessaire
# (nécessaire si vous utilisez des API ESP-IDF natives)
# ARDUINO_VERSION_OVERRIDE = "espressif32@4.4.0"

es7210_ns = cg.esphome_ns.namespace("es7210")
ES7210 = es7210_ns.class_("ES7210", i2c.I2CDevice, cg.Component)

# Ajouter les fichiers source
CONFIG_SCHEMA = cv.Schema({
    cv.GenerateID(): cv.declare_id(ES7210),
}).extend(i2c.i2c_device_schema(0x40)).extend(cv.COMPONENT_SCHEMA)

def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    
    # Ajouter les fichiers sources ESP-IDF
    cg.add_library("driver/i2c.h", None)  # Inclure la bibliothèque I2C d'ESP-IDF
    cg.add_source("es7210_idf.cpp")  # Ajouter le fichier source ESP-IDF
    
    yield cg.register_component(var, config)
    yield i2c.register_i2c_device(var, config)
