
#include <stdint.h>
#include <string.h>
#include <math.h>
#include "esp_log.h"
#include "esp_err.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/spi_slave_hd.h"
#include "driver/gpio.h"
#include "soc/rtc_cntl_reg.h"
#include "i2c.h"

typedef int8_t s8;
typedef int16_t s16;
typedef int32_t s32;
typedef int64_t s64;
typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

#define max(x, y) (((x) > (y)) ? (x) : (y))
#define min(x, y) (((x) < (y)) ? (x) : (y))

// Pin setting
#define GPIO_MOSI     11
#define GPIO_MISO     13
#define GPIO_SCLK     12
#define GPIO_CS       15
#define GPIO_TEST     18

#define SLAVE_HOST    SPI2_HOST
#define DMA_CHAN      SPI_DMA_CH_AUTO
#define QUEUE_SIZE    4

#define DMA_BUF_SIZE  32768

typedef union {
    struct {
        uint32_t byte_num:    8,
                 ack_en:      1,
                 ack_exp:     1,
                 ack_val:     1,
                 op_code:     3,
                 reserved14: 17,
                 done:        1;
    };
    uint32_t val;
} i2c_hw_cmd_t; 

typedef struct {
    i2c_hw_cmd_t hw_cmd;
    union {
        uint8_t* data;      // When total_bytes > 1
        uint8_t data_byte;  //when total_byte == 1
    };
    size_t bytes_used;
    size_t total_bytes;
} i2c_cmd_t;

typedef struct i2c_cmd_link {
    i2c_cmd_t cmd;              /*!< command in current cmd link */
    struct i2c_cmd_link *next;  /*!< next cmd link */
} i2c_cmd_link_t;

typedef struct {
    i2c_cmd_link_t *head;     /*!< head of the command link */
    i2c_cmd_link_t *cur;      /*!< last node of the command link */
    i2c_cmd_link_t *free;     /*!< the first node to free of the command link */

    void     *free_buffer;    /*!< pointer to the next free data in user's buffer */
    uint32_t  free_size;      /*!< remaining size of the user's buffer */
} i2c_cmd_desc_t;

#define I2C_TRANS_BUF_MINIMUM_SIZE     (sizeof(i2c_cmd_desc_t) + \
                                        sizeof(i2c_cmd_link_t) * 8) /* It is required to have allocate one i2c_cmd_desc_t per command:
                                                                     * start + write (device address) + write buffer +
                                                                     * start + write (device address) + read buffer + read buffer for NACK +
                                                                     * stop */
extern "C" void app_main()
{
  printf("ZX-Revo v.0.0\n\r");
  
  esp_err_t err = ESP_OK;
  uint8_t buffer[I2C_TRANS_BUF_MINIMUM_SIZE] = { 0 };

  err = i2c_master_init();
  printf("I2C init: %X\r\n", err);
  
  u8 buf = 0;
  
  for (int a = 0; a < 128; a++)
  {
    i2c_cmd_handle_t handle = i2c_cmd_link_create_static(buffer, sizeof(buffer));
    assert (handle != NULL);
  
    printf("Addr: %02X  ", a);
    
    err = i2c_master_start(handle);
    if (err != ESP_OK) goto exit;
    
    err = i2c_master_write_byte(handle, a << 1 | I2C_MASTER_WRITE, true);
    if (err != ESP_OK) goto exit;
    if (err != ESP_OK) goto exit;
    
    // err = i2c_master_write(handle, &buf, 1, true);
    // if (err != ESP_OK) goto exit;
    
    i2c_master_stop(handle);
    err = i2c_master_cmd_begin(0, handle, 1000 / portTICK_RATE_MS);
    
  exit:
    i2c_cmd_link_delete_static(handle);
    printf("Error: %X  ", (u16)err);
    
    printf("\r\n");
  }
}
