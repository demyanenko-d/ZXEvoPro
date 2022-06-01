#include "hal.hpp"
#include "tools.hpp"

DeviceStatus wm8960_state{};
DeviceStatus si5351_state{};
DeviceStatus gpio_state[4]{};

SpiMasterMode spi_master_mode = SpiMasterMode::Unconfig;

spi_device_handle_t master_spi = nullptr;
spi_device_handle_t slave_spi = nullptr;

// ********************************************************************************************
// I2C
// ********************************************************************************************

static esp_err_t i2c_test_dev(uint8_t addr)
{
    uint8_t buffer[I2C_LINK_RECOMMENDED_SIZE(1)] = {0};

    auto cmd = i2c_cmd_link_create_static(buffer, sizeof(buffer));
    auto err = i2c_master_start(cmd);

    if (err == ESP_OK)
        err = i2c_master_write_byte(cmd, addr << 1 | I2C_MASTER_WRITE, true);

    if (err == ESP_OK)
    {
        i2c_master_stop(cmd);
        err = i2c_master_cmd_begin(0, cmd, 1000 / portTICK_RATE_MS);
    }

    i2c_cmd_link_delete_static(cmd);
    return err;
}

static esp_err_t i2c_wr_a8d8(uint8_t dev_addr, uint8_t addr, uint8_t data)
{
    esp_err_t err;
    uint8_t buffer[I2C_LINK_RECOMMENDED_SIZE(1)] = {0};

    i2c_cmd_handle_t cmd = i2c_cmd_link_create_static(buffer, sizeof(buffer));

    if ((err = i2c_master_start(cmd)) != ESP_OK)
        goto exit;

    // dev addr
    if ((err = i2c_master_write_byte(cmd, (dev_addr << 1) | I2C_MASTER_WRITE, I2C_MASTER_ACK) != ESP_OK))
        goto exit;

    if ((err = i2c_master_write_byte(cmd, (addr)&0xFF, I2C_MASTER_ACK)) != ESP_OK)
        goto exit;

    // reg data, nask
    if ((err = i2c_master_write_byte(cmd, (data), I2C_MASTER_ACK)) != ESP_OK)
        goto exit;

    if ((err = i2c_master_stop(cmd)) != ESP_OK)
        goto exit;

    err = i2c_master_cmd_begin(I2C_MASTER_NUM, cmd, I2C_MASTER_TIMEOUT_MS / portTICK_PERIOD_MS);

exit:

    i2c_cmd_link_delete_static(cmd);
    return err;
}

static esp_err_t i2c_rd_a8d8(uint8_t dev_addr, uint8_t addr, uint8_t &val)
{
    esp_err_t err;
    uint8_t buffer[I2C_LINK_RECOMMENDED_SIZE(2)] = {0};

    i2c_cmd_handle_t cmd = i2c_cmd_link_create_static(buffer, sizeof(buffer));

    if ((err = i2c_master_start(cmd)) != ESP_OK)
        goto exit;

    // dev addr
    if ((err = i2c_master_write_byte(cmd, (dev_addr << 1) | I2C_MASTER_WRITE, I2C_MASTER_ACK)) != ESP_OK)
        goto exit;

    if ((err = i2c_master_write_byte(cmd, (addr)&0xFF, I2C_MASTER_ACK)) != ESP_OK)
        goto exit;

    // split transaction
    if ((err = i2c_master_start(cmd)) != ESP_OK)
        goto exit;

    // dev addr
    if ((err = i2c_master_write_byte(cmd, (dev_addr << 1) | I2C_MASTER_READ, I2C_MASTER_ACK)) != ESP_OK)
        goto exit;

    if ((err = i2c_master_read_byte(cmd, &val, I2C_MASTER_NACK)) != ESP_OK)
        goto exit;

    if ((err = i2c_master_stop(cmd)) != ESP_OK)
        goto exit;

    err = i2c_master_cmd_begin(I2C_MASTER_NUM, cmd, I2C_MASTER_TIMEOUT_MS / portTICK_PERIOD_MS);

exit:

    i2c_cmd_link_delete_static(cmd);
    return err;
}

static esp_err_t i2c_wr_si5351a(si5351a_revb_register_t data)
{
    return i2c_wr_a8d8(SI5351A_I2C_ADDR, data.address, data.value);
}

static esp_err_t i2c_rd_si5351a(uint8_t addr, uint8_t &val)
{
    return i2c_rd_a8d8(SI5351A_I2C_ADDR, addr, val);
}

// ********************************************************************************************
// SPI
// ********************************************************************************************

