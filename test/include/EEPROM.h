#pragma once

class MyEeprom {
 public:
  int length();
  char read(int idx);
  void put(char c, int idx);
  unsigned int getWritesPerformed();

 private:
  static const int EEPROM_SIZE = 512;
  char eeprom[EEPROM_SIZE];
  unsigned long numWrites = 0;
};

extern MyEeprom EEPROM;
