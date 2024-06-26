#pragma once

class EEPROMClass {
 public:
  int length();
  char read(int idx);
  void update(int idx, char c);
  unsigned int getWritesPerformed();

 private:
  static const int EEPROM_SIZE = 512;
  char eeprom[EEPROM_SIZE];
  unsigned long numWrites = 0;
};

extern EEPROMClass EEPROM;
