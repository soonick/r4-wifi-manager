#include <catch.hpp>
#include <r4-wifi-manager/r4-wifi-manager.hpp>

TEST_CASE("startAp") {
  R4WifiManager wm;

  SECTION("Fails if pass is shorter than 8 characters") {
    IPAddress ip;
    String actual = wm.startAp("thessid", "pass", ip);

    String expected = "Password must be 8 characters";
    REQUIRE(actual == expected);
  }

  SECTION("Creating AP failed") {
    IPAddress ip;
    String actual = wm.startAp("badssid", "password", ip);

    String expected = "Creating access point failed";
    REQUIRE(actual == expected);
  }

  SECTION("Success") {
    IPAddress ip;
    String actual = wm.startAp("thessid", "password", ip);

    String expected = "";
    REQUIRE(actual == expected);
  }
}
