#pragma once
#include <WiFi.h>

class WifiRadio {
 public:
  void begin();
  void startScan();
  bool scanDone();
  int count();
 String ssid(int i);
String bssid(int i);
int32_t rssi(int i);
int32_t channel(int i);
const char* securityProtocol(int i);
 private:
  int networkCount = 0;
};