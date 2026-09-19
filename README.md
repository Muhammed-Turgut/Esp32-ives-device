## ESP32 IVES DEVICE
---

<p align="center">
  <img src="https://github.com/Muhammed-Turgut/imageRaw/blob/main/esp32_wroom_3d.png?raw=true" width="1600" height="1090" />
</p>





## Donanım Bağlantı Haritası (ESP32-WROOM-32D)

<p align="center">
  <img src="https://github.com/Muhammed-Turgut/imageRaw/blob/main/esp32_wroom_3d.png?raw=true" width="1600" height="1090" />
</p>


### Güç Hattı

| Bileşen | Pin | Bağlantı |
|---|---|---|
| LiPo Pil | + / - | TP4056 BAT+ / BAT- |
| TP4056 | OUT+ / OUT- | MT3608 VIN+ / VIN- |
| MT3608 | OUT+ / OUT- | ESP32 5V / GND |
| TP4056 | USB IN | Harici şarj kaynağı |

> MT3608 çıkışı potansiyometre ile **5V**'a ayarlanmalı.

### TFT Ekran (1.44" 128x128, SPI)

| TFT Pin | ESP32 Pin |
|---|---|
| VCC | 3.3V |
| GND | GND |
| SCK | GPIO 18 |
| SDA/MOSI | GPIO 23 |
| CS | GPIO 15 |
| DC/A0 | GPIO 21 |
| RST | GPIO 22 |
| LED | GPIO 25 |

### Button input

| Buton | ESP32 Pin | Not |
|---|---|---|
| Buton 1 | GPIO 13 | Dahili pull-down (INPUT_PULLDOWN) |
| Buton 2 | GPIO 14 | Dahili pull-down (INPUT_PULLDOWN) |
| Buton 3 | GPIO 12 | Dahili pull-down — açılışta basılı tutma! |
| Buton 4 | GPIO 32 | Dahili pull-down (INPUT_PULLDOWN) |
| Buton 5 | GPIO 33 | Dahili pull-down (INPUT_PULLDOWN) |
| Buton 6 | GPIO 26 | Dahili pull-down (INPUT_PULLDOWN) |

### SD Kart Modülü (SPI)

| SD Pin | ESP32 Pin |
|---|---|
| VCC | 3.3V |
| GND | GND |
| SCK | GPIO 18 |
| MOSI | GPIO 23 |
| MISO | GPIO 19 |
| CS | GPIO 5 |

### NRF24L01 (SPI)

| NRF24L01 Pin | ESP32 Pin |
|---|---|
| VCC | 3.3V (10-100µF kondansatör ile) |
| GND | GND |
| SCK | GPIO 18 |
| MOSI | GPIO 23 |
| MISO | GPIO 19 |
| CE | GPIO 27 |
| CSN | GPIO 4 |
| IRQ | Kullanılmıyor |

### GPS Modülü (GY-NEO6MV2, UART2)

| GPS Pin | ESP32 Pin |
|---|---|
| VCC | 3.3V |
| GND | GND |
| TX | GPIO 16 (RX2) |
| RX | GPIO 17 (TX2) |

### Ortak SPI Hattı Özeti

| Sinyal | Pin | Paylaşan Cihazlar |
|---|---|---|
| SCK | GPIO 18 | TFT, SD, NRF24L01 |
| MOSI | GPIO 23 | TFT, SD, NRF24L01 |
| MISO | GPIO 19 | SD, NRF24L01 |

### Kullanılmayan / Riskli Pinler

| Pin | Not |
|---|---|
| GPIO 0, 2, 12 | Boot-strapping — kullanma |
| GPIO 34-39 | Sadece giriş, PWM/çıkış için kullanılamaz |
