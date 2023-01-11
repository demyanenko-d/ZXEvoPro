#include "spi.hpp"
#include "io.hpp"

SPIClass SlaveSPI(HSPI);
CPLDClass CPLD;

void load_fpga_firmware(const uint8_t *buffer, uint32_t size)
{
    SPI.end();
    SPI.begin(DCLK_IO, -1, DATA0_IO, -1);

    digitalWrite(CONFIG_IO, LOW);
    delay(5);
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
        // delay(10);
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

    digitalWrite(SOFTRST_IO, LOW);
    Serial.printf("end fpga config \n");

    delay(1);
    digitalWrite(SOFTRST_IO, HIGH);
}

void CPLDClass::enter_video_mode()
{
    if (cpld_video_mode)
        return;

    // подключить fpga в качестве slave
    auto config_data = create_config_data(MasterSelect::Esp32, MasterSelect::Esp32, SlaveSelect::ESP32_FPGA, false);
    write_cpld_config(config_data);

    // отключить синхронизацию в fpga
    spi_tns_transfer(TNSCommand::SCR_MODE, 0x40);

    // включить video_mode
    config_data = create_config_data(MasterSelect::Esp32, MasterSelect::Esp32, SlaveSelect::ESP32_FPGA, true);
    write_cpld_config(config_data);
    cpld_video_mode = true;

    // отключение spi
    spi_master_done();
    spi_slave_done();
}

void CPLDClass::exit_video_mode()
{
    if (!cpld_video_mode)
        return;

    // по posedge cpld_ssel происходит выход из video_mode
    set_cpld_ssel_mode(Mode::Config);
    set_cpld_ssel_mode(Mode::Transfer);
    cpld_video_mode = false;

    // инициализацця SPI
    spi_master_init();
    spi_slave_init();

    // подключить fpga в качестве slave
    auto config_data = create_config_data(MasterSelect::Esp32, MasterSelect::Esp32, SlaveSelect::ESP32_FPGA, false);
    write_cpld_config(config_data);

    // включить синхронизацию в fpga
    spi_tns_transfer(TNSCommand::SCR_MODE, tns_video_mode & (~0x40));

    // востановить конфигурацию cpld
    update();
}

bool CPLDClass::is_video_mode()
{
    return cpld_video_mode;
}

CPLDClass &CPLDClass::select_sd0_master(MasterSelect master)
{
    sd0_master = master;
    return *this;
}

CPLDClass &CPLDClass::select_sd1_master(MasterSelect master)
{
    sd1_master = master;
    return *this;
}

CPLDClass &CPLDClass::select_esp32_slave(SlaveSelect slave)
{
    esp32_slave = slave;
    return *this;
}

CPLDClass &CPLDClass::set_tns_video_mode(bool vga, uint8_t mode)
{
    tns_video_mode = mode & 7;
    if (vga)
        tns_video_mode |= 0x80;

    return *this;
}

uint8_t CPLDClass::create_config_data(MasterSelect sd0, MasterSelect sd1, SlaveSelect slave, bool video_mode) const
{
    uint8_t config_data = 0;

    if (sd0_master == MasterSelect::FPGA)
        config_data |= 1 << 0;

    if (sd1_master == MasterSelect::FPGA)
        config_data |= 1 << 1;

    switch (esp32_slave)
    {
    case SlaveSelect::SD0:
        config_data |= 0 << 4;
        break;

    case SlaveSelect::SD1:
        config_data |= 1 << 4;
        break;

    case SlaveSelect::ESP32_FPGA:
        config_data |= 2 << 4;
        break;

    default:
        config_data |= 3 << 4;
        break;
    }

    if (video_mode)
        config_data |= 0x80;

    return config_data;
}

void CPLDClass::write_cpld_config(uint8_t config)
{
    if (config == cpld_config_value)
        return;

    set_cpld_ssel_mode(Mode::Config);
    spi_tns_clr_ssel();

    SPI.transfer(config);

    spi_tns_set_ssel();
    set_cpld_ssel_mode(Mode::Transfer);

    cpld_config_value = config;
}

bool CPLDClass::update()
{
    if (cpld_video_mode)
        return false;

    auto config_data = create_config_data(sd0_master, sd1_master, esp32_slave, false);

    if (cpld_config_value != config_data)
        write_cpld_config(config_data);

    return true;
}

void CPLDClass::update_tns_video_mode()
{
    spi_tns_transfer(TNSCommand::SCR_MODE, tns_video_mode);
}

void CPLDClass::set_cpld_ssel_mode(Mode mode)
{
    switch (mode)
    {
    case Mode::Config:
        digitalWrite(CPLD_SSEL_IO, HIGH);
        break;
    case Mode::Transfer:
        digitalWrite(CPLD_SSEL_IO, LOW);
        break;
    }
}

void CPLDClass::spi_master_init()
{
    SPI.setFrequency(20000000);
    SPI.setHwCs(false);
    SPI.setBitOrder(LSBFIRST);

    digitalWrite(SPI_MASTER_SSEL_IO, HIGH);

    SPI.end();
    SPI.begin(
        SPI_MASTER_SCLK_IO,
        SPI_MASTER_MISO_IO,
        SPI_MASTER_MOSI_IO,
        -1);
}

void CPLDClass::spi_slave_init()
{
    SlaveSPI.end();
    SlaveSPI.begin(DCLK_IO, -1, DATA0_IO, -1);
}

void CPLDClass::spi_master_done()
{
    SPI.end();
}

void CPLDClass::spi_slave_done()
{
    SlaveSPI.end();
}

void CPLDClass::spi_tns_set_ssel()
{
    gpio_set_level(SPI_MASTER_SSEL_IO, 1);
}

void CPLDClass::spi_tns_clr_ssel()
{
    gpio_set_level(SPI_MASTER_SSEL_IO, 0);
}

void CPLDClass::spi_tns_sel_reg(TNSCommand number)
{
    spi_tns_set_ssel();
    SPI.transfer((uint8_t)number);
}

uint8_t CPLDClass::spi_tns_same_reg(uint8_t data)
{
    spi_tns_clr_ssel();
    auto res = SPI.transfer(data);
    spi_tns_set_ssel();
    return res;
}

uint8_t CPLDClass::spi_tns_transfer(TNSCommand number, uint8_t data)
{
    spi_tns_sel_reg(number);
    auto res = spi_tns_same_reg(data);
    Serial.printf("spi addr: %i val: %i res: %i\n", (int)number, (int)data, (int)res);
    return res;
}
