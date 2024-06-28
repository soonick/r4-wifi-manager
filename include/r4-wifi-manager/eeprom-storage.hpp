#pragma once

#include "EEPROM.h"
#include "Hashtable.h"
#include <Arduino.h>

/**
 * Manages eeprom storage for r4-wifi-manager
 * Bytes 0 to 4 are for CRC. At byte 5 we have the network name followed by a
 * 0x0, followed by the password, then another 0x0 and finally the device id,
 * followed by 0x0
 */
class EepromStorage {
 public:
  /**
   * Uses the global EEPROM as the eeprom storage
   */
  EepromStorage();

  /**
   * Uses given object as eeprom storage
   */
  EepromStorage(EEPROMClass &eeprom);

  /**
   * Updates data in eeprom (if the data is the same as the data currently in,
   * no writes are done)
   *
   * @return empty string on success. Error message, otherwise
   */
  String update(const Hashtable<String, String> &data);

  /**
   *  Gets the data from the eeprom
   *
   * @return data stored on the eeprom. If there is no data or it's corrupted,
   *         the hashtable will be empty
   */
  Hashtable<String, String> get();

  /**
   * Resets data in eeprom so `get` returns an empty hashtable
   */
  void reset();

 protected:
  class Payload {
   public:
    Payload(EEPROMClass&);
    Payload(const Hashtable<String, String>&, EEPROMClass&);
    ~Payload();

    unsigned long getSize() const;

    /**
     * Returns a pointer to the internal char array. It will be invalidated when
     * this object is invalidated
     */
    char* getData() const;
    Hashtable<String, String> getHashtable() const;

   private:
    char *payload = nullptr;
    unsigned long payloadLength = -1;
    Hashtable<String, String> hashtable;
    EEPROMClass& eeprom;

    /**
     * Copies the given string to the given char array. A '\0' character will be
     * written at the end
     *
     * @param from - String we will copy from
     * @param to - Char array we will copy to
     * @param startPos - We start writing to `startPos` in `to`
     * @return the position after the last character written
     */
    unsigned long copyStringToCharArr(const String& from, char* to, unsigned long startPos) const;
  };

 private:
  EEPROMClass& eeprom;

  bool dataMatchesEeprom(const Hashtable<String, String>);
  void construct(const EEPROMClass &e);
};
