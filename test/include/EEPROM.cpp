#include "EEPROM.h"

int EEPROMClass::length() {
  return EEPROM_SIZE;
}

char EEPROMClass::read(int idx) {
  return eeprom[idx];
}

void EEPROMClass::put(char c, int idx) {
  numWrites++;
  eeprom[idx] = c;
}

unsigned int EEPROMClass::getWritesPerformed() {
  return numWrites;
}

EEPROMClass EEPROM;
