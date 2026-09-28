#pragma once
#include <Arduino.h>
#include <Adafruit_ST7735.h>

extern Adafruit_ST7735 tft;

inline void uiPrint(int16_t x, int16_t y, const char* s) {
  tft.setCursor(x, y);
  tft.print(s);
}

void drawGpsView();
void drawWifiView();
void drawBluetoothView();
void drawTestView();
void drawSettingsView();

void tickGpsAnim();
void tickWifiAnim();
void tickTestAnim();
void tickSettingsAnim();
