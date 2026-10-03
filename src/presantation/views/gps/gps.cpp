#include "gps.h"   // yoluna göre

void Gps::begin() {
  Serial2.begin(9600, SERIAL_8N1, 16, 17);
}

void Gps::update() {
 while (Serial2.available()) {
    char c = Serial2.read();
    parser.encode(c);
    Serial.write(c);   // monitor istiyorsan
  }
}
bool Gps::hasFix() { return parser.location.isValid(); }
double Gps::latitude() { return parser.location.lat(); }
double Gps::longitude() { return parser.location.lng(); }
uint32_t Gps::satellites() { return parser.satellites.value(); }
double Gps::courseDeg() { return parser.course.deg(); }