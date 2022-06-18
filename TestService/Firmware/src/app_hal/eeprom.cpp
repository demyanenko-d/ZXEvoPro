#include "eeprom.hpp"
#include "config.hpp"
#include <EEPROM.h>

void eeprom_init_default()
{
    eeprom_write_u32(eeprom_key::tns_signature, tns_signature);
    eeprom_write_u32(eeprom_key::tns_version, tns_version);
    eeprom_write_u8(eeprom_key::tns_lang, 0);
}

void eeprom_write_u32(eeprom_key key, uint32_t value)
{
    EEPROM.writeUInt((int)key, value);
}

void eeprom_write_u8(eeprom_key key, uint8_t value)
{
    EEPROM.writeByte((int)key, value);
}

uint32_t eeprom_read_u32(eeprom_key key)
{
    return EEPROM.readUInt((int)key);
}

uint8_t eeprom_read_u8(eeprom_key key)
{
    return EEPROM.readByte((int)key);
}

void eeprom_init()
{
    EEPROM.begin(1024);

    auto signature = eeprom_read_u32(eeprom_key::tns_signature);
    auto version = eeprom_read_u32(eeprom_key::tns_version);

    if (signature != tns_signature && version != tns_version)
        eeprom_init_default();
}

 