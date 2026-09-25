#pragma once
#include <Adafruit_ST7735.h>

class CustomText {
 public:
  int x, y;
  uint8_t size;
  uint16_t color;
  const char* text;
  Adafruit_ST7735& screen;

  CustomText(
    int x, int y, uint8_t size, uint16_t color, const char* text, Adafruit_ST7735& screen
  ): screen(screen){
  this->x = x,
  this->y = y;
  this->size = size;
  this->color = color;
  this->text = text;

  draw();
};


 void  draw(){
  screen.setCursor(x, y);
  screen.setTextColor(color);
  screen.setTextSize(size);
  screen.print(text);
}
};
