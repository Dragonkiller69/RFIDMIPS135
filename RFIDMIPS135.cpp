

#include "Arduino.h"
#include <SPI.h>
#include <MFRC522.h>
#include <EEPROM.h>
#include "RFIDMIPS135.h"

/*
Constructor de la clase que recibe parametros de varios pines.
*/
RFID::RFID(byte _LEDverde, byte _LEDrojo, byte _Buzzer, byte _Relay)
{
   Serial.begin(9600);
   /*El ESP8266 simula la eeprom en la memoria flash, debe indicarse 
   cuanta memoria se alojara con EEPROM.begin()*/
   EEPROM.begin(512);
   LEDverde = _LEDverde;
   LEDrojo = _LEDrojo;
   Buzzer = _Buzzer;
   Relay = _Relay;
   pinMode(LEDverde, OUTPUT);
   pinMode(LEDrojo, OUTPUT);
   pinMode(Relay, OUTPUT);
   pinMode(Buzzer, OUTPUT);
   noTone(Buzzer);
   digitalWrite(Relay, HIGH);
   MFRC522 _mfrc522(SDA, RST); // Create MFRC522 instance.
   mfrc522 = _mfrc522;
   SPI.begin();        // Initiate  SPI bus
   mfrc522.PCD_Init(); // Initiate MFRC522
   actualizarLLAVES();
   Serial.println();
}

/*
Este metodo devuelve el UID de una llave RFID, conviertiendo 
caracteres ASCII a hexadecimal.
*/
String RFID::getUID()
{
   String content = "";
   byte *buffer = mfrc522.uid.uidByte; //Datos de UID
   byte bufferSize = mfrc522.uid.size; //Tamagno de UID
   for (byte i = 0; i < bufferSize; i++)
   {
      content.concat(String(buffer[i] < 0x10 ? " 0" : ":"));
      content.concat(String(buffer[i], HEX));
   }
   content.toUpperCase();
   Serial.println(content);
   return content.substring(1);
}

/*
Valida la existencia de las llaves 
*/
boolean RFID::validarLlave(String llave)
{
   for (int index = 0; index < 43; index++)
   {
      if (LLAVES[index].equals(llave))
      {
         return true;
      }
   }
   return false;
}

/*
Este metodo se encarga de abrir o cerar la cerradura
*/
void RFID::operarCerradura()
{
   Serial.println();
   Serial.print("Message : ");
   if (validarLlave(getUID())) //Se valida la llave
   {
      Serial.println("Acceso Autorizado");
      Serial.println();
      digitalWrite(Relay, LOW);
      digitalWrite(LEDverde, HIGH);
      delay(ACCESS_DELAY);
      digitalWrite(Relay, HIGH);
      digitalWrite(LEDverde, LOW);
   }
   else
   {
      Serial.println("Acceso denegado");
      digitalWrite(LEDrojo, HIGH);
      tone(Buzzer, 255);
      //digitalWrite(BUZZER, HIGH);
      delay(DENIED_DELAY);
      digitalWrite(LEDrojo, LOW);
      noTone(Buzzer);
   }
}

/*
Metodo utilizado para escribir una nueva llave en la EEPROM
*/
boolean RFID::addCard(){
   while (!mfrc522.PICC_IsNewCardPresent()) //Mientras no se acerca una nueva llave.
   {
      digitalWrite(LEDrojo, HIGH);
      delay(300);
      digitalWrite(LEDrojo, LOW);
      digitalWrite(LEDverde, HIGH);
      delay(300);
      digitalWrite(LEDverde, LOW);
   }

   if (mfrc522.PICC_ReadCardSerial()){ //Selecciona la llave cercana
   
      if (!validarLlave(getUID())){ //Se verifica que la llave no exista en la EEPROM;
         if (cantidadLlaves == 0){
            EEPROM.put(0, getUID());
         }else{
            EEPROM.put(sizeof(String) * cantidadLlaves, getUID()); //Se desplaza a la direccion vacia despues de la ultima llave. 
         }
         EEPROM.commit(); //------------------Sin este comando no se escriben los cambios en la EEPROM.
         actualizarLLAVES(); //----------------Se actualiza el array LLAVES cuando una nueva es agregada.
         Serial.println("Llave agregada");
         mfrc522.PICC_HaltA();
         return true;
      } else{
         Serial.println("Llave existente");
      }
      mfrc522.PICC_HaltA(); //Comando utilizado para que no lea la misma llave hasta q se aleje.
      return false;
   }
}

/*
Actializa las llaves leyendo directamente
de la EEPROM donde estan almacendas y las 
asignas a un vector.
*/
void RFID::actualizarLLAVES()
{
   String temp;
   for (int index = 0; index < 43; index++)
   {
      EEPROM.get((sizeof(String) * index), temp); //salta de 12 en 12 buscando las llaves
      if (temp != 0)
      {
         LLAVES[index] = temp;
         cantidadLlaves = index;
         Serial.print("Cantidad de llaves: ");
         Serial.println(index);
         Serial.println(temp);
      }
   }
}

/*
Borra los datos de la EEPROM
unicamente de los primeros 512 bytes usados
*/
void RFID::limpiarEEPROM()
{
   for (int i = 0; i < 512; i++)
   {
      EEPROM.write(i, 0);
      EEPROM.commit();
   }
}
