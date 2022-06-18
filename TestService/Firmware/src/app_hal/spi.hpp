#pragma once
#include "stdint.h"
#include <driver/spi_slave_hd.h>

__attribute__((used)) static const uint8_t test_service_firmware[] = {
#include "../../../../TestService/FPGA/main.ttf"
};

enum class cpld_mode_t {
    config,
    transfer,
};

enum cpld_master_t {
    esp32,
    fpga,
};

enum cpld_slave_t {
    sd0,
    sd1,
    esp32_fpga,
    none,
};

struct cpld_config {
    cpld_master_t   sd0_master;
    cpld_master_t   sd1_master;
    cpld_slave_t    esp32_slave;
};

enum class tns_commands {
    TEMP_REG      = 0xa0,

    SD_CS0        = 0xa1,
    SD_CS1        = 0xa2,
    FLASH_LOADDR  = 0xa3,
    FLASH_MIDADDR = 0xa4,
    FLASH_HIADDR  = 0xa5,
    FLASH_DATA    = 0xa6,
    FLASH_CTRL    = 0xa7,
    SCR_LOADDR    = 0xa8,
    SCR_HIADDR    = 0xa9,
    SCR_SET_ATTR  = 0xaa, // запись в ATTR
    SCR_FILL      = 0xab, // прединкремент адреса и запись в ATTR и в память
                          // (если только дергать spics_n, то в память будет писаться предыдущее значение)
    SCR_CHAR      = 0xac, // прединкремент адреса и запись в память символов и ATTR в память атрибутов
                          // (если только дергать spics_n, то в память будет писаться предыдущие значения)
    SCR_MOUSE_X   = 0xad,
    SCR_MOUSE_Y   = 0xae,
    SCR_MODE      = 0xaf, // [7] - 0=VGAmode, 1=TVmode; [2:0] - 0=TXT, иначе ScrTESTs

    MTST_CONTROL  = 0x50, // [0] - тест памяти (0=сброс, 1=работа)
    MTST_PASS_CNT0= 0x51,
    MTST_PASS_CNT1= TEMP_REG,
    MTST_FAIL_CNT0= 0x52,
    MTST_FAIL_CNT1= TEMP_REG,

    COVOX         = 0x53,

    INT_CONTROL   = 0x54, // [0] - разрешение прерываний от covox-а (27343.75 Hz)
                          // [1] - разрешение кадровых прерываний (~49 Hz)

};

void load_fpga_firmware(const uint8_t *buffer, uint32_t size);

void spi_master_init();

void spi_set_cpld_ssel_mode(cpld_mode_t mode);
void spi_set_cpld_config(cpld_config &config);

uint8_t spi_tns_transfer(tns_commands number, uint8_t data);

void spi_tns_set_video_mode(bool vga, uint8_t mode);

