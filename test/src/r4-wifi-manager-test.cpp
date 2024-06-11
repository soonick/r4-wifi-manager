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

TEST_CASE("getUserConfig") {
  SECTION("Returns nullptr if submitted data is invalid") {
    String request = "GET /save\n\n";
    R4WifiManager wm =
        R4WifiManager(new WiFiServer(80, new WiFiClient(request)));
    Hashtable<String, String>* actual = wm.getUserConfig();
    REQUIRE(actual == nullptr);
  }

  SECTION("Returns data if submitted data is valid") {
    String request =
        "GET /save?network=mynetwork&password=secret&key=somesecret\n\n";
    R4WifiManager wm =
        R4WifiManager(new WiFiServer(80, new WiFiClient(request)));
    Hashtable<String, String>* actual = wm.getUserConfig();
    REQUIRE(actual->elements() == 3);
    REQUIRE(*actual->get("network") == "mynetwork");
    REQUIRE(*actual->get("password") == "secret");
    REQUIRE(*actual->get("key") == "somesecret");
  }
}
