#include "assets/images/image_bluetooth_bits.h"
#include "assets/images/bluetooth_beacon_tracking_icon_bits.h"
#include "assets/images/bluetooth_ble_spam_icon_bits.h"
#include "assets/images/bluetooth_hid_icon_bits.h.h"
#include "assets/images/bluetooth_sniffing_icon_bits.h"
#include "assets/images/image_ButtonRight_bits.h"
#include "ui_common.h"

void drawScreenBluetoothDetailView(int index){
  int bluethoothCount = 4;
  drawBluetoothFramView();

  drawBluetoothChoosMenu(index);

}

void drawBluetoothChoosMenu(int chooseIndex) {

    uint16_t isActiveColor = 0x8E09;
    uint16_t isDeActiveColor = 0x5AEC;

    int rightIconLocation;
    const unsigned char* icon;


    switch (chooseIndex)
    {
    case 0:
     rightIconLocation = 31;
     icon = bluetooth_ble_spam_icon_bits;

    break;
    
    case 1:
     rightIconLocation = 37;
     icon = bluetooth_hid_icon_bits;
    break;

    case 2:
     rightIconLocation = 63;
     icon = bluetooth_sniffing_icon_bits;
    break;

    case 3:
     rightIconLocation = 79;
     icon = bluetooth_beacon_tracking_icon_bits;
    break;

    default:
    break;

    }
    drawIconFieldCompanenet(icon);
    

    tft.setTextColor(chooseIndex == 0 ? isActiveColor : isDeActiveColor);
    tft.setTextSize(1);
    uiPrint(12, 31,"BLE Spam");
    
    tft.setTextColor(chooseIndex == 1 ? isActiveColor : isDeActiveColor);
    uiPrint(12, 47, "HID");
    
    tft.setTextColor(chooseIndex == 2 ? isActiveColor : isDeActiveColor);
    uiPrint(12, 63, "Bluetooth Sniffing");
    // string 43
    tft.setTextColor(chooseIndex == 3 ? isActiveColor : isDeActiveColor);
    uiPrint(12, 79,"Beacon Tracking");
    // ButtonRight
    tft.drawBitmap(5, rightIconLocation, image_ButtonRight_bits, 4, 7, 0x8E09);

}

void drawBluetoothFramView(){



    tft.fillScreen(0x0);
    // rect 1
    tft.drawRect(0, 0, 128, 128, 0x8E09);
    // rect 18
    tft.drawRect(0, 0, 128, 22, 0x8E09);
    // string 19
    tft.setTextColor(0x8E09);
    tft.setTextSize(1);
    uiPrint(24, 8,"Bluethooth");
    // string 22
    uiPrint(7, 115,"back");
    // rect 21
    tft.fillRoundRect(77, 112, 47, 12, 5, 0x8E09);
    // string 22
    tft.setTextColor(0x0);
    uiPrint( 84, 114,"Select");
    // bluetooth
    tft.drawBitmap(6, 3, image_bluetooth_mini_bits, 14, 16, 0x8E09);

    

}

