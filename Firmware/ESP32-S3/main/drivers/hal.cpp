#include "hal.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/i2c.h"
#include "config.h"

esp_err_t i2c_master_init()
{
  i2c_config_t conf =
      {
          .mode = I2C_MODE_MASTER,
          .sda_io_num = I2C_MASTER_SDA_IO,
          .scl_io_num = I2C_MASTER_SCL_IO,
          .sda_pullup_en = GPIO_PULLUP_ENABLE,
          .scl_pullup_en = GPIO_PULLUP_ENABLE,
          .master.clk_speed = I2C_MASTER_FREQ_HZ,
      };

  i2c_param_config(I2C_MASTER_NUM, &conf);

  return i2c_driver_install(I2C_MASTER_NUM, conf.mode, I2C_MASTER_RX_BUF_DISABLE, I2C_MASTER_TX_BUF_DISABLE, 0);
}

esp_err_t i2c_wr_si5351a(si5351a_revb_register_t data)
{
  esp_err_t err;
  uint8_t buffer[I2C_LINK_RECOMMENDED_SIZE(1)] = {0};

  i2c_cmd_handle_t cmd = i2c_cmd_link_create_static(buffer, sizeof(buffer));

  if (err = i2c_master_start(cmd) != ESP_OK)
    goto exit;

  // dev addr
  if (err = i2c_master_write_byte(cmd, (SI5351A_I2C_ADDR << 1) | I2C_MASTER_WRITE, I2C_MASTER_ACK) != ESP_OK)
    goto exit;

  // reg addr
  //if (err = i2c_master_write_byte(cmd, (data.address >> 8) & 0xFF, I2C_MASTER_ACK) != ESP_OK)
  //  goto exit;
  if (err = i2c_master_write_byte(cmd, (data.address) & 0xFF, I2C_MASTER_ACK) != ESP_OK)
    goto exit;

  // reg data, nask
  if (err = i2c_master_write_byte(cmd, (data.value), I2C_MASTER_ACK) != ESP_OK)
    goto exit;

  if (err = i2c_master_stop(cmd) != ESP_OK)
    goto exit;

  err = i2c_master_cmd_begin(I2C_MASTER_NUM, cmd, I2C_MASTER_TIMEOUT_MS / portTICK_PERIOD_MS);

exit:

  i2c_cmd_link_delete_static(cmd);
  return err;
}

esp_err_t i2c_rd_si5351a(uint16_t addr, uint8_t *val)
{

  esp_err_t err;
  uint8_t buffer[I2C_LINK_RECOMMENDED_SIZE(2)] = {0};

  i2c_cmd_handle_t cmd = i2c_cmd_link_create_static(buffer, sizeof(buffer));

  if (err = i2c_master_start(cmd) != ESP_OK)
    goto exit;

  // dev addr
  if (err = i2c_master_write_byte(cmd, (SI5351A_I2C_ADDR << 1) | I2C_MASTER_WRITE, I2C_MASTER_ACK) != ESP_OK)
    goto exit;

  // port addr
  //if (err = i2c_master_write_byte(cmd, (addr >> 8) & 0xFF, I2C_MASTER_ACK) != ESP_OK)
  //  goto exit;
  if (err = i2c_master_write_byte(cmd, (addr) & 0xFF, I2C_MASTER_ACK) != ESP_OK)
    goto exit;

  // split transaction
  if (err = i2c_master_start(cmd) != ESP_OK)
    goto exit;

  // dev addr
  if (err = i2c_master_write_byte(cmd, (SI5351A_I2C_ADDR << 1) | I2C_MASTER_READ, I2C_MASTER_ACK) != ESP_OK)
    goto exit;

  if (err = i2c_master_read_byte(cmd, val, I2C_MASTER_NACK) != ESP_OK)
    goto exit;

  if (err = i2c_master_stop(cmd) != ESP_OK)
    goto exit;

  err = i2c_master_cmd_begin(I2C_MASTER_NUM, cmd, I2C_MASTER_TIMEOUT_MS / portTICK_PERIOD_MS);

exit:

  i2c_cmd_link_delete_static(cmd);
  return err;
}