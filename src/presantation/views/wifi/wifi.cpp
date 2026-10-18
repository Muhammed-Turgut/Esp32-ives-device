#include "wifi.h"

static const char* authName(wifi_auth_mode_t a) {
  switch (a) {
    case WIFI_AUTH_OPEN:            return "OPEN";
    case WIFI_AUTH_WEP:             return "WEP";
    case WIFI_AUTH_WPA_PSK:         return "WPA-PSK";
    case WIFI_AUTH_WPA2_PSK:        return "WPA2-PSK";
    case WIFI_AUTH_WPA_WPA2_PSK:    return "WPA/WPA2-PSK";
    case WIFI_AUTH_WPA2_ENTERPRISE: return "WPA2-ENT";
    case WIFI_AUTH_WPA3_PSK:        return "WPA3-PSK";
    case WIFI_AUTH_WPA2_WPA3_PSK:   return "WPA2/WPA3-PSK";
    default:                        return "UNKNOWN";
  }
}

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

String WifiRadio::ssid(int i) {
  if (i < 0 || i >= networkCount) return "";
  return WiFi.SSID(i);
}

String WifiRadio::bssid(int i) {
  if (i < 0 || i >= networkCount) return "";
  return WiFi.BSSIDstr(i);   // "AA:BB:CC:DD:EE:FF" gibi gerçek yazı
}

int32_t WifiRadio::rssi(int i) {
  if (i < 0 || i >= networkCount) return 0;
  return WiFi.RSSI(i);
}

int32_t WifiRadio::channel(int i) {
  if (i < 0 || i >= networkCount) return 0;
  return WiFi.channel(i);
}

const char* WifiRadio::securityProtocol(int i) {
  if (i < 0 || i >= networkCount) return "UNKNOWN";
  return authName(WiFi.encryptionType(i));
}