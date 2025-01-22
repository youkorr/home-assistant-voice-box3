#include "es7210_idf.h"
#include "driver/i2c.h"

#define ES7210_I2C_ADDR 0x40  // Adresse I2C du ES7210

void es7210_init(i2c_port_t i2c_port) {
    // Configuration des registres du ES7210 (exemple)
    uint8_t reg_data[2] = {0x00, 0x01};  // Registre 0x00, Valeur 0x01
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, (ES7210_I2C_ADDR << 1) | I2C_MASTER_WRITE, true);
    i2c_master_write(cmd, reg_data, 2, true);
    i2c_master_stop(cmd);
    i2c_master_cmd_begin(i2c_port, cmd, 1000 / portTICK_PERIOD_MS);
    i2c_cmd_link_delete(cmd);
}
