/*
  GPS (GY-NEO6MV2) BAGLANTI TESTI (Serial Monitor)
  ------------------------------------------------------
  Pinler:
    GPS TX -> ESP32 GPIO 16 (RX2)
    GPS RX -> ESP32 GPIO 17 (TX2)
    VCC    -> 3.3V
    GND    -> GND

  Bu kod GPS modulunden gelen ham NMEA cumlelerini oldugu
  gibi Serial Monitor'e basar. Modul dogru bagliysa, acik
  havada ya da pencere kenarinda birkaç saniye icinde
  "$GPGGA", "$GPRMC" gibi satirlar gormeye baslarsin.

  NOT: Ic mekanda (bina icinde) GPS fix almak zor olabilir,
  bu normaldir - satirlarin gelmesi zaten baglantinin
  calistigini kanitlar, fix almasi ayri bir konu.
*/

#include <Arduino.h>
#include <HardwareSerial.h>

#define GPS_RX_PIN 16   // ESP32 RX2 <- GPS TX
#define GPS_TX_PIN 17   // ESP32 TX2 -> GPS RX
#define GPS_BAUD   9600 // GY-NEO6MV2 varsayilan baud hizi

HardwareSerial GPSSerial(2); // UART2 kullan

unsigned long lastByteTime = 0;
bool anyDataSeen = false;

void setup() {
  Serial.begin(115200);
  delay(300);
  Serial.println();
  Serial.println("=== GPS baglanti testi basliyor ===");
  Serial.println("NMEA verisi bekleniyor (birkac saniye surebilir)...");
  Serial.println("--------------------------------------------------");

  GPSSerial.begin(GPS_BAUD, SERIAL_8N1, GPS_RX_PIN, GPS_TX_PIN);
}

void loop() {
  // GPS'den gelen her baytı oldugu gibi Serial'e aktar
  while (GPSSerial.available()) {
    char c = GPSSerial.read();
    Serial.write(c);
    anyDataSeen = true;
    lastByteTime = millis();
  }

  // 5 saniyede hic veri gelmediyse uyari ver
  static unsigned long lastWarn = 0;
  if (!anyDataSeen && millis() - lastWarn > 5000) {
    Serial.println(">>> HENUZ VERI GELMEDI - kablolamayi kontrol et <<<");
    Serial.println("    (TX/RX ters baglanmis olabilir, VCC/GND kontrol et)");
    lastWarn = millis();
  }
}

/*
  SORUN GIDERME
  --------------
  1) Hic satir gelmiyor:
     - TX/RX'i ters baglamis olabilirsin: GPS TX -> ESP32 RX (16),
       GPS RX -> ESP32 TX (17). Ters baglarsan hic veri gelmez.
     - VCC/GND kontrol et, modulun uzerindeki LED yaniyor mu bak
       (cogu GY-NEO6MV2 modulunde guc LED'i ve fix LED'i ayri olur).
     - Baud hizi yanlis olabilir - bazi GY-NEO6MV2 modulleri 4800
       veya 38400 ile gelir, 9600 calismazsa bunlari dene.

  2) Anlamsiz/bozuk karakterler geliyor (kare, cizgi vs):
     - Baud hizi uyusmuyor demektir, GPS_BAUD degerini degistir.

  3) "$GP..." ile baslayan satirlar geliyor ama fix alamiyor:
     - Baglanti calisiyor demektir, sorun yok. Acik havada,
       gokyuzu gorebilecegin bir yerde birkac dakika bekle,
       ilk fix (cold start) 30 saniye - birkac dakika surebilir.
*/