#include <stdio.h>
#include "hal.hpp"
#include <driver/spi_master.h>

void init()
{
    init_all_devs();
}

void main_loop()
{
    printf("hello\n\r");

    esp_err_t ret;
    static spi_transaction_t transact{};

    transact.tx_data[0] = 1;
    transact.tx_data[1] = 5;
    transact.tx_data[2] = 7;
    transact.tx_data[3] = 44;

    transact.length = 32;
    transact.flags = SPI_TRANS_USE_TXDATA;

    while (true)
    {
        spi_device_transmit(master_spi, &transact);
        spi_device_transmit(master_spi, &transact);
        spi_device_transmit(master_spi, &transact);
        spi_device_transmit(master_spi, &transact);

        vTaskDelay(10 / portTICK_RATE_MS);
    }
}

extern "C"
{
    void app_main(void)
    {
        init();
        main_loop();
    }
}