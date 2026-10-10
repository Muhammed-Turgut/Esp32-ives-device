#pragma once
#include <Arduino.h>
#include <Adafruit_ST7735.h>
#include "gps.h"

extern Gps gps;
extern Adafruit_ST7735 tft;

inline void uiPrint(int16_t x, int16_t y, const char* s) {
  tft.setCursor(x, y);
  tft.print(s);
}


//drawScreenView

//Gps ve detay ekranları burda tutuluyor
void drawGpsView();
void drawScreenGPSDetailView();
void drawScreenNavigasyonCursorCompanenet(float courseDeg);
void drawScreenGPSFrameView();
void drawScreenCompassCompanent();
void drawCompanentNumberOfDetectedSatellites(uint32_t count);
void drawCompanentLatituedAndLongitued(double latitude, double longitude);

//wifi detay ekrnındaki elemanları ekrana çizdiren fonksiyon
void drawWifiView();
void drawScreenWifiDetailView(int selectChoose);
void drawScreenWifiFrameView();
void drawWifiChooseMenu(int chooseIndex);
void drawScreenWifiDetailView();

//Bluetooth için detay ekranındaki elemanları ekrana çizdiren fonskiyon
void drawBluetoothView();
void drawScreenBluetoothDetailView(int index);
void drawBluetoothFramView();
void drawBluetoothChoosMenu(int chooseIndex);

void drawTestView();

void drawSettingsView();

//ortak fonksiyonlar
void drawIconFieldCompanenet(const unsigned char *icon);


//animations
void tickGpsAnim();
void tickWifiAnim();
void tickTestAnim();
void tickSettingsAnim();
