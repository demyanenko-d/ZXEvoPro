#pragma once
#include <stdint.h>
#include <esp_log.h>
#include <driver/i2c.h>
#include <driver/gpio.h>
#include <driver/spi_master.h>

typedef struct
{
	unsigned int address; /* 16-bit register address */
	unsigned char value; /* 8-bit register data */

} si5351a_revb_register_t;

enum class SpiMasterMode {
    Unconfig,
    ConfigFpga,
    MasterDevice
};

enum class DeviceStatus {
    Unconfig,
    Ready,
    NotPresend
};

extern DeviceStatus wm8960_state;
extern DeviceStatus si5351_state;
extern DeviceStatus gpio_state[4];
extern SpiMasterMode spi_master_mode;

extern spi_device_handle_t master_spi;
extern spi_device_handle_t slave_spi;

__attribute__((used)) static const uint8_t firmware[] = {
#include "../../../FPGA/main.ttf"
};

constexpr auto SI5351A_REVB_REG_CONFIG_NUM_REGS = 61;

si5351a_revb_register_t const si5351a_revb_registers[SI5351A_REVB_REG_CONFIG_NUM_REGS] =
{
	{ 0x0002, 0x53 },
	{ 0x0003, 0x00 },
	{ 0x0004, 0x20 },
	{ 0x0007, 0x00 },
	{ 0x000F, 0x00 },
	{ 0x0010, 0x0F },
	{ 0x0011, 0x0F },
	{ 0x0012, 0x0F },
	{ 0x0013, 0x8C },
	{ 0x0014, 0x8C },
	{ 0x0015, 0x8C },
	{ 0x0016, 0x8C },
	{ 0x0017, 0x8C },
	{ 0x001A, 0x00 },
	{ 0x001B, 0x19 },
	{ 0x001C, 0x00 },
	{ 0x001D, 0x0F },
	{ 0x001E, 0xEB },
	{ 0x001F, 0x00 },
	{ 0x0020, 0x00 },
	{ 0x0021, 0x0D },
	{ 0x002A, 0x00 },
	{ 0x002B, 0x01 },
	{ 0x002C, 0x00 },
	{ 0x002D, 0x0E },
	{ 0x002E, 0x00 },
	{ 0x002F, 0x00 },
	{ 0x0030, 0x00 },
	{ 0x0031, 0x00 },
	{ 0x0032, 0x00 },
	{ 0x0033, 0x0C },
	{ 0x0034, 0x00 },
	{ 0x0035, 0x22 },
	{ 0x0036, 0x75 },
	{ 0x0037, 0x00 },
	{ 0x0038, 0x00 },
	{ 0x0039, 0x04 },
	{ 0x003A, 0x00 },
	{ 0x003B, 0x01 },
	{ 0x003C, 0x00 },
	{ 0x003D, 0xFE },
	{ 0x003E, 0x00 },
	{ 0x003F, 0x00 },
	{ 0x0040, 0x00 },
	{ 0x0041, 0x00 },
	{ 0x005A, 0x00 },
	{ 0x005B, 0x00 },
	{ 0x0095, 0x00 },
	{ 0x0096, 0x00 },
	{ 0x0097, 0x00 },
	{ 0x0098, 0x00 },
	{ 0x0099, 0x00 },
	{ 0x009A, 0x00 },
	{ 0x009B, 0x00 },
	{ 0x00A2, 0x00 },
	{ 0x00A3, 0x00 },
	{ 0x00A4, 0x00 },
	{ 0x00A5, 0x00 },
	{ 0x00A6, 0x00 },
	{ 0x00A7, 0x00 },
	{ 0x00B7, 0x92 },
};

/* 
 * Design
 * ======
 * I2C Address: 0x60
 * 
 * Inputs:
 *     IN0: 25 MHz
 * 
 * Outputs:
 *    OUT0: 28 MHz
 *          Enabled LVCMOS 8 mA
 *          Offset 0.000 s 
 *    OUT1: 12.288 MHz
 *          Enabled LVCMOS 8 mA
 *          Offset 0.000 s 
 *    OUT2: 1.75 MHz
 *          Enabled LVCMOS 8 mA
 *          Offset 0.000 s 
 * 
 * Frequency Plan
 * ==============
 * PLL_A:
 *    Enabled Features = None
 *    Fvco             = 896 MHz
 *    M                = 35.84
 *    Input0:
 *       Source           = Crystal
 *       Source Frequency = 25 MHz
 *       Fpfd             = 25 MHz
 *       Load Capacitance = Load_08pF
 *    Output0:
 *       Features       = None
 *       Disabled State = StopLow
 *       R              = 1  (2^0)
 *       Fout           = 28 MHz
 *       N              = 32
 *    Output1:
 *       Features       = None
 *       Disabled State = StopLow
 *       R              = 1  (2^0)
 *       Fout           = 12.288 MHz
 *       N              = 72.9166666666666666... [ 72 + 11/12 ]
 *    Output2:
 *       Features       = None
 *       Disabled State = StopLow
 *       R              = 1  (2^0)
 *       Fout           = 1.75 MHz
 *       N              = 512
 */

constexpr auto SOFTRST_IO = GPIO_NUM_0;
constexpr auto I2C_SDA_IO = GPIO_NUM_1;
constexpr auto I2C_SCL_IO = GPIO_NUM_2;
constexpr auto CPLD_SSEL_IO = GPIO_NUM_3;

// ps2
constexpr auto PS2_KEYB_DAT_IO = GPIO_NUM_4;
constexpr auto PS2_MOUS_DAT_IO = GPIO_NUM_5;
constexpr auto PS2_KEYB_CLK_IO = GPIO_NUM_6;
constexpr auto PS2_MOUS_CLK_IO = GPIO_NUM_7;

constexpr auto STATUS_IO = GPIO_NUM_8;
constexpr auto CONFDONE_IO = GPIO_NUM_9;

// slave spi
constexpr auto SPI_SLAVE_SSEL_IO = GPIO_NUM_10;
constexpr auto SPI_SLAVE_MISO_IO = GPIO_NUM_11;
constexpr auto SPI_SLAVE_SCLK_IO = GPIO_NUM_12;
constexpr auto SPI_SLAVE_MOSI_IO = GPIO_NUM_13;

constexpr auto CONFIG_IO = GPIO_NUM_14;
constexpr auto DCLK_IO = GPIO_NUM_17;
constexpr auto DATA0_IO = GPIO_NUM_18;
constexpr auto TAPE_BIT_IO = GPIO_NUM_21;

// master spi
constexpr auto SPI_MASTER_MISO_IO = GPIO_NUM_35;
constexpr auto SPI_MASTER_SCLK_IO = GPIO_NUM_36;
constexpr auto SPI_MASTER_MOSI_IO = GPIO_NUM_37;
constexpr auto SPI_MASTER_SSEL_IO = GPIO_NUM_38;

// i2s
constexpr auto I2S_BCLK_IO = GPIO_NUM_39;
constexpr auto I2S_DACLRC_IO = GPIO_NUM_40;
constexpr auto I2S_DACDAT_IO = GPIO_NUM_41;
constexpr auto I2S_ADCDAT_IO = GPIO_NUM_42;

constexpr auto ESP_IO45 = GPIO_NUM_45;
constexpr auto ESP_IO46 = GPIO_NUM_46;

constexpr auto ATX_POWERON_IO = GPIO_NUM_47;
constexpr auto FPGA_IRQ_IO = GPIO_NUM_48;

constexpr uint8_t SI5351A_I2C_ADDR = 0x60;
constexpr uint8_t WM8960_I2C_ADDR = 0x1A;
constexpr uint8_t GPIO0_I2C_ADDR = 0x20;
constexpr uint8_t GPIO1_I2C_ADDR = 0x21;
constexpr uint8_t GPIO2_I2C_ADDR = 0x22;
constexpr uint8_t GPIO3_I2C_ADDR = 0x23;

constexpr auto I2C_MASTER_NUM = 0;
constexpr auto I2C_MASTER_FREQ_HZ = 100000;
constexpr auto I2C_MASTER_TX_BUF_DISABLE = 0;
constexpr auto I2C_MASTER_RX_BUF_DISABLE = 0;
constexpr auto I2C_MASTER_TIMEOUT_MS = 1000;

esp_err_t spi_master_init();
esp_err_t spi_master_free();

esp_err_t spi_slave_init();
esp_err_t spi_slave_free();

esp_err_t spi_get_master(spi_device_handle_t &spi);

void init_all_devs();