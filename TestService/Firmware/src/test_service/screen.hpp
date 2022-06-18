#pragma once
#include <stdint.h>
#include <vector>
#include <functional>

namespace tns
{
    struct WIND_DESC
    {
        uint8_t x;      // коорд.лев.верхн угола окна
        uint8_t y;      //
        uint8_t width;  // ширина (без учёта тени)
        uint8_t height; // высота (без учёта тени)
        uint8_t attr;   // атрибут окна
        uint8_t flag;   // флаги: .0 - "с тенью/без тени"
    };

    struct MENU_DESC
    {
        uint8_t x;                                   // коорд.лев.верхн угола окна
        uint8_t y;                                   //
        uint8_t width;                               // длина_строки + 2 = ширина без учёта рамки и тени
        uint8_t items;                               // количество пунктов меню
        std::function<void()> bkgnd_task;            // ссылка на фоновую задачу
        uint16_t bgtask_period;                      // период вызова фоновой задачи, мс (1..16383)
        std::vector<std::function<void()>> handlers; // указатель на структуру указателей на обработчики
        std::vector<std::string> strings;            // указатель на текст меню
    };

    void scr_set_attr(uint8_t attr);
    void scr_set_cursor(uint8_t x, uint8_t y);
    void scr_print_msg(const uint8_t *msg);
    void scr_print_mlmsg(const uint8_t *const mlmsg[]);
    void scr_print_msg_n(const char *msg, uint8_t size);
    void scr_print_rammsg_n(uint8_t *msg, uint8_t size);
    void scr_putchar(uint8_t ch);
    void scr_fill_char(uint8_t ch, uint16_t count);
    void scr_fill_char_attr(uint8_t ch, uint8_t attr, uint16_t count);
    void scr_fill_attr(uint8_t attr, uint16_t count);
    void scr_backgnd(void);
    void scr_fade(void);
    void scr_window(const WIND_DESC &pwindesc);
    void scr_menu(const MENU_DESC &pmenudesc);
}