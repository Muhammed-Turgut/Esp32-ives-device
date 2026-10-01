#include "../ui_common.h"
#include "assets/images/image_ButtonLeft_bits.h"
#include "assets/images/image_ButtonRight_bits.h"
#include "assets/animations/map_marker_64_64_28f.h"

void drawGpsView() {
  tft.fillScreen(0x0);
  tft.drawRect(1, 0, 127, 128, 0x8E09);
  tft.setTextColor(0xFFFF);
  tft.setTextSize(1);
  uiPrint(58, 111, "GPS");
  tft.drawBitmap(16, 112, image_ButtonLeft_bits, 4, 7, 0xFFFF);
  tft.drawBitmap(112, 112, image_ButtonRight_bits, 4, 7, 0xFFFF);
  tft.fillRect(1, 0, 128, 16, 0x8E09);
  tft.setTextColor(0x0);
  uiPrint(5, 5, "ESP32 ives Device");
  map_marker_64_64_28f_frame = -1;
  tickGpsAnim();
}

void tickGpsAnim() {
  
  int frame = (millis() / 42) % 28;
  if (frame == map_marker_64_64_28f_frame) {
    return;
  }
  map_marker_64_64_28f_frame = frame;
  tft.drawBitmap(33, 34, map_marker_64_64_28f_frames[frame], 64, 64, 0x8E09, 0x0000);
}
