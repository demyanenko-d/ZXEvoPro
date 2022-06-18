#include "messages.hpp"
#include <stdint.h>

namespace tns
{

    //-----------------------------------------------------------------------------

    const uint8_t msg_title1[] =
        "          ZX Evolution Test&Service ";
    //               |          ZX Evolution Test&Service (110203)         |
    // width limited! "01234567890123456789012345678901234567890123456789012"

    //-----------------------------------------------------------------------------

    const uint8_t msg_title2[] = "http://www.NedoPC.com/";

    //-----------------------------------------------------------------------------

    const uint8_t msg_pintest_rus[] = "\r\n\nПроверка выводов ATMEGA128... ";
    const uint8_t msg_pintest_eng[] = "\r\n\nATMEGA128 pins check... ";
    const uint8_t *const mlmsg_pintest[] = {msg_pintest_rus, msg_pintest_eng};

    //-----------------------------------------------------------------------------

    const uint8_t msg_pintest_ok_rus[] = "Проблем не обнаружено.";
    const uint8_t msg_pintest_ok_eng[] = "No problems found.";
    const uint8_t *const mlmsg_pintest_ok[] = {msg_pintest_ok_rus, msg_pintest_ok_eng};

    //-----------------------------------------------------------------------------

    const uint8_t msg_pintest_error_rus[] = "\r\nОбнаружена проблема на порту(-ах): ";
    const uint8_t msg_pintest_error_eng[] = "\r\nHave a problem at port(s): ";
    const uint8_t *const mlmsg_pintest_error[] = {msg_pintest_error_rus, msg_pintest_error_eng};

    //-----------------------------------------------------------------------------

    const uint8_t msg_pintest_pa[] = "PAx ";
    const uint8_t msg_pintest_pb[] = "PBx ";
    const uint8_t msg_pintest_pc[] = "PCx ";
    const uint8_t msg_pintest_pd[] = "PD5 ";
    const uint8_t msg_pintest_pe[] = "PEx ";
    const uint8_t msg_pintest_pg[] = "PGx ";

    //-----------------------------------------------------------------------------

    const uint8_t msg_halt_rus[] = "\r\nПрограмма остановлена!";
    const uint8_t msg_halt_eng[] = "\r\nProgram is halted!";
    const uint8_t *const mlmsg_halt[] = {msg_halt_rus, msg_halt_eng};

    //-----------------------------------------------------------------------------

    const uint8_t msg_statusof_rus[] = "\n\rСостояние ";
    const uint8_t msg_statusof_eng[] = "\n\rStatus of ";
    const uint8_t *const mlmsg_statusof_crlf[] = {msg_statusof_rus, msg_statusof_eng};
    const uint8_t *const mlmsg_statusof_cr[] = {msg_statusof_rus + 1, msg_statusof_eng + 1};

    const uint8_t msg_power_pg[] = "POWERGOOD=";
    const uint8_t msg_power_vcc5[] = ", VCC5=";

    const uint8_t msg_power_on_rus[] = "\r\nВключение питания ATX...\n";
    const uint8_t msg_power_on_eng[] = "\r\nATX power up...\n";
    const uint8_t *const mlmsg_power_on[] = {msg_power_on_rus, msg_power_on_eng};

    //-----------------------------------------------------------------------------

    const uint8_t msg_cfgfpga_rus[] = "\r\nЗагрузка конфигурации в FPGA... ";
    const uint8_t msg_cfgfpga_eng[] = "\r\nSet FPGA configuration... ";
    const uint8_t *const mlmsg_cfgfpga[] = {msg_cfgfpga_rus, msg_cfgfpga_eng};

    //-----------------------------------------------------------------------------

    const uint8_t msg_done1_rus[] = "Завершено.\r\nПроверка обмена с FPGA... ";
    const uint8_t msg_done1_eng[] = "Done.\r\nFPGA data exchange test... ";
    const uint8_t *const mlmsg_done1[] = {msg_done1_rus, msg_done1_eng};

    //-----------------------------------------------------------------------------

    const uint8_t msg_ok[] = "Ok.";

    //-----------------------------------------------------------------------------

    const uint8_t msg_someerrors_rus[] = "Есть ошибки!";
    const uint8_t msg_someerrors_eng[] = "We have some errors!";
    const uint8_t *const mlmsg_someerrors[] = {msg_someerrors_rus, msg_someerrors_eng};

    //-----------------------------------------------------------------------------

    const uint8_t msg_spi_test_rus[] = "\r\nКоличество неправильных байт из 50000 -";
    const uint8_t msg_spi_test_eng[] = "\r\nQuantity wrong byte from 50000 -";
    const uint8_t *const mlmsg_spi_test[] = {msg_spi_test_rus, msg_spi_test_eng};

    //-----------------------------------------------------------------------------

    const uint8_t msg_kbd_detect_rus[] = "\r\nПроверка клавиатуры PS/2...\r\n";
    const uint8_t msg_kbd_detect_eng[] = "\r\nPS/2 keyboard check...\r\n";
    const uint8_t *const mlmsg_kbd_detect[] = {msg_kbd_detect_rus, msg_kbd_detect_eng};

    //-----------------------------------------------------------------------------

    const uint8_t msg_noresponse_rus[] = " ...нет ответа\r\n";
    const uint8_t msg_noresponse_eng[] = " ...no response\r\n";
    const uint8_t *const mlmsg_noresponse[] = {msg_noresponse_rus, msg_noresponse_eng};

    //-----------------------------------------------------------------------------

    const uint8_t msg_unwanted_rus[] = " <- неожидаемый ответ\r\n";
    const uint8_t msg_unwanted_eng[] = " <- unwanted response\r\n";
    const uint8_t *const mlmsg_unwanted[] = {msg_unwanted_rus, msg_unwanted_eng};

    //-----------------------------------------------------------------------------

    const uint8_t msg_txfail_rus[] = " ...сбой при передаче\r\n";
    const uint8_t msg_txfail_eng[] = " ...fail to transmit\r\n";
    const uint8_t *const mlmsg_txfail[] = {msg_txfail_rus, msg_txfail_eng};

    //-----------------------------------------------------------------------------

    const uint8_t msg_ready[] = "---\r\n";

    //-----------------------------------------------------------------------------

    const uint8_t msg_menu_help_rus[] =
        "\x16\x05\x0e"
        "Основные клавиши управления:"
        "\x16\x05\x0f"
        "<>, <>"
        "\x16\x05\x10"
        "<Enter> - \"Да\", выбор"
        "\x16\x05\x11"
        "<Esc> - \"Нет\", отмена, выход"
        "\x16\x05\x12"
        "Горячие клавиши (только в меню):"
        "\x16\x05\x13"
        "<ScrollLock> - режим TV/VGA"
        "\x16\x05\x14"
        "<CapsLock> - язык интерфейса";
    const uint8_t msg_menu_help_eng[] =
        "\x16\x05\x0e"
        "Usage:"
        "\x16\x05\x0f"
        "<>, <>"
        "\x16\x05\x10"
        "<Enter> - \"Yes\", select"
        "\x16\x05\x11"
        "<Esc> - \"No\", cancel, exit"
        "\x16\x05\x12"
        "Hot-keys (in menu only):"
        "\x16\x05\x13"
        "<ScrollLock> - toggle TV/VGA mode"
        "\x16\x05\x14"
        "<CapsLock> - language switch";
    // width limited! "567890123456789012345678901234567"
    const uint8_t *const mlmsg_menu_help[] = {msg_menu_help_rus, msg_menu_help_eng};

    //-----------------------------------------------------------------------------

    const uint8_t msg_tbeep_rus[] =
        "\x16\x1a\x0a"
        "Гц"
        "\x16\x0a\x0c"
        "<>, <> - изменение частоты"
        "\x15\x0f";
    const uint8_t msg_tbeep_eng[] =
        "\x16\x1a\x0a"
        "Hz"
        "\x16\x0e\x0c"
        "<>, <> - frequence"
        "\x15\x0f";
    // width limited!                       "0123456789012345678901234567"
    const uint8_t *const mlmsg_tbeep[] = {msg_tbeep_rus, msg_tbeep_eng};

    //-----------------------------------------------------------------------------

    const uint8_t msg_tzxk1_rus[] = "\x16\x0e\x07Клавиатура ZX\x16\x23\x07Джойстик";
    const uint8_t msg_tzxk1_eng[] = "\x16\x0f\x07ZX Keyboard\x16\x23\x07Joystick";
    const uint8_t *const mlmsg_tzxk1[] = {msg_tzxk1_rus, msg_tzxk1_eng};

    //-----------------------------------------------------------------------------

    const uint8_t msg_tzxk2[] =
        "\x16\x0b\x09"
        "1 2 3 4 5 6 7 8 9 0"
        "\x16\x0b\x0a"
        "Q W E R T Y U I O P"
        "\x16\x0b\x0b"
        "A S D F G H J K L e"
        "\x16\x0b\x0c"
        "c Z X C V B N M s s"
        "\x16\x26\x0a"
        "\x18"
        "\x16\x24\x0b"
        "\x1b F \x1a"
        "\x16\x26\x0c"
        "\x19"
        "\x16\x0e\x0f"
        "SoftReset"
        "\x16\x1e\x0f"
        "TurboKey";

    //-----------------------------------------------------------------------------

    const uint8_t msg_tps2k0_rus[] =
        "\x16\x05\x13"
        "Трёхкратное нажатие <ESC> - выход из теста.";
    const uint8_t msg_tps2k0_eng[] =
        "\x16\x0a\x13"
        "Press <ESC> three times to exit.";
    // width limited! "5678901234567890123456789012345678901234567"
    const uint8_t *const mlmsg_tps2k0[] = {msg_tps2k0_rus, msg_tps2k0_eng};

    //-----------------------------------------------------------------------------

    const uint8_t msg_tps2k1[] =
        "\x16\x05\x07"
        "e   1 2 3 4 5 6 7 8 9 0 1 2  p s p  \x07 \x07 \x07"
        "\x16\x05\x09"
        "` 1 2 3 4 5 6 7 8 9 0 - = \x1b  i h u  n / * -"
        "\x16\x05\x0a"
        "t Q W E R T Y U I O P [ ] \\  d e d  7 8 9"
        "\x16\x05\x0b"
        "c A S D F G H J K L ; '   e         4 5 6 +"
        "\x16\x05\x0c"
        "s Z X C V B N M , . /     s    \x18    1 2 3"
        "\x16\x05\x0d"
        "c w a       s       a w m c  \x1b \x19 \x1a  0   . e"
        "\x16\x05\x10"
        "Raw data:\x16\x04\x0f";

    //-----------------------------------------------------------------------------

    const uint8_t msg_mouse_test_rus[] = "\r\nТестирование мыши... ";
    const uint8_t msg_mouse_test_eng[] = "\r\nMouse test... ";
    const uint8_t *const mlmsg_mouse_test[] = {msg_mouse_test_rus, msg_mouse_test_eng};

    //-----------------------------------------------------------------------------

    const uint8_t msg_mouse_detect_rus[] = "Обнаружение мыши...  ";
    const uint8_t msg_mouse_detect_eng[] = "Detecting mouse...  ";
    const uint8_t *const mlmsg_mouse_detect[] = {msg_mouse_detect_rus, msg_mouse_detect_eng};

    //-----------------------------------------------------------------------------

    const uint8_t msg_mouse_setup_rus[] = "Настройка... ";
    const uint8_t msg_mouse_setup_eng[] = "Customization... ";
    const uint8_t *const mlmsg_mouse_setup[] = {msg_mouse_setup_rus, msg_mouse_setup_eng};

    //-----------------------------------------------------------------------------

    const uint8_t msg_mouse_letsgo_rus[] = "Поехали!";
    const uint8_t msg_mouse_letsgo_eng[] = "Let's go!";
    const uint8_t *const mlmsg_mouse_letsgo[] = {msg_mouse_letsgo_rus, msg_mouse_letsgo_eng};

