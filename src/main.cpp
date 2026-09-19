/*
  TFT (ST7735, 1.44" 128x128) BAGLANTI TEST KODU
  ------------------------------------------------
  Pin haritasi (README ile ayni):
    TFT VCC  -> 3.3V
    TFT GND  -> GND
    TFT SCK  -> GPIO 18
    TFT MOSI -> GPIO 23
    TFT CS   -> GPIO 15
    TFT DC   -> GPIO 21
    TFT RST  -> GPIO 22
    TFT LED  -> GPIO 25

  Bu kod ekrani sirayla renklerle doldurur, sonra bir yazi ve
  cerceve cizer. Ekranda bir sey gorunmuyorsa "Sorun Giderme"
  bolumune (asagida yorum satirlarinda) bak.
*/

#include <Arduino.h>
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h>

// --- Pin tanimlari ---
#define TFT_CS   15
#define TFT_DC   21
#define TFT_RST  22
#define TFT_LED  25   // Backlight - simdilik dijital ac/kapa

Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST);

void colorTest() {
  Serial.println("Renk testi: KIRMIZI");
  tft.fillScreen(ST77XX_RED);
  delay(800);

  Serial.println("Renk testi: YESIL");
  tft.fillScreen(ST77XX_GREEN);
  delay(800);

  Serial.println("Renk testi: MAVI");
  tft.fillScreen(ST77XX_BLUE);
  delay(800);

  Serial.println("Renk testi: SIYAH (temizle)");
  tft.fillScreen(ST77XX_BLACK);
  delay(300);
}

void drawUI() {
  tft.setTextColor(ST77XX_WHITE);
  tft.setTextSize(1);
  tft.setCursor(10, 10);
  tft.println("ESP32 TFT");
  tft.setCursor(10, 22);
  tft.println("BAGLANTI OK");

  // Kenarlara cerceve ciz - kenarlarin kesilip kesilmedigini
  // (offset sorunu) gormek icin
  tft.drawRect(0, 0, tft.width(), tft.height(), ST77XX_YELLOW);

  // Merkeze bir daire
  tft.fillCircle(tft.width() / 2, tft.height() / 2, 15, ST77XX_MAGENTA);
}

void setup() {
  Serial.begin(115200);
  delay(300);
  Serial.println();
  Serial.println("=== TFT baglanti testi basliyor ===");

  // Backlight'i ac
  pinMode(TFT_LED, OUTPUT);
  digitalWrite(TFT_LED, HIGH);

  // 1.44" 128x128 ST7735 icin dogru init komutu bu.
  // Ekran renkleri ters/kaydirilmis gorunurse asagidaki
  // satiri INITR_BLACKTAB veya INITR_MINI160x80 ile
  // degistirip tekrar dene.
  tft.initR(INITR_144GREENTAB);
  tft.setRotation(0); // 0-3 arasi dene, ekran yonu yanlissa degistir

  Serial.println("tft.initR() tamamlandi.");

  colorTest();
  drawUI();

  Serial.println("=== Test tamamlandi. Ekranda yazi ve cerceve gorunmeli. ===");
}

void loop() {
  // Backlight'i yaniyor-sonuyor yaparak GPIO25 baglantisini da
  // ayrica dogrulamak istersen asagidaki satirlari acabilirsin:
  //
  // digitalWrite(TFT_LED, LOW);
  // delay(500);
  // digitalWrite(TFT_LED, HIGH);
  // delay(500);
}

/*
  SORUN GIDERME
  -------------
  1) Ekran hic yanmiyor / arka isik yok:
     - TFT_LED (GPIO25) baglantisini ve VCC/GND'yi kontrol et.
     - MT3608 cikisinin gercekten 5V oldugunu multimetreyle olc.

  2) Backlight yaniyor ama ekran beyaz/bos kaliyor:
     - CS, DC, RST pin numaralarini fiziksel baglantiyla karsilastir.
     - SCK/MOSI hattinin SD karta da bagli olmasi sorun cikarabilir;
       SD karti gecici olarak sokup tekrar dene (CS celismesi olabilir).

  3) Renkler ters/negatif gorunuyor:
     - tft.initR(INITR_144GREENTAB) yerine INITR_BLACKTAB dene.

  4) Yazi/cerceve ekranin disina tasiyor veya kirpiliyor:
     - tft.setRotation() degerini 0,1,2,3 olarak degistirip dene.

  5) Serial monitorde "tft.initR() tamamlandi" yaziyor ama ekran
     tepki vermiyor:
     - SPI hizi/kablo uzunlugu sorunu olabilir, kablolari kisalt.
     - GND'lerin hepsinin (ESP32, TFT, SD, NRF24L01, GPS) ortak
       oldugundan emin ol - ortak GND olmazsa SPI calismaz.
*/