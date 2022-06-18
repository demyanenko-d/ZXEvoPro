#pragma once
#include <stdint.h>

constexpr auto SI5351A_I2C_ADDR = 0x60;
constexpr uint8_t WM8960_I2C_ADDR = 0x1A;
constexpr uint8_t GPIO0_I2C_ADDR = 0x20;
constexpr uint8_t GPIO1_I2C_ADDR = 0x21;

extern bool si5131_presend;
extern bool wm8969_presend;
extern bool gpio0_presend;
extern bool gpio1_presend;

void i2c_init();
