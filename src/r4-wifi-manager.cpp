#include "r4-wifi-manager/r4-wifi-manager.hpp"

#include <string.h>

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
  this->server.begin();
  this->printStatus();

  return "";
}

Hashtable<String, String>* R4WifiManager::getUserConfig() {
  if (WiFi.status() != WL_IDLE_STATUS) {
    this->status = WiFi.status();

    if (this->status == WL_AP_CONNECTED) {
      Serial.println("Devices connected to AP");
    } else {
      Serial.println("No devices connected to AP");
    }
  }

  return this->handleClientRequest();
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

Hashtable<String, String>* R4WifiManager::handleClientRequest() {
  WiFiClient client = server.available();
  Hashtable<String, String>* userConfig = nullptr;
  if (client) {
    Serial.println("New request");
    while (client.connected()) {
      String firstLine = readLine(client);
      // Read until the request ends
      String currentLine = firstLine;
      String previousLine = "";
      while (true) {
        previousLine = currentLine;
        currentLine = readLine(client);

        // Two empty lines, means the request finished
        if (currentLine.isEmpty() && previousLine.isEmpty()) {
          break;
        }
      }

      if (firstLine.indexOf("GET /save") == 0) {
        userConfig = http.parseQueryString(firstLine);
        saved(client);
        // TODO: Save to eeprom
      } else {
        Serial.println("Other page submitted");
        homepage(client);
      }
      client.stop();
      Serial.println("Request closed");
      return userConfig;
    }
  }

  return userConfig;
}

String R4WifiManager::readLine(WiFiClient client) {
  String line = "";
  while (client.available()) {
    char c = client.read();
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
      "content='width=device-width, initial-scale=1.0'> <script> function "
      "addVariable() { var div = document.createElement('div'); "
      "div.classList.add('c'); div.innerHTML = '<div class=\"f\"><label "
      "class=\"l\">Name:</label> <input name=\"name\" class=\"t\"></div><div "
      "class=\"f\"><label class=\"l\">Value:</label> <input name=\"value\" "
      "class=\"t\"></div>'; document.getElementById('fs').appendChild(div); } "
      "</script> <style> .f { display: block; float: left; } .c { overflow: "
      "auto; border-bottom: 1px solid #000; padding-bottom: 5px; } .l, .t { "
      "margin-right: 2px; } .t { width: 150px; } </style> <h1>Set "
      "variables</h1> <form action='/save'> <div id='fs'> <div class='c'> <div "
      "class='f'> <label class='l'>Name:</label> <input class='t' name='name'> "
      "</div> <div class='f'> <label class='l'>Value:</label> <input class='t' "
      "name='value'> </div> </div> </div> <div style='margin-top: 10px'> "
      "<input type='button' value='Add another variable' "
      "onClick='addVariable()'> <input type='submit' value='Submit'> </div> "
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
