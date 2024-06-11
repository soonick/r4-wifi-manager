#pragma once

#include "Arduino.h"

enum {
  WL_IDLE_STATUS = 0,
  WL_AP_LISTENING = 1,
  WL_AP_CONNECTED = 2,
};

struct IPAddress {};

class WiFiClient {
 public:
  WiFiClient(String);

  explicit operator bool() const;
  bool available();
  bool connected();
  void print(String);
  void println();
  void println(String);
  char read();
  void stop();

 private:
  String request;
  uint currentCharPos = 0;
};

class WiFiServer {
 public:
  WiFiServer(int);
  WiFiServer(int, WiFiClient*);
  ~WiFiServer();

  WiFiClient available();
  void begin();

 private:
  WiFiClient* client = nullptr;
};

class HardwareWifi {
 public:
  IPAddress localIP();
  String SSID();
  int beginAP(String, String);
  int status();
  void config(IPAddress);
};

extern HardwareWifi WiFi;
