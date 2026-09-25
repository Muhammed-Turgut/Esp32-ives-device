/*
  Menu.
  Yukari  GPIO 36
  Sag     GPIO 39
  Sol     GPIO 34
  Asagi   GPIO 14
  Onay    GPIO 35
  Geri    GPIO 13
  GPS TX -> GPIO 16, GPS RX -> GPIO 17, 9600 baud
*/

#include <Arduino.h>
#include <SPI.h>
#include <HardwareSerial.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h>
#include <RF24.h>

#define GPS_RX_PIN 16
#define GPS_TX_PIN 17
#define GPS_BAUD   9600

#define TFT_CS  15
#define TFT_DC  21
#define TFT_RST 22
#define TFT_LED 25
#define SD_CS   5

#define BTN_UP    36
#define BTN_RIGHT 39
#define BTN_LEFT  34
#define BTN_DOWN  14
#define BTN_OK    35
#define BTN_BACK  13

#define MENU_COUNT 6
#define RF1_CE  4
#define RF1_CSN 27
#define RF2_CE  32
#define RF2_CSN 33

HardwareSerial GPSSerial(2);
Adafruit_ST7735 tft(TFT_CS, TFT_DC, TFT_RST);

enum Screen { SCREEN_MENU, SCREEN_GPS, SCREEN_BUTTONS, SCREEN_INFO, SCREEN_TEST, SCREEN_EXTRA1, SCREEN_EXTRA2 };

struct Key {
  uint8_t pin;
  bool held;
  uint32_t changedAt;
};

Key keyUp = {BTN_UP, false, 0};
Key keyRight = {BTN_RIGHT, false, 0};
Key keyLeft = {BTN_LEFT, false, 0};
Key keyDown = {BTN_DOWN, false, 0};
Key keyOk = {BTN_OK, false, 0};
Key keyBack = {BTN_BACK, false, 0};

const char* menuNames[MENU_COUNT] = {"GPS", "Butonlar", "Bilgi", "Test", "Link", "Dinle"};
const uint16_t menuColors[MENU_COUNT] = {0x0320, 0x0012, 0x4008, 0x0210, 0x7800, 0x780F};

char line[90];
uint8_t lineLen = 0;
uint32_t gpsBytes = 0;

bool rf1Found = false;
bool rf2Found = false;
const char* rf1Note = "yok";
const char* rf2Note = "yok";

RF24 radio1(RF1_CE, RF1_CSN, 1000000);
RF24 radio2(RF2_CE, RF2_CSN, 1000000);
bool radiosUp = false;
bool triedSend = false;
bool sent12 = false;
bool got12 = false;
bool sent21 = false;
bool got21 = false;
uint8_t arc12 = 0;
uint8_t arc21 = 0;
int listenWhich = 1;
int listenCount = 0;
char listenText[8] = "";
bool listenUp = false;
uint32_t lastSendAt = 0;
int sent1 = 0;
int sent2 = 0;

bool seenSentence = false;
bool hasFix = false;
int satellites = 0;
float latitude = 0;
float longitude = 0;
char latHemi = 'N';
char lonHemi = 'E';

Screen screen = SCREEN_MENU;
int menuIndex = 0;
bool dirty = true;
uint8_t shownButtons = 0xFF;

uint8_t buttonMask();
bool gpsStatusChanged(bool nextSeen, bool nextFix, int nextSats);

void parkSpiDevices() {
  const uint8_t holdHigh[] = {SD_CS, 4, 27, 32, 33};
  for (uint8_t pin : holdHigh) {
    pinMode(pin, OUTPUT);
    digitalWrite(pin, HIGH);
  }
}

bool checksumOk(const char* sentence) {
  if (sentence[0] != '$') {
    return false;
  }
  const char* star = strchr(sentence, '*');
  if (star == nullptr || star[1] == '\0' || star[2] == '\0') {
    return false;
  }
  uint8_t sum = 0;
  for (const char* p = sentence + 1; p < star; p++) {
    sum ^= (uint8_t)*p;
  }
  char* end = nullptr;
  unsigned long given = strtoul(star + 1, &end, 16);
  return end != star + 1 && given == sum;
}

