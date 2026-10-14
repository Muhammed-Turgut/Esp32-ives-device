#include "assets/images/image_wifi_bits.h"
#include "assets/images/wifi_deauthentication_icon_bits.h"
#include "assets/images/wifi_evil_twin_icon_bits.h"
#include "assets/images/wifi_packet_sniffing_icon_bits.h"
#include "assets/images/wifi_scanning_icon_bits.h"
#include "assets/images/image_ButtonRight_bits.h"
#include "presantation/ui_common.h"

void drawScreenWifiDetailView(int selectChoose) {
    drawScreenWifiFrameView();
    drawWifiChooseMenu(selectChoose);
}

void drawWifiChooseMenu(int chooseIndex) {

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

    uiPrint(12, 31,"Scanning",chooseIndex == 0 ? isActiveColor : isDeActiveColor);
    uiPrint(12, 47, "Evil Twin",chooseIndex == 1 ? isActiveColor : isDeActiveColor);
    uiPrint(12, 63, "Deauthentication",chooseIndex == 2 ? isActiveColor : isDeActiveColor);

    uiPrint(12, 79,"Packet Sniffing",chooseIndex == 3 ? isActiveColor : isDeActiveColor);
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
     uiPrint( 27, 8,"Wifi",0x8E09);
    // wifi
     tft.drawBitmap(4, 3, image_wifi_bits, 19, 16, 0x8E09);
     // rect 21
     tft.fillRoundRect(82, 111, 42, 12, 5, 0x8E09);
    
     // string 22
     uiPrint( 86, 113,"select",0x0);
    
     // string 22
     uiPrint(6, 114,"back",0x8E09);
}

void drawIconFieldCompanenet(const unsigned char *icon){
// ekrana iconları çizen eleman
tft.drawBitmap(90, 24, icon, 32, 32, 0x8E09);

}

