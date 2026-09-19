/*
  NRF24L01 #2 ALGILAMA TESTI (sadece Serial Monitor)
  ------------------------------------------------------
  Pinler:
    CE  = GPIO 32
    CSN = GPIO 33
    SCK = GPIO 18 (ortak SPI)
    MOSI = GPIO 23 (ortak SPI)
    MISO = GPIO 19 (ortak SPI)

  Serial Monitor'u 115200 baud'da ac, sonucu orada gor.
*/

#include <Arduino.h>
#include <SPI.h>
#include <RF24.h>

#define NRF2_CE   32
#define NRF2_CSN  33

RF24 radio2(NRF2_CE, NRF2_CSN);

void setup() {
  Serial.begin(115200);
  delay(300);
  Serial.println();
  Serial.println("=== NRF24L01 #2 baglanti testi ===");

  // Ortak SPI hattini baslat (SCK, MISO, MOSI)
  SPI.begin(18, 19, 23);

  bool beginOk = radio2.begin();
  bool connected = radio2.isChipConnected();

  Serial.print("begin(): ");
  Serial.println(beginOk ? "basarili" : "basarisiz");

  Serial.print("isChipConnected(): ");
  Serial.println(connected ? "EVET" : "HAYIR");

  Serial.println("--------------------------------");
  if (beginOk && connected) {
    Serial.println(">>> NRF24L01 #2 ALGILANDI <<<");
  } else {
    Serial.println(">>> NRF24L01 #2 ALGILANMADI - kablolamayi kontrol et <<<");
  }
}

void loop() {
  // Her 2 saniyede bir tekrar kontrol et, kablo oynatirken
  // canli olarak takip edebilesin diye.
  delay(2000);
  bool connected = radio2.isChipConnected();
  Serial.println(connected ? "NRF #2: BAGLI" : "NRF #2: BAGLANTI YOK");
}