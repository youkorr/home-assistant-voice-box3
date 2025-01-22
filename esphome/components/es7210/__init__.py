import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import i2c, microphone
from esphome.const import CONF_ID

DEPENDENCIES = ["i2c"]
AUTO_LOAD = ["microphone"]

# Déclarer le composant et ses fichiers source
es7210_ns = cg.esphome_ns.namespace("es7210")
ES7210 = es7210_ns.class_("ES7210", i2c.I2CDevice, cg.Component)

# Configuration du schéma YAML
CONFIG_SCHEMA = cv.Schema({
    cv.GenerateID(): cv.declare_id(ES7210),
}).extend(i2c.i2c_device_schema(0x40)).extend(cv.COMPONENT_SCHEMA)

def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    
    # Ajouter explicitement les fichiers source C++
    cg.add_library("espressif/esp32-camera", None)  # Exemple de dépendance externe (si nécessaire)
    cg.add_source("es7210.cpp")                    # Fichier principal du composant
    cg.add_source("es7210_idf.cpp")                # Fichier ESP-IDF personnalisé
    
    # Enregistrer le composant et le périphérique I2C
    yield cg.register_component(var, config)
    yield i2c.register_i2c_device(var, config)
