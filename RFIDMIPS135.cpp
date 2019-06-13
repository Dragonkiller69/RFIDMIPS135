

#include "Arduino.h"
#include <SPI.h>
#include <MFRC522.h>
#include "RFIDMIPS135.h"

RFID::RFID(){
    MFRC522 _mfrc522(SDA, RST);   // Create MFRC522 instance.
    mfrc522 = _mfrc522;
    SPI.begin();          // Initiate  SPI bus
    mfrc522.PCD_Init();   // Initiate MFRC522
}

String RFID::getUID() {
   UID = "";
   byte *buffer = mfrc522.uid.uidByte;
   byte bufferSize = mfrc522.uid.size;
   for (byte i = 0; i < bufferSize; i++) {
     // Serial.print(buffer[i] < 0x10 ? " 0" : " ");
     // Serial.print(buffer[i], HEX);
      UID.concat(String(buffer[i] < 0x10 ? " 0" : " "));
      UID.concat(String(buffer[i], HEX));
   }
   return UID;
}