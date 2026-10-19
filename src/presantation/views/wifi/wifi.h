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
  struct Ap {
    char ssid[33];
    char bssid[18];
    char security[16];
    int32_t rssi;
    int32_t channel;
  };
  static const int kMaxAps = 32;
  Ap list[kMaxAps];
  int networkCount = 0;
};