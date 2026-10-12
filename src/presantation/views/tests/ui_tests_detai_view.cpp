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
  tft.fillRoundRect(77, 112, 47, 12, 5, 0x8e09);
  tft.drawRoundRect(77, 112, 47, 12, 5, 0x8e09);

  // Metin
  tft.setTextColor(0x0000);
  tft.setTextSize(1);
  uiPrint(83,14,"select");

  tft.setTextColor(0x8e09);
  tft.setTextSize(1);
  uiPrint(7,114,"Back");

  // Dikdörtgen
  tft.drawRect(0, 0, 128, 22, 0x8e09);

  // Metin
  tft.setTextColor(0x8e09);
  tft.setTextSize(1);
  uiPrint(27,7,"Tests");

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

  uint16_t isActiveColor = 0x8E09;
  uint16_t isDeActiveColor = 0x5AEC;
  
  tft.setTextColor(chooseIndex == 0 ? isActiveColor : isDeActiveColor);
    tft.setTextSize(1);
    uiPrint(12, 31,"RF-1 Test");
    
    tft.setTextColor(chooseIndex == 1 ? isActiveColor : isDeActiveColor);
    uiPrint(12, 47, "RF-2 Test");
    
    tft.setTextColor(chooseIndex == 2 ? isActiveColor : isDeActiveColor);
    uiPrint(12, 63, "GPS Test");
    // string 43
    tft.setTextColor(chooseIndex == 3 ? isActiveColor : isDeActiveColor);
    uiPrint(12, 79,"SD Module Test");
    // ButtonRight
    tft.drawBitmap(5, rightIconLocation, image_ButtonRight_bits, 4, 7, 0x8E09);

}