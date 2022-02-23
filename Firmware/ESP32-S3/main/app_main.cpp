
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

extern "C" void app_main()
{
  printf("ZX-Revo v.0.0\n\r");
}
