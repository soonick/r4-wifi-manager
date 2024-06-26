#include "EEPROM.h"

int EEPROMClass::length() {
  return EEPROM_SIZE;
}

char EEPROMClass::read(int idx) {
  return eeprom[idx];
}

void EEPROMClass::update(int idx, char c) {
  numWrites++;
  eeprom[idx] = c;
}

unsigned int EEPROMClass::getWritesPerformed() {
  return numWrites;
}

EEPROMClass EEPROM;
