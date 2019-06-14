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
		byte SDA = 2;
		byte SCK = 14;
		byte MOSI = 13;
		byte MISO = 12;
		byte RST = 5;
		byte LEDverde;
		byte LEDrojo;
		byte Buzzer;
		byte Relay;
		MFRC522 mfrc522;
		String UID = "";
		int DENIED_DELAY = 1000;
		int ACCESS_DELAY = 1000;
		String LLAVES[42];
		int cantidadLlaves = 0;

		RFID(byte _LEDverde, byte _LEDrojo, byte _Buzzer, byte _Relay);

		String getUID();

		boolean addCard();

		boolean validarLlave(String llave);

		void operarCerradura();

		void limpiarEEPROM();

		void actualizarLLAVES();

};


#endif
