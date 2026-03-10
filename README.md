# ESP32 DevKit V2 Smart Device with AI

## 📋 ภาพรวม
โปรเจกต์นี้ใช้บอร์ด **ESP32 DevKit V2** ในการสร้างอุปกรณ์อัจฉริยะที่มี AI integration

## 🔧 ข้อมูลเทคนิคบอร์ด

### ข้อมูลพื้นฐาน
| รายการ | ค่า |
|--------|-----|
| Microcontroller | ESP32 Dual-core Xtensa 32-bit |
| RAM | 520 KB SRAM |
| Flash | 4 MB |
| Board | DevKit V2 |

---

## 📍 GPIO Pin Configuration

### 💡 LED Status Indicator
```
GPIO2 → Status LED (ใช้บ่งชี้สถานะบอร์ด)
```

### 🔌 Relay Control (Active Low)
| Relay | GPIO | ฟังก์ชัน |
|-------|------|---------|
| Relay 1 | GPIO4 | ควบคุมอุปกรณ์ 1 |
| Relay 2 | GPIO16 | ควบคุมอุปกรณ์ 2 |
| Relay 3 | GPIO17 | ควบคุมอุปกรณ์ 3 |

### 🔘 Switch Input (Active Low, 10K Pull-up)
| Switch | GPIO | ฟังก์ชัน |
|--------|------|---------|
| SW1 | GPIO34 | ปุ่มกด 1 |
| SW2 | GPIO35 | ปุ่มกด 2 |
| SW3 | GPIO32 | ปุ่มกด 3 |

### 🔗 Isolated Input (10-24V TTL, Active Low)
| Input | GPIO | ช่วงแรงดัน |
|-------|------|-----------|
| ISO1 | GPIO33 | 10-24V |
| ISO2 | GPIO27 | 10-24V |

---

## 📺 OLED Display (I2C)

### การเชื่อมต่อ
| OLED Pin | ESP32 GPIO |
|----------|------------|
| VCC | 3.3V |
| GND | GND |
| SDA | GPIO21 (I2C SDA) |
| SCL | GPIO22 (I2C SCL) |
| RST | GPIO5 (optional) |

### ข้อมูล
- **ดิสเพลย์**: 0.96" / 1.3" OLED
- **Driver**: SSD1306
- **ความละเอียด**: 128x64 (หรือ 128x32)
- **I2C Address**: 0x3C (บางรุ่น 0x3D)
- **แรงดัน**: 3.3V เท่านั้น ⚠️

---

## 🔄 RS485 Serial Communication

### สถาปัตยกรรม
- **UART**: UART0 (Serial0)
- **IC**: MAX13487 (Auto-Direction Control)
- **Mode**: Half-duplex

### การเชื่อมต่อ
| ESP32 (UART0) | MAX13487 Pin | ฟังก์ชัน |
|---------------|--------------|---------|
| TX0 (GPIO1) | DI | Data Input |
| RX0 (GPIO3) | RO | Receive Output |
| GPIO (RTS) | DE/RE | Direction Control (Auto) |
| 3.3V | VCC | Power |
| GND | GND | Ground |

### Bus Connections
| RS485 Signal | บัส |
|--------------|-----|
| A | Address A |
| B | Address B |

---

## ⚙️ การตั้งค่า PlatformIO

### platformio.ini
```ini
[env:esp32doit-devkit-v1]
platform = espressif32
board = esp32doit-devkit-v1
framework = arduino
monitor_speed = 9600
```

### Serial Monitor
- **Baud Rate**: 9600
- **Port**: COM (Windows) / /dev/ttyUSB0 (Linux) / /dev/cu.* (macOS)

---

## 🚀 เริ่มต้นใช้งาน

### 1. ติดตั้ง Library
```bash
platformio lib install "Adafruit SSD1306"
platformio lib install "Adafruit GFX Library"
```

### 2. โค้ด Template พื้นฐาน
```cpp
#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_SSD1306.h>

// GPIO Definitions
#define LED_PIN 2
#define RELAY1_PIN 4
#define RELAY2_PIN 16
#define RELAY3_PIN 17
#define SW1_PIN 34
#define SW2_PIN 35
#define SW3_PIN 32
#define ISO1_PIN 33
#define ISO2_PIN 27

// I2C OLED
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_ADDR 0x3C
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

void setup() {
  Serial.begin(9600);
  
  // GPIO Setup
  pinMode(LED_PIN, OUTPUT);
  pinMode(RELAY1_PIN, OUTPUT);
  pinMode(RELAY2_PIN, OUTPUT);
  pinMode(RELAY3_PIN, OUTPUT);
  pinMode(SW1_PIN, INPUT);
  pinMode(SW2_PIN, INPUT);
  pinMode(SW3_PIN, INPUT);
  pinMode(ISO1_PIN, INPUT);
  pinMode(ISO2_PIN, INPUT);
  
  // OLED Init
  if (!display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR)) {
    Serial.println("OLED init failed");
    while (1);
  }
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.println("ESP32 Ready!");
  display.display();
}

void loop() {
  // LED Blink
  digitalWrite(LED_PIN, HIGH);
  delay(500);
  digitalWrite(LED_PIN, LOW);
  delay(500);
}
```

---

## 📝 หมายเหตุสำคัญ

⚠️ **ความปลอดภัย**
- Relay ใช้ Active Low → ต้องส่ง LOW เพื่อเปิด
- OLED ต้อง 3.3V เท่านั้น ห้าม 5V
- Isolated Input รับ 10-24V TTL

---

## 🔗 Link ที่เกี่ยวข้อง
- [ESP32 Datasheet](https://www.espressif.com/sites/default/files/documentation/esp32_datasheet_en.pdf)
- [PlatformIO ESP32](https://platformio.org/boards/espressif32/esp32doit-devkit-v1)
- [Adafruit SSD1306](https://github.com/adafruit/Adafruit_SSD1306)

---

**ปรับปรุงล่าสุด**: March 10, 2026