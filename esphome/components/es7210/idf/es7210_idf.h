#ifndef ES7210_IDF_H
#define ES7210_IDF_H

#include "driver/i2c.h"

#define ES7210_I2C_ADDR 0x40  // Adresse I2C du ES7210

void es7210_init(i2c_port_t i2c_port);
void es7210_write_reg(i2c_port_t i2c_port, uint8_t reg, uint8_t value);

#endif // ES7210_IDF_H
