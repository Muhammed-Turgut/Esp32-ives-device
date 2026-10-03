#include "presantation/ui_common.h"
#include "assets/images/image_ButtonLeft_bits.h"
#include "assets/images/image_ButtonRight_bits.h"
#include "assets/animations/settings_64_64_28f.h"

void drawSettingsView() {
  tft.fillScreen(0x0);
  tft.drawRect(1, 0, 127, 128, 0x8E09);
  tft.setTextColor(0xFFFF);
  tft.setTextSize(1);
  uiPrint(44, 111, "Settings");
  tft.drawBitmap(16, 112, image_ButtonLeft_bits, 4, 7, 0xFFFF);
  tft.drawBitmap(112, 112, image_ButtonRight_bits, 4, 7, 0xFFFF);
  tft.fillRect(1, 0, 128, 16, 0x8E09);
  tft.setTextColor(0x0);
  uiPrint(5, 5, "ESP32 ives Device");
  settings_64_64_28f_frame = -1;
  tickSettingsAnim();
}

void tickSettingsAnim() {
  int frame = (millis() / 42) % 28;
  if (frame == settings_64_64_28f_frame) {
    return;
  }
  settings_64_64_28f_frame = frame;
  tft.drawBitmap(36, 34, settings_64_64_28f_frames[frame], 64, 64, 0x8E09, 0x0000);
}
