#include "output.hpp"
#include "screen.hpp"
#include <Arduino.h>

uint8_t tns::flags1 = 0;
uint8_t tns::lang = 0;

//-----------------------------------------------------------------------------

void tns::put_char(uint8_t ch)
{
    if (flags1 & ENABLE_DIRECTUART)
        //directuart_putchar(ch);
    if (flags1 & ENABLE_UART)
        Serial.write(ch);
    if (flags1 & ENABLE_SCR)
        scr_putchar(ch);
}

//-----------------------------------------------------------------------------

void tns::print_hexhalf(uint8_t b)
{
    b &= 0x0f;
    if (b > 9)
        b += 7;
    b += 0x30;
    put_char(b);
}

//-----------------------------------------------------------------------------

void tns::print_hexbyte(uint8_t b)
{
    print_hexhalf(b >> 4);
    print_hexhalf(b);
}

//-----------------------------------------------------------------------------

void tns::print_hexbyte_for_dump(uint8_t b)
{
    print_hexbyte(b);
    put_char(0x20);
}

//-----------------------------------------------------------------------------

void tns::print_hexlong(uint32_t l)
{
    print_hexbyte((uint8_t)(l >> 24));
    print_hexbyte((uint8_t)(l >> 16));
    print_hexbyte((uint8_t)(l >> 8));
    print_hexbyte((uint8_t)l);
}

//-----------------------------------------------------------------------------

void tns::put_char_for_dump(uint8_t ch)
{
    if (ch < 0x20)
        put_char('.');
    else
        put_char(ch);
}

//-----------------------------------------------------------------------------

void tns::print_dec99(uint8_t b)
{
    uint8_t tmp = (b / 10 * 16 + b % 10);
    print_hexbyte(tmp);
}

//-----------------------------------------------------------------------------

const static uint16_t decwtab[4] = {10000, 1000, 100, 10};

static void rint_dec16(uint16_t w)
{
    uint8_t i, f;
    i = 0;
    f = 0;
    do
    {
        uint16_t k;
        k = decwtab[i];
        uint8_t x;
        x = 0;
        while (w >= k)
        {
            w -= k;
            x++;
        }
        if ((x == 0) && (f == 0))
            tns::put_char(' ');
        else
        {
            f = 1;
            x |= 0x30;
            tns::put_char(x);
        }
        i++;
    } while (i < 4);
    i = (uint8_t)w;
    i |= 0x30;
    tns::put_char(i);
}

//-----------------------------------------------------------------------------

void tns::print_msg(const uint8_t *msg)
{
    uint8_t ch;
    do
    {
        ch = *msg;
        msg++;
        if (ch)
            put_char(ch);
    } while (ch);
}

//-----------------------------------------------------------------------------

void tns::print_mlmsg(const uint8_t *mlmsg[])
{
    print_msg(mlmsg[lang]);
}

//-----------------------------------------------------------------------------

void tns::print_short_vers(void)
{
    uint16_t vers;
    vers = 0x2020; // ddv dummy
    uint8_t day;
    day = vers & 0x1f;
    if (day)
    {
        uint8_t month, year;
        month = (vers >> 5) & 0x0f;
        year = (vers >> 9) & 0x3f;
        if ((month) && (month < 13) && (year > 10))
        {
            put_char('(');
            print_dec99(year);
            print_dec99(month);
            print_dec99(day);
            put_char(')');
        }
    }
}

//-----------------------------------------------------------------------------
