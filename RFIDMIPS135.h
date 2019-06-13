/*
  Libreria de RFID NFC
Universidad de El Salvador, Sede Santa Ana
ciclo I-2019 
*/
#ifndef RFIDMIPS135_h
#define RFIDMIPS135_h
#include "Arduino.h"
#include <SPI.h>
#include <MFRC522.h>


class RFID {
    public:
		byte SDA = 2;
		byte SCK = 14;
		byte MOSI = 13;
		byte MISO = 12;
		byte RST = 5;
		MFRC522 mfrc522;
		String UID ="";

		RFID();

		String getUID();


  
};


#endif
