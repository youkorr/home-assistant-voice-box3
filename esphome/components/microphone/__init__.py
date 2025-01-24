import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.core import coroutine_with_priority

# Déclaration des propriétés du composant
CODEOWNERS = ["@youkorr"]
IS_PLATFORM_COMPONENT = True

# Déclaration de l'espace de noms et de la classe principale
microphone_ns = cg.esphome_ns.namespace("microphone")
Microphone = microphone_ns.class_("Microphone")

# Schéma de configuration principal pour le composant
MICROPHONE_SCHEMA = cv.Schema({})

# Fonction principale pour générer le code
@coroutine_with_priority(100.0)
async def to_code(config):
    cg.add_global(microphone_ns.using)
    cg.add_define("USE_MICROPHONE")
