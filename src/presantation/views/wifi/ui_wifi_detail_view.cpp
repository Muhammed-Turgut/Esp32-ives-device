#include "assets/images/image_wifi_bits.h"
#include "assets/images/wifi_deauthentication_icon_bits.h"
#include "assets/images/wifi_evil_twin_icon_bits.h"
#include "assets/images/wifi_packet_sniffing_icon_bits.h"
#include "assets/images/wifi_scanning_icon_bits.h"
#include "assets/images/image_ButtonRight_bits.h"
#include "presantation/ui_common.h"

void drawScreenWifiDetailView(int selectChoose) {
    int WifiScreenCount = 4; // wifi detay eknaında kaç tane seçenek olduğunu tutuyor.

    drawScreenWifiFrameView();
    drawWifiChooseMenu(selectChoose);

}

void drawWifiChooseMenu(int chooseIndex) {

    uint16_t isActiveColor = 0x8E09;
    uint16_t isDeActiveColor = 0x5AEC;

    int rightIconLocation;
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

    tft.setTextColor(chooseIndex == 0 ? isActiveColor : isDeActiveColor);
    tft.setTextSize(1);
    uiPrint(12, 31,"Scanning");
    
    tft.setTextColor(chooseIndex == 1 ? isActiveColor : isDeActiveColor);
    uiPrint(12, 47, "Evil Twin");
    
    tft.setTextColor(chooseIndex == 2 ? isActiveColor : isDeActiveColor);
    uiPrint(12, 63, "Deauthentication");
    // string 43
    tft.setTextColor(chooseIndex == 3 ? isActiveColor : isDeActiveColor);
    uiPrint(12, 79,"Packet Sniffing");
    // ButtonRight
    tft.drawBitmap(5, rightIconLocation, image_ButtonRight_bits, 4, 7, 0x8E09);

}

void drawScreenWifiFrameView(){
   tft.fillScreen(0x0);
    // rect 1
     tft.drawRect(0, 0, 128, 128, 0x8E09);
    // rect 18
     tft.drawRect(0, 0, 128, 22, 0x8E09);
    // string 19
     tft.setTextColor(0x8E09);
     tft.setTextSize(1);
     uiPrint( 27, 8,"Wifi");
    // wifi
     tft.drawBitmap(4, 3, image_wifi_bits, 19, 16, 0x8E09);
     // rect 21
     tft.fillRoundRect(82, 111, 42, 12, 5, 0x8E09);
    // string 22
     tft.setTextColor(0x0);
     uiPrint( 86, 113,"select");
    // string 22
     tft.setTextColor(0x8E09);
     uiPrint(6, 114,"back");
}

void drawIconFieldCompanenet(const unsigned char *icon){
// ekrana iconları çizen eleman
tft.drawBitmap(90, 24, icon, 32, 32, 0x8E09);

}

