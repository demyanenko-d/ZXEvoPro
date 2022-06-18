#include <Arduino.h>
#include "app_hal/io.hpp"
#include "app_hal/i2c.hpp"
#include "app_hal/spi.hpp"
#include "app_hal/hid.hpp"
#include "app_hal/eeprom.hpp"
#include "test_service/messages.hpp"

void setup()
{
  Serial.begin(115200);
  delay(250);

  Serial.println("\nReady");

  io_init();
  i2c_init();
  hid_init();
  eeprom_init();

  load_fpga_firmware(test_service_firmware, sizeof(test_service_firmware));

  delay(20);

  cpld_config cfg = {
      .sd0_master = cpld_master_t::esp32,
      .sd1_master = cpld_master_t::esp32,
      .esp32_slave = cpld_slave_t::esp32_fpga,
  };

  spi_master_init();
  spi_set_cpld_config(cfg);

  spi_tns_set_video_mode(true, 0);
}

bool init_fpga = true;
uint8_t mode = 0;

#include "test_service/screen.hpp"
#include "test_service/messages.hpp"
#include <vector>
#include <functional>

const std::vector<std::function<void()>> hndl_menu_main{
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
};

const tns::MENU_DESC menu_main = {
    6,
    3,
    26,
    8,
    nullptr,
    1000,
    hndl_menu_main,
    tns::str_menu_main,
};

void loop()
{
  tns::scr_backgnd();
  tns::scr_menu(menu_main);

  delay(100);
}