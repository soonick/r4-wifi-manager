#include "Arduino.h"
#include <iostream>

void delay(int) {}

void HardwareSerial::print(String a) {
  std::cout << a;
}
void HardwareSerial::print(const char a) {
  std::cout << a;
}
void HardwareSerial::print(const int a) {
  std::cout << a;
}
void HardwareSerial::print(const long a) {
  std::cout << a;
}
void HardwareSerial::println(String a) {
  std::cout << a << "\n";
}
void HardwareSerial::println(const int a) {
  std::cout << a << "\n";
}
void HardwareSerial::println(const unsigned long a) {
  std::cout << a << "\n";
}
void HardwareSerial::println(const long a) {
  std::cout << a << "\n";
}
void HardwareSerial::println(const char a) {
  std::cout << a << "\n";
}
void HardwareSerial::println(IPAddress a) {
  std::cout << &a << "\n";
}

HardwareSerial Serial;
