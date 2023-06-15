#include <Arduino.h>
#include "firebase.h"
#include "LoRa_OS.h"
#include "actuadores.h"


/* MODULO PRINCIPAL (Servidor)*/
void setup() {
	
	firebase_Setup();
	lora_Setup();

}

//Bucle principal
void loop() {
	if(firebase_Loop() == "OPEN");
	{
		
	}
}