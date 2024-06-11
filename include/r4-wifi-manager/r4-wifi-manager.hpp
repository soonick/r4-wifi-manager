#pragma once

#include <Arduino.h>
#include <Hashtable.h>
#include <WiFiS3.h>
#include <httpino/httpino.hpp>

class R4WifiManager {
 public:
  /**
   * Starts an access point
   *
   * @param ssid - Name of the SSID
   * @param pass - Passowrd for the access point. It must be at least 8
   * characters
   * @param ip - IP address in which the AP will listen
   *
   * @return String with an error message, if there were errors. Empty string if no errors
   */
  String startAp(const char* ssid, const char* pass, const IPAddress ip);

  /**
   * If there is a user configuration already on the eeprom, it retrieves it and
   * returns it immediately. If there is no user configuration, it will listen
   * for connections until the configuration is set. Once the configuration is
   * set, it returns that configuration
   *
   * @return can return nullptr if the configuration can't be retrieved
   */
  Hashtable<String, String>* getUserConfig();

 private:
  int status = WL_IDLE_STATUS;
  WiFiServer server = WiFiServer(80);
  HttpIno http;

  void printStatus();
  Hashtable<String, String>* handleClientRequest();
  String readLine(WiFiClient client);
  void homepage(WiFiClient& client);
  void saved(WiFiClient& client);
};
