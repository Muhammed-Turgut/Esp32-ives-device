#include <Arduino.h>
#include <SPI.h>
#include <SD.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h>

#define TFT_CS  15
#define TFT_DC  21
#define TFT_RST 22
#define TFT_LED 25
#define SD_CS   5

#define SPI_SCK  18
#define SPI_MISO 19
#define SPI_MOSI 23

Adafruit_ST7735 tft(TFT_CS, TFT_DC, TFT_RST);
int lineY = 4;

void logLine(const char* msg, uint16_t color = ST77XX_WHITE) {
  tft.setCursor(2, lineY);
  tft.setTextColor(color);
  tft.setTextSize(1);
  tft.print(msg);
  lineY += 10;
  Serial.println(msg);
}

void parkSpi() {
  const uint8_t highPins[] = {SD_CS, TFT_CS, 4, 27, 32, 33};
  for (uint8_t p : highPins) {
    pinMode(p, OUTPUT);
    digitalWrite(p, HIGH);
  }
}

void setup() {
  Serial.begin(115200);
  parkSpi();

  pinMode(TFT_LED, OUTPUT);
  digitalWrite(TFT_LED, HIGH);
  tft.initR(INITR_144GREENTAB);
  tft.setRotation(0);
  tft.fillScreen(ST77XX_BLACK);
  logLine("SD yaz / oku");

  digitalWrite(TFT_CS, HIGH);
  SPI.begin(SPI_SCK, SPI_MISO, SPI_MOSI, SD_CS);

  if (!SD.begin(SD_CS, SPI, 1000000, "/sd", 5, true)) {
    logLine("SD.begin YOK", ST77XX_RED);
    return;
  }
  logLine("SD.begin VAR", ST77XX_GREEN);

  File f = SD.open("/ives.txt", FILE_WRITE);
  if (!f) {
    logLine("yazma YOK", ST77XX_RED);
    return;
  }
  f.println("Merhaba IVES");
  f.close();
  logLine("yazildi");

  f = SD.open("/ives.txt");
  if (!f) {
    logLine("okuma YOK", ST77XX_RED);
    return;
  }
  String s = f.readStringUntil('\n');
  s.trim();
  f.close();
  logLine("okunan:", ST77XX_CYAN);
  logLine(s.c_str(), ST77XX_CYAN);

  if (s == "Merhaba IVES") {
    logLine("eslesme TAMAM", ST77XX_GREEN);
  } else {
    logLine("eslesme YOK", ST77XX_YELLOW);
  }
}

void loop() {
}
