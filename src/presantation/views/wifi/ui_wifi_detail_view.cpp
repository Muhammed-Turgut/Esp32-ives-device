#include "assets/images/image_wifi_bits.h"
#include "presantation/ui_common.h"

void drawScreenWifiDetailView() {
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
// [END lopaka generated]
