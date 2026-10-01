#include "../ui_common.h"
#include "assets/images/image_ButtonLeft_bits.h"
#include "assets/images/image_ButtonRight_bits.h"
#include "assets/images/image_bluetooth_bits.h"

void drawBluetoothView() {
  tft.drawRect(1, 0, 127, 128, 0x8E09);
  tft.setTextColor(0xFFFF);
  tft.setTextSize(1);
  uiPrint(41, 111, "Bluetooth");
  tft.drawBitmap(16, 112, image_ButtonLeft_bits, 4, 7, 0xFFFF);
  tft.drawBitmap(112, 112, image_ButtonRight_bits, 4, 7, 0xFFFF);
  tft.fillRect(1, 0, 128, 16, 0x8E09);
  tft.setTextColor(0x0);
  uiPrint(5, 5, "ESP32 ives Device");
  tft.drawBitmap(45, 43, image_bluetooth_bits, 42, 48, 0x8E09);
}
