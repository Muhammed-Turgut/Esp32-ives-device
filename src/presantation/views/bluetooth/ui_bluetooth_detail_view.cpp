#include "assets/images/image_bluetooth_bits.h"
#include "assets/images/bluetooth_beacon_tracking_icon_bits.h"
#include "assets/images/bluetooth_ble_spam_icon_bits.h"
#include "assets/images/bluetooth_hid_icon_bits.h.h"
#include "assets/images/bluetooth_sniffing_icon_bits.h"
#include "assets/images/image_ButtonRight_bits.h"
#include "ui_common.h"

void drawScreenBluetoothDetailView(int index){
  drawScreenFrameView("Bluethooth", image_bluetooth_mini_bits);
  drawBluetoothChoosMenu(index);
}

void drawBluetoothChoosMenu(int chooseIndex) {

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

    drawIconFieldCompanenet(icon); //Sağ üsteki iconları çizdiren fonskiyon.
    drawBlueotoothMenuItem(chooseIndex, rightIconLocation); 
}


void drawBlueotoothMenuItem(int& chooseIndex, int& rightIconLocation) {
    uiPrint(12, 31,"BLE Spam",chooseIndex == 0 ? isActiveColor : isDeActiveColor);
    uiPrint(12, 47, "HID",chooseIndex == 1 ? isActiveColor : isDeActiveColor);
    uiPrint(12, 63, "Bluetooth Sniffing",chooseIndex == 2 ? isActiveColor : isDeActiveColor);
    uiPrint(12, 79,"Beacon Tracking",chooseIndex == 3 ? isActiveColor : isDeActiveColor);
    tft.drawBitmap(5, rightIconLocation, image_ButtonRight_bits, 4, 7, 0x8E09);
}