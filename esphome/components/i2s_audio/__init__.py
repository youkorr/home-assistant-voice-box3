from esphome import pins
import esphome.codegen as cg
from esphome.components.esp32 import get_esp32_variant
from esphome.components.esp32.const import (
    VARIANT_ESP32,
    VARIANT_ESP32C3,
    VARIANT_ESP32S2,
    VARIANT_ESP32S3,
)
import esphome.config_validation as cv
from esphome.const import CONF_BITS_PER_SAMPLE, CONF_CHANNEL, CONF_ID, CONF_SAMPLE_RATE
from esphome.cpp_generator import MockObjClass
import esphome.final_validate as fv
from esphome.components import microphone  # Importation du module microphone

# Ajout de l'importation du module microphone
from . import microphone

CODEOWNERS = ["@youkorr"]
DEPENDENCIES = ["esp32"]
MULTI_CONF = True

CONF_I2S_DOUT_PIN = "i2s_dout_pin"
CONF_I2S_DIN_PIN = "i2s_din_pin"
CONF_I2S_MCLK_PIN = "i2s_mclk_pin"
CONF_I2S_BCLK_PIN = "i2s_bclk_pin"
CONF_I2S_LRCLK_PIN = "i2s_lrclk_pin"

CONF_I2S_AUDIO = "i2s_audio"
CONF_I2S_AUDIO_ID = "i2s_audio_id"

CONF_I2S_MODE = "i2s_mode"
CONF_PRIMARY = "primary"
CONF_SECONDARY = "secondary"

CONF_USE_APLL = "use_apll"
CONF_BITS_PER_CHANNEL = "bits_per_channel"
CONF_MONO = "mono"
CONF_LEFT = "left"
CONF_RIGHT = "right"
CONF_STEREO = "stereo"
CONF_NUM_CHANNELS = "num_channels"

# Déclaration de l'espace de noms et des classes
i2s_audio_ns = cg.esphome_ns.namespace("i2s_audio")
I2SAudioComponent = i2s_audio_ns.class_("I2SAudioComponent", cg.Component)
I2SAudioBase = i2s_audio_ns.class_("I2SAudioBase", cg.Parented.template(I2SAudioComponent))
I2SAudioIn = i2s_audio_ns.class_("I2SAudioIn", I2SAudioBase)
I2SAudioOut = i2s_audio_ns.class_("I2SAudioOut", I2SAudioBase)

# Ajout de la classe pour le microphone I2S
I2SAudioMicrophone = i2s_audio_ns.class_("I2SAudioMicrophone", microphone.Microphone)

# Schéma de configuration pour le microphone I2S
MICROPHONE_SCHEMA = microphone.MICROPHONE_SCHEMA.extend({
    cv.GenerateID(): cv.declare_id(I2SAudioMicrophone),
    cv.Required(CONF_I2S_DIN_PIN): pins.internal_gpio_input_pin_number,
    cv.Optional(CONF_SAMPLE_RATE, default=16000): cv.int_range(min=1),
    cv.Optional(CONF_BITS_PER_SAMPLE, default=16): cv.enum(I2S_BITS_PER_SAMPLE),
    cv.Optional(CONF_CHANNEL, default=CONF_MONO): cv.enum(I2S_CHANNELS),
    cv.Optional(CONF_USE_APLL, default=False): cv.boolean,
})

# Fonction pour générer le code du microphone I2S
async def register_i2s_audio_microphone(var, config):
    await cg.register_component(var, config)
    cg.add(var.set_din_pin(config[CONF_I2S_DIN_PIN]))
    cg.add(var.set_sample_rate(config[CONF_SAMPLE_RATE]))
    cg.add(var.set_bits_per_sample(config[CONF_BITS_PER_SAMPLE]))
    cg.add(var.set_channel(config[CONF_CHANNEL]))
    cg.add(var.set_use_apll(config[CONF_USE_APLL]))

# Ajout du microphone I2S au schéma global
CONFIG_SCHEMA = cv.Schema(
    {
        cv.GenerateID(): cv.declare_id(I2SAudioComponent),
        cv.Required(CONF_I2S_LRCLK_PIN): pins.internal_gpio_output_pin_number,
        cv.Optional(CONF_I2S_BCLK_PIN): pins.internal_gpio_output_pin_number,
        cv.Optional(CONF_I2S_MCLK_PIN): pins.internal_gpio_output_pin_number,
        cv.Optional(CONF_I2S_DOUT_PIN): pins.internal_gpio_output_pin_number,
        cv.Optional(CONF_I2S_DIN_PIN): pins.internal_gpio_input_pin_number,
    }
).extend(MICROPHONE_SCHEMA)

# Validation finale
def _final_validate(_):
    i2s_audio_configs = fv.full_config.get()[CONF_I2S_AUDIO]
    variant = get_esp32_variant()
    if variant not in I2S_PORTS:
        raise cv.Invalid(f"Unsupported variant {variant}")
    if len(i2s_audio_configs) > I2S_PORTS[variant]:
        raise cv.Invalid(
            f"Only {I2S_PORTS[variant]} I2S audio ports are supported on {variant}"
        )

FINAL_VALIDATE_SCHEMA = _final_validate

# Fonction principale pour générer le code
async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)

    cg.add(var.set_lrclk_pin(config[CONF_I2S_LRCLK_PIN]))
    if CONF_I2S_BCLK_PIN in config:
        cg.add(var.set_bclk_pin(config[CONF_I2S_BCLK_PIN]))
    if CONF_I2S_MCLK_PIN in config:
        cg.add(var.set_mclk_pin(config[CONF_I2S_MCLK_PIN]))
    if CONF_I2S_DOUT_PIN in config:
        cg.add(var.set_dout_pin(config[CONF_I2S_DOUT_PIN]))
    if CONF_I2S_DIN_PIN in config:
        await register_i2s_audio_microphone(var, config)