bool fieldAt(const char* sentence, int index, char* out, size_t outLen) {
  const char* p = sentence;
  if (*p == '$') {
    p++;
  }
  while (index > 0 && *p != '\0' && *p != '*') {
    if (*p == ',') {
      index--;
    }
    p++;
  }
  size_t n = 0;
  while (*p != '\0' && *p != ',' && *p != '*' && n + 1 < outLen) {
    out[n++] = *p++;
  }
  out[n] = '\0';
  return n > 0;
}

float nmeaToDegrees(const char* raw) {
  float value = atof(raw);
  int degrees = (int)(value / 100.0f);
  float minutes = value - (degrees * 100.0f);
  return degrees + (minutes / 60.0f);
}

void formatCoord(char* out, size_t outLen, float degrees, char hemi) {
  if (degrees < 0) {
    degrees = -degrees;
  }
  int whole = (int)degrees;
  int frac = (int)((degrees - whole) * 100000.0f + 0.5f);
  if (frac >= 100000) {
    whole++;
    frac -= 100000;
  }
  snprintf(out, outLen, "%d.%05d %c", whole, frac, hemi);
}

void drawHeader(const char* title, uint16_t color) {
  tft.fillRect(0, 0, 128, 22, color);
  tft.setTextSize(2);
  tft.setTextColor(ST77XX_WHITE);
  tft.setCursor(6, 3);
  tft.print(title);
}

void drawMenu() {
  const int visible = 4;
  static int scroll = 0;
  if (menuIndex < scroll) {
    scroll = menuIndex;
  }
  if (menuIndex >= scroll + visible) {
    scroll = menuIndex - visible + 1;
  }
  int maxScroll = MENU_COUNT > visible ? MENU_COUNT - visible : 0;
  if (scroll > maxScroll) {
    scroll = maxScroll;
  }
  if (scroll < 0) {
    scroll = 0;
  }

  tft.fillScreen(ST77XX_BLACK);
  drawHeader("MENU", 0x001F);

  int shown = MENU_COUNT - scroll;
  if (shown > visible) {
    shown = visible;
  }
  for (int row = 0; row < shown; row++) {
    int i = scroll + row;
    int y = 26 + row * 22;
    bool selected = i == menuIndex;
    tft.fillRoundRect(6, y, 110, 20, 4, selected ? menuColors[i] : 0x1082);
    tft.drawRoundRect(6, y, 110, 20, 4, selected ? ST77XX_WHITE : 0x3186);
    tft.setTextSize(1);
    tft.setTextColor(ST77XX_WHITE);
    tft.setCursor(12, y + 6);
    tft.print(menuNames[i]);
    if (i == 0) {
      tft.setCursor(62, y + 6);
      if (!seenSentence) {
        tft.print("veri yok");
      } else if (!hasFix) {
        tft.print("fix yok");
      } else {
        tft.print("fix var");
      }
    }
  }

  tft.setTextSize(1);
  tft.setTextColor(ST77XX_WHITE);
  if (scroll > 0) {
    tft.setCursor(118, 30);
    tft.print("^");
  }
  if (scroll + visible < MENU_COUNT) {
    tft.setCursor(118, 96);
    tft.print("v");
  }

  tft.setTextColor(ST77XX_CYAN);
  tft.setCursor(4, 116);
  tft.print("36 yukari  14 asagi");
}

void drawGps() {
  char latText[20];
  char lonText[20];

  tft.fillScreen(ST77XX_BLACK);
  drawHeader("GPS", hasFix ? 0x0320 : 0x4200);

  tft.setTextSize(1);
  tft.setCursor(6, 30);
  if (!seenSentence) {
    tft.setTextColor(ST77XX_YELLOW);
    tft.print("Veri yok");
  } else if (!hasFix) {
    tft.setTextColor(ST77XX_YELLOW);
    tft.print("Fix yok");
  } else {
    tft.setTextColor(ST77XX_GREEN);
    tft.print("Fix var");
  }

  tft.setTextColor(ST77XX_WHITE);
  tft.setCursor(70, 30);
  tft.print("Uydu ");
  tft.print(satellites);

  tft.setTextColor(ST77XX_CYAN);
  tft.setCursor(6, 48);
  tft.print("Enlem");
  tft.setTextColor(ST77XX_WHITE);
  tft.setCursor(6, 60);
  if (hasFix) {
    formatCoord(latText, sizeof(latText), latitude, latHemi);
    tft.print(latText);
  } else {
    tft.print("--");
  }

  tft.setTextColor(ST77XX_CYAN);
  tft.setCursor(6, 76);
  tft.print("Boylam");
  tft.setTextColor(ST77XX_WHITE);
  tft.setCursor(6, 88);
  if (hasFix) {
    formatCoord(lonText, sizeof(lonText), longitude, lonHemi);
    tft.print(lonText);
  } else {
    tft.print("--");
  }

  tft.setTextColor(ST77XX_CYAN);
  tft.setCursor(6, 116);
  tft.print("13 geri");
}

