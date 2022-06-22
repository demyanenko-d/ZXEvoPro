#include <Arduino.h>
#include <Wire.h>
#include "app_hal/io.hpp"
#include "app_hal/i2c.hpp"
#include "app_hal/spi.hpp"
#include "app_hal/hid.hpp"
#include "app_hal/eeprom.hpp"
#include "test_service/messages.hpp"

#include "libs/PCF8574.h"

PCF8574 pcf8574(&Wire, GPIO0_I2C_ADDR);

uint8_t random8()
{
  static uint8_t val = 0;
  return val++;
}

void test_fpga_exchange(void)
{
  uint8_t i, curr, prev;
  curr = random8();
  spi_tns_transfer(tns_commands::SCR_SET_ATTR, curr);
  i = 0;
  do
  {
    prev = ~curr;
    curr = random8();
    if (spi_tns_transfer(tns_commands::SCR_SET_ATTR, curr) != prev)
    {
      Serial.printf("We have some errors!");
      while (1)
      {
        uint16_t j, errors;
        errors = 0;
        j = 50000;
        do
        {
          prev = ~curr;
          curr = random8();
          if (spi_tns_transfer(tns_commands::SCR_SET_ATTR, curr) != prev)
            errors++;
        } while (--j);
        Serial.printf("\r\nQuantity wrong byte from 50000 - %i", (int)errors);
      }
    }
  } while (--i);
}

void setup()
{
  Serial.begin(115200);
  delay(1000);

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

  pcf8574.pinMode(P6, OUTPUT);

  Serial.print("Init pcf8574...");
  if (pcf8574.begin())
  {
    Serial.println("OK");
  }
  else
  {
    Serial.println("KO");
  }

  test_fpga_exchange();

  uint8_t tmp = 0;

  tmp = spi_tns_transfer(tns_commands::INT_CONTROL, 0b00000000);
  Serial.printf("int control 1: %i\n", (int)tmp);
  tmp = spi_tns_transfer(tns_commands::MTST_CONTROL, 0b00000001);
  tmp = spi_tns_transfer(tns_commands::MTST_CONTROL, 0b00000000);
  tmp = spi_tns_transfer(tns_commands::MTST_CONTROL, 0b00000001);
  Serial.printf("mist control 1: %i\n", (int)tmp);
  tmp = spi_tns_transfer(tns_commands::INT_CONTROL, 0b00000000);
  Serial.printf("int control 2: %i\n", (int)tmp);
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

int val = 0;
int cnt = 0;
bool menu_done = false;
uint16_t mtst_pass, mtst_fail;

void loop()
{
  if (!menu_done)
  {
    tns::scr_backgnd();
    menu_done = true;
  }
  // tns::scr_menu(menu_main);

  pcf8574.digitalWrite(P6, val);
  val ^= 1;

  delay(300);

  if (cnt % 10 == 0)
  {
    mtst_pass = spi_tns_transfer(tns_commands::MTST_PASS_CNT0, 0xff);
    mtst_pass |= (spi_tns_transfer(tns_commands::MTST_PASS_CNT1, 0xff) << 8);
    mtst_fail = spi_tns_transfer(tns_commands::MTST_FAIL_CNT0, 0xff);
    mtst_fail |= (spi_tns_transfer(tns_commands::MTST_FAIL_CNT1, 0xff) << 8);

    Serial.printf("pass: %i fail: %i\n", (int)mtst_pass, (int)mtst_fail);
  }
  cnt++;
}