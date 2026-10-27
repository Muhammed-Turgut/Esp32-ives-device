#pragma once
#include <Arduino.h>
#include <Adafruit_ST7735.h>
#include "gps.h"
#include "views/wifi/wifi.h"

extern Gps gps;
extern Adafruit_ST7735 tft;
extern WifiRadio wifiRadio;


extern uint16_t isActiveColor;
extern uint16_t isDeActiveColor;

inline void uiPrint(int16_t x, int16_t y, const char* s, uint16_t color = 0x8e09, int size = 1) {
  tft.setTextColor(color);
  tft.setTextSize(size);
  tft.setCursor(x, y);
  tft.print(s);
}

struct UiButtonStruct {
    const char* text = "text";
    int width = 42;
    int height = 12;
    int buttonX = 82;
    int buttonY = 111;
    int buttonCorner = 5;
    int textX = 86;
    int textY = 113;
    uint16_t textColor = 0x0000;
    uint16_t buttonColor = 0x8e09;

    UiButtonStruct& setText(const char* t)      { text = t; return *this; }
    UiButtonStruct& setSize(int w, int h)       { width = w; height = h; return *this; }
    UiButtonStruct& setButtonPos(int x, int y)        { buttonX = x; buttonY = y; return *this; }
    UiButtonStruct& setButtonCorner(int c)            { buttonCorner = c; return *this; }
    UiButtonStruct& setTextPos(int x, int y)    { textX = x; textY = y; return *this; }
    UiButtonStruct& setTextColor(uint16_t c)    { textColor = c; return *this; }
    UiButtonStruct& setButtonColor(uint16_t c)        { buttonColor = c; return *this; }
};

extern UiButtonStruct buttonObject;


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
void drawWifiScanningView();
void drawWifiEvilTwinView();
void evilTwinSelectCountNetwork();
void drawWifiDeauthenticationView();
void drawWifiPacketSniffingView();

//Bluetooth için detay ekranındaki elemanları ekrana çizdiren fonskiyon
void drawBluetoothView();
void drawScreenBluetoothDetailView(int index);
void drawBluetoothFramView();
void drawBluetoothChoosMenu(int chooseIndex);
void drawBlueotoothMenuItem(int& chooseIndex, int& rightIconLocation);

//BUrası test ekranının companentlerinin bulunduğu alan.
void drawTestView();
void drawTestScreenDetailView(int index);
void drawTestChoosMenu(int &chooseIndex);
void drawTestDetailFrameView();
void drawTestMenuItem(int chooseIndex, int rightIconLocation);

void drawSettingsView();

//ortak fonksiyonlar
void drawIconFieldCompanenet(const unsigned char *icon);
void drawScreenFrameView(const char* frameName, const unsigned char *icon);
void uiButton(const UiButtonStruct& uiButtonStruct);

//animations
void tickGpsAnim();
void tickWifiAnim();
void tickTestAnim();
void tickSettingsAnim();
