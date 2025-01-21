#include "esphome.h"
#include "driver/i2c.h"

class ES7210 : public Component {
 public:
  void setup() override;
  void configure(uint8_t sample_rate, uint8_t gain);
  void start_recording();
  void stop_recording();

 private:
  uint8_t address = 0x40; // Adresse I2C de l'ES7210
  bool is_recording = false;

  void write_register(uint8_t reg, uint8_t value);
  uint8_t read_register(uint8_t reg);
};