    //-----------------------------------------------------------------------------

    const uint8_t msg_mouse_fail0_rus[] = "      Нет ответа от мыши.";
    const uint8_t msg_mouse_fail0_eng[] = "      No mouse response.";
    // width limited!                         "1234567890123456789012345678901"
    const uint8_t *const mlmsg_mouse_fail0[] = {msg_mouse_fail0_rus, msg_mouse_fail0_eng};

    //-----------------------------------------------------------------------------

    const uint8_t msg_mouse_fail1_rus[] = "   Имеются некоторые проблемы.";
    const uint8_t msg_mouse_fail1_eng[] = "    There are some problems.";
    // width limited!                         "1234567890123456789012345678901"
    const uint8_t *const mlmsg_mouse_fail1[] = {msg_mouse_fail1_rus, msg_mouse_fail1_eng};

    //-----------------------------------------------------------------------------

    const uint8_t msg_mouse_restart_rus[] = "  <Enter> - перезапустить тест.";
    const uint8_t msg_mouse_restart_eng[] = "     <Enter> - restart test.";
    // width limited!                           "1234567890123456789012345678901"
    const uint8_t *const mlmsg_mouse_restart[] = {msg_mouse_restart_rus, msg_mouse_restart_eng};

    //-----------------------------------------------------------------------------

    const uint8_t msg_tpsm_1[] =
        "\x16\x25\x0f"
        "Wheel ="
        "\x16\x25\x0c"
        "L   M   R"
        "\x16\x25\x11"
        "X  ="
        "\x16\x25\x12"
        "Y  =";

    //-----------------------------------------------------------------------------

    const uint8_t msg_mtst_rus[] =
        "\x16\x23\x12"
        " Тест DRAM "
        "\x16\x20\x13"
        "Проведено циклов"
        "\x16\x20\x14"
        "без ошибок"
        "\x16\x20\x15"
        "с ошибками";
    const uint8_t msg_mtst_eng[] =
        "\x16\x23\x12"
        " DRAM test "
        // width limited! "23456789012345678"
        "\x16\x20\x13"
        "Loops"
        // width limited! "23456789012345678"
        "\x16\x20\x14"
        "Pass"
        // width limited! "2345678901"
        "\x16\x20\x15"
        "Fail";
    // width limited! "2345678901"
    const uint8_t *const mlmsg_mtst[] = {msg_mtst_rus, msg_mtst_eng};

    //-----------------------------------------------------------------------------

    const uint8_t msg_swlng_rus[] = "\x16\x17\x0c"
                                    "Русский";
    const uint8_t msg_swlng_eng[] = "\x16\x17\x0c"
                                    "English";
    // width limited!                                  "456789012345678901234567"
    const uint8_t *const mlmsg_swlng[] = {msg_swlng_rus, msg_swlng_eng};

    //-----------------------------------------------------------------------------

    const uint8_t msg_fl_menu_rus[] =
        "\x16\x02\x02"
        "Выход"
        "\x16\x02\x03"
        "Всё снова"
        "\x16\x02\x04"
        "Стереть м/сх."
        "\x16\x02\x05"
        "Добав.задание"
        "\x16\x02\x06"
        "Выполнить";
    const uint8_t msg_fl_menu_eng[] =
        "\x16\x02\x02"
        "Exit"
        "\x16\x02\x03"
        "Retrieve all"
        "\x16\x02\x04"
        "Erase chip"
        "\x16\x02\x05"
        "Add job"
        "\x16\x02\x06"
        "Execute jobs";
    // width limited! "2345678901234"
    const uint8_t *const mlmsg_fl_menu[] = {msg_fl_menu_rus, msg_fl_menu_eng};

    //-----------------------------------------------------------------------------

    const uint8_t msg_fp_nofiles_rus[] = "\x15\x9f"
                                         " Нет файлов ";
    const uint8_t msg_fp_nofiles_eng[] = "\x15\x9f"
                                         "  No files  ";
    // width fixed!                                      "123456789012"
    const uint8_t *const mlmsg_fp_nofiles[] = {msg_fp_nofiles_rus, msg_fp_nofiles_eng};

    //-----------------------------------------------------------------------------

    const uint8_t msg_fl_readrom_rus[] =
        "\x16\x02\x0a"
        "Чтение Flash"
        "\x16\x02\x0b"
        "<ESC> - выход";
    const uint8_t msg_fl_readrom_eng[] =
        "\x16\x02\x0a"
        "Read Flash..."
        "\x16\x02\x0b"
        "<ESC> - exit";
    // width limited! "2345678901234"
    const uint8_t *const mlmsg_fl_readrom[] = {msg_fl_readrom_rus, msg_fl_readrom_eng};

    //-----------------------------------------------------------------------------

    const uint8_t msg_fl_sdinit_rus[] =
        "\x16\x02\x0b\x15\x9f"
        "Иниц.SD карты";
    const uint8_t msg_fl_sdinit_eng[] =
        "\x16\x02\x0b\x15\x9f"
        "SDcard init. ";
    // width limited!         "2345678901234"
    const uint8_t *const mlmsg_fl_sdinit[] = {msg_fl_sdinit_rus, msg_fl_sdinit_eng};

    //-----------------------------------------------------------------------------

    const uint8_t msg_fl_sderror1_rus[] = " Нет SD карты! ";
    const uint8_t msg_fl_sderror1_eng[] = "  No SD-card!  ";
    // width fixed!                           "123456789012345"
    const uint8_t *const mlmsg_fl_sderror1[] = {msg_fl_sderror1_rus, msg_fl_sderror1_eng};

    //-----------------------------------------------------------------------------

    const uint8_t msg_fl_sderror2_rus[] = " Ошибка чт. SD ";
    const uint8_t msg_fl_sderror2_eng[] = " SD read error ";
    // width fixed!                           "123456789012345"
    const uint8_t *const mlmsg_fl_sderror2[] = {msg_fl_sderror2_rus, msg_fl_sderror2_eng};

    //-----------------------------------------------------------------------------

    const uint8_t msg_fl_sderror3_rus[] = "   Нет FAT !   ";
    const uint8_t msg_fl_sderror3_eng[] = " FAT no found! ";
    // width fixed!                           "123456789012345"
    const uint8_t *const mlmsg_fl_sderror3[] = {msg_fl_sderror3_rus, msg_fl_sderror3_eng};

    //-----------------------------------------------------------------------------

    const uint8_t msg_fl_sderror4_rus[] = "  Ошибка FAT ! ";
    const uint8_t msg_fl_sderror4_eng[] = "  FAT error !  ";
    // width fixed!                           "123456789012345"
    const uint8_t *const mlmsg_fl_sderror4[] = {msg_fl_sderror4_rus, msg_fl_sderror4_eng};

    //-----------------------------------------------------------------------------

    // const uint8_t msg_fl_sderrorx_rus[]  = " О, глюкануло! ";
    // const uint8_t msg_fl_sderrorx_eng[]  = " Great glitch! ";
    ////width fixed!                           "123456789012345"
    // const uint8_t * const mlmsg_fl_sderrorx[]  = {msg_fl_sderrorx_rus, msg_fl_sderrorx_eng};

    //-----------------------------------------------------------------------------

    const uint8_t msg_fl_sure_rus[] =
        "\x16\x02\x0c\x15\x9e"
        " Уверен? <Y> ";
    const uint8_t msg_fl_sure_eng[] =
        "\x16\x02\x0c\x15\x9e"
        "You sure? <Y>";
    // width fixed!           "2345678901234"
    const uint8_t *const mlmsg_fl_sure[] = {msg_fl_sure_rus, msg_fl_sure_eng};

    //-----------------------------------------------------------------------------

    const uint8_t msg_fl_erase_rus[] =
        "\x16\x02\x0c\x15\x9e"
        "Стирание...  ";
    const uint8_t msg_fl_erase_eng[] =
        "\x16\x02\x0c\x15\x9e"
        "Erase...     ";
    // width fixed!           "2345678901234"
    const uint8_t *const mlmsg_fl_erase[] = {msg_fl_erase_rus, msg_fl_erase_eng};

    //-----------------------------------------------------------------------------

    const uint8_t msg_fl_write_rus[] =
        "\x16\x02\x0c\x15\x9e"
        "Запись...    ";
    const uint8_t msg_fl_write_eng[] =
        "\x16\x02\x0c\x15\x9e"
        "Write...     ";
    // width fixed!           "2345678901234"
    const uint8_t *const mlmsg_fl_write[] = {msg_fl_write_rus, msg_fl_write_eng};

    //-----------------------------------------------------------------------------

    const uint8_t msg_fl_verify_rus[] =
        "\x16\x02\x0c\x15\x9e"
        "Проверка...  ";
    const uint8_t msg_fl_verify_eng[] =
        "\x16\x02\x0c\x15\x9e"
        "Verify...    ";
    // width fixed!           "2345678901234"
    const uint8_t *const mlmsg_fl_verify[] = {msg_fl_verify_rus, msg_fl_verify_eng};

    //-----------------------------------------------------------------------------

    const uint8_t msg_fl_complete_rus[] =
        "\x16\x02\x0c\x15\x9e"
        "Завершено.   ";
    const uint8_t msg_fl_complete_eng[] =
        "\x16\x02\x0c\x15\x9e"
        "Complete.    ";
    // width fixed!           "2345678901234"
    const uint8_t *const mlmsg_fl_complete[] = {msg_fl_complete_rus, msg_fl_complete_eng};

    //-----------------------------------------------------------------------------

    const uint8_t msg_flres0_rus[] =
        "\x16\x0d\x06"
        "Запись в FlashROM завершена";
    const uint8_t msg_flres0_eng[] =
        "\x16\x12\x06"
        "Job(s) completed.";
    // width limited! "23456789012345678901234567890"
    const uint8_t *const mlmsg_flres0[] = {msg_flres0_rus, msg_flres0_eng};

    //-----------------------------------------------------------------------------

    const uint8_t msg_flres1_rus[] =
        "\x16\x15\x07"
        "без ошибок.";
    const uint8_t msg_flres1_eng[] =
        "\x16\x11\x07"
        "No errors detected.";
    // width limited! "23456789012345678901234567890"
    const uint8_t *const mlmsg_flres1[] = {msg_flres1_rus, msg_flres1_eng};

    //-----------------------------------------------------------------------------

    const uint8_t msg_flres2_rus[] =
        "\x16\x15\x07"
        "с ошибками!";
    const uint8_t msg_flres2_eng[] =
        "\x16\x0e\x07"
        "Some errors are detected!";
    // width limited! "23456789012345678901234567890"
    const uint8_t *const mlmsg_flres2[] = {msg_flres2_rus, msg_flres2_eng};

    //-----------------------------------------------------------------------------

    const uint8_t msg_sensors_rus[] = "   Датчики: ";
    const uint8_t msg_sensors_eng[] = "   Sensors: ";
    // width fixed!                       "012345678901"
    const uint8_t *const mlmsg_sensors[] = {msg_sensors_rus, msg_sensors_eng};

    //-----------------------------------------------------------------------------

    const uint8_t msg_s_nocard_rus[] = "     Нет карты     ";
    const uint8_t msg_s_nocard_eng[] = "      No card      ";
    // width fixed!                        "2345678901234567890"
    const uint8_t *const mlmsg_s_nocard[] = {msg_s_nocard_rus, msg_s_nocard_eng};

    //-----------------------------------------------------------------------------

    const uint8_t msg_s_inserted_rus[] = " Карта установлена ";
    const uint8_t msg_s_inserted_eng[] = "   Card inserted   ";
    // width fixed!                          "2345678901234567890"
    const uint8_t *const mlmsg_s_inserted[] = {msg_s_inserted_rus, msg_s_inserted_eng};

    //-----------------------------------------------------------------------------

    const uint8_t msg_s_readonly_rus[] = "  Защита от записи ";
    const uint8_t msg_s_readonly_eng[] = "     Read only     ";
    // width fixed!                          "1234567890123456789"
    const uint8_t *const mlmsg_s_readonly[] = {msg_s_readonly_rus, msg_s_readonly_eng};

    //-----------------------------------------------------------------------------

    const uint8_t msg_s_writeen_rus[] = "  Запись разрешена ";
    const uint8_t msg_s_writeen_eng[] = "   Write enabled   ";
    // width fixed!                         "1234567890123456789"
    const uint8_t *const mlmsg_s_writeen[] = {msg_s_writeen_rus, msg_s_writeen_eng};

    //-----------------------------------------------------------------------------

    const uint8_t msg_tsd_init_rus[] = "Инициализация карточки...";
    const uint8_t msg_tsd_init_eng[] = "Card initialization...";
    const uint8_t *const mlmsg_tsd_init[] = {msg_tsd_init_rus, msg_tsd_init_eng};

    //-----------------------------------------------------------------------------

    const uint8_t msg_tsd_nocard_rus[] = "SD/MMC карта не обнаружена.";
    const uint8_t msg_tsd_nocard_eng[] = "No SD/MMC card found.";
    const uint8_t *const mlmsg_tsd_nocard[] = {msg_tsd_nocard_rus, msg_tsd_nocard_eng};

    //-----------------------------------------------------------------------------

    const uint8_t msg_tsd_foundcard_rus[] = "Обнаружена карта: ";
    const uint8_t msg_tsd_foundcard_eng[] = "Found card: ";
    const uint8_t *const mlmsg_tsd_foundcard[] = {msg_tsd_foundcard_rus, msg_tsd_foundcard_eng};

    //-----------------------------------------------------------------------------

    const uint8_t msg_tsd_menu_rus[] =
        "\x16\x10\x0b"
        "Начать диагностику"
        "\x16\x0c\x0c"
        "[ ] Подробный отчёт в RS-232";
    const uint8_t msg_tsd_menu_eng[] =
        "\x16\x11\x0b"
        "Start diagnostic"
        "\x16\x0c\x0c"
        "[ ] Detailed log to RS-232";
    // width limited! "2345678901234567890123456789"
    const uint8_t *const mlmsg_tsd_menu[] = {msg_tsd_menu_rus, msg_tsd_menu_eng};

    //-----------------------------------------------------------------------------

    const uint8_t msg_tsd_foundfat_rus[] = "Обнаружена FAT";
    const uint8_t msg_tsd_foundfat_eng[] = "Found FAT";
    const uint8_t *const mlmsg_tsd_foundfat[] = {msg_tsd_foundfat_rus, msg_tsd_foundfat_eng};

    //-----------------------------------------------------------------------------

    const uint8_t msg_tsd_detect_rus[] = "Поиск файловой системы...";
    const uint8_t msg_tsd_detect_eng[] = "Detecting of file system...";
    const uint8_t *const mlmsg_tsd_detect[] = {msg_tsd_detect_rus, msg_tsd_detect_eng};

    //-----------------------------------------------------------------------------

    const uint8_t msg_tsd_readfile_rus[] = "Чтение тестового файла...";
    const uint8_t msg_tsd_readfile_eng[] = "Reading of test file...";
    const uint8_t *const mlmsg_tsd_readfile[] = {msg_tsd_readfile_rus, msg_tsd_readfile_eng};

    //-----------------------------------------------------------------------------

    const uint8_t msg_tsd_complete_rus[] = "Диагностика завершена.";
    const uint8_t msg_tsd_complete_eng[] = "Diagnostic is complete.";
    const uint8_t *const mlmsg_tsd_complete[] = {msg_tsd_complete_rus, msg_tsd_complete_eng};

    //-----------------------------------------------------------------------------

    const uint8_t msg_tsd_out[] = "\r\nout ";
    const uint8_t msg_tsd_in[] = ", in ";
    const uint8_t msg_tsd_cmd[] = "\r\n;CMD";
    const uint8_t msg_tsd_acmd41[] = "\r\n;ACMD41";
    const uint8_t msg_tsd_csup[] = "\r\nCS up";
    const uint8_t msg_tsd_csdown[] = "\r\nCS down";
    const uint8_t msg_tsd_mmc[] = "MMC";
    const uint8_t msg_tsd_sdv1[] = "SD v1";
    const uint8_t msg_tsd_sdsc[] = "SD v2+ Standard Capacity";
    const uint8_t msg_tsd_sdhc[] = "SD v2+ High Capacity";
    const uint8_t msg_tsd_ocr[] = "OCR: ";
    const uint8_t msg_tsd_csd[] = "CSD: ";
    const uint8_t msg_tsd_cid0[] = "CID: ";
    const uint8_t msg_tsd_cid1[] = "Manufacturer ID    ";
    const uint8_t msg_tsd_cid2[] = "OEM/Application ID ";
    const uint8_t msg_tsd_cid3[] = "Product name       ";
    const uint8_t msg_tsd_cid4[] = "Product revision   ";
    const uint8_t msg_tsd_cid5[] = "Product serial #   ";
    const uint8_t msg_tsd_cid6[] = "Manufacturing date ";
    const uint8_t msg_tsd_cid6b[] = ".20";
    const uint8_t msg_tsd_cid6c[] = ".19";
    const uint8_t msg_tsd_crc[] = "CRC=";
    const uint8_t msg_tsd_readsector[] = "\r\n;Read sector ";
    const uint8_t msg_tsd_skip[] = "\r\n;512 operations is skiped";

    //-----------------------------------------------------------------------------

    const uint8_t msg_trs_1[] =
        "\x16\x14\x03"
        "┬"
        "\x16\x0b\x04"
        "pc/win32"
        "\x16\x17\x04"
        "TESTCOM"
        "\x16\x09\x05"
        "├"
        "\x16\x0b\x06"
        "Bit rate 115200   No parity"
        "\x16\x0b\x07"
        "Data bits 8"
        "\x16\x1d\x07"
        "Flow control"
        "\x16\x0b\x08"
        "Stop bits 2"
        "\x16\x1e\x08"
        "√ RTS/CTS"
        "\x16\x1d\x09"
        "DSR - Ignored"
        "\x16\x15\x0a"
        "Start BERT "
        "\x16\x19\x0b"
        "┬"
        "\x16\x19\x0c"
        "│COM port"
        "\x16\x19\x0d"
        "│"
        "\x16\x13\x0e"
        "RS-232│"
        "\x16\x0c\x0f"
        "┬"
        "\x16\x19\x0f"
        "┴"
        "\x16\x05\x10"
        "ZX Evo"
        "\x16\x10\x10"
        "Last sec"
        "\x16\x26\x10"
        "sec"
        "\x16\x03\x11"
        "├"
        "\x16\x05\x12"
        "RxBuff"
        "\x16\x2d\x12"
        "RTS"
        "\x16\x05\x13"
        "TxBuff"
        "\x16\x2d\x13"
        "CTS";

    //-----------------------------------------------------------------------------

    // const uint8_t msg__rus[]  = "";
    // const uint8_t msg__eng[]  = "";
    // const uint8_t * const mlmsg_[]  = {msg__rus, msg__eng};

    //-----------------------------------------------------------------------------

    const std::vector<std::string> str_menu_main{
        "Тест клавиатуры PS/2      ",
        "Тест клавиатуры ZX и др.  ",
        "Тест мыши                 ",
        "Тест BEEP/TAPEOUT/COVOX   ",
        "Тест видео                ",
        "Тест RS-232               ",
        "Диагностика SD/MMC        ",
        "Программирование Flash-ROM",

        "PS/2 keyboard test        ",
        "ZX keyboard test and etc  ",
        "Mouse test                ",
        "BEEP/TAPEOUT/COVOX test   ",
        "Video test                ",
        "RS-232 test               ",
        "SD/MMC diagnostic         ",
        "Write Flash-ROM           ",
    };
    //"12345678901234567890123456"  width fixed!

    //-----------------------------------------------------------------------------
}