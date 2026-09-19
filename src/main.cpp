/*
  NRF24L01 #1 ALGILAMA TESTI (sadece Serial Monitor)
  ------------------------------------------------------
  Pinler:
    CE  = GPIO 27
    CSN = GPIO 4
    SCK = GPIO 18 (ortak SPI)
    MOSI = GPIO 23 (ortak SPI)
    MISO = GPIO 19 (ortak SPI)

  Serial Monitor'u 115200 baud'da ac, sonucu orada gor.
*/

#include <Arduino.h>
#include <SPI.h>
#include <RF24.h>

#define NRF1_CE   4
#define NRF1_CSN  27


RF24 radio1(NRF1_CE, NRF1_CSN);

void setup() {
  Serial.begin(115200);
  delay(300);
  Serial.println();
  Serial.println("=== NRF24L01 #1 baglanti testi ===");

  // Ortak SPI hattini baslat (SCK, MISO, MOSI)
  SPI.begin(18, 19, 23);

  bool beginOk = radio1.begin();
  bool connected = radio1.isChipConnected();

  Serial.print("begin(): ");
  Serial.println(beginOk ? "basarili" : "basarisiz");

  Serial.print("isChipConnected(): ");
  Serial.println(connected ? "EVET" : "HAYIR");

  Serial.println("--------------------------------");
  if (beginOk && connected) {
    Serial.println(">>> NRF24L01 #1 ALGILANDI <<<");
  } else {
    Serial.println(">>> NRF24L01 #1 ALGILANMADI - kablolamayi kontrol et <<<");
  }
}

void loop() {
  delay(2000);
  bool connected = radio1.isChipConnected();
  Serial.println(connected ? "NRF #1: BAGLI" : "NRF #1: BAGLANTI YOK");
}