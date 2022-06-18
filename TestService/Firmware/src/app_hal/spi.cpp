#include "spi.hpp"
#include "io.hpp"
#include <Arduino.h>
#include <SPI.h>

SPIClass SlaveSPI(HSPI);

void load_fpga_firmware(const uint8_t *buffer, uint32_t size)
{
    SPI.end();
    SPI.begin(DCLK_IO, -1, DATA0_IO, -1);

    digitalWrite(CONFIG_IO, LOW);
    delay(50);
    digitalWrite(CONFIG_IO, HIGH);

    int cnt = 0;
    bool ready = false;

    Serial.printf("begin fpga config \n");

    while (cnt++ < 100)
    {
        ready = digitalRead(STATUS_IO) == HIGH;

        if (ready)
        {
            Serial.printf("fpga ready ....\n");
            break;
        }
        delay(10);
    }

    if (ready)
    {
        Serial.printf("begin fpga data...\n");
        auto total = size;
        auto offset = 0;

        while (digitalRead(CONFDONE_IO) != HIGH && digitalRead(STATUS_IO) != LOW)
        {
            auto rest = total % 64;
            offset += rest;

            SPI.beginTransaction(SPISettings(20000000, LSBFIRST, SPI_MODE2));
            SPI.writeBytes(buffer, size);
            SPI.endTransaction();
        }
    }

    SPI.transfer(0xff);
    SPI.end();

    Serial.printf("end fpga config \n");
}

void spi_master_init()
{
    SPI.setFrequency(7000000);
    SPI.setHwCs(false);

    digitalWrite(SPI_MASTER_SSEL_IO, HIGH);

    SPI.end();
    SPI.begin(
        SPI_MASTER_SCLK_IO,
        SPI_MASTER_MISO_IO,
        SPI_MASTER_MOSI_IO,
        -1);
}

void spi_slave_init()
{
    SlaveSPI.end();
    SlaveSPI.begin(DCLK_IO, -1, DATA0_IO, -1);
}

void spi_set_cpld_ssel_mode(cpld_mode_t mode)
{
    switch (mode)
    {
    case cpld_mode_t::config:
        digitalWrite(CPLD_SSEL_IO, HIGH);
        break;
    case cpld_mode_t::transfer:
        digitalWrite(CPLD_SSEL_IO, LOW);
        break;
    }
}

void spi_set_cpld_config(cpld_config &config)
{
    uint8_t config_data = 0;

    if (config.sd0_master == cpld_master_t::fpga)
        config_data |= 1 << 0;

    if (config.sd1_master == cpld_master_t::fpga)
        config_data |= 1 << 1;

    switch (config.esp32_slave)
    {
    case cpld_slave_t::sd0:
        config_data |= 0 << 4;
        break;

    case cpld_slave_t::sd1:
        config_data |= 1 << 4;
        break;

    case cpld_slave_t::esp32_fpga:
        config_data |= 2 << 4;
        break;

    default:
        config_data |= 3 << 4;
        break;
    }

    spi_set_cpld_ssel_mode(cpld_mode_t::config);
    SPI.transfer(config_data);
    spi_set_cpld_ssel_mode(cpld_mode_t::transfer);
}

uint8_t spi_tns_transfer(tns_commands number, uint8_t data)
{

    digitalWrite(SPI_MASTER_SSEL_IO, LOW);
    SPI.transfer((uint8_t)number);
    digitalWrite(SPI_MASTER_SSEL_IO, HIGH);
    SPI.transfer(data);
    digitalWrite(SPI_MASTER_SSEL_IO, LOW);

    return 0;
}

void spi_tns_set_video_mode(bool vga, uint8_t mode) {
    uint8_t data = 0;

    if (!vga) data|= 0x80;
    data |= mode & 7;

    spi_tns_transfer(tns_commands::SCR_MODE, data);
}

