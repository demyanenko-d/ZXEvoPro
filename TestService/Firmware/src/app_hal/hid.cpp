#include "hid.hpp"
#include "io.hpp"

PS2Kbd *ps2_keyboard;

void hid_init()
{
    ps2_keyboard = new PS2Kbd(PS2_KEYB_DAT_IO, PS2_KEYB_CLK_IO);
    ps2_keyboard->begin();
}

void waitkey()
{
    while (!ps2_keyboard->available())
    {
        vPortYield();
    }
}

bool inkey(uint8_t &key)
{
    if (!ps2_keyboard->available())
        return false;

    key = ps2_keyboard->read();
    return true;
}
