#pragma once

#ifdef HOST
#define charAt(x) at(x)
#define indexOf(...) find(__VA_ARGS__)
#define isEmpty(...) empty(__VA_ARGS__)
#define substring(start, end) substr(start, (end - start))
#endif

#include <string>

typedef std::string String;

void delay(int);

#include "WiFiS3.h"
class IPAddress;
class HardwareSerial {
 public:
  void print(String);
  void print(const char);
  void print(const int);
  void print(const long);
  void println(IPAddress);
  void println(String);
  void println(const char);
  void println(const int);
  void println(const long);
  void println(const unsigned long);
};

extern HardwareSerial Serial;
