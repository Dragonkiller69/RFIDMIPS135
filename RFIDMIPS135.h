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
#include <EEPROM.h>


class RFID {
    public:
		byte SDA = D4;
		byte SCK = D5;
		byte MOSI = D7;
		byte MISO = D6;
		byte RST = 5;
		byte LEDverde;
		byte LEDrojo;
		byte Buzzer;
		byte Relay;
		String UID = "";
		int DENIED_DELAY = 1000;
		int ACCESS_DELAY = 1000;
		String LLAVES[42];
		int totalLlaves = 0;
		MFRC522 mfrc522;

		RFID(byte _LEDverde, byte _LEDrojo, byte _Buzzer, byte _Relay);

		String getUID();

		String getUIDSecure();

		boolean addCard();

		boolean validarLlave(String llave);

		void operarCerradura();

		void limpiarEEPROM();

		void actualizarLLAVES();

};


#endif
