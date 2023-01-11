#include "screen.hpp"
#include "output.hpp"
#include "messages.hpp"
#include "init.hpp"
#include "app_hal/spi.hpp"
#include "app_hal/hid.hpp"
#include <Arduino.h>

namespace tns
{

#define WIN_ATTR 0x9f
#define CURSOR_ATTR 0xf0
#define WIN_SHADOW_ATTR 0x01

    const WIND_DESC wind_swlng = {13, 11, 27, 3, 0x9f, 0x01};
    constexpr auto TOTAL_LANG = 2;

    //-----------------------------------------------------------------------------
    // ��⠭���� ⥪�饣� ��ਡ��
    void scr_set_attr(uint8_t attr)
    {
        CPLD.spi_tns_transfer(TNSCommand::SCR_SET_ATTR, attr);
    }

    //-----------------------------------------------------------------------------
    // ��⠭���� ����樨 ���� �� ��࠭� (x - 0..52; y - 0..24)
    void scr_set_cursor(uint8_t x, uint8_t y)
    {
        uint16_t addr;
        addr = y * 53 + x - 1;
        CPLD.spi_tns_transfer(TNSCommand::SCR_LOADDR, addr & 0xff);
        CPLD.spi_tns_transfer(TNSCommand::SCR_HIADDR, (addr >> 8) & 0x07);
    }

    //-----------------------------------------------------------------------------

    void scr_print_msg(const uint8_t *msg)
    {
        CPLD.spi_tns_sel_reg(TNSCommand::SCR_CHAR);

        uint8_t ch;
        do
        {
            ch = *msg;
            msg++;
            if (ch == 0x15)
            {
                uint8_t attr;
                attr = *msg;
                msg++;
                scr_set_attr(attr);
                CPLD.spi_tns_sel_reg(TNSCommand::SCR_CHAR);
            }
            else if (ch == 0x16)
            {
                uint8_t x, y;
                x = *msg;
                msg++;
                y = *msg;
                msg++;
                scr_set_cursor(x, y);
                CPLD.spi_tns_sel_reg(TNSCommand::SCR_CHAR);
            }
            else if (ch)
                CPLD.spi_tns_same_reg(ch);
        } while (ch);
    }

    //-----------------------------------------------------------------------------

    void scr_print_mlmsg(const uint8_t *const mlmsg[])
    {
        scr_print_msg(mlmsg[lang]);
    }

    //-----------------------------------------------------------------------------

    void scr_print_msg_n(const char *msg, uint8_t size)
    {
        CPLD.spi_tns_sel_reg(TNSCommand::SCR_CHAR);
        uint8_t ch;
        do
        {
            ch = *msg;
            msg++;
            CPLD.spi_tns_same_reg(ch);
        } while (--size);
    }

    //-----------------------------------------------------------------------------

    void scr_print_rammsg_n(uint8_t *msg, uint8_t size)
    {
        CPLD.spi_tns_sel_reg(TNSCommand::SCR_CHAR);
        do
        {
            CPLD.spi_tns_same_reg(*msg++);
        } while (--size);
    }

    //-----------------------------------------------------------------------------

    void scr_putchar(uint8_t ch)
    {
        CPLD.spi_tns_transfer(TNSCommand::SCR_CHAR, ch);
    }

    //-----------------------------------------------------------------------------

    void scr_fill_char(uint8_t ch, uint16_t count)
    {
        CPLD.spi_tns_transfer(TNSCommand::SCR_CHAR, ch);
        while (--count)
        {
            CPLD.spi_tns_clr_ssel();
            CPLD.spi_tns_set_ssel();
        }
    }

    //-----------------------------------------------------------------------------

    void scr_fill_char_attr(uint8_t ch, uint8_t attr, uint16_t count)
    {
        CPLD.spi_tns_transfer(TNSCommand::SCR_SET_ATTR, attr);
        CPLD.spi_tns_transfer(TNSCommand::SCR_CHAR, ch);
        while (--count)
        {
            CPLD.spi_tns_clr_ssel();
            CPLD.spi_tns_set_ssel();
        }
    }

    //-----------------------------------------------------------------------------

