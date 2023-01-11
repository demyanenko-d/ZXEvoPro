#pragma once
#include <Arduino.h>
#include <SPI.h>
#include <stdint.h>
#include <driver/spi_slave_hd.h>
#include "io.hpp"

__attribute__((used)) static const uint8_t test_service_firmware[] = {
#include "../../../FPGA/main.ttf"
};

/**
 * @brief загрузить прошивку FPGA
 * @param buffer ссылка на буфер битстрима
 * @param size длинна битстрима в байтах
 */
void load_fpga_firmware(const uint8_t *buffer, uint32_t size);

enum class TNSCommand
{
    TEMP_REG = 0xa0,

    SD_CS0 = 0xa1,
    SD_CS1 = 0xa2,
    FLASH_LOADDR = 0xa3,
    FLASH_MIDADDR = 0xa4,
    FLASH_HIADDR = 0xa5,
    FLASH_DATA = 0xa6,
    FLASH_CTRL = 0xa7,
    SCR_LOADDR = 0xa8,
    SCR_HIADDR = 0xa9,
    SCR_SET_ATTR = 0xaa, // запись в ATTR
    SCR_FILL = 0xab,     // прединкремент адреса и запись в ATTR и в память
                         // (если только дергать spics_n, то в память будет писаться предыдущее значение)
    SCR_CHAR = 0xac,     // прединкремент адреса и запись в память символов и ATTR в память атрибутов
                         // (если только дергать spics_n, то в память будет писаться предыдущие значения)
    SCR_MOUSE_X = 0xad,
    SCR_MOUSE_Y = 0xae,
    SCR_MODE = 0xaf, // [7] - 0=VGAmode, 1=TVmode;
                     // [6] 0-включить синхроимпульсы 1-заблокировать синхронимпульсы [2:0] - 0=TXT, иначе ScrTESTs

    MTST_CONTROL = 0x50, // [0] - тест памяти (0=сброс, 1=работа)
    MTST_PASS_CNT0 = 0x51,
    MTST_PASS_CNT1 = TEMP_REG,
    MTST_FAIL_CNT0 = 0x52,
    MTST_FAIL_CNT1 = TEMP_REG,

    COVOX = 0x53,

    INT_CONTROL = 0x54, // [0] - разрешение прерываний от covox-а (27343.75 Hz)
                        // [1] - разрешение кадровых прерываний (~49 Hz)

};

class CPLDClass
{
public:
    // Режим работы CPLD
    enum class Mode
    {
        Config,   // запись управляющего слова CPLD
        Transfer, // передача данных SPI
    };

    //  Выбор управляющего устроства для SPI каналов SD-Card
    enum class MasterSelect
    {
        Esp32, // Портом управляет ESP32
        FPGA,  // Портом управляет FPGA
    };

    // Выбор ведомого канала SPI для ESP32
    enum class SlaveSelect
    {
        SD0,        // SD-Card 0
        SD1,        // SD-Card 1
        ESP32_FPGA, // FPGA
        none,       // Нет подключения/резер
    };

    /**
     * @brief  Переключает CPLD в режим video_moe
     */
    void enter_video_mode();

    /**
     * @brief  Выводит CPLD из режима video_mode, переключает в режим мультиплексора
     */
    void exit_video_mode();

    /**
     * @brief  Возвращает режим работы CPLD
     * @return
     *     - true режим video_mode
     *     - false режим мультиплексора
     */
    bool is_video_mode();

    /**
     * @brief выбор мастера для SPI порта SD-Card 0
     * @param master выбор владельца, перечисление MasterSelect
     * @return ссылку ссылка на объект CPLDClass
     */
    CPLDClass &select_sd0_master(MasterSelect master);

    /**
     * @brief выбор мастера для SPI порта SD-Card 1
     * @param master выбор владельца, перечисление MasterSelect
     * @return ссылку ссылка на объект CPLDClass
     */
    CPLDClass &select_sd1_master(MasterSelect master);

    /**
     * @brief выбор SPI порта, куда будет подключе SPI Master
     * @param slave выбор порта
     * @return ссылку ссылка на объект CPLDClass
     */
    CPLDClass &select_esp32_slave(SlaveSelect slave);

    /**
     * @brief Выбор видео режима для Test&Service
     * @param vga - включить скандаблер
     * @param mode - видео режим (0=TXT, иначе ScrTESTs)
     * @return ссылку ссылка на объект CPLDClass
     */
    CPLDClass &set_tns_video_mode(bool vga, uint8_t mode);

    /**
     * @brief обновить состояние CPLD или видео режима Test&Service
     * @return bool ture-конфигурация CPLD была обновлена
     */
    bool update();

    void begin();

    void spi_tns_sel_reg(TNSCommand number);
    uint8_t spi_tns_same_reg(uint8_t data);
    uint8_t spi_tns_transfer(TNSCommand number, uint8_t data);

private:
    MasterSelect sd0_master = MasterSelect::Esp32;
    MasterSelect sd1_master = MasterSelect::Esp32;
    SlaveSelect esp32_slave = SlaveSelect::SD0;

    bool cpld_video_mode = false;
    uint8_t tns_video_mode = 0xff;
    uint8_t cpld_config_value = 0;

    /**
     * @brief  GPIO set output level
     *
     * @param  gpio_num GPIO number. If you want to set the output level of e.g. GPIO16, gpio_num should be GPIO_NUM_16 (16);
     * @param  level Output level. 0: low ; 1: high
     *
     * @return
     *     - ESP_OK Success
     *     - ESP_ERR_INVALID_ARG GPIO number error
     *
     */
    void update_tns_video_mode();

    void spi_tns_set_ssel();
    void spi_tns_clr_ssel();

    /**
     * @brief  GPIO set output level
     *
     * @param  gpio_num GPIO number. If you want to set the output level of e.g. GPIO16, gpio_num should be GPIO_NUM_16 (16);
     * @param  level Output level. 0: low ; 1: high
     *
     * @return
     *     - ESP_OK Success
     *     - ESP_ERR_INVALID_ARG GPIO number error
     *
     */
    void set_cpld_ssel_mode(Mode mode);

    /**
     * @brief  GPIO set output level
     *
     * @param  gpio_num GPIO number. If you want to set the output level of e.g. GPIO16, gpio_num should be GPIO_NUM_16 (16);
     * @param  level Output level. 0: low ; 1: high
     *
     * @return
     *     - ESP_OK Success
     *     - ESP_ERR_INVALID_ARG GPIO number error
     *
     */
    uint8_t create_config_data(MasterSelect sd0, MasterSelect sd1, SlaveSelect slave, bool video_mode) const;

    /**
     * @brief  GPIO set output level
     *
     * @param  gpio_num GPIO number. If you want to set the output level of e.g. GPIO16, gpio_num should be GPIO_NUM_16 (16);
     * @param  level Output level. 0: low ; 1: high
     *
     * @return
     *     - ESP_OK Success
     *     - ESP_ERR_INVALID_ARG GPIO number error
     *
     */
    void write_cpld_config(uint8_t config);

    /**
     * @brief  GPIO set output level
     *
     * @param  gpio_num GPIO number. If you want to set the output level of e.g. GPIO16, gpio_num should be GPIO_NUM_16 (16);
     * @param  level Output level. 0: low ; 1: high
     *
     * @return
     *     - ESP_OK Success
     *     - ESP_ERR_INVALID_ARG GPIO number error
     *
     */
    void spi_master_init();

    /**
     * @brief  GPIO set output level
     *
     * @param  gpio_num GPIO number. If you want to set the output level of e.g. GPIO16, gpio_num should be GPIO_NUM_16 (16);
     * @param  level Output level. 0: low ; 1: high
     *
     * @return
     *     - ESP_OK Success
     *     - ESP_ERR_INVALID_ARG GPIO number error
     *
     */
    void spi_slave_init();

    /**
     * @brief  GPIO set output level
     *
     * @param  gpio_num GPIO number. If you want to set the output level of e.g. GPIO16, gpio_num should be GPIO_NUM_16 (16);
     * @param  level Output level. 0: low ; 1: high
     *
     * @return
     *     - ESP_OK Success
     *     - ESP_ERR_INVALID_ARG GPIO number error
     *
     */
    void spi_master_done();

    /**
     * @brief  GPIO set output level
     *
     * @param  gpio_num GPIO number. If you want to set the output level of e.g. GPIO16, gpio_num should be GPIO_NUM_16 (16);
     * @param  level Output level. 0: low ; 1: high
     *
     * @return
     *     - ESP_OK Success
     *     - ESP_ERR_INVALID_ARG GPIO number error
     *
     */
    void spi_slave_done();
};

extern CPLDClass CPLD;
