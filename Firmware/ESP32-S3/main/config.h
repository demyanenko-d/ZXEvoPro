#pragma once

#define SOFTRST_IO                  0
#define I2C_MASTER_SDA_IO           1           /*!< GPIO number used for I2C master data  */
#define I2C_MASTER_SCL_IO           2           /*!< GPIO number used for I2C master clock */
#define CPLD_SSEL_IO                3

// ps2
#define PS2_KEYB_DAT_IO             4
#define PS2_MOUS_DAT_IO             5
#define PS2_KEYB_CLK_IO             6
#define PS2_MOUS_CLK_IO             7

#define STATUS_IO                   8
#define CONFDONE_IO                 9

// slave spi
#define SPI_SLAVE_SSEL_IO           10
#define SPI_SLAVE_MISO_IO           11
#define SPI_SLAVE_SCLK_IO           12
#define SPI_SLAVE_MOSI_IO           13

#define CONFIG_IO                   14
#define DCLK_IO                     17
#define DATA0_IO                    18
#define TAPE_BIT_IO                 21

// master spi
#define SPI_MASTER_MISO_IO          35
#define SPI_MASTER_SCLK_IO          36
#define SPI_MASTER_MOSI_IO          37
#define SPI_MASTER_SSEL_IO          38

// i2s
#define I2S_BCLK_IO                 39
#define I2S_DACLRC_IO               40
#define I2S_DACDAT_IO               41
#define I2S_ADCDAT_IO               42

#define ESP_IO45                    45
#define ESP_IO46                    46

#define ATX_POWERON_IO              47
#define FPGA_IRQ_IO                 48


#define SI5351A_I2C_ADDR            0x60
#define WM8960_I2C_ADDR             0x1A
#define GPIO0_I2C_ADDR              0x20
#define GPIO1_I2C_ADDR              0x21



