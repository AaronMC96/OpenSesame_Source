#include <Arduino.h>
#include "firebase.h"
#include "LoRa_OS.h"



/* MODULO PRINCIPAL (Servidor)*/
void setup() {
  Serial.begin(115200);  //arduino serial config.
  firebase_Setup();      //lora module config.
  lora_Setup();          //sensor and rele config.
}

/*
  BUCLE PRICINPAL

  Si se recibe un comando de apertura mediante el servidor (firebase)
  comienza la tranmisión desde PRINCIPAL a MECANISMO

  Sino se queda en modo RX (recepción)

  Cosas que no estaría mal agregar:
    INFORMACIÓN SOBRE EL MODULO MECANISMO:
    - TEMPERATURA
    - EST. DE BATERIA

    - TRANSMITIR MÁS DE UNA VEZ (?)
    - CRC !



    La función lora_Loop esta medio al pedo. Podría quedar para hacer testeos.
    Y es una buena idea: Implementar funciones de prueba x si algo falla cuando este todo armado. 
*/
void loop() {
  if (firebase_Loop() == "OPEN")  //Si se envia el comando "abrir" - Tal vez el comando puede ser "ACCIONAR" y togglear MECANISMO
  {
    lora_Noloop(true, "ALGO")         //Se pone el modulo en modo TX - No hay timeout
  } 
  //Esto que sigue estaría obteniendo información del modulo mecanismo
  //else lora_Noloop(false, "1000");  //Se pone en modo RX - Esta función tiene una respuesta lenta. Se queda 'tF' segundos en ella.


  delay(100) //si fuera necesario
}

/*
  La idea es de alguna manera el firebase loop trabaje tomando varios comandos mediante un case...pj.
  y esto lo enviará el lora loop. 

  Creo que las funciones terminan siendo medio parecidas y deberiamos trabajarlas en conjunto.
  Si es posible mediante switch que quede bien ordenado en cada una cada cual opción. Y estas opciones deben 
  propocionar distintas configuraciones agregadas.
*/