#pragma once
#include "esphome.h"

class ES7210 : public Component {
public:
    void setup() override;
    void loop() override;

    void set_i2c_address(uint8_t address);
    void set_sample_rate(uint32_t sample_rate);
    void set_bits_per_sample(uint8_t bits_per_sample);

private:
    uint8_t i2c_address;
    uint32_t sample_rate;
    uint8_t bits_per_sample;
};
