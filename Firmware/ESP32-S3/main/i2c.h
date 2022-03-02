
#include <stdio.h>
#include "esp_log.h"
#include "driver/i2c.h"
#include "drivers/Si5351A-RevB-Registers.h"


#define SI5351A_I2C_ADDR    (0x60)

#define I2C_MASTER_SCL_IO           2           /*!< GPIO number used for I2C master clock */
#define I2C_MASTER_SDA_IO           1           /*!< GPIO number used for I2C master data  */
#define I2C_MASTER_NUM              0           /*!< I2C master i2c port number, the number of i2c peripheral interfaces available will depend on the chip */
#define I2C_MASTER_FREQ_HZ          100000      /*!< I2C master clock frequency */
#define I2C_MASTER_TX_BUF_DISABLE   0           /*!< I2C master doesn't need buffer */
#define I2C_MASTER_RX_BUF_DISABLE   0           /*!< I2C master doesn't need buffer */
#define I2C_MASTER_TIMEOUT_MS       1000

#ifdef __cplusplus
extern "C"
{
#endif

esp_err_t i2c_master_init();
esp_err_t i2c_wr_si5351a(si5351a_revb_register_t data);
esp_err_t i2c_rd_si5351a(uint16_t addr, uint8_t *val);

#ifdef __cplusplus
}
#endif
