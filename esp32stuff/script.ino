#include <SPI.h>
#include <MFRC522.h>
#include <ESP32Servo.h>
#include <WiFi.h>
#include <WebServer.h>

const char* ssid = "YOUR_WIFI_SSID";
const char* password = "YOUR_WIFI_PASSWORD";

#define SS_PIN 5
#define RST_PIN 27
#define SERVO_PIN 13
#define RELAY_PIN 12

MFRC522 rfid(SS_PIN, RST_PIN);
Servo servo1;
WebServer server(80);

bool isExtended = false;
String lastTagData = "";
unsigned long lastRFIDReset = 0;
int readCount = 0;

void setup() {
  Serial.begin(115200);
  delay(1000);
  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, HIGH);
  ESP32PWM::allocateTimer(0);
  servo1.setPeriodHertz(50);
  servo1.attach(SERVO_PIN, 500, 2500);
  SPI.begin(18, 19, 23, 5);
  rfid.PCD_Init();
  rfid.PCD_SetAntennaGain(rfid.RxGain_max);
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  Serial.print("Connected! IP address: ");
  Serial.println(WiFi.localIP());
  server.on("/extend", HTTP_GET, handleExtend);
  server.begin();
  Serial.println("HTTP server started");
  Serial.println("Ready");
  Serial.println("Commands: retract (serial only)");
  Serial.println("HTTP: GET /extend");
}

void loop() {
  server.handleClient();
  if (Serial.available()) {
    String cmd = Serial.readStringUntil('\n');
    cmd.trim();
    cmd.toLowerCase();
    if (cmd == "retract") {
      doRetract();
    }
  }
  if (millis() - lastRFIDReset > 5000) {
    rfid.PCD_Reset();
    rfid.PCD_Init();
    rfid.PCD_SetAntennaGain(rfid.RxGain_max);
    lastRFIDReset = millis();
    readCount = 0;
  }
  checkRFID();
  delay(100);
}

void handleExtend() {
  if (isExtended) {
    server.send(200, "text/plain", "Already extended");
    Serial.println("HTTP: Already extended");
    return;
  }
  servo1.write(135);
  digitalWrite(RELAY_PIN, LOW);
  delay(500);
  digitalWrite(RELAY_PIN, HIGH);
  isExtended = true;
  server.send(200, "text/plain", "Extended");
  Serial.println("HTTP: Extended");
}

void doRetract() {
  if (!isExtended) {
    Serial.println("Already retracted");
    return;
  }
  servo1.write(45);
  digitalWrite(RELAY_PIN, LOW);
  delay(500);
  digitalWrite(RELAY_PIN, HIGH);
  isExtended = false;
  Serial.println("Retracted");
}

void checkRFID() {
  byte atqa[2];
  byte atqaLen = sizeof(atqa);
  rfid.PCD_Init();
  if (rfid.PICC_RequestA(atqa, &atqaLen) != MFRC522::STATUS_OK) {
    return;
  }
  if (rfid.PICC_Select(&rfid.uid) != MFRC522::STATUS_OK) {
    return;
  }
  readCount++;
  if (readCount >= 10) {
    rfid.PCD_Reset();
    rfid.PCD_Init();
    rfid.PCD_SetAntennaGain(rfid.RxGain_max);
    readCount = 0;
  }
  String tagData = "";
  for (byte page = 4; page <= 15; page++) {
    byte buffer[18];
    byte size = sizeof(buffer);
    if (rfid.MIFARE_Read(page, buffer, &size) == MFRC522::STATUS_OK) {
      for (byte i = 0; i < 4; i++) {
        if (buffer[i] == 0xFE) goto done;
        if (buffer[i] >= 32 && buffer[i] <= 126) {
          tagData += (char)buffer[i];
        }
      }
    }
  }
  done:
  rfid.PICC_HaltA();
  if (tagData != lastTagData && tagData.length() > 0) {
    lastTagData = tagData;
    Serial.print("UID: ");
    for (byte i = 0; i < rfid.uid.size; i++) {
      Serial.print(rfid.uid.uidByte[i] < 0x10 ? "0" : "");
      Serial.print(rfid.uid.uidByte[i], HEX);
    }
    Serial.println();
    Serial.print("Data: ");
    Serial.println(tagData);
  }
  delay(300);
  byte check[2];
  byte checkLen = sizeof(check);
  rfid.PCD_Init();
  while (rfid.PICC_RequestA(check, &checkLen) == MFRC522::STATUS_OK) {
    delay(200);
    rfid.PCD_Init();
    checkLen = sizeof(check);
  }
  lastTagData = "";
}
