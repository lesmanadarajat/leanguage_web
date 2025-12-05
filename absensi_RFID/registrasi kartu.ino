#include <SPI.h>
#include <MFRC522.h>

const uint8_t RST_PIN = D3;
const uint8_t SS_PIN = D4;

MFRC522 mfrc522(SS_PIN, RST_PIN);
MFRC522::MIFARE_Key key;

int blockNum = 4;
byte bufferLen = 18;
byte readBlockData[18];
MFRC522::StatusCode status;

void setup() {
  Serial.begin(9600);
  SPI.begin();
  mfrc522.PCD_Init();
  Serial.println("Scan a ID card...");
}

void loop() {
  for (byte i = 0; i < 6; i++) {
    key.keyByte[i] = 0xFF;
  }

  if (!mfrc522.PICC_IsNewCardPresent()) return;
  if (!mfrc522.PICC_ReadCardSerial()) return;

  Serial.println("\n**Card Detected**");
  Serial.print(F("Card UID:"));
  for (byte i = 0; i < mfrc522.uid.size; i++) {
    Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
    Serial.print(mfrc522.uid.uidByte[i], HEX);
  }
  Serial.println();
  Serial.print(F("PICC type: "));
  MFRC522::PICC_Type piccType = mfrc522.PICC_GetType(mfrc522.uid.sak);
  Serial.println(mfrc522.PICC_GetTypeName(piccType));

  byte buffer[18];
  byte len;
  Serial.setTimeout(20000L);

  // --- Nama ---
  Serial.println(F("---------------------------------------"));
  Serial.print(F("Nama: "));
  len = Serial.readBytesUntil('#', (char*)buffer, 16);
  for (byte i = len; i < 16; i++) buffer[i] = ' ';
  blockNum = 4;
  WriteDataToBlock(blockNum, buffer);
  ReadDataFromBlock(blockNum, readBlockData);
  Serial.print(F("Nama: "));
  Serial.println((char*)readBlockData);

  // --- No Absen ---
  Serial.println(F("---------------------------------------"));
  Serial.print(F("No Absen: "));
  len = Serial.readBytesUntil('#', (char*)buffer, 16);
  for (byte i = len; i < 16; i++) buffer[i] = ' ';
  blockNum = 5;
  WriteDataToBlock(blockNum, buffer);
  ReadDataFromBlock(blockNum, readBlockData);
  Serial.print(F("No Absen: "));
  Serial.println((char*)readBlockData);

  // --- Kelas ---
  Serial.println(F("---------------------------------------"));
  Serial.print(F("Kelas: "));
  len = Serial.readBytesUntil('#', (char*)buffer, 16);
  for (byte i = len; i < 16; i++) buffer[i] = ' ';
  blockNum = 6;
  WriteDataToBlock(blockNum, buffer);
  ReadDataFromBlock(blockNum, readBlockData);
  Serial.print(F("Kelas: "));
  Serial.println((char*)readBlockData);

  // --- Jurusan ---
  Serial.println(F("---------------------------------------"));
  Serial.print(F("Jurusan: "));
  len = Serial.readBytesUntil('#', (char*)buffer, 16);
  for (byte i = len; i < 16; i++) buffer[i] = ' ';
  blockNum = 8;
  WriteDataToBlock(blockNum, buffer);
  ReadDataFromBlock(blockNum, readBlockData);
  Serial.print(F("Jurusan: "));
  Serial.println((char*)readBlockData);

  // --- Jenis Kelamin ---
  Serial.println(F("---------------------------------------"));
  Serial.print(F("Jenis Kelamin: "));
  len = Serial.readBytesUntil('#', (char*)buffer, 16);
  for (byte i = len; i < 16; i++) buffer[i] = ' ';
  blockNum = 9;
  WriteDataToBlock(blockNum, buffer);
  ReadDataFromBlock(blockNum, readBlockData);
  Serial.print(F("Jenis Kelamin: "));
  Serial.println((char*)readBlockData);

  Serial.println(F("---------------------------------------"));
}

void WriteDataToBlock(int blockNum, byte blockData[]) {
  status = mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A,
                                     blockNum, &key, &(mfrc522.uid));
  if (status != MFRC522::STATUS_OK) {
    Serial.print(F("Authentication failed for Write: "));
    Serial.println(mfrc522.GetStatusCodeName(status));
    return;
  }

  status = mfrc522.MIFARE_Write(blockNum, blockData, 16);
  if (status != MFRC522::STATUS_OK) {
    Serial.print(F("Writing to Block failed: "));
    Serial.println(mfrc522.GetStatusCodeName(status));
    return;
  }
}

void ReadDataFromBlock(int blockNum, byte readBlockData[]) {
  for (byte i = 0; i < 6; i++) {
    key.keyByte[i] = 0xFF;
  }
  status = mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A,
                                     blockNum, &key, &(mfrc522.uid));
  if (status != MFRC522::STATUS_OK) {
    Serial.print(F("Authentication failed for Read: "));
    Serial.println(mfrc522.GetStatusCodeName(status));
    return;
  }

  status = mfrc522.MIFARE_Read(blockNum, readBlockData, &bufferLen);
  if (status != MFRC522::STATUS_OK) {
    Serial.print(F("Reading failed: "));
    Serial.println(mfrc522.GetStatusCodeName(status));
    return;
  }
}
