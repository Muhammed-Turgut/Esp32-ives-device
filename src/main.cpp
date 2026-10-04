#include <Arduino.h>
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h>
#include <stack>
#include "presantation/views/ui.h"
#include "presantation/ui_common.h"
#include "gps.h"
using namespace std;

#define TFT_CS  15
#define TFT_DC  21
#define TFT_RST 22
#define TFT_LED 25
#define SD_CS   5

#define BTN_LEFT  34 //LEFT
#define BTN_RIGHT 39 //Right
#define BTN_UP  36 //Up
#define BTN_DOWN 14 //Down
#define BTN_OK 35 //ok buttonu
#define BTN_BACK 13 //BACK BUTTON



//Stack tekrarı yasaklamıyor o sorun çzöülmemli
stack<int> screenStack;  

Adafruit_ST7735 tft(TFT_CS, TFT_DC, TFT_RST);
Gps gps;

static int screen = 0;
const int SCREEN_COUNT = 5;

static int wifiChooseIndex = 0; // wifi detay ekranında hangi menü seçeneğini seçtiğimizi gösteriyor.


void parkSpi() {
  const uint8_t highPins[] = {SD_CS, TFT_CS, 4, 27, 32, 33};
  for (uint8_t p : highPins) {
    pinMode(p, OUTPUT);
    digitalWrite(p, HIGH);
  }
}

void showScreen(std::stack<int>& s) {
  if(s.empty()) {
    selecteMenuView(screen); 
      // menü
  } else {
     
      switch(s.top()){

        case 0:
        //Gps Detaylarını içerenbe ekran dönülecek.
        //ui_gps_detail_view();
        drawScreenGPSDetailView();
        break;

        case 1:
        //Wifi özellikleri için detay sayfası listelenecek.
        // ui_wifi_detail_view();
        drawScreenWifiDetailView(wifiChooseIndex);
        break;

        case 2:
        //Bluetooth özellikleri için ekranda listeleme yapıalcak
        //ui_bluetooth_detail_view();
        break;

        case 3:
        //test özellikleri için ekrnada listeleme yapılacak
        //ui_tests_detail_view();
        break;

        case 4:
        //Settings özellikleri için ekranda listeleme yapılacak
        //ui_settings_detail_view();
        break;

      }


  }
}

void setup() {
  parkSpi();
  pinMode(TFT_LED, OUTPUT);
  digitalWrite(TFT_LED, HIGH);

  // Buttonlardan veri okuma noktaları
  pinMode(BTN_LEFT, INPUT);
  pinMode(BTN_RIGHT, INPUT);
  pinMode(BTN_UP, INPUT);
  pinMode(BTN_DOWN, INPUT);
  pinMode(BTN_OK, INPUT);
  pinMode(BTN_BACK, INPUT);

  Serial.begin(115200);
  Serial2.begin(9600, SERIAL_8N1, 16, 17);

  tft.initR(INITR_144GREENTAB);
  tft.setRotation(0);

  showScreen(screenStack);
}

void loop() {

  static bool leftWas = false;
  static bool rightWas = false;
  static bool upWas = false;
  static bool downWas = false;
  static bool btnOkWas = false;
  static bool btnBackWas = false;

  bool left = digitalRead(BTN_LEFT);
  bool right = digitalRead(BTN_RIGHT);
  bool btnUp = digitalRead(BTN_UP);
  bool btnDown = digitalRead(BTN_DOWN);
  bool btnOk = digitalRead(BTN_OK);
  bool btnBack = digitalRead(BTN_BACK);

 
  if(screenStack.empty()){

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
    selecteMenuAnimations(screen);
  }

  if(btnOk && !btnOkWas){
    //Bu if bloğu screen stack boş ise ekelme yapılsın boş 
    //değilse ekleme yepılmasına gerek yoktur

    if (screenStack.empty())
    {
     screenStack.push(screen);
    showScreen(screenStack);
    }

  }

  if(btnBack && !btnBackWas){

    if(!screenStack.empty()){
      screenStack.pop();
      showScreen(screenStack);
    }
    
  }

  static unsigned long lastGpsDraw = 0;
  if (!screenStack.empty() && screenStack.top() == 0) {
          gps.update(); // GPS sürekli update atması için var. 
          if (millis() - lastGpsDraw > 1000) {
           drawScreenGPSDetailView();
           lastGpsDraw = millis();
    }
  }

 
  if (!screenStack.empty() && screenStack.top() == 1) {

  static unsigned long lastNav = 0;
  
  if (millis() - lastNav < 180) {
    // yut, artırma
  } else if (btnDown && !downWas) {
    wifiChooseIndex = (wifiChooseIndex + 1) % 4;
    lastNav = millis();
    drawScreenWifiDetailView(wifiChooseIndex);
  } else if (btnUp && !upWas) {
    wifiChooseIndex = (wifiChooseIndex + 4 - 1) % 4;
    lastNav = millis();
    drawScreenWifiDetailView(wifiChooseIndex);
  }
}

  

  //Bu değişkenelr buttonlara bir kez mi basıldı hala 
  //basılımı buttonlara, bunu denetliyor.
  leftWas = left;
  rightWas = right;
  btnOkWas = btnOk;
  btnBackWas = btnBack;
  upWas = btnUp;
  downWas = btnDown;

  delay(20);


}
