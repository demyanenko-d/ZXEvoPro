#include "io.hpp"
#include <Arduino.h>

void io_init()
{
    pinMode(SOFTRST_IO, OUTPUT);
    pinMode(CPLD_SSEL_IO, OUTPUT);

    pinMode(I2C_SDA_IO, OUTPUT_OPEN_DRAIN);
    pinMode(I2C_SCL_IO, OUTPUT_OPEN_DRAIN);

    pinMode(PS2_KEYB_CLK_IO, OUTPUT_OPEN_DRAIN);
    pinMode(PS2_KEYB_DAT_IO, OUTPUT_OPEN_DRAIN);
    pinMode(PS2_MOUS_CLK_IO, OUTPUT_OPEN_DRAIN);
    pinMode(PS2_MOUS_DAT_IO, OUTPUT_OPEN_DRAIN);

    pinMode(STATUS_IO, INPUT);
    pinMode(CONFDONE_IO, INPUT);
    pinMode(CONFIG_IO, OUTPUT);
    pinMode(TAPE_BIT_IO, INPUT);

    pinMode(ATX_POWERON_IO, OUTPUT);
    pinMode(FPGA_IRQ_IO, INPUT);
    pinMode(ESP_IO45, INPUT);
    pinMode(ESP_IO46, INPUT);

    pinMode(DATA0_IO, OUTPUT);
    pinMode(DCLK_IO, OUTPUT);
    pinMode(SPI_MASTER_SSEL_IO, OUTPUT);

    pinMode(SPI_SLAVE_MOSI_IO, INPUT);
    pinMode(SPI_SLAVE_SSEL_IO, INPUT);
    pinMode(SPI_SLAVE_SCLK_IO, INPUT);

    io_reset_pins();
}

void io_reset_pins()
{
    digitalWrite(SOFTRST_IO, HIGH);
    digitalWrite(CPLD_SSEL_IO, HIGH);

    digitalWrite(I2C_SCL_IO, HIGH);
    digitalWrite(I2C_SDA_IO, HIGH);

    digitalWrite(PS2_KEYB_CLK_IO, HIGH);
    digitalWrite(PS2_KEYB_DAT_IO, HIGH);
    digitalWrite(PS2_MOUS_CLK_IO, HIGH);
    digitalWrite(PS2_MOUS_DAT_IO, HIGH);

    digitalWrite(CONFIG_IO, LOW);
    digitalWrite(ATX_POWERON_IO, HIGH);
}