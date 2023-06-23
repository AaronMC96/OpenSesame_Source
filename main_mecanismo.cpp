#include <Arduino.h>
#include "LoRa_OS.h"
#include "actuadores.h"

#include <Time.h>
#include <TimeLib.h>

time_t tiempo;

/* MODULO MECANISMO (Tranquera)*/
void setup() {

  Serial.begin(115200);  //arduino serial config.
  lora_Setup();          //lora module config.
  actuadores_Setup();    //sensor and rele config.
  // setTime(16, 20, 7, 20, 6, 2023);  //time init
}
void loop() {

  if (lora_Noloop(false, 1000) == "OPEN")  //modo RX -  Si recibe la señal de OPEN
  {
    actuadores_Loop();  //En este caso, comienza la medición de distancia y togglea los rele.
  }
  /* tiempo = now();

  if (minute(tiempo)) {
    lora_Noloop(true, tiempo);
  }*/
}