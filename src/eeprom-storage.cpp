#include "r4-wifi-manager/eeprom-storage.hpp"

#include <EEPROM.h>
#include <cstdint>
#include "Arduino.h"
#include "CRC.h"
#include "r4-wifi-manager/constants.hpp"

EepromStorage::EepromStorage() : eeprom{EEPROM} {}

EepromStorage::EepromStorage(EEPROMClass& e) : eeprom{e} {}

Hashtable<String, String> EepromStorage::get() {
  Payload payload(eeprom);
  return payload.getHashtable();
}

String EepromStorage::update(const Hashtable<String, String>& data) {
  if (dataMatchesEeprom(data)) {
    return "";
  }

  const Payload payload = Payload(data, eeprom);
  char* payloadChars = payload.getData();
  const char crc = calcCRC8((const uint8_t*)payloadChars, payload.getSize());

  const unsigned long eepromSize = eeprom.length();
  // The 1 we are adding is for the CRC byte
  if (payload.getSize() + 1 > eepromSize) {
    return "Data too large for eeprom";
  }

  eeprom.update(R4WifiManagerConstants::CRC_POSITION, crc);
  for (unsigned long i = 0; i < payload.getSize(); i++) {
    eeprom.update(R4WifiManagerConstants::PAYLOAD_POSITION + i,
                  payloadChars[i]);
  }

  return "";
}

bool EepromStorage::dataMatchesEeprom(const Hashtable<String, String> data) {
  Hashtable<String, String> current = get();
  const String* currentNetwork =
      current.get(R4WifiManagerConstants::NETWORK_KEY);
  const String* currentPassword =
      current.get(R4WifiManagerConstants::PASSWORD_KEY);
  const String* currentDeviceId =
      current.get(R4WifiManagerConstants::DEVICE_ID_KEY);
  if (currentNetwork == nullptr || currentPassword == nullptr ||
      currentDeviceId == nullptr) {
    return false;
  }

  return *data.get(R4WifiManagerConstants::NETWORK_KEY) == *currentNetwork &&
         *data.get(R4WifiManagerConstants::PASSWORD_KEY) == *currentPassword &&
         *data.get(R4WifiManagerConstants::DEVICE_ID_KEY) == *currentDeviceId;
}

EepromStorage::Payload::Payload(EEPROMClass& e) : eeprom{e} {
  // Figure out the null terminators for the 3 sections
  const int eepromSize = eeprom.length();
  int zeroPositions[3] = {0, 0, 0};
  int zerosCount = 0;
  for (int i = R4WifiManagerConstants::PAYLOAD_POSITION; i < eepromSize; i++) {
    const char currentChar = eeprom.read(i);
    if (currentChar == 0) {
      zeroPositions[zerosCount] = i;
      zerosCount++;
      if (zerosCount == 3) {
        break;
      }
    }
  }

  if (zerosCount != 3) {
    return;
  }

  // Get the payload for the eeprom
  payloadLength =
      zeroPositions[2] + 1 - R4WifiManagerConstants::PAYLOAD_POSITION;
  payload = new char[payloadLength];
  for (unsigned long i = R4WifiManagerConstants::PAYLOAD_POSITION;
       i < payloadLength + R4WifiManagerConstants::PAYLOAD_POSITION; i++) {
    payload[i - R4WifiManagerConstants::PAYLOAD_POSITION] = eeprom.read(i);
  }

  // If CRC is different, it means the data was corrupted or the eeprom was
  // empty
  if (eeprom.read(R4WifiManagerConstants::CRC_POSITION) !=
      calcCRC8((const uint8_t*)payload, payloadLength)) {
    delete[] payload;
    payload = nullptr;
    payloadLength = -1;
  } else {
    char* networkChars = new char[zeroPositions[0] + 1];
    for (int i = 0; i <= zeroPositions[0]; i++) {
      networkChars[i] = payload[i];
    }
    char* passwordChars = new char[zeroPositions[1] - zeroPositions[0]];
    for (int i = 0; i <= zeroPositions[1] - zeroPositions[0]; i++) {
      passwordChars[i] = payload[zeroPositions[0] + i];
    }
    char* deviceIdChars = new char[zeroPositions[2] - zeroPositions[1]];
    for (int i = 0; i <= zeroPositions[2] - zeroPositions[1]; i++) {
      deviceIdChars[i] = payload[zeroPositions[1] + i];
    }

    hashtable.put(R4WifiManagerConstants::NETWORK_KEY, String(networkChars));
    hashtable.put(R4WifiManagerConstants::PASSWORD_KEY, String(passwordChars));
    hashtable.put(R4WifiManagerConstants::DEVICE_ID_KEY, String(deviceIdChars));

    delete[] networkChars;
    delete[] passwordChars;
    delete[] deviceIdChars;
  }
}

EepromStorage::Payload::Payload(const Hashtable<String, String>& in,
                                EEPROMClass& e)
    : eeprom{e} {
  String* network = in.get(R4WifiManagerConstants::NETWORK_KEY);
  String* password = in.get(R4WifiManagerConstants::PASSWORD_KEY);
  String* key = in.get(R4WifiManagerConstants::DEVICE_ID_KEY);
  payloadLength = network->length() + password->length() + key->length() + 3;
  payload = new char[payloadLength];

  unsigned long payloadPos = 0;
  payloadPos = copyStringToCharArr(*network, payload, payloadPos);
  payloadPos = copyStringToCharArr(*password, payload, payloadPos);
  copyStringToCharArr(*key, payload, payloadPos);
}

EepromStorage::Payload::~Payload() {
  delete[] payload;
}

unsigned long EepromStorage::Payload::getSize() const {
  return payloadLength;
}

char* EepromStorage::Payload::getData() const {
  return payload;
}

Hashtable<String, String> EepromStorage::Payload::getHashtable() const {
  return hashtable;
}

unsigned long EepromStorage::Payload::copyStringToCharArr(
    const String& from,
    char* to,
    unsigned long startPos) const {
  int payloadPos = startPos;
  for (unsigned long i = 0; i < from.length(); i++, payloadPos++) {
    to[payloadPos] = from.charAt(i);
  }
  to[payloadPos] = 0;
  payloadPos++;
  return payloadPos;
}