    void scr_fill_attr(uint8_t attr, uint16_t count)
    {
        CPLD.spi_tns_transfer(TNSCommand::SCR_FILL, attr);
        while (--count)
        {
            CPLD.spi_tns_clr_ssel();
            CPLD.spi_tns_set_ssel();
        }
    }

    //-----------------------------------------------------------------------------

    void scr_backgnd(void)
    {
        scr_set_cursor(0, 0);
        scr_fill_char_attr(0x20, 0x1f, 53);      // ' '
        //scr_fill_char_attr(0xb0, 0x77, 53 * 23); // '�'
        scr_fill_char_attr(0x20, 0xf0, 53 * 23); // '�'
        scr_fill_char_attr(0x20, 0x1f, 53);      // ' '
        flags1 &= ~(ENABLE_DIRECTUART | ENABLE_UART);
        flags1 |= ENABLE_SCR;
        scr_set_cursor(0, 0);
        scr_print_msg(msg_title1);
        print_short_vers();
        scr_set_cursor(15, 24);
        scr_print_msg(msg_title2);

        scr_set_attr(0xf0);

        uint8_t ypos = 3;
        uint8_t xpos = 4;
        scr_set_cursor(xpos, ypos++);  
    }

    //-----------------------------------------------------------------------------

    void scr_fade(void)
    {
        scr_set_cursor(0, 1);
        scr_fill_attr(0x77, 53 * 23);
    }

    void menu_draw_cursor(uint8_t x, uint8_t y, uint8_t attr, uint16_t width)
    {
        scr_set_cursor(x, y);
        scr_fill_attr(attr, width);
    }

    //-----------------------------------------------------------------------------

    void scr_window(const WIND_DESC &pwindesc)
    {
        uint8_t x, y, width, height, wind_attr, wind_flag;
        x = pwindesc.x;
        y = pwindesc.y;
        width = pwindesc.width - 2;
        height = pwindesc.height - 2;
        wind_attr = pwindesc.attr;
        wind_flag = pwindesc.flag;

        scr_set_cursor(x, y);
        scr_set_attr(wind_attr);
        scr_putchar(0xda);          // '�'
        scr_fill_char(0xc4, width); // '�'
        scr_putchar(0xbf);          // '�'
        uint8_t i;
        for (i = 0; i < height; i++)
        {
            scr_set_cursor(x, y + i + 1);
            scr_set_attr(wind_attr);
            scr_putchar(0xb3);          // '�'
            scr_fill_char(0x20, width); // '�'
            scr_putchar(0xb3);          // '�'
            if (wind_flag & 0x01)
                scr_fill_attr(WIN_SHADOW_ATTR, 1);
        }
        scr_set_cursor(x, y + height + 1);
        scr_set_attr(wind_attr);
        scr_putchar(0xc0);          // '�'
        scr_fill_char(0xc4, width); // '�'
        scr_putchar(0xd9);          // '�'
        if (wind_flag & 0x01)
        {
            scr_fill_attr(WIN_SHADOW_ATTR, 1);
            scr_set_cursor(x + 1, y + height + 2);
            scr_fill_attr(WIN_SHADOW_ATTR, width + 2);
            scr_set_attr(wind_attr);
        }
    }

    //-----------------------------------------------------------------------------

    const WIND_DESC wind_menu_help = {3, 13, 37, 9, 0xcf, 0x01};

    void menu_help(void)
    {
        scr_fade();
        scr_window(wind_menu_help);
        scr_print_mlmsg(mlmsg_menu_help);

        waitkey();
    }

    void menu_swlng(void)
    {
        uint8_t go2;
        do
        {
            lang++;
            if (lang >= TOTAL_LANG)
                lang = 0;

            save_params();
            scr_fade();
            scr_window(wind_swlng);
            scr_set_attr(0x9e);
            scr_print_mlmsg(mlmsg_swlng);
            uint16_t to;
            go2 = GO_READKEY;

            uint8_t key = 0;

            do
            {

                if (inkey(key))
                {
                    if (key == KEY_CAPSLOCK)
                        go2 = GO_REPEAT;
                    if (key == KEY_ESC)
                        go2 = GO_EXIT;
                }

            } while (go2 == GO_READKEY);

        } while (go2 != GO_EXIT);
    }

    //-----------------------------------------------------------------------------

    void scr_menu(const MENU_DESC &pmenudesc)
    {
        uint8_t menu_select = 0, go2;
        do
        {
            uint8_t x, y, width, items;
            const char *strptr;
            uint16_t to, BkTO;

            uint8_t key = 0;

            scr_backgnd();
            x = pmenudesc.x;
            y = pmenudesc.y;
            width = pmenudesc.width;
            items = pmenudesc.items;
            BkTO = pmenudesc.bgtask_period;
            scr_set_cursor(x, y);
            scr_set_attr(WIN_ATTR);
            scr_putchar(0xda);              // '�'
            scr_fill_char(0xc4, width + 2); // '�'
            scr_putchar(0xbf);              // '�'
            uint8_t i;
            for (i = 0; i < items; i++)
            {
                scr_set_cursor(x, y + i + 1);
                scr_set_attr(WIN_ATTR);
                scr_putchar(0xb3); // '�'
                scr_putchar(0x20); // ' '
                strptr = pmenudesc.strings[(lang * items) + i].c_str();
                scr_print_msg_n(strptr, width);
                scr_putchar(0x20); // ' '
                scr_putchar(0xb3); // '�'
                scr_fill_attr(WIN_SHADOW_ATTR, 1);
            }
            scr_set_cursor(x, y + items + 1);
            scr_set_attr(WIN_ATTR);
            scr_putchar(0xc0);              // '�'
            scr_fill_char(0xc4, width + 2); // '�'
            scr_putchar(0xd9);              // '�'
            scr_fill_attr(WIN_SHADOW_ATTR, 1);

            scr_set_cursor(x + 1, y + items + 2);
            scr_fill_attr(WIN_SHADOW_ATTR, width + 4);

            menu_draw_cursor(x + 1, y + 1 + menu_select, CURSOR_ATTR, width + 2);
            go2 = GO_READKEY;

            do
            {
                if (!inkey(key))
                {
                    vPortYield();
                    continue;
                }

                switch (key)
                {
                case KEY_ENTER:
                    scr_fade();
                    {
                        /*const uint16_t *ptr = (const uint16_t *)pmenudesc.handlers;
                        auto pItemHndl = ptr[menu_select];
                        if (pItemHndl)
                            pItemHndl();
                            */
                    }
                    go2 = GO_REDRAW;
                    break;
                case KEY_UP:
                    if (menu_select)
                    {
                        menu_draw_cursor(x + 1, y + 1 + menu_select, WIN_ATTR, width + 2);
                        menu_select--;
                        menu_draw_cursor(x + 1, y + 1 + menu_select, CURSOR_ATTR, width + 2);
                    }
                    break;
                case KEY_DOWN:
                    if (menu_select < (items - 1))
                    {
                        menu_draw_cursor(x + 1, y + 1 + menu_select, WIN_ATTR, width + 2);
                        menu_select++;
                        menu_draw_cursor(x + 1, y + 1 + menu_select, CURSOR_ATTR, width + 2);
                    }
                    break;
                case KEY_PAGEUP:
                case KEY_HOME:
                    menu_draw_cursor(x + 1, y + 1 + menu_select, WIN_ATTR, width + 2);
                    menu_select = 0;
                    menu_draw_cursor(x + 1, y + 1 + menu_select, CURSOR_ATTR, width + 2);
                    break;
                case KEY_PAGEDOWN:
                case KEY_END:
                    menu_draw_cursor(x + 1, y + 1 + menu_select, WIN_ATTR, width + 2);
                    menu_select = items - 1;
                    menu_draw_cursor(x + 1, y + 1 + menu_select, CURSOR_ATTR, width + 2);
                    break;
                case KEY_ESC:
                    go2 = GO_EXIT;
                    break;
                case KEY_CAPSLOCK:
                    menu_swlng();
                    go2 = GO_REDRAW;
                    break;
                case KEY_SCROLLLOCK:
                    // toggle_vga();
                    // ps2k_setsysled();
                    break;
                case KEY_F1:
                    menu_help();
                    go2 = GO_REDRAW;
                }

            } while (go2 == GO_READKEY);

        } while (go2 == GO_REDRAW);
    }
}