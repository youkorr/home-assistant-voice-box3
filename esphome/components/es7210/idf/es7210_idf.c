#include "es7210_idf.h"

void es7210_init(i2c_port_t i2c_port) {
    // Initialisation du ES7210
    es7210_write_reg(i2c_port, 0x00, 0x01);  // Exemple : Configurer le registre 0x00
    es7210_write_reg(i2c_port, 0x01, 0x80);  // Exemple : Configurer le registre 0x01
}

void es7210_write_reg(i2c_port_t i2c_port, uint8_t reg, uint8_t value) {
    uint8_t data[2] = {reg, value};
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, (ES7210_I2C_ADDR << 1) | I2C_MASTER_WRITE, true);
    i2c_master_write(cmd, data, 2, true);
    i2c_master_stop(cmd);
    i2c_master_cmd_begin(i2c_port, cmd, 1000 / portTICK_PERIOD_MS);
    i2c_cmd_link_delete(cmd);
}