static esp_err_t init_config_spi()
{
    esp_err_t ret = ESP_OK;

    spi_bus_config_t buscfg = {
        .mosi_io_num = DATA0_IO,
        .miso_io_num = -1,
        .sclk_io_num = DCLK_IO,
        .quadwp_io_num = -1,
        .quadhd_io_num = -1,
        .data4_io_num = -1,
        .data5_io_num = -1,
        .data6_io_num = -1,
        .data7_io_num = -1,
        .max_transfer_sz = 16,
        .flags = 0,
        .intr_flags = 0};

    spi_device_interface_config_t devcfg = {
        .command_bits = 0,
        .address_bits = 0,
        .dummy_bits = 0,
        .mode = 0, // SPI mode 0
        .duty_cycle_pos = 0,
        .cs_ena_pretrans = 0,
        .cs_ena_posttrans = 0,
        .clock_speed_hz = 3 * 1000 * 1000, // Clock out at 3MHz
        .input_delay_ns = 0,
        .spics_io_num = -1, // CS pin
        .flags = 0,
        .queue_size = 7, // We want to be able to queue 7 transactions at a time
        .pre_cb = nullptr,
        .post_cb = nullptr
    };

    ret = spi_bus_initialize(SPI2_HOST, &buscfg, SPI_DMA_CH_AUTO);
    ESP_ERROR_CHECK(ret);

    ret = spi_bus_add_device(SPI2_HOST, &devcfg, &master_spi);
    ESP_ERROR_CHECK(ret);

    if (ret == ESP_OK)
        spi_master_mode = SpiMasterMode::ConfigFpga;

    return ret;
}

static esp_err_t init_slave_spi() {
    esp_err_t err = ESP_OK;

    return err;
}

static void init_spi()
{
    esp_err_t err = ESP_OK;

    err = init_config_spi();
    esp_pass_fail("Config SPI: init  ", err);

    err = init_slave_spi();
    esp_pass_fail("Slave SPI: init  ", err);
}

static void init_gpio()
{
    esp_err_t res = ESP_OK;
}

static void init_wm8960()
{
    esp_err_t err = ESP_OK;
    wm8960_state = DeviceStatus::Unconfig;

    err = i2c_test_dev(WM8960_I2C_ADDR);
    esp_pass_fail("I2C: Audio code      ", err);
}

static void init_si5451()
{
    esp_err_t err = ESP_OK;
    si5351_state = DeviceStatus::Unconfig;

    err = i2c_test_dev(SI5351A_I2C_ADDR);
    esp_pass_fail("I2C: Clock gen       ", err);

    if (err == ESP_OK)
    {
        printf("init si5351a\n\r");

        uint8_t status_reg = 0;
        do
        {
            i2c_rd_si5351a(0, status_reg);
        } while (status_reg >> 7 == 1);

        printf("si5351a ready\n\r");

        for (int i = 0; i < SI5351A_REVB_REG_CONFIG_NUM_REGS; i++)
            i2c_wr_si5351a(si5351a_revb_registers[i]);

        printf("si5351a init done\n\r");

        si5351_state = DeviceStatus::Ready;
    }
    else
        si5351_state = DeviceStatus::NotPresend;
}

static void init_gpio_extender()
{
    esp_err_t err = ESP_OK;
    gpio_state[0] = DeviceStatus::Unconfig;
    gpio_state[1] = DeviceStatus::Unconfig;
    gpio_state[2] = DeviceStatus::Unconfig;
    gpio_state[3] = DeviceStatus::Unconfig;

    err = i2c_test_dev(GPIO0_I2C_ADDR);
    esp_pass_fail("I2C: GPIO Expander 0 ", err);

    err = i2c_test_dev(GPIO1_I2C_ADDR);
    esp_pass_fail("I2C: GPIO Expander 1 ", err);

    err = i2c_test_dev(GPIO2_I2C_ADDR);
    esp_pass_fail("I2C: GPIO Expander 2 ", err);

    err = i2c_test_dev(GPIO3_I2C_ADDR);
    esp_pass_fail("I2C: GPIO Expander 3 ", err);
}

static void init_i2c()
{
    esp_err_t err = ESP_OK;

    i2c_config_t conf =
        {
            .mode = I2C_MODE_MASTER,
            .sda_io_num = I2C_SDA_IO,
            .scl_io_num = I2C_SCL_IO,
            .sda_pullup_en = GPIO_PULLUP_ENABLE,
            .scl_pullup_en = GPIO_PULLUP_ENABLE,
            .master = {.clk_speed = I2C_MASTER_FREQ_HZ},
            .clk_flags = I2C_SCLK_SRC_FLAG_FOR_NOMAL};

    i2c_param_config(I2C_MASTER_NUM, &conf);
    err = i2c_driver_install(I2C_MASTER_NUM, conf.mode, I2C_MASTER_RX_BUF_DISABLE, I2C_MASTER_TX_BUF_DISABLE, 0);
    esp_pass_fail("I2C: init            ", err);

    init_wm8960();
    init_si5451();
    init_gpio_extender();
}

void init_all_devs()
{
    init_gpio();
    init_i2c();
    init_spi();
}
