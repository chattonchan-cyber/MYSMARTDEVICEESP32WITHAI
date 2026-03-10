#include <Arduino.h>

// GPIO Definitions
#define LED_PIN 2

// LED Status States
enum LEDState {
  IDLE,           // LED ปกติ - กระพริบช้า (1 วินาที)
  ACTIVE,         // LED ทำงาน - กระพริบเร็ว (500ms)
  ERROR_STATE,    // LED แสดงข้อผิดพลาด - กระพริบเร็วมาก (200ms)
  CONNECTED       // LED เชื่อมต่อสำเร็จ - ติดตลอด
};

// LED Control Structure
struct LEDController {
  int pin;
  LEDState state;
  unsigned long lastToggleTime;
  unsigned long toggleInterval;
  bool isOn;
};

LEDController ledCtrl = {
  .pin = LED_PIN,
  .state = IDLE,
  .lastToggleTime = 0,
  .toggleInterval = 1000,  // 1 second for IDLE
  .isOn = false
};

// ฟังก์ชันตั้งค่าสถานะ LED
void setLEDState(LEDState newState) {
  ledCtrl.state = newState;
  
  switch (newState) {
    case IDLE:
      ledCtrl.toggleInterval = 1000;  // กระพริบ 1 วินาที
      Serial.println("LED State: IDLE (Normal Blink)");
      break;
    case ACTIVE:
      ledCtrl.toggleInterval = 500;   // กระพริบ 500ms
      Serial.println("LED State: ACTIVE (Fast Blink)");
      break;
    case ERROR_STATE:
      ledCtrl.toggleInterval = 200;   // กระพริบ 200ms
      Serial.println("LED State: ERROR (Very Fast Blink)");
      break;
    case CONNECTED:
      digitalWrite(ledCtrl.pin, HIGH);
      ledCtrl.isOn = true;
      Serial.println("LED State: CONNECTED (Always ON)");
      break;
  }
}

// ฟังก์ชันอัปเดต LED โดยไม่ใช้ Delay
void updateLED() {
  unsigned long currentTime = millis();
  
  // ถ้าเป็น CONNECTED ให้ติดตลอด ไม่ต้องเช็ก
  if (ledCtrl.state == CONNECTED) {
    return;
  }
  
  // ตรวจสอบว่าถึงเวลา toggle หรือยัง
  if (currentTime - ledCtrl.lastToggleTime >= ledCtrl.toggleInterval) {
    ledCtrl.lastToggleTime = currentTime;
    ledCtrl.isOn = !ledCtrl.isOn;
    
    digitalWrite(ledCtrl.pin, ledCtrl.isOn ? HIGH : LOW);
  }
}

// ฟังก์ชันแสดงสถานะปัจจุบัน
void printLEDStatus() {
  Serial.print("LED Status: ");
  Serial.print(ledCtrl.isOn ? "ON" : "OFF");
  Serial.print(" | State: ");
  
  switch (ledCtrl.state) {
    case IDLE:
      Serial.println("IDLE");
      break;
    case ACTIVE:
      Serial.println("ACTIVE");
      break;
    case ERROR_STATE:
      Serial.println("ERROR");
      break;
    case CONNECTED:
      Serial.println("CONNECTED");
      break;
  }
}

void setup() {
  Serial.begin(9600);
  delay(1000);
  
  // ตั้งค่า GPIO
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);
  
  Serial.println("\n=== ESP32 LED Status Indicator ===");
  Serial.println("GPIO2 - Non-blocking LED Controller");
  Serial.println("=====================================\n");
  
  // เริ่มต้นด้วยสถานะ IDLE
  setLEDState(IDLE);
}

// ตัวแปรสำหรับเปลี่ยนสถานะ (สำหรับทดสอบ)
unsigned long lastStateChangeTime = 0;
unsigned long stateChangeInterval = 10000;  // เปลี่ยนสถานะทุก 10 วินาที
int stateCounter = 0;

void loop() {
  // อัปเดต LED อย่างต่อเนื่อง (non-blocking)
  updateLED();
  
  // แสดงสถานะ LED ทุก 2 วินาที
  static unsigned long lastPrintTime = 0;
  if (millis() - lastPrintTime >= 2000) {
    lastPrintTime = millis();
    printLEDStatus();
  }
  
  // เปลี่ยนสถานะ LED ทุก 10 วินาที (สำหรับทดสอบ)
  if (millis() - lastStateChangeTime >= stateChangeInterval) {
    lastStateChangeTime = millis();
    
    LEDState states[] = {IDLE, ACTIVE, ERROR_STATE, CONNECTED};
    setLEDState(states[stateCounter % 4]);
    stateCounter++;
  }
}
