#include "assets/images/image_map_navigation_pin_location_bits.h"
#include "assets/images/image_cursor_bits.h"
#include "assets/images/image_satellite_icon_bits.h"
#include "presantation/ui_common.h"
#include <math.h>




void drawScreenGPSDetailView() {

   drawScreenGPSFrameView();
   drawCompanentNumberOfDetectedSatellites(gps.satellites());

    if (!gps.hasFix())
    {
      tft.setTextColor(0xC0E5);
      uiPrint(44, 52,"no fix");
    // string 25
      tft.setTextColor(0x5AEC);
      uiPrint(24, 62, "Please wait...");
    // string 22
    }

    else {
        drawScreenCompassCompanent();
        drawScreenNavigasyonCursorCompanenet((float)gps.courseDeg());
        drawCompanentLatituedAndLongitued(gps.latitude(), gps.longitude());
    }

}

void drawCompanentNumberOfDetectedSatellites(uint32_t count) {
  tft.drawBitmap(92, 5, image_satellite_icon_bits, 13, 13, 0xFFFF);

  char buf[8];
  snprintf(buf, sizeof(buf), ":%u", (unsigned)count);

  tft.setTextColor(0xFFFF);
  tft.setTextSize(1);
  uiPrint(110, 8, buf);
}

void drawScreenGPSFrameView(){
     tft.fillScreen(0x0);
    tft.drawRect(0, 0, 128, 128, 0x8E09);
    tft.drawRect(0, 0, 128, 22, 0x8E09);
    tft.setTextColor(0x8E09);
    tft.setTextSize(1);
    uiPrint(27, 8, "GPS");
    tft.fillRoundRect(65, 111, 59, 12, 5, 0x8E09);
    tft.setTextColor(0x0);
    uiPrint(72, 113,"look map");
    // string 22
    tft.setTextColor(0x8E09);
    uiPrint(6, 114,"back");
    // map_navigation_pin_location_2__Streamline_Pixel
    tft.drawBitmap(5, 3, image_map_navigation_pin_location_bits, 16, 16, 0x8E09);
}

void drawCompanentLatituedAndLongitued(double latitude, double longitude) {
  char lat[16];
  char lon[16];
  dtostrf(latitude, 0, 6, lat);
  dtostrf(longitude, 0, 6, lon);

  tft.setTextColor(0x8E09);
  tft.setTextSize(1);
  uiPrint(6, 29, "latitude:");
  uiPrint(6, 41, "longitude:");

  tft.setTextColor(0xFFFF);
  uiPrint(61, 29, lat);
  uiPrint(66, 41, lon);
}

void drawScreenCompassCompanent() {
 
    // ellipse 25
    tft.drawEllipse(98, 84, 20, 19, 0x8E09);
    // rect 30
    tft.fillRect(74, 78, 9, 11, 0x0);
    // rect 30
    tft.fillRect(114, 78, 9, 11, 0x0);
    // rect 30
    tft.fillRect(94, 96, 9, 11, 0x0);
    // rect 30
    tft.fillRect(94, 60, 9, 11, 0x0);
    // string 26
    tft.setTextColor(0xFFFF);
    tft.setTextSize(1);
    uiPrint( 96, 62,"N");
    // string 26
    uiPrint( 96, 100,"S");
    // string 26
    uiPrint( 116, 80,"E");
    // string 26
    uiPrint(77, 80,"W");
}

void drawScreenNavigasyonCursorCompanenet(float courseDeg) {
  // Bu fonksiyon oluşturlan pusula alnının ortasıdan yön okunu okuduğu veriden çeviriyor.   
  const int16_t x0 = 93;
  const int16_t y0 = 79;
  const int16_t w = 11;
  const int16_t h = 11;
  const int16_t cx = x0 + w / 2;
  const int16_t cy = y0 + h / 2;
  const float rad = courseDeg * (PI / 180.0f);
  const float c = cosf(rad);
  const float s = sinf(rad);
  tft.fillRect(cx - 8, cy - 8, 17, 17, 0x0000);
  for (int16_t dy = -8; dy <= 8; dy++) {
    for (int16_t dx = -8; dx <= 8; dx++) {
      float sx = dx * c + dy * s + w / 2.0f;
      float sy = -dx * s + dy * c + h / 2.0f;
      int ix = (int)(sx + 0.5f);
      int iy = (int)(sy + 0.5f);
      if (ix < 0 || iy < 0 || ix >= w || iy >= h) {
        continue;
      }
      uint16_t color = pgm_read_word(&image_cursor_bits[iy * w + ix]);
      if (color == 0) {
        continue;
      }
      tft.drawPixel(cx + dx, cy + dy, color);
    }
  }
}

