
#include <stdint.h>
#include <string.h>
#include <math.h>
#include "config.h"
#include "esp_log.h"
#include "esp_err.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/spi_slave_hd.h"
#include "driver/i2c.h"
#include "driver/gpio.h"
#include "driver/spi_master.h"
#include "soc/rtc_cntl_reg.h"
#include "drivers/hal.h"

#include "drivers\Si5351A-RevB-Registers.h"
//#include "drivers\si5351.h"

#define CFG_SPI SPI2_HOST

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

void spi_init()
{
    esp_err_t ret;
    spi_device_handle_t spi;

    spi_bus_config_t buscfg={
        .mosi_io_num = DATA0_IO,
        .miso_io_num = -1,
        .sclk_io_num = DCLK_IO,
        .quadwp_io_num = -1,
        .quadhd_io_num = -1,
        .max_transfer_sz = 32,
    };

    spi_device_interface_config_t dev_config = {
      .command_bits = 0,
      .address_bits = 0,
      .dummy_bits = 0,
      .mode = 0,
      .clock_speed_hz = 4*1000*1000,
      .spics_io_num = -1,
      .queue_size = 1
    };

    ret = spi_bus_initialize(CFG_SPI, &buscfg, SPI_DMA_DISABLED);
    ESP_ERROR_CHECK(ret);

    ret = spi_bus_add_device(CFG_SPI, &dev_config, &spi);
    ESP_ERROR_CHECK(ret);

    uint8_t txbuf[4] = {2,3,4,6};
 
    spi_transaction_t transact = {
      .length = 32,
      .tx_buffer = &txbuf,
    };
    
    int cnt = 20;

    while(cnt-- > 0) { 
      spi_device_transmit(spi, &transact);
      vTaskDelay(10 / portTICK_RATE_MS);
    }

    
}

extern "C" void app_main()
{
  printf("ZX-Revo v.0.0\n\r");
  
  init_i2c();
  configSi5351A();
  spi_init();
}
