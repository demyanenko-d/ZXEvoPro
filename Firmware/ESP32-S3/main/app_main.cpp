
#include <stdint.h>
#include <string.h>
#include <math.h>
#include "esp_log.h"
#include "esp_err.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/spi_slave_hd.h"
#include "driver/i2c.h"
#include "driver/gpio.h"
#include "soc/rtc_cntl_reg.h"
#include "i2c.h"

#include "drivers\Si5351A-RevB-Registers.h"
//#include "drivers\si5351.h"

void esp_pass_fail(const char* msg, esp_err_t err) {
  printf(msg);
  (err == ESP_OK) ? printf("[PASS]\r\n") : printf("[FAIL]\r\n");
}

esp_err_t test_i2c_dev(uint8_t addr) {

  auto cmd = i2c_cmd_link_create();
  auto err = i2c_master_start(cmd);
    
  if (err == ESP_OK)
    err = i2c_master_write_byte(cmd, addr << 1 | I2C_MASTER_WRITE, true);

  if (err == ESP_OK) {
    i2c_master_stop(cmd);
    err = i2c_master_cmd_begin(0, cmd, 1000 / portTICK_RATE_MS);
  }

  i2c_cmd_link_delete_static(cmd);
  return err;
}

void init_i2c() {
 
  esp_err_t err = ESP_OK;

  err = i2c_master_init();
  esp_pass_fail("I2C: init            ", err);

  err = test_i2c_dev(0x1A);
  esp_pass_fail("I2C: Audio code      ", err);

  err = test_i2c_dev(0x20);
  esp_pass_fail("I2C: GPIO Expander 0 ", err);

  err = test_i2c_dev(0x21);
  esp_pass_fail("I2C: GPIO Expander 1 ", err);

  err = test_i2c_dev(0x60);
  esp_pass_fail("I2C: Clock gen       ", err);
}

void configSi5351A() {
  printf("init si5351a\n\r");

  uint8_t status_reg = 0;
  do
  {
    i2c_rd_si5351a(0, &status_reg);
  } 
  while (status_reg >> 7 == 1);

  printf("si5351a ready\n\r");

  for(int i=0; i < SI5351A_REVB_REG_CONFIG_NUM_REGS; i++) 
    esp_pass_fail("si5351 : ", i2c_wr_si5351a(si5351a_revb_registers[i]));

  uint8_t res;

  for(int i=0; i < 70; i++) {
    esp_pass_fail("si5351 check : ", i2c_rd_si5351a(i, &res));
    printf("addr: %i, %i\n\r", (int)i, (int)res);
  }

  printf("si5351a init done\n\r");
}

extern "C" void app_main()
{
  printf("ZX-Revo v.0.0\n\r");
  
  init_i2c();
  configSi5351A();
}
