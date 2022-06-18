#pragma once
#include <stdint.h>
#include "libs/PS2Kbd.h"

constexpr uint8_t KEY_ESC = 0x76;
constexpr uint8_t KEY_ENTER = 0x5A;
constexpr uint8_t KEY_UP = 0x75;
constexpr uint8_t KEY_DOWN = 0x72;
constexpr uint8_t KEY_LEFT = 0x6B;
constexpr uint8_t KEY_RIGHT = 0x74;
constexpr uint8_t KEY_PAGEUP = 0x7D;
constexpr uint8_t KEY_PAGEDOWN = 0x7A;
constexpr uint8_t KEY_HOME = 0x6C;
constexpr uint8_t KEY_END = 0x69;
constexpr uint8_t KEY_SPACE = 0x29;
constexpr uint8_t KEY_F1 = 0x05;
constexpr uint8_t KEY_NUMLOCK = 0x77;
constexpr uint8_t KEY_CAPSLOCK = 0x58;
constexpr uint8_t KEY_SCROLLLOCK = 0x7E;
constexpr uint8_t KEY_Y = 0x35;

extern PS2Kbd *ps2_keyboard;

void hid_init();

// keyboard
void waitkey();
bool inkey(uint8_t &key);
