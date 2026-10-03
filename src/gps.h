#pragma once
#include <TinyGPSPlus.h>  // lib_deps'e TinyGPSPlus eklersen

class Gps {
 public:
  void begin();
  void update();
  bool hasFix();
  double latitude();
  double longitude();
  uint32_t satellites();
  double courseDeg();   // cursor açısı
 private:
  TinyGPSPlus parser;
};