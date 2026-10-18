#include "presantation/ui_common.h"

void drawIconFieldCompanenet(const unsigned char *icon){
// ekrana iconları çizen eleman
tft.drawBitmap(90, 24, icon, 32, 32, 0x8E09);

}

void drawScreenFrameView(const char* frameName, const unsigned char *icon){
   tft.fillScreen(0x0);
    // rect 1
     tft.drawRect(0, 0, 128, 128, 0x8E09);
    // rect 18
     tft.drawRect(0, 0, 128, 22, 0x8E09);
    
     // string 19
     uiPrint( 27, 8,frameName,0x8E09);
    // wifi
     tft.drawBitmap(4, 3, icon, 19, 16, 0x8E09);
     // rect 21
     tft.fillRoundRect(82, 111, 42, 12, 5, 0x8E09);
    
     // string 22
     uiPrint( 86, 113,"select",0x0);
    
     // string 22
     uiPrint(6, 114,"back",0x8E09);
}