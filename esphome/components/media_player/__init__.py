# Ajoutez dans la partie validation
CONFIG_SCHEMA = cv.All(
    media_player.MEDIA_PLAYER_SCHEMA.extend({
        cv.GenerateID(): cv.declare_id(I2SAudioMediaPlayer),
        cv.Optional("i2s_audio_id"): cv.use_id(i2s_audio.I2SAudioComponent),
    }).extend(cv.COMPONENT_SCHEMA),
    cv.has_at_least_one_key(),
)

# Modifiez la fonction to_code
@coroutine_with_priority(100.0)
async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
    await media_player.register_media_player(var, config)
    
    # Récupération automatique si ID non spécifié
    if "i2s_audio_id" in config:
        i2s_audio_var = await cg.get_variable(config["i2s_audio_id"])
    else:
        # Utilise le premier composant I2S trouvé
        i2s_audio_var = await cg.get_variable(i2s_audio.I2SAudioComponent)
    
    cg.add(var.set_i2s_audio(i2s_audio_var))
