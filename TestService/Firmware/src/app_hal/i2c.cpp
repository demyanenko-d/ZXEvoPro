#include "i2c.hpp"
#include <Wire.h>
#include <cstring>

#include "io.hpp"
#include "libs/si5351.h"

constexpr uint32_t I2C_MASTER_FREQ_HZ = 10000;

bool si5131_presend = false;
bool wm8969_presend = false;
bool gpio0_presend = false;
bool gpio1_presend = false;

static void init_si5131();
static void init_wm8969();
static void init_gpio();

Si5351 si5351;

static bool detectDevice(uint8_t addr, const std::string &device)
{
    Wire.beginTransmission(addr);
    auto error = Wire.endTransmission();

    if (error == 0)
    {
        Serial.printf("%s found!\n", device.c_str());
        return true;
    }
    else
    {
        Serial.printf("#Error: %s not found!\n", device.c_str());
        return false;
    }
}

void i2c_init()
{

    Wire.begin(I2C_SDA_IO, I2C_SCL_IO, I2C_MASTER_FREQ_HZ);

    si5131_presend = detectDevice(SI5351A_I2C_ADDR, "SI5351A");
    wm8969_presend = detectDevice(WM8960_I2C_ADDR, "WM8960");
    gpio0_presend = detectDevice(GPIO0_I2C_ADDR, "GPIO0");
    gpio1_presend = detectDevice(GPIO1_I2C_ADDR, "GPIO1");

    if (si5131_presend)
    {
        init_si5131();

        if (wm8969_presend)
            init_wm8969();
    }

    init_gpio();
}

static void init_si5131()
{
    Serial.printf("init si5351a\n\r");

    auto i2c_found = si5351.init(SI5351_CRYSTAL_LOAD_8PF, 0, 0);

    if (!i2c_found)
    {
        Serial.println("Device not found on I2C bus!\n");
    } else {
        si5351.set_freq(2800000000ULL, SI5351_CLK0); // 28MHz
        si5351.set_freq(1228800000ULL, SI5351_CLK1); // 12.288 MHz
        si5351.set_freq( 175000000ULL, SI5351_CLK2); // 1.75 MHz
        si5351.update_status();
    }

    Serial.printf("si5351a init done\n");    
}

static void init_wm8969()
{
}

static void init_gpio()
{
}
