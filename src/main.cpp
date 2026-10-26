#include <Arduino.h>
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h>
#include <stack>
#include "presantation/views/ui.h"
#include "presantation/ui_common.h"
#include "presantation/views/wifi/wifi.h"
#include <BLEDevice.h> 
#include "presantation/views/bluetooth/server_call_back.h"
#include "gps.h"
#include "core/buttons.h"
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

static BLECharacteristic* isi = nullptr;
static ServerCallBack info;

Gps gps;
WifiRadio wifiRadio;
UiButtonStruct buttonObject;

uint16_t isActiveColor = 0x8E09;
uint16_t isDeActiveColor = 0x5AEC;

static int screen = 0;
const int SCREEN_COUNT = 5;

static int wifiChooseIndex = -1; // wifi detay ekranında hangi menü seçeneğini seçtiğimizi gösteriyor.
static int BluetoothChooseIndex = 0; // bluetooth detay ekranında hangi menü seçeneğini seçtiğimizi gösteriyor.
static int TestChooseIndex = 0; // Test detay ekranında hangi menü seçeneğini seçtiğimizi gösteriyor.

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
        
        drawScreenBluetoothDetailView(BluetoothChooseIndex);

        break;

        case 3:
        drawTestScreenDetailView(TestChooseIndex);
        break;

        case 4:
        //Settings özellikleri için ekranda listeleme yapılacak
        //ui_settings_detail_view();
        break;

      }


  }
}


void drawLoopView(std::function<void(int index)> drawView, bool lastNav, int stackNumber, int& indexChoose) {

  //Tek noktadan ekranları çizdirimek için
  if (!screenStack.empty() && screenStack.top() == stackNumber) {
  if (millis() - lastNav < 180) {
    // yut, artırma
  } else if (buttons.downPressed) {
    indexChoose = (indexChoose + 1) % 4;
    lastNav = millis();
    drawView(indexChoose);
  } else if (buttons.upPressed) {
    indexChoose = (indexChoose + 4 - 1) % 4;
    lastNav = millis();
    drawView(indexChoose);
  }
} 
}



void setup() {
  parkSpi();
  pinMode(TFT_LED, OUTPUT);
  digitalWrite(TFT_LED, HIGH);

  buttonsBegin();

  //bluetooth ayarları
  BLEDevice::init("Merhaba ben bluetooth"); //Burası cihaza isim verme alanı.

  BLEServer* server = BLEDevice::createServer(); 
  server->setCallbacks(&info);

  BLEService* service = server->createService(BLEUUID((uint16_t)0x180F));
  isi = service->createCharacteristic(
  BLEUUID((uint16_t)0x2A6E),
  BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_NOTIFY);
  isi->setValue("23.5");
  service->start();

  BLEAdvertising* reklam = BLEDevice::getAdvertising();
  reklam->addServiceUUID(BLEUUID((uint16_t)0x180F));
  reklam->setScanResponse(true);

  Serial.begin(115200);
  Serial2.begin(9600, SERIAL_8N1, 16, 17);

  tft.initR(INITR_144GREENTAB);
  tft.setRotation(0);

showScreen(screenStack);

}

void loop() {
  buttonsUpdate();
  static unsigned long lastNav = 0;
  static unsigned long lastWifiDraw =0;


 if (buttons.okPressed) {
  if (screenStack.empty()) {
    screenStack.push(screen);
    if (screen == 2) {
      BLEDevice::startAdvertising(); // Bluetooth sistemini balatan katman
    }
    showScreen(screenStack);
  }
}

if (buttons.backPressed) {
  if (!screenStack.empty()) {
    if (screenStack.top() == 2) {
      BLEDevice::stopAdvertising(); //Bluetooth sistemini reklam yanni ben burdayım yayınını durduran katman
    }
    wifiChooseIndex =-1;
    screenStack.pop();
    showScreen(screenStack);
  }
}

  if(screenStack.empty()){

  if (buttons.leftPressed && screenStack.empty()) {
    screen = (screen + SCREEN_COUNT - 1) % SCREEN_COUNT;
    selecteMenuView(screen);
    tft.fillRect(31, 32, 69, 66, 0x0);
  }

  if (buttons.rightPressed && screenStack.empty()) {
    screen = (screen + 1) % SCREEN_COUNT;
    selecteMenuView(screen);
    tft.fillRect(31, 32, 69, 66, 0x0);
  }
    selecteMenuAnimations(screen);
  }



  if (buttons.okPressed) {
  buttons.okPressed = false;

  if (screenStack.empty()) {
    screenStack.push(screen);
    if (screen == 2) BLEDevice::startAdvertising();
    showScreen(screenStack);
    return;
  }

  switch (screenStack.top()) {
    case 1: // WiFi menüsü

      switch (wifiChooseIndex) {
        case 0: screenStack.push(11); wifiRadio.startScan(); drawWifiScanningView(); break;
        case 1: screenStack.push(12); drawWifiEvilTwinView(); break;
        case 2: screenStack.push(13); drawWifiDeauthenticationView(); break;
        case 3: screenStack.push(14); drawWifiPacketSniffingView(); break;
      }

    break;

    case 2: // Bluetooth menüsü
      switch (BluetoothChooseIndex) {
        case 0: /* BLE Spam */ break;
        case 1: /* HID */ break;
        case 2: /* Sniffing */ break;
        case 3: /* Beacon */ break;
      }
      break;

    case 3: // Test menüsü
      switch (TestChooseIndex) {
        case 0: /* RF-1 */ break;
        case 1: /* RF-2 */ break;
        case 2: /* GPS Test */ break;
        case 3: /* SD Test */ break;
      }
      break;
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

  if (!screenStack.empty() && screenStack.top() == 11) {
  if (buttons.leftPressed || buttons.rightPressed || millis() - lastWifiDraw > 500) {
  drawWifiScanningView();
  lastWifiDraw = millis();
}
}


  //oid drawLoopView(function<void(int index)> drawView,bool lastNav, bool btnDown, bool downWas, bool btnUp, bool upWas, int stackNumber, int index)
  //Wifi ekranını yöneten alan
  drawLoopView(static_cast<void(*)(int)>(drawScreenWifiDetailView), lastNav, 1, wifiChooseIndex);
   
  //Bluetooth ekranının yöneten alan 
  drawLoopView(static_cast<void(*)(int)>(drawScreenBluetoothDetailView), lastNav,2, BluetoothChooseIndex);

  drawLoopView(static_cast<void(*)(int)>(drawTestScreenDetailView), lastNav, 3, TestChooseIndex);

  delay(20);


}