void drawKey(int x, int y, int w, const char* name, bool high) {
  tft.fillRoundRect(x, y, w, 18, 3, high ? ST77XX_GREEN : 0x1082);
  tft.drawRoundRect(x, y, w, 18, 3, ST77XX_WHITE);
  tft.setTextSize(1);
  tft.setTextColor(ST77XX_WHITE);
  tft.setCursor(x + 4, y + 5);
  tft.print(name);
}

void drawButtons() {
  uint8_t mask = buttonMask();
  tft.fillScreen(ST77XX_BLACK);
  drawHeader("BUTON", 0x0012);

  drawKey(40, 28, 48, "36 yuk", mask & 0x01);
  drawKey(4, 50, 40, "34 sol", mask & 0x04);
  drawKey(84, 50, 40, "39 sag", mask & 0x02);
  drawKey(40, 72, 48, "14 asa", mask & 0x08);
  drawKey(4, 94, 58, "35 onay", mask & 0x10);
  drawKey(66, 94, 58, "13 geri", mask & 0x20);

  shownButtons = buttonMask();
}

void drawInfo() {
  tft.fillScreen(ST77XX_BLACK);
  drawHeader("BILGI", 0x4008);
  tft.setTextSize(1);
  tft.setTextColor(ST77XX_WHITE);
  tft.setCursor(6, 30);
  tft.print("36 yukari");
  tft.setCursor(6, 42);
  tft.print("14 asagi");
  tft.setCursor(6, 54);
  tft.print("34 sol   39 sag");
  tft.setCursor(6, 66);
  tft.print("35 onay");
  tft.setCursor(6, 78);
  tft.print("13 geri");
  tft.setCursor(6, 94);
  tft.print("GPS 16 RX  17 TX");
  tft.setTextColor(ST77XX_CYAN);
  tft.setCursor(6, 116);
  tft.print("13 geri");
}

void busIdle() {
  const uint8_t pins[] = {4, 27, 32, 33, TFT_CS, SD_CS};
  for (uint8_t pin : pins) {
    pinMode(pin, OUTPUT);
    digitalWrite(pin, HIGH);
  }
}

bool readChip(uint8_t csn, uint8_t& status, uint8_t& aw) {
  busIdle();
  SPI.beginTransaction(SPISettings(1000000, MSBFIRST, SPI_MODE0));
  digitalWrite(csn, LOW);
  status = SPI.transfer(0xFF);
  digitalWrite(csn, HIGH);
  delayMicroseconds(20);
  digitalWrite(csn, LOW);
  SPI.transfer(0x03);
  aw = SPI.transfer(0xFF);
  digitalWrite(csn, HIGH);
  SPI.endTransaction();
  busIdle();
  return status != 0x00 && status != 0xFF && (aw == 0x01 || aw == 0x02 || aw == 0x03);
}

const char* faultNote(uint8_t status, uint8_t aw) {
  if (status == 0x00 && aw == 0x00) {
    return "MISO0";
  }
  if (status == 0xFF && aw == 0xFF) {
    return "bos";
  }
  return "bozuk";
}

void testPair(uint8_t primary, uint8_t alternate, bool& found, const char*& note) {
  uint8_t status = 0;
  uint8_t aw = 0;
  if (readChip(primary, status, aw)) {
    found = true;
    note = "tamam";
    return;
  }
  uint8_t status2 = 0;
  uint8_t aw2 = 0;
  if (readChip(alternate, status2, aw2)) {
    found = true;
    note = "ters";
    return;
  }
  found = false;
  note = faultNote(status, aw);
}

void runModuleTest() {
  SPI.begin(18, 19, 23);
  testPair(RF1_CSN, RF1_CE, rf1Found, rf1Note);
  testPair(RF2_CSN, RF2_CE, rf2Found, rf2Note);
  busIdle();
}

void drawResult(int y, const char* name, bool found) {
  tft.setTextColor(ST77XX_WHITE);
  tft.setCursor(8, y);
  tft.print(name);
  tft.setCursor(48, y);
  if (found) {
    tft.setTextColor(ST77XX_GREEN);
    tft.print("VAR");
  } else {
    tft.setTextColor(ST77XX_RED);
    tft.print("YOK");
  }
}

void drawTest() {
  tft.fillScreen(ST77XX_BLACK);
  drawHeader("TEST", 0x0210);
  tft.setTextSize(1);

  drawResult(32, "GPS", seenSentence);
  if (!seenSentence && gpsBytes > 0) {
    tft.setTextColor(ST77XX_YELLOW);
    tft.setCursor(84, 32);
    tft.print("byte");
  }

  drawResult(48, "RF1", rf1Found);
  tft.setTextColor(ST77XX_WHITE);
  tft.setCursor(84, 48);
  tft.print(rf1Note);

  drawResult(70, "RF2", rf2Found);
  tft.setTextColor(ST77XX_WHITE);
  tft.setCursor(84, 70);
  tft.print(rf2Note);

  tft.setTextColor(ST77XX_CYAN);
  tft.setCursor(8, 96);
  tft.print("SPI cevap, yayin yok");
  tft.setCursor(8, 116);
  tft.print("35 tekrar  13 geri");
}

void holdBus() {
  pinMode(TFT_CS, OUTPUT);
  pinMode(SD_CS, OUTPUT);
  digitalWrite(TFT_CS, HIGH);
  digitalWrite(SD_CS, HIGH);
}

void setupRadio(RF24& radio) {
  radio.setDataRate(RF24_250KBPS);
  radio.setPALevel(RF24_PA_LOW);
  radio.setChannel(76);
  radio.setCRCLength(RF24_CRC_16);
  radio.disableDynamicPayloads();
  radio.setPayloadSize(5);
  radio.setAutoAck(true);
  radio.setRetries(5, 15);
}

bool ping(RF24& tx, RF24& rx, uint8_t& arc) {
  const uint8_t addr[5] = {0xE7, 0xE7, 0xE7, 0xE7, 0xE7};
  const char msg[6] = "hello";
  char buf[6] = {0};
  holdBus();
  tx.stopListening();
  rx.stopListening();
  tx.openWritingPipe(addr);
  rx.openReadingPipe(1, addr);
  rx.flush_rx();
  tx.flush_tx();
  rx.startListening();
  delay(8);
  bool sent = tx.write(msg, 5);
  arc = tx.getARC();
  bool got = false;
  uint32_t start = millis();
  while (millis() - start < 80) {
    if (rx.available()) {
      rx.read(buf, 5);
      buf[5] = '\0';
      got = strcmp(buf, "hello") == 0;
      break;
    }
  }
  rx.stopListening();
  return sent && got;
}

