#include "wifi.h"

void WifiRadio::begin() {
  WiFi.mode(WIFI_STA);
  WiFi.disconnect(true);
}

void WifiRadio::startScan() {
  networkCount = 0;
  WiFi.mode(WIFI_STA);
  WiFi.disconnect(false);
  WiFi.scanDelete();
  WiFi.scanNetworks(true);
}

bool WifiRadio::scanDone() {
  int n = WiFi.scanComplete();
  if (n < 0) return false;
  networkCount = n;
  return true;
}

int WifiRadio::count() {
  return networkCount;
}

const char* WifiRadio::ssid(int i) {
  return WiFi.SSID(i).c_str();
}

int32_t WifiRadio::rssi(int i) {
  return WiFi.RSSI(i);
}

int32_t WifiRadio::channel(int i) {
  return WiFi.channel(i);
}
