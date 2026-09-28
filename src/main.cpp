#include <Arduino.h>
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h>
#include "ui_common.h"

#define TFT_CS  15
#define TFT_DC  21
#define TFT_RST 22
#define TFT_LED 25
#define SD_CS   5

#define BTN_LEFT  34
#define BTN_RIGHT 39

Adafruit_ST7735 tft(TFT_CS, TFT_DC, TFT_RST);

static int screen = 0;
const int SCREEN_COUNT = 5;

void parkSpi() {
  const uint8_t highPins[] = {SD_CS, TFT_CS, 4, 27, 32, 33};
  for (uint8_t p : highPins) {
    pinMode(p, OUTPUT);
    digitalWrite(p, HIGH);
  }
}

void showScreen() {
  switch (screen) {
    case 0:
      drawGpsView();
      break;
    case 1:
      drawWifiView();
      break;
    case 2:
      drawBluetoothView();
      break;
    case 3:
      drawTestView();
      break;
    default:
      drawSettingsView();
      break;
  }
}

void setup() {
  parkSpi();
  pinMode(TFT_LED, OUTPUT);
  digitalWrite(TFT_LED, HIGH);
  pinMode(BTN_LEFT, INPUT);
  pinMode(BTN_RIGHT, INPUT);

  tft.initR(INITR_144GREENTAB);
  tft.setRotation(0);
  showScreen();
}

void loop() {
  static bool leftWas = false;
  static bool rightWas = false;
  bool left = digitalRead(BTN_LEFT);
  bool right = digitalRead(BTN_RIGHT);

  if (left && !leftWas) {
    screen = (screen + SCREEN_COUNT - 1) % SCREEN_COUNT;
    showScreen();
  }
  if (right && !rightWas) {
    screen = (screen + 1) % SCREEN_COUNT;
    showScreen();
  }
  leftWas = left;
  rightWas = right;

  switch (screen) {
    case 0:
      tickGpsAnim();
      break;
    case 1:
      tickWifiAnim();
      break;
    case 3:
      tickTestAnim();
      break;
    case 4:
      tickSettingsAnim();
      break;
    default:
      break;
  }
  delay(20);
}
