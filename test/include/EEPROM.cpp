#include "EEPROM.h"

int MyEeprom::length() {
  return EEPROM_SIZE;
}

char MyEeprom::read(int idx) {
  return eeprom[idx];
}

void MyEeprom::put(char c, int idx) {
  numWrites++;
  eeprom[idx] = c;
}

unsigned int MyEeprom::getWritesPerformed() {
  return numWrites;
}

MyEeprom EEPROM;
