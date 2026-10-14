#include "assets/images/image_ButtonRight_bits.h"
#include "assets/images/test_icon_bits.h"
#include "ui_common.h"

void drawTestScreenDetailView(int index) {
  drawTestDetailFrameView();
  drawTestChoosMenu(index);

}

void drawTestDetailFrameView() {
 
  tft.fillScreen(0);
  // Dikdörtgen
   tft.drawRect(0, 0, 128, 128, 0x8e09);
  // Dikdörtgen
   tft.fillRoundRect(54, 112, 72, 12, 5, 0x8e09);
   tft.drawRoundRect(54, 112, 72, 12, 5, 0x8e09);
  // Metin
   uiPrint(60,114, "Start Test", 0x0000, 1);

   // Metin
   uiPrint(7,114, "Back", 0x8e09, 1);
  
  // Dikdörtgen
   tft.drawRect(0, 0, 128, 22, 0x8e09);

  // Metin
   uiPrint(27,7, "Tests", 0x8e09, 1);

  // Book open
   tft.drawBitmap(7, 3, test_icon_bits, 16, 16, 0x8e09);

}

void drawTestChoosMenu(int &chooseIndex) {

    int rightIconLocation;
    const unsigned char* icon;

    switch (chooseIndex)
    {
    case 0:
     rightIconLocation = 31;
    break;
    
    case 1:
     rightIconLocation = 43;
    break;

    case 2:
     rightIconLocation = 63;
    break;

    case 3:
     rightIconLocation = 79;
    break;

    default:
    break;
    }

    drawTestMenuItem(chooseIndex, rightIconLocation); 
}

void drawTestMenuItem(int chooseIndex, int rightIconLocation) {
  
    uiPrint(12,31, "RF-1 Test", chooseIndex == 0 ? isActiveColor : isDeActiveColor, 1);
    uiPrint(12,47, "RF-2 Test", chooseIndex == 1 ? isActiveColor : isDeActiveColor, 1);
    uiPrint(12,63, "GPS Test", chooseIndex == 2 ? isActiveColor : isDeActiveColor, 1);
    uiPrint(12,79, "SD Module Test", chooseIndex == 3 ? isActiveColor : isDeActiveColor, 1);

    tft.drawBitmap(5, rightIconLocation, image_ButtonRight_bits, 4, 7, 0x8E09);
}