#pragma once
#include <stdint.h>

enum class eeprom_key
{
    tns_signature = 0, // 4
    tns_version = 4,   // 4
    tns_lang = 8,      // 1
};

void eeprom_write_u32(eeprom_key key, uint32_t value);
void eeprom_write_u8(eeprom_key key, uint8_t value);

uint32_t eeprom_read_u32(eeprom_key key);
uint8_t eeprom_read_u8(eeprom_key key);

void eeprom_init();
void eeprom_load();
