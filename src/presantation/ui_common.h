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

//wifi detay ekrnındaki elemanları ekrana çizdiren fonksiyon
void drawWifiView();
void drawScreenWifiDetailView();

void drawBluetoothView();

void drawTestView();

void drawSettingsView();


//animations
void tickGpsAnim();
void tickWifiAnim();
void tickTestAnim();
void tickSettingsAnim();
