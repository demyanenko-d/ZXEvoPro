#include "init.hpp"
#include "output.hpp"
#include "app_hal/eeprom.hpp"

namespace tns
{
    void init()
    {
        load_params();
    }

    void load_params()
    {
        lang = eeprom_read_u8(eeprom_key::tns_lang);
    }

    void save_params()
    {
        eeprom_write_u8(eeprom_key::tns_lang, lang);
    }
}
