/* 
#include "dram.hpp"
#include "screen.hpp"
#include "output.hpp"
#include "messages.hpp"
#include "app_hal/spi.hpp"

// 3         4         5
// 01234567890123456789012
//┌──── Тест DRAM ────┐  18
//│ Проведено циклов  │  19
//│ без ошибок   1234 │  20
//│ с ошибками      0 │  21
//└───────────────────┘  22

const tns::WIND_DESC wind_t_dram_1 = {30, 18, 21, 5, 0x77, 0x00};
const tns::WIND_DESC wind_t_dram_2 = {30, 18, 21, 5, 0xae, 0x00};

//-----------------------------------------------------------------------------

void mtst_decword(uint16_t data)
{
    if ((data + 1) == 0)
        tns::scr_putchar('>');
    else
        tns::scr_putchar(' ');
    tns::print_dec16(data);
}

//-----------------------------------------------------------------------------

void tns::Test_DRAM(uint8_t callflag)
{
    // u8 stored_flags1=flags1;
    uint16_t mtst_pass, mtst_fail;
    mtst_pass = spi_tns_transfer(tns_commands::MTST_FAIL_CNT0, 0xff);
    mtst_pass |= (spi_tns_transfer(tns_commands::MTST_PASS_CNT1, 0xff) << 8);
    mtst_fail = spi_tns_transfer(tns_commands::MTST_FAIL_CNT0, 0xff);
    mtst_fail |= (spi_tns_transfer(tns_commands::MTST_FAIL_CNT1, 0xff) << 8);

    if (mtst_fail)
    {
        scr_window(wind_t_dram_2);
        scr_print_mlmsg(mlmsg_mtst);
    }
    else
    {
        if (callflag)
            scr_set_attr(0x77);
        else
        {
            scr_window(wind_t_dram_1);
            scr_print_mlmsg(mlmsg_mtst);
        }
    }

    scr_set_cursor(43, 20);
    mtst_decword(mtst_pass);
    scr_set_cursor(43, 21);
    mtst_decword(mtst_fail);
}

//-----------------------------------------------------------------------------

*/
