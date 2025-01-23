import esphome.codegen as cg
import esphome.config_validation as cv

CODEOWNERS = ["@youkorr"]
audio_ns = cg.esphome_ns.namespace("audio")

CONFIG_SCHEMA = cv.All(
    cv.Schema({}),
)
