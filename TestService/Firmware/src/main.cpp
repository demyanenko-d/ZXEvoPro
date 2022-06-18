#include <Arduino.h>
#include "app_hal/io.hpp"
#include "app_hal/i2c.hpp"
#include "app_hal/spi.hpp"


void setup() {
  Serial.begin(115200);
  Serial.println("\nReady");

  io_init();
  i2c_init();

}

bool init_fpga = true;
uint8_t mode = 0;

void loop() {
  delay(1000);
  Serial.print("1\n");

  if (init_fpga) {
    init_fpga = false;
    load_fpga_firmware(test_service_firmware, sizeof(test_service_firmware));

    delay(100);

    cpld_config cfg = {
      .sd0_master = cpld_master_t::esp32,
      .sd1_master = cpld_master_t::esp32,
      .esp32_slave = cpld_slave_t::esp32_fpga,
    };

    spi_master_init();
    spi_set_cpld_config(cfg);
  }

  spi_tns_set_video_mode(true, mode++);
}