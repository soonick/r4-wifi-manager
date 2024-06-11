#include "WiFiS3.h"
#include <iostream>

/**
 * WiFiSserver
 */

WiFiServer::WiFiServer(int port) {
  std::cout << "port: " << port << "\n";
}
WiFiServer::WiFiServer(int port, WiFiClient* c) {
  std::cout << "port: " << port << "\n";
  client = c;
}
WiFiServer::~WiFiServer() {
  delete client;
}

WiFiClient WiFiServer::available() {
  return *client;
}
void WiFiServer::begin() {}

/**
 * WiFiClient
 */

WiFiClient::WiFiClient(String r) {
  std::cout << r << "\n";
  request = r;
}

WiFiClient::operator bool() const {
  return true;
}
bool WiFiClient::available() {
  return request.size() > currentCharPos;
}
bool WiFiClient::connected() {
  return true;
}
void WiFiClient::println() {}
void WiFiClient::println(String a) {
  std::cout << a;
}
void WiFiClient::print(String a) {
  std::cout << a;
}
char WiFiClient::read() {
  currentCharPos++;
  return request[currentCharPos - 1];
}
void WiFiClient::stop() {}

String HardwareWifi::SSID() {
  return "";
}
IPAddress HardwareWifi::localIP() {
  IPAddress address;
  return address;
}
void HardwareWifi::config(IPAddress ip) {
  std::cout << &ip;
}
int HardwareWifi::beginAP(String ssid, String) {
  if (ssid == "badssid") {
    return WL_IDLE_STATUS;
  }
  return WL_AP_LISTENING;
}
int HardwareWifi::status() {
  return WL_AP_LISTENING;
}

HardwareWifi WiFi;
