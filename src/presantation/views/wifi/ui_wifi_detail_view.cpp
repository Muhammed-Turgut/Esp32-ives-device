#include "assets/images/image_wifi_bits.h"
#include "assets/images/wifi_deauthentication_icon_bits.h"
#include "assets/images/wifi_evil_twin_icon_bits.h"
#include "assets/images/wifi_packet_sniffing_icon_bits.h"
#include "assets/images/wifi_scanning_icon_bits.h"
#include "assets/images/image_ButtonRight_bits.h"
#include "assets/images/image_ButtonLeft_bits.h"
#include "assets/images/image_ButtonRight_bits.h"
#include "presantation/ui_common.h"
#include <WiFi.h>
#include "core/buttons.h"

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

    if(buttons.leftPressed){
        Serial.println("Left Tıklması");
       wifiIndex = (wifiIndex + wifiRadio.count()  - 1) % wifiRadio.count();
    }
    if (buttons.rightPressed)
    {
      Serial.println("Right Tıklması");
      wifiIndex = (wifiIndex + 1) % wifiRadio.count() ;
    }
    

  tft.fillScreen(0);
  // Dikdörtgen
  tft.drawRect(0, 0, 128, 128, 0x8e09);
  // Dikdörtgen
  tft.fillRoundRect(77, 112, 47, 12, 5, 0x8e09);
  tft.drawRoundRect(77, 112, 47, 12, 5, 0x8e09);
  uiPrint(86,114,"Start",0x0000);
  uiPrint(7,114,"Back",0x8e09);
  tft.drawRect(0, 0, 128, 22, 0x8e09);
  uiPrint(7,7,"Wifi Scanning",0x8e09);

  
  if (wifiRadio.scanDone()) {

    char buf[16], ssid[33], bssid[18], channel[8], rssi[16], securtyProtocol[16];

    snprintf(buf, sizeof(buf), "%d", wifiRadio.count());
    snprintf(ssid, sizeof(ssid), "%s", wifiRadio.ssid(wifiIndex));
   snprintf(bssid, sizeof(bssid), "%s", wifiRadio.bssid(wifiIndex).c_str());
    snprintf(channel, sizeof(channel),"%d",wifiRadio.channel(wifiIndex));
    snprintf(rssi, sizeof(rssi), "%d",wifiRadio.rssi(wifiIndex));
   // snprintf(securtyProtocol, sizeof(securtyProtocol), "%S",wifiRadio.securityProtocol(wifiIndex));
    

   Serial.println(bssid);
   Serial.println(wifiIndex);


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
    
  }
  else{
  tft.fillScreen(0);

  uiPrint(34,53,"no network",0xf800);
  // Metin
 
  uiPrint(28,64,"searching...",0x7bef);


  }
  

  // Angle right
  tft.drawBitmap(16, 100, image_ButtonLeft_bits, 4, 7, 0xFFFF);
  tft.drawBitmap(112, 100, image_ButtonRight_bits, 4, 7, 0xFFFF);

}

void drawWifiEvilTwinView() {
    tft.fillScreen(0x0);
    uiPrint( 27, 8,"Evil Twin",0x8E09);
}

void drawWifiDeauthenticationView() {
    tft.fillScreen(0x0);
    uiPrint( 27, 8,"Deauthentication",0x8E09);
}


void drawWifiPacketSniffingView() {
    tft.fillScreen(0x0);
    uiPrint( 27, 8,"Packet Sniffing",0x8E09);
}