void drawLink() {
  tft.fillScreen(ST77XX_BLACK);
  tft.fillRect(0, 0, 128, 22, 0x0210);
  tft.setTextSize(2);
  tft.setTextColor(ST77XX_WHITE);
  tft.setCursor(6, 3);
  tft.print("LINK");
  tft.setTextSize(1);
  tft.setCursor(6, 28);
  if (!radiosUp) {
    tft.setTextColor(ST77XX_RED);
    tft.print("radyo yok");
  } else if (!triedSend) {
    tft.setTextColor(ST77XX_YELLOW);
    tft.print("35 gonder");
  } else {
    tft.setTextColor(sent12 ? ST77XX_GREEN : ST77XX_RED);
    tft.setCursor(6, 28);
    tft.print("1>2 ");
    tft.print(sent12 ? "OK" : "YOK");
    tft.print("  ARC");
    tft.print(arc12);
    tft.setTextColor(got12 ? ST77XX_GREEN : ST77XX_RED);
    tft.setCursor(6, 44);
    tft.print(got12 ? "aldi hello" : "almadi");
    tft.setTextColor(sent21 ? ST77XX_GREEN : ST77XX_RED);
    tft.setCursor(6, 66);
    tft.print("2>1 ");
    tft.print(sent21 ? "OK" : "YOK");
    tft.print("  ARC");
    tft.print(arc21);
    tft.setTextColor(got21 ? ST77XX_GREEN : ST77XX_RED);
    tft.setCursor(6, 82);
    tft.print(got21 ? "aldi hello" : "almadi");
  }
  tft.setTextColor(ST77XX_CYAN);
  tft.setCursor(6, 112);
  tft.print("35 tekrar  13 geri");
}

void startLink() {
  holdBus();
  SPI.begin(18, 19, 23);
  radio1.begin();
  radio2.begin();
  setupRadio(radio1);
  setupRadio(radio2);
  radiosUp = radio1.isChipConnected() && radio2.isChipConnected();
  triedSend = false;
  sent12 = got12 = sent21 = got21 = false;
  arc12 = arc21 = 0;
}

void sendHello() {
  if (!radiosUp) {
    triedSend = true;
    return;
  }
  sent12 = ping(radio1, radio2, arc12);
  got12 = sent12;
  sent21 = ping(radio2, radio1, arc21);
  got21 = sent21;
  triedSend = true;
  Serial.printf("1>2 sent=%d arc=%u\n", sent12, arc12);
  Serial.printf("2>1 sent=%d arc=%u\n", sent21, arc21);
}

void stopLink() {
  if (!radiosUp) {
    return;
  }
  radio1.stopListening();
  radio2.stopListening();
  radio1.powerDown();
  radio2.powerDown();
  radiosUp = false;
  digitalWrite(RF1_CE, LOW);
  digitalWrite(RF2_CE, LOW);
  digitalWrite(RF1_CSN, HIGH);
  digitalWrite(RF2_CSN, HIGH);
}

void startListen() {
  const uint8_t addr[5] = {0xE7, 0xE7, 0xE7, 0xE7, 0xE7};
  holdBus();
  SPI.begin(18, 19, 23);
  radio1.begin();
  radio2.begin();
  setupRadio(radio1);
  setupRadio(radio2);
  radio1.openWritingPipe(addr);
  radio2.openWritingPipe(addr);
  radio1.stopListening();
  radio2.stopListening();
  listenUp = radio1.isChipConnected() && radio2.isChipConnected();
  listenCount = 0;
  sent1 = 0;
  sent2 = 0;
  listenText[0] = '\0';
}

void stopListen() {
  if (!listenUp) {
    return;
  }
  radio1.stopListening();
  radio2.stopListening();
  radio1.powerDown();
  radio2.powerDown();
  listenUp = false;
}

void pollListen() {
  if (!listenUp || millis() - lastSendAt < 400) {
    return;
  }
  lastSendAt = millis();
  holdBus();
  if (radio1.write("rf1ok", 5)) {
    sent1++;
  }
  delay(15);
  if (radio2.write("rf2ok", 5)) {
    sent2++;
  }
  dirty = true;
}

void drawListen() {
  tft.fillScreen(ST77XX_BLACK);
  drawHeader("GONDER", 0x780F);
  tft.setTextSize(1);
  tft.setTextColor(ST77XX_WHITE);
  tft.setCursor(8, 32);
  tft.print(listenUp ? "Uno ya gonder" : "radyo yok");
  tft.setTextColor(sent1 ? ST77XX_GREEN : ST77XX_YELLOW);
  tft.setCursor(8, 52);
  tft.print("RF1 ");
  tft.print(sent1);
  tft.setTextColor(sent2 ? ST77XX_GREEN : ST77XX_YELLOW);
  tft.setCursor(8, 68);
  tft.print("RF2 ");
  tft.print(sent2);
  tft.setTextColor(ST77XX_CYAN);
  tft.setCursor(8, 116);
  tft.print("13 geri");
}

