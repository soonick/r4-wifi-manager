#include <catch.hpp>
#include <r4-wifi-manager/constants.hpp>
#include <r4-wifi-manager/eeprom-storage.hpp>
#include "Arduino.h"
#include "EEPROM.h"
#include "Hashtable.h"

TEST_CASE("get") {
  SECTION("Returns empty hashtable if there is no data") {
    EEPROMClass e;
    EepromStorage eeprom(e);

    eeprom.get();
    const Hashtable<String, String> actual = eeprom.get();
    REQUIRE(actual.elements() == 0);
  }
}

TEST_CASE("update") {
  SECTION("Sets data if it's different") {
    EEPROMClass e;
    EepromStorage eeprom(e);

    Hashtable<String, String> expected;
    expected.put(R4WifiManagerConstants::NETWORK_KEY, "my-network");
    expected.put(R4WifiManagerConstants::PASSWORD_KEY, "secret");
    expected.put(R4WifiManagerConstants::DEVICE_ID_KEY,
                 "abcdefgh-xxx-999900001");

    String res = eeprom.update(expected);
    REQUIRE(res == "");

    const Hashtable<String, String> actual = eeprom.get();
    REQUIRE(*actual.get(R4WifiManagerConstants::NETWORK_KEY) ==
            *expected.get(R4WifiManagerConstants::NETWORK_KEY));
    REQUIRE(*actual.get(R4WifiManagerConstants::PASSWORD_KEY) ==
            *expected.get(R4WifiManagerConstants::PASSWORD_KEY));
    REQUIRE(*actual.get(R4WifiManagerConstants::DEVICE_ID_KEY) ==
            *expected.get(R4WifiManagerConstants::DEVICE_ID_KEY));
  }

  SECTION("Doesn't write to eeprom if data hasn't changed") {
    EEPROMClass e;
    EepromStorage eeprom(e);

    Hashtable<String, String> expected;
    expected.put(R4WifiManagerConstants::NETWORK_KEY, "my-network");
    expected.put(R4WifiManagerConstants::PASSWORD_KEY, "secret");
    expected.put(R4WifiManagerConstants::DEVICE_ID_KEY,
                 "abcdefgh-xxx-999900001");

    String res = eeprom.update(expected);
    unsigned int writesBefore = EEPROM.getWritesPerformed();

    res = eeprom.update(expected);
    REQUIRE(writesBefore == EEPROM.getWritesPerformed());
  }
}
