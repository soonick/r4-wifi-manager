#include "r4-wifi-manager/r4-wifi-manager.hpp"
#include "r4-wifi-manager/constants.hpp"

#include <string.h>

R4WifiManager::R4WifiManager(WiFiServer* s) {
  construct(s);
}

R4WifiManager::R4WifiManager() {
  construct(new WiFiServer(80));
}

R4WifiManager::~R4WifiManager() {
  delete server;
}

void R4WifiManager::construct(WiFiServer* s) {
  server = s;
}

String R4WifiManager::startAp(const char* ssid,
                              const char* pass,
                              const IPAddress ip) {
  if (strlen(pass) < 8) {
    return "Password must be 8 characters";
  }

  WiFi.config(ip);

  Serial.print("Creating access point named: ");
  Serial.println(ssid);

  this->status = WiFi.beginAP(ssid, pass);
  if (this->status != WL_AP_LISTENING) {
    return "Creating access point failed";
  }

  // wait 5 seconds for connection:
  delay(5000);
  this->server->begin();
  this->printStatus();

  return "";
}

Hashtable<String, String> R4WifiManager::getUserConfig() {
  Hashtable<String, String> currentConfig = eeprom.get();
  if (isConfigValid(currentConfig)) {
    return currentConfig;
  }

  return this->handleClientRequest();
}

void R4WifiManager::reset() {
  eeprom.reset();
}

void R4WifiManager::printStatus() {
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);
  Serial.print("To see this page in action, open a browser to http://");
  Serial.println(ip);
}

Hashtable<String, String> R4WifiManager::handleClientRequest() {
  WiFiClient client = server->available();
  Hashtable<String, String> userConfig;
  if (client) {
    Serial.println("New request");
    while (client.connected()) {
      String firstLine = readLine(&client);
      // Read until the request ends
      String currentLine = firstLine;
      String previousLine = "";
      while (true) {
        previousLine = currentLine;
        currentLine = readLine(&client);

        // Two empty lines, means the request finished
        if (currentLine.isEmpty() && previousLine.isEmpty()) {
          break;
        }
      }

      if (firstLine.indexOf("GET /save") == 0) {
        userConfig = http.parseQueryString(firstLine);

        if (isConfigValid(userConfig)) {
          eeprom.update(userConfig);
          saved(client);
        } else {
          homepage(client);
        }
      } else {
        homepage(client);
      }
      client.stop();
      Serial.println("Request closed");
      return userConfig;
    }
  }

  return userConfig;
}

bool R4WifiManager::isConfigValid(const Hashtable<String, String>& config) {
  return config.containsKey(R4WifiManagerConstants::NETWORK_KEY) &&
         config.containsKey(R4WifiManagerConstants::PASSWORD_KEY) &&
         config.containsKey(R4WifiManagerConstants::DEVICE_ID_KEY);
}

String R4WifiManager::readLine(WiFiClient* client) {
  String line = "";
  while (client->available()) {
    char c = client->read();
    if (c == '\n') {
      return line;
    } else if (c != '\r') {
      line += c;
    }
  }

  return line;
}

void R4WifiManager::homepage(WiFiClient& client) {
  // This is just a minified version of homepage.html in this repo. I'm going to
  // try to keep them in sync, but I'm sure I'm going to fail. I need to find a
  // better way to do this
  String homepage =
      "<!DOCTYPE html> <html> <body> <meta name='viewport' "
      "content='width=device-width, initial-scale=1.0'> <style> .f { display: "
      "block; float: left; } .c { overflow: auto; border-bottom: 1px solid "
      "#000; padding-bottom: 5px; } .l, .t { margin-right: 2px; } .t { width: "
      "150px; } </style> <h1>Configure device</h1> <form action='/save'> <div "
      "id='fs'> <div class='c'> <div class='f'> <label class='l'>Wifi "
      "Network:</label> <input class='t' name='network'> </div> <div "
      "class='f'> <label class='l'>Wifi Password:</label> <input class='t' "
      "name='password'> </div> <div class='f'> <label class='l'>Device "
      "key:</label> <input class='t' name='key'> </div> </div> </div> <div "
      "style='margin-top: 10px'> <input type='submit' value='Submit'> </div> "
      "</form> </body> </html>";
  http.renderHtml(client, homepage);
}

void R4WifiManager::saved(WiFiClient& client) {
  // This is just a minified version of saved.html in this repo. I'm going to
  // try to keep them in sync, but I'm sure I'm going to fail. I need to find a
  // better way to do this
  String saved =
      "<!DOCTYPE html> <html> <body> <meta name='viewport' "
      "content='width=device-width, initial-scale=1.0'> <h1>Values saved</h1> "
      "</body> </html>";
  http.renderHtml(client, saved);
}
