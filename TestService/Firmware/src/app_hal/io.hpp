#pragma once
#include <Arduino.h>

constexpr auto SOFTRST_IO = GPIO_NUM_0;
constexpr auto I2C_SDA_IO = 1;
constexpr auto I2C_SCL_IO = 2;
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

constexpr auto RGB_VIDEO_R1 = SPI_MASTER_SSEL_IO;
constexpr auto RGB_VIDEO_R0 = SPI_MASTER_SCLK_IO;
constexpr auto RGB_VIDEO_G1 = SPI_MASTER_MOSI_IO;
constexpr auto RGB_VIDEO_G0 = SPI_MASTER_MISO_IO;
constexpr auto RGB_VIDEO_B1 = SPI_SLAVE_SSEL_IO;
constexpr auto RGB_VIDEO_B0 = SPI_SLAVE_SCLK_IO;
constexpr auto RGB_VIDEO_VS = SPI_SLAVE_MISO_IO;
constexpr auto RGB_VIDEO_HS = SPI_SLAVE_MOSI_IO;

void io_init();
void io_reset_pins();