#pragma once
#include <stdint.h>

namespace tns
{
    constexpr uint8_t GO_READKEY = 0;
    constexpr uint8_t GO_REPEAT = 1;
    constexpr uint8_t GO_REDRAW = 2;
    constexpr uint8_t GO_RESTART = 3;
    constexpr uint8_t GO_CONTINUE = 4;
    constexpr uint8_t GO_ERROR = 5;
    constexpr uint8_t GO_EXIT = 6;

    constexpr uint8_t ENABLE_DIRECTUART = 0b00000001;
    constexpr uint8_t ENABLE_UART = 0b00000010;
    constexpr uint8_t ENABLE_SCR = 0b00000100;
    constexpr uint8_t ENABLE_SD_LOG = 0b00001000;
    constexpr uint8_t BIT_ENABLE_SD_LOG = 3;
    constexpr uint8_t RTSCTS_FLOWCTRL = 0b00010000;

    // .0 - put_char вызывает directuart_putchar
    // .1 - put_char вызывает uart_putchar
    // .2 - put_char вызывает scr_putchar
    // .3 - лог обмена SD в RS-232
    // .4 - RS-232 RTS/CTS flow control
    extern uint8_t flags1;

    extern uint8_t lang;

    void put_char(uint8_t ch);
    void print_hexhalf(uint8_t b);
    void print_hexbyte(uint8_t b);
    void print_hexbyte_for_dump(uint8_t b);
    void print_hexlong(uint32_t l);
    void put_char_for_dump(uint8_t ch);
    void print_dec99(uint8_t b);
    void print_dec16(uint16_t w);
    void print_msg(const uint8_t *msg);
    void print_mlmsg(const uint8_t *mlmsg[]);
    void print_short_vers(void);
}
