#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <SPI.h>
#include <MFRC522.h>
#include <HTTPSRedirect.h>

#include <Wire.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Enter Google Script Deployment ID:
const char *GScriptId = "MASUKKAN ID PENERAPAN";

// Enter network credentials:
const char* ssid     = "XXXXXXXX";
const char* password = "xxxxxxxx";

// Enter command and sheet name:
String payload_base = "{\"command\": \"insert_row\", \"sheet_name\": \"Sheet1\", \"values\": ";
String payload = "";

// Google Sheets setup:
const char* host        = "script.google.com";
const int   httpsPort   = 443;
const char* fingerprint = "";
String url = String("/macros/s/") + GScriptId + "/exec";
HTTPSRedirect* client = nullptr;

// Declare variables
String nama;
String no_absen;
String kelas;
String jurusan;
String jenis_kelamin;

int blocks[] = {4, 5, 6, 8, 9};
#define total_blocks  (sizeof(blocks) / sizeof(blocks[0]))

#define RST_PIN  0  // D3
#define SS_PIN   2  // D4
#define BUZZER   16 // D0

MFRC522 mfrc522(SS_PIN, RST_PIN);
MFRC522::MIFARE_Key key;
MFRC522::StatusCode status;

int blockNum = 2;
byte bufferLen = 18;
byte readBlockData[18];

void setup() {
  Serial.begin(9600);
  delay(10);
  SPI.begin();
  pinMode(BUZZER, OUTPUT);
  digitalWrite(BUZZER, LOW);
  lcd.init();
  lcd.backlight();
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Connecting to");
  lcd.setCursor(0,1);
  lcd.print("WiFi...");

  WiFi.begin(ssid, password);
  Serial.print("Connecting to ");
  Serial.print(ssid); Serial.println(" ...");

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("\nConnection established!");
  Serial.print("IP address:\t");
  Serial.println(WiFi.localIP());

  client = new HTTPSRedirect(httpsPort);
  client->setInsecure();
  client->setPrintResponseBody(true);
  client->setContentTypeHeader("application/json");

  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Connecting to");
  lcd.setCursor(0,1);
  lcd.print("Google ");
  delay(3000);

  Serial.print("Connecting to ");
  Serial.println(host);

  bool flagConn = false;
  for(int i=0; i<5; i++) {
    int retval = client->connect(host, httpsPort);
    if (retval == 1) {
      flagConn = true;
      String msg = "Connected. OK";
      Serial.println(msg);
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print(msg);
      delay(1000);
      break;
    } else {
      Serial.println("Connection failed. Retrying...");
    }
  }

  if (!flagConn) {
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Connection fail");
    Serial.print("Could not connect to server: ");
    Serial.println(host);
    delay(3000);
    return;
  }
  delete client;
  client = nullptr;
}

void loop() {
  static bool flag = false;
  if (!flag) {
    client = new HTTPSRedirect(httpsPort);
    client->setInsecure();
    flag = true;
    client->setPrintResponseBody(true);
    client->setContentTypeHeader("application/json");
  }
  if (client != nullptr) {
    if (!client->connected()) {
      client->connect(host, httpsPort);
    }
  } else {
    Serial.println("Error creating client object!");
  }

  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Scan your Tag");

  mfrc522.PCD_Init();
  if (!mfrc522.PICC_IsNewCardPresent()) { return; }
  if (!mfrc522.PICC_ReadCardSerial()) { return; }

  Serial.println();
  Serial.println(F("Reading last data from RFID..."));

  String values = "";
  String data;

  for (byte i = 0; i < total_blocks; i++) {
    ReadDataFromBlock(blocks[i], readBlockData);
    data = String((char*) readBlockData);
    data.trim();

    if (i == 0) {
      nama = data;
      values = "\"" + data + ",";
    }
    else if (i == total_blocks - 1) {
      jenis_kelamin = data;
      values += data + "\"}";
    }
    else if (i == 1) {
      no_absen = data;
      values += data + ",";
    }
    else if (i == 2) {
      kelas = data;
      values += data + ",";
    }
    else if (i == 3) {
      jurusan = data;
      values += data + ",";
    }
    else {
      values += data + ",";
    }
  }

  payload = payload_base + values;
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Publishing Data");
  lcd.setCursor(0,1);
  lcd.print("Please Wait...");

  digitalWrite(BUZZER, HIGH);
  delay(100);
  digitalWrite(BUZZER, LOW);
  delay(100);
  digitalWrite(BUZZER, HIGH);
  delay(100);
  digitalWrite(BUZZER, LOW);
  delay(100);

  Serial.println("Publishing data...");
  Serial.println(payload);
  if (client->POST(url, host, payload)) {
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Hallo");
    lcd.setCursor(0,1);
    lcd.print(nama);
  } else {
    Serial.println("Error while connecting");
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Failed.");
    lcd.setCursor(0,1);
    lcd.print("Try Again");
  }

  delay(2000);
}

void ReadDataFromBlock(int blockNum, byte readBlockData[])
{
  for (byte i = 0; i < 6; i++){
    key.keyByte[i] = 0xFF;
  }

  status = mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A,
                                     blockNum, &key, &(mfrc522.uid));
  if (status != MFRC522::STATUS_OK){
    Serial.print("Authentication failed for Read: ");
    Serial.println(mfrc522.GetStatusCodeName(status));
    return;
  }

  status = mfrc522.MIFARE_Read(blockNum, readBlockData, &bufferLen);
  if (status != MFRC522::STATUS_OK){
    Serial.print("Reading failed: ");
    Serial.println(mfrc522.GetStatusCodeName(status));
    return;
  } else {
    readBlockData[16] = ' ';
    readBlockData[17] = ' ';
    Serial.println("Block was read successfully");
  }
}

