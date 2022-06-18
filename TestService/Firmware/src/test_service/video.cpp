/*

#include "video.hpp"
#include "screen.hpp"
#include "app_hal/spi.hpp"

//-----------------------------------------------------------------------------

void Test_Video(void)
{
    scr_set_cursor(0, 0);
    scr_fill_char_attr(0x00, 0x00, 53 * 25);
    scr_set_cursor(15, 4);
    scr_print_msg(msg_title2);

    uint8_t tvid_tst_cnt;
    tvid_tst_cnt = 0;
    do
    {
        tvid_tst_cnt++;
        spi_tns_transfer(tns_commands::SCR_MODE, tvid_tst_cnt | (mode1 & 0b10000000));
        if (tvid_tst_cnt >= 6)
            tvid_tst_cnt = 0;
    } while ((waitkey() >> 8) != KEY_ESC);

    spi_tns_transfer(tns_commands::SCR_MODE, mode1 & 0b10000000);
}

//-----------------------------------------------------------------------------

*/
