import esphome.codegen as cg
import esphome.config_validation as cv
from esphome import automation
from esphome.automation import maybe_simple_id
from esphome.core import coroutine_with_priority
from esphome.const import CONF_ID, CONF_VOLUME

CODEOWNERS = ["@youkorr"]
IS_PLATFORM_COMPONENT = True

audio_dac_ns = cg.esphome_ns.namespace("audio_dac")
AudioDac = audio_dac_ns.class_("AudioDac")

MuteOffAction = audio_dac_ns.class_("MuteOffAction", automation.Action)
MuteOnAction = audio_dac_ns.class_("MuteOnAction", automation.Action)
SetVolumeAction = audio_dac_ns.class_("SetVolumeAction", automation.Action)

MUTE_ACTION_SCHEMA = maybe_simple_id(
    {
        cv.GenerateID(): cv.use_id(AudioDac),
    }
)

SET_VOLUME_ACTION_SCHEMA = cv.maybe_simple_value(
    {
        cv.GenerateID(): cv.use_id(AudioDac),
        cv.Required(CONF_VOLUME): cv.templatable(cv.percentage),
    },
    key=CONF_VOLUME,
)

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

# Utilisez un import différé pour éviter la dépendance circulaire
def register_speaker_class():
    from esphome.components import speaker
    i2s_audio_speaker_ns = cg.esphome_ns.namespace("i2s_audio_speaker")
    I2SAudioSpeaker = i2s_audio_speaker_ns.class_("I2SAudioSpeaker", speaker.Speaker, cg.Component)
    return I2SAudioSpeaker

CONFIG_SCHEMA = cv.Schema({
    cv.GenerateID(): cv.declare_id(register_speaker_class()),
    cv.Required("dout_pin"): cv.int_,
    cv.Optional("sample_rate", default=16000): cv.int_,
    cv.Optional("bits_per_sample", default=16): cv.int_,
    cv.Optional("dac_type", default="external"): cv.string,
}).extend(cv.COMPONENT_SCHEMA)

async def to_code_i2s_audio_speaker(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
    await speaker.register_speaker(var, config)

    cg.add(var.set_dout_pin(config["dout_pin"]))
    cg.add(var.set_sample_rate(config["sample_rate"]))
    cg.add(var.set_bits_per_sample(config["bits_per_sample"]))
    cg.add(var.set_dac_type(config["dac_type"]))

@coroutine_with_priority(100.0)
async def to_code(config):
    cg.add_define("USE_AUDIO_DAC")
    cg.add_global(audio_dac_ns.using)
    await to_code_i2s_audio_speaker(config)
