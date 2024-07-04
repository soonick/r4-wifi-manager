#include "CRC.h"
char calcCRC8(const uint8_t* str, int strLen) {
  char crc = 100;

  for (int i = 0; i < strLen; i++) {
    crc += str[i];
  }

  return crc;
}
