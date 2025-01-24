import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.core import coroutine_with_priority
from esphome.const import CONF_ID, CONF_SAMPLE_RATE, CONF_BITS_PER_SAMPLE

# Déclaration des propriétés du composant
CODEOWNERS = ["@youkorr"]
IS_PLATFORM_COMPONENT = True
DEPENDENCIES = ["i2s_audio"]  # Dépendances nécessaires

# Déclaration de l'espace de noms et de la classe principale
audio_dac_ns = cg.esphome_ns.namespace("audio_dac")
AudioDac = audio_dac_ns.class_("AudioDac", cg.Component)

# Schéma de configuration principal pour le composant
CONFIG_SCHEMA = cv.Schema({
    cv.GenerateID(): cv.declare_id(AudioDac),
    cv.Required(CONF_SAMPLE_RATE): cv.int_range(min=8000, max=48000),
    cv.Required(CONF_BITS_PER_SAMPLE): cv.one_of(16, 24, 32, int=True),
}).extend(cv.COMPONENT_SCHEMA)

# Fonction principale pour générer le code
@coroutine_with_priority(100.0)
async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)

    # Configuration du DAC ES8311
    cg.add(var.set_sample_rate(config[CONF_SAMPLE_RATE]))
    cg.add(var.set_bits_per_sample(config[CONF_BITS_PER_SAMPLE]))

    cg.add_define("USE_AUDIO_DAC")
    cg.add_global(audio_dac_ns.using)
