#include "wifi.h"
#include <string.h>


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
  if (WiFi.scanComplete() == WIFI_SCAN_RUNNING) return;
  if (WiFi.getMode() != WIFI_STA) WiFi.mode(WIFI_STA);
  WiFi.scanNetworks(true);
}

bool WifiRadio::scanDone() {
  int n = WiFi.scanComplete();
  if (n < 0) return false;
  if (n > kMaxAps) n = kMaxAps;

  for (int i = 0; i < n; i++) {
  String ssid = WiFi.SSID(i);
  String bssid = WiFi.BSSIDstr(i);
  const char* security = authName(WiFi.encryptionType(i));

  strncpy(list[i].ssid, ssid.c_str(), sizeof(list[i].ssid) - 1);
  list[i].ssid[sizeof(list[i].ssid) - 1] = '\0';

  strncpy(list[i].bssid, bssid.c_str(), sizeof(list[i].bssid) - 1);
  list[i].bssid[sizeof(list[i].bssid) - 1] = '\0';

  strncpy(list[i].security, security, sizeof(list[i].security) - 1);
  list[i].security[sizeof(list[i].security) - 1] = '\0';

  list[i].rssi = WiFi.RSSI(i);
  list[i].channel = WiFi.channel(i);
  }
  networkCount = n;
  return true;
}

int WifiRadio::count() {
  return networkCount;
}

String WifiRadio::ssid(int i) {
  if (i < 0 || i >= networkCount) return "";
  return list[i].ssid;
}

String WifiRadio::bssid(int i) {
  if (i < 0 || i >= networkCount) return "";
  return list[i].bssid;
}

int32_t WifiRadio::rssi(int i) {
  if (i < 0 || i >= networkCount) return 0;
  return list[i].rssi;
}

int32_t WifiRadio::channel(int i) {
  if (i < 0 || i >= networkCount) return 0;
  return list[i].channel;
}

const char* WifiRadio::securityProtocol(int i) {
  if (i < 0 || i >= networkCount) return "UNKNOWN";
  return list[i].security;
}
