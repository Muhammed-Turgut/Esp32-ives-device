#pragma once
#include <WiFi.h>

class WifiRadio {
 public:
  void begin();
  void startScan();
  bool scanDone();
  int count();
  const char* ssid(int i);
  int32_t rssi(int i);
  int32_t channel(int i);
 private:
  int networkCount = 0;
};