void drawCurrent() {
  if (screen == SCREEN_MENU) {
    drawMenu();
  } else if (screen == SCREEN_GPS) {
    drawGps();
  } else if (screen == SCREEN_BUTTONS) {
    drawButtons();
  } else if (screen == SCREEN_TEST) {
    drawTest();
  } else if (screen == SCREEN_EXTRA1) {
    drawLink();
  } else if (screen == SCREEN_EXTRA2) {
    drawListen();
  } else {
    drawInfo();
  }
}

void applyGga(const char* sentence) {
  char lat[16];
  char lon[16];
  char ns[4];
  char ew[4];
  char quality[4];
  char sats[4];

  seenSentence = true;
  bool fix = fieldAt(sentence, 6, quality, sizeof(quality)) && quality[0] != '0';
  fieldAt(sentence, 7, sats, sizeof(sats));
  satellites = sats[0] ? atoi(sats) : 0;

  if (fix &&
      fieldAt(sentence, 2, lat, sizeof(lat)) &&
      fieldAt(sentence, 3, ns, sizeof(ns)) &&
      fieldAt(sentence, 4, lon, sizeof(lon)) &&
      fieldAt(sentence, 5, ew, sizeof(ew))) {
    latitude = nmeaToDegrees(lat);
    longitude = nmeaToDegrees(lon);
    latHemi = ns[0];
    lonHemi = ew[0];
    hasFix = true;
  } else {
    hasFix = false;
  }
  if (screen == SCREEN_GPS || screen == SCREEN_TEST ||
      (screen == SCREEN_MENU && gpsStatusChanged(seenSentence, hasFix, satellites))) {
    dirty = true;
  }
}

void applyRmc(const char* sentence) {
  char status[4];
  char lat[16];
  char lon[16];
  char ns[4];
  char ew[4];

  seenSentence = true;
  bool fix = fieldAt(sentence, 2, status, sizeof(status)) && status[0] == 'A';
  if (!fix) {
    hasFix = false;
    if (screen == SCREEN_GPS || screen == SCREEN_TEST ||
        (screen == SCREEN_MENU && gpsStatusChanged(seenSentence, hasFix, satellites))) {
      dirty = true;
    }
    return;
  }
  if (fieldAt(sentence, 3, lat, sizeof(lat)) &&
      fieldAt(sentence, 4, ns, sizeof(ns)) &&
      fieldAt(sentence, 5, lon, sizeof(lon)) &&
      fieldAt(sentence, 6, ew, sizeof(ew))) {
    latitude = nmeaToDegrees(lat);
    longitude = nmeaToDegrees(lon);
    latHemi = ns[0];
    lonHemi = ew[0];
    hasFix = true;
    if (screen == SCREEN_GPS || screen == SCREEN_TEST ||
        (screen == SCREEN_MENU && gpsStatusChanged(seenSentence, hasFix, satellites))) {
      dirty = true;
    }
  }
}

void handleSentence(char* sentence) {
  if (!checksumOk(sentence)) {
    return;
  }
  if (strncmp(sentence, "$GPGGA", 6) == 0 || strncmp(sentence, "$GNGGA", 6) == 0) {
    applyGga(sentence);
  } else if (strncmp(sentence, "$GPRMC", 6) == 0 || strncmp(sentence, "$GNRMC", 6) == 0) {
    applyRmc(sentence);
  }
}

bool gpsStatusChanged(bool nextSeen, bool nextFix, int nextSats) {
  static bool drawnSeen = false;
  static bool drawnFix = false;
  static int drawnSats = -1;
  bool changed = nextSeen != drawnSeen || nextFix != drawnFix || nextSats != drawnSats;
  drawnSeen = nextSeen;
  drawnFix = nextFix;
  drawnSats = nextSats;
  return changed;
}

bool clicked(Key& key) {
  bool level = digitalRead(key.pin) == HIGH;
  uint32_t now = millis();
  if (level != key.held && now - key.changedAt > 40) {
    key.held = level;
    key.changedAt = now;
    return key.held;
  }
  return false;
}

void openSelection() {
  if (menuIndex == 0) {
    screen = SCREEN_GPS;
  } else if (menuIndex == 1) {
    screen = SCREEN_BUTTONS;
  } else if (menuIndex == 2) {
    screen = SCREEN_INFO;
  } else if (menuIndex == 3) {
    screen = SCREEN_TEST;
    runModuleTest();
  } else if (menuIndex == 4) {
    screen = SCREEN_EXTRA1;
    startLink();
  } else {
    screen = SCREEN_EXTRA2;
    startListen();
  }
  dirty = true;
}

void pollButtons() {
  bool up = clicked(keyUp);
  bool down = clicked(keyDown);
  bool ok = clicked(keyOk);
  bool back = clicked(keyBack);
  clicked(keyLeft);
  clicked(keyRight);

  if (screen == SCREEN_MENU) {
    if (down) {
      menuIndex = (menuIndex + 1) % MENU_COUNT;
      dirty = true;
    }
    if (up) {
      menuIndex = (menuIndex + MENU_COUNT - 1) % MENU_COUNT;
      dirty = true;
    }
    if (ok) {
      openSelection();
    }
    return;
  }

  if (screen == SCREEN_TEST && ok) {
    runModuleTest();
    dirty = true;
  }

  if (screen == SCREEN_EXTRA1 && ok) {
    sendHello();
    dirty = true;
  }

  if (screen == SCREEN_EXTRA2 && ok) {
    listenWhich = listenWhich == 1 ? 2 : 1;
    startListen();
    dirty = true;
  }

  if (back) {
    if (screen == SCREEN_EXTRA1) {
      stopLink();
    }
    if (screen == SCREEN_EXTRA2) {
      stopListen();
    }
    screen = SCREEN_MENU;
    dirty = true;
  }
}

uint8_t buttonMask() {
  uint8_t mask = 0;
  if (digitalRead(BTN_UP) == HIGH) {
    mask |= 0x01;
  }
  if (digitalRead(BTN_RIGHT) == HIGH) {
    mask |= 0x02;
  }
  if (digitalRead(BTN_LEFT) == HIGH) {
    mask |= 0x04;
  }
  if (digitalRead(BTN_DOWN) == HIGH) {
    mask |= 0x08;
  }
  if (digitalRead(BTN_OK) == HIGH) {
    mask |= 0x10;
  }
  if (digitalRead(BTN_BACK) == HIGH) {
    mask |= 0x20;
  }
  return mask;
}

void setup() {
  Serial.begin(115200);
  parkSpiDevices();

  pinMode(BTN_UP, INPUT);
  pinMode(BTN_RIGHT, INPUT);
  pinMode(BTN_LEFT, INPUT);
  pinMode(BTN_DOWN, INPUT_PULLDOWN);
  pinMode(BTN_OK, INPUT);
  pinMode(BTN_BACK, INPUT_PULLDOWN);

  pinMode(TFT_LED, OUTPUT);
  digitalWrite(TFT_LED, HIGH);
  tft.initR(INITR_144GREENTAB);
  tft.setRotation(0);
  tft.setTextWrap(false);
  drawCurrent();
  dirty = false;

  GPSSerial.begin(GPS_BAUD, SERIAL_8N1, GPS_RX_PIN, GPS_TX_PIN);
  Serial.println("Menu basladi");
  startListen();
}

void loop() {
  while (GPSSerial.available()) {
    char c = GPSSerial.read();
    if (gpsBytes < 1000000) {
      gpsBytes++;
    }
    if (screen == SCREEN_TEST && gpsBytes == 1) {
      dirty = true;
    }
    if (c == '\r') {
      continue;
    }
    if (c == '\n') {
      line[lineLen] = '\0';
      if (lineLen > 6) {
        handleSentence(line);
      }
      lineLen = 0;
      continue;
    }
    if (lineLen + 1 < sizeof(line)) {
      line[lineLen++] = c;
    } else {
      lineLen = 0;
    }
  }

  pollButtons();

  if (listenUp) {
    pollListen();
  }

  if (screen == SCREEN_BUTTONS) {
    uint8_t mask = buttonMask();
    if (mask != shownButtons) {
      shownButtons = mask;
      dirty = true;
    }
  }

  if (dirty) {
    drawCurrent();
    dirty = false;
  }
}
