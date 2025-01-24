import esphome.codegen as cg
import esphome.config_validation as cv
from esphome import automation
from esphome.automation import maybe_simple_id
from esphome.core import coroutine_with_priority
from esphome.const import CONF_ID, CONF_VOLUME, CONF_SAMPLE_RATE, CONF_BITS_PER_SAMPLE

# Déclaration des propriétés du composant
CODEOWNERS = ["@youkorr"]
IS_PLATFORM_COMPONENT = True
DEPENDENCIES = ["i2s_audio"]  # Dépendances nécessaires

# Déclaration de l'espace de noms et de la classe principale
audio_dac_ns = cg.esphome_ns.namespace("audio_dac")
AudioDac = audio_dac_ns.class_("AudioDac", cg.Component)

# Déclaration des actions automatisées
MuteOffAction = audio_dac_ns.class_("MuteOffAction", automation.Action)
MuteOnAction = audio_dac_ns.class_("MuteOnAction", automation.Action)
SetVolumeAction = audio_dac_ns.class_("SetVolumeAction", automation.Action)

# Schéma pour les actions de mute
MUTE_ACTION_SCHEMA = maybe_simple_id(
    {
        cv.GenerateID(): cv.use_id(AudioDac),
    }
)

# Schéma pour l'action de réglage du volume
SET_VOLUME_ACTION_SCHEMA = cv.maybe_simple_value(
    {
        cv.GenerateID(): cv.use_id(AudioDac),
        cv.Required(CONF_VOLUME): cv.templatable(cv.percentage),
    },
    key=CONF_VOLUME,
)

# Schéma de configuration principal pour le composant
CONFIG_SCHEMA = cv.Schema({
    cv.GenerateID(): cv.declare_id(AudioDac),
    cv.Required(CONF_SAMPLE_RATE): cv.int_range(min=8000, max=48000),
    cv.Required(CONF_BITS_PER_SAMPLE): cv.one_of(16, 24, 32, int=True),
}).extend(cv.COMPONENT_SCHEMA)

# Enregistrement des actions automatisées
@automation.register_action("audio_dac.mute_off", MuteOffAction, MUTE_ACTION_SCHEMA)
@automation.register_action("audio_dac.mute_on", MuteOnAction, MUTE_ACTION_SCHEMA)
async def audio_dac_mute_action_to_code(config, action_id, template_arg, args):
    paren = await cg.get_variable(config[CONF_ID])
    return cg.new_Pvariable(action_id, template_arg, paren)

@automation.register_action(
    "audio_dac.set_volume", SetVolumeAction, SET_VOLUME_ACTION_SCHEMA
)
async def audio_dac_set_volume_to_code(config, action_id, template_arg, args):
    paren = await cg.get_variable(config[CONF_ID])
    var = cg.new_Pvariable(action_id, template_arg, paren)

    template_ = await cg.templatable(config.get(CONF_VOLUME), args, float)
    cg.add(var.set_volume(template_))

    return var

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
