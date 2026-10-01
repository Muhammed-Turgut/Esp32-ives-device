#include <Arduino.h>
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h>
#include <stack>
#include "presantation/views/ui.h"
using namespace std;

#define TFT_CS  15
#define TFT_DC  21
#define TFT_RST 22
#define TFT_LED 25
#define SD_CS   5

#define BTN_LEFT  34
#define BTN_RIGHT 39
#define BTN_OK 35 // ok buttonu


std::stack<int> screenStack;  

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
  selecteMenuView(screen);
}

void setup() {
  parkSpi();
  pinMode(TFT_LED, OUTPUT);
  digitalWrite(TFT_LED, HIGH);
  pinMode(BTN_LEFT, INPUT);
  pinMode(BTN_RIGHT, INPUT);
  pinMode(BTN_OK, INPUT);
  Serial.begin(115200);

  tft.initR(INITR_144GREENTAB);
  tft.setRotation(0);
  showScreen();
}

void loop() {

  static bool leftWas = false;
  static bool rightWas = false;
  static bool btnOkWas = false;

  bool left = digitalRead(BTN_LEFT);
  bool right = digitalRead(BTN_RIGHT);
  bool btnOk = digitalRead(BTN_OK);

  if (left && !leftWas) {
    screen = (screen + SCREEN_COUNT - 1) % SCREEN_COUNT;
    selecteMenuView(screen);
    tft.fillRect(31, 32, 69, 66, 0x0);
  }
  if (right && !rightWas) {
    screen = (screen + 1) % SCREEN_COUNT;
    selecteMenuView(screen);
    tft.fillRect(31, 32, 69, 66, 0x0);
  }
  if(btnOk && !btnOkWas){
    screenStack.push(screen);
    Serial.println("buttona tiklandi");
    Serial.println(screenStack.top());

    
  }

  leftWas = left;
  rightWas = right;

  selecteMenuAnimations(screen);

  delay(20);

}
