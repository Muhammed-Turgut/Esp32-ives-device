#include "assets/images/image_wifi_bits.h"
#include "assets/images/wifi_deauthentication_icon_bits.h"
#include "assets/images/wifi_evil_twin_icon_bits.h"
#include "assets/images/wifi_packet_sniffing_icon_bits.h"
#include "assets/images/wifi_scanning_icon_bits.h"
#include "assets/images/image_ButtonRight_bits.h"
#include "assets/images/image_ButtonLeft_bits.h"
#include "assets/images/image_ButtonRight_bits.h"
#include "assets/images/angle_down_bits.h"
#include "assets/images/angle_up_bits.h"
#include "presantation/ui_common.h"
#include <WiFi.h>
#include "core/buttons.h"


int evilTwinCount;
int evilTwinSelectedChoos;
bool evilTwinisSelected = false;

void drawScreenWifiDetailView(int selectChoose) {
    
    if(selectChoose ==-1){
        selectChoose = 0;
    }
    drawScreenFrameView("Wifi", image_wifi_bits);
    drawWifiChooseMenu(selectChoose);
}

void drawWifiChooseMenu(int chooseIndex) {

    int rightIconLocation = 31;
    const unsigned char* icon;
    
    switch (chooseIndex)
    {
    case 0:
     rightIconLocation = 31;
     icon = wifi_scanning_icon_bits;

    break;
    
    case 1:
     rightIconLocation = 47;
     icon = wifi_evil_twin_icon_bits;
    break;

    case 2:
     rightIconLocation = 63;
     icon = wifi_deauthentication_icon_bits;
    break;

    case 3:
     rightIconLocation = 79;
     icon = wifi_packet_sniffing_icon_bits;
    break;

    default:
    break;

    }

    drawIconFieldCompanenet(icon);

    uiPrint(12, 31,"Scanning",chooseIndex == 0 ? isActiveColor : isDeActiveColor);
    uiPrint(12, 47, "Evil Twin",chooseIndex == 1 ? isActiveColor : isDeActiveColor);
    uiPrint(12, 63, "Deauthentication",chooseIndex == 2 ? isActiveColor : isDeActiveColor);
    uiPrint(12, 79,"Packet Sniffing",chooseIndex == 3 ? isActiveColor : isDeActiveColor);
    // ButtonRight
    tft.drawBitmap(5, rightIconLocation, image_ButtonRight_bits, 4, 7, 0x8E09);

}

static int wifiIndex = 0;

void drawWifiScanningView(){

    char keep[18] = "";
if (wifiRadio.count() > 0 && wifiIndex >= 0 && wifiIndex < wifiRadio.count()) {
  strncpy(keep, wifiRadio.bssid(wifiIndex).c_str(), sizeof(keep) - 1);
}

if (wifiRadio.scanDone() && keep[0] != '\0') {
  for (int i = 0; i < wifiRadio.count(); i++) {
    if (strcmp(wifiRadio.bssid(i).c_str(), keep) == 0) {
      wifiIndex = i;
      break;
    }
  }
}
wifiRadio.startScan();

if (wifiRadio.count() > 0 && wifiIndex >= wifiRadio.count()) {
  wifiIndex = wifiRadio.count() - 1;
}


    if(wifiRadio.count() > 0 && buttons.leftPressed){
        Serial.println("Left Tıklması");
       wifiIndex = (wifiIndex + wifiRadio.count()  - 1) % wifiRadio.count();
    }

    if (wifiRadio.count() > 0 && buttons.rightPressed)
    {
      Serial.println("Right Tıklması");
      wifiIndex = (wifiIndex + 1) % wifiRadio.count() ;
    }
    

  tft.fillScreen(0);
  // Dikdörtgen
  tft.drawRect(0, 0, 128, 128, 0x8e09);
  uiPrint(7,114,"Back",0x8e09);

  tft.fillRoundRect(77, 112, 47, 12, 5, 0x8e09);
  tft.drawRoundRect(77, 112, 47, 12, 5, 0x8e09);
  // Metin
  uiPrint(89,114,"save",0x0000);

  tft.drawRect(0, 0, 128, 22, 0x8e09);
  uiPrint(7,7,"Wifi Scanning",0x8e09);

  
 if (wifiRadio.count() > 0) {

    char buf[16], ssid[33], bssid[18], channel[8], rssi[16];

    snprintf(buf, sizeof(buf), "%d", wifiRadio.count());
    snprintf(ssid, sizeof(ssid), "%s", wifiRadio.ssid(wifiIndex).c_str());
    snprintf(bssid, sizeof(bssid), "%s", wifiRadio.bssid(wifiIndex).c_str());
    snprintf(channel, sizeof(channel),"%d",wifiRadio.channel(wifiIndex));
    snprintf(rssi, sizeof(rssi), "%d",wifiRadio.rssi(wifiIndex));
    

    uiPrint(110,7, buf,0xffff); //ortamdaki ağ sayısı

    uiPrint(4,25,"ssid:",0xf800);
    uiPrint(34,25,ssid,0x7bef);
    
    uiPrint(4,36,"bssid:",0xf800);
    uiPrint(4,47,bssid,0x7bef);
    
    uiPrint(4,56,"channel:",0xf800);
    uiPrint(52,56,channel,0x7bef);

    uiPrint(4,68,"rssi:",0xf800);
    uiPrint(34,68,rssi,0xfc00);
    
    
    uiPrint(4,79,"security:",0xf800);
    uiPrint(58, 79, wifiRadio.securityProtocol(wifiIndex), 0x7bef);

      // Angle right
    tft.drawBitmap(8, 100, image_ButtonLeft_bits, 4, 7, 0xFFFF);
    tft.drawBitmap(114, 100, image_ButtonRight_bits, 4, 7, 0xFFFF);
    
  }
  else{
  tft.fillScreen(0);

  uiPrint(34,53,"no network",0xf800);
  // Metin
 
  uiPrint(28,64,"searching...",0x7bef);

  }
}

void drawWifiEvilTwinView() {

   int evilTwin = true;
  if(buttons.okPressed && evilTwin){
    evilTwinisSelected = true;
  }

  if(evilTwinisSelected){

   evilTwinSelectCountNetwork();

  }else{
    tft.fillScreen(0x0);
    // Dikdörtgen
    tft.drawRect(0, 0, 128, 128, 0x8e09);

    // Dikdörtgen
    tft.drawRect(0, 0, 128, 22, 0x8e09);

    // Metin
    uiPrint(7,7,"Evil Twin",0x8e09);

    uiButton(buttonObject.setSize(47, 12)
       .setButtonPos(77, 112)
       .setButtonColor(0x8e09)
       .setTextPos(86, 113)
       .setText("Start")
       .setTextColor(0x0000));

    // Metin
    uiPrint(7,114,"Back",0x8e09);

  
    // Metin
    uiPrint(10,30,"eduroam",0x8e09);

    // Metin
  
    uiPrint(116,7,"6",1);
    // Chevron right 2
    tft.drawBitmap(-1, 26, image_ButtonRight_bits, 16, 16, 0x8e09);
  }
 
  


}

void evilTwinSelectCountNetwork() {

  //bu fonksiyon eviltwinde seçilen ağ dan kaç kopya üretileceğini seçmeye yarıyor.

  tft.fillScreen(0);
  // Dikdörtgen
  tft.drawRect(0, 0, 128, 128, 0x8e09);
  // Dikdörtgen
  tft.fillRoundRect(67, 112, 58, 12, 5, 0x8e09);
  tft.drawRoundRect(67, 112, 58, 12, 5, 0x8e09);

  // Metin
  uiPrint(83,114,"Start",0x0000);

  // Metin
  uiPrint(7,114,"Back",0x8e09);

  // Metin
  uiPrint(37,8,"Evil Twin",0x8e09);

  // Dikdörtgen
  tft.fillRect(1, 60, 126, 28, 0x8e09);
  tft.drawRect(1, 60, 126, 28, 0x8e09);
  // Dikdörtgen
  tft.fillRoundRect(36, 60, 55, 28, 8, 0x0000);
  tft.drawRoundRect(36, 60, 55, 28, 8, 0x8e09);

  // Metin
  uiPrint(52,66,"10",0x8e09,2);

  // Metin
  uiPrint(13,21,"Please select how",0xf800);

  // Metin
  uiPrint(4,33,"many evil twins will",0xf800);

  // Metin
  uiPrint(34,44,"be created.",0xf800);

  
  // Angle up
  tft.drawBitmap(91, 60, angle_up_bits, 14, 14, 0x0000);
  // Angle down
  tft.drawBitmap(91, 74, angle_down_bits, 14, 14, 0x0000);

}

void drawWifiDeauthenticationView() {
    tft.fillScreen(0x0);
    uiPrint( 27, 8,"Deauthentication",0x8E09);
}


void drawWifiPacketSniffingView() {
    tft.fillScreen(0x0);
    uiPrint( 27, 8,"Packet Sniffing",0x8E09);
}
