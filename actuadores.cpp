#include "actuadores.h"

#define DMAX 10  //Distancia máxima de sensado

void actuadores_config() {
  Serial.begin(115200);
  //Configuramos el pin del led como salida
  pinMode(pinLedOn, OUTPUT);
  pinMode(pinLedOff, OUTPUT);
  pinMode(R1, OUTPUT);
  pinMode(R2, OUTPUT);
  pinMode(R3, OUTPUT);
  pinMode(R4, OUTPUT);

  digitalWrite(pinLedOn, LOW);
  digitalWrite(pinLedOff, LOW);
  digitalWrite(R1, LOW);
  digitalWrite(R2, LOW);
  digitalWrite(R3, LOW);
  digitalWrite(R4, LOW);

}


long readUltrasonicDistance(int triggerPin, int echoPin)
{
  //Iniciamos el pin del emisor de reuido en salida
  pinMode(triggerPin, OUTPUT);
  //Apagamos el emisor de sonido
  digitalWrite(triggerPin, LOW);
  //Retrasamos la emision de sonido por 2 milesismas de segundo
  delayMicroseconds(2);
  // Comenzamos a emitir sonido
  digitalWrite(triggerPin, HIGH);
  //Retrasamos la emision de sonido por 2 milesismas de segundo
  delayMicroseconds(10);
  //Apagamos el emisor de sonido
  digitalWrite(triggerPin, LOW);
  //Comenzamos a escuchar el sonido
  pinMode(echoPin, INPUT);
  // Calculamos el tiempo que tardo en regresar el sonido
  return pulseIn(echoPin, HIGH);
}


void relay_status(int d)
{
  if( d < DMAX)
  {
    digitalWrite(R1, HIGH);
    digitalWrite(R2, HIGH);
    digitalWrite(R3, LOW);
    digitalWrite(R4, LOW);
    
    digitalWrite(pinLedOn, HIGH);
    digitalWrite(pinLedOff, LOW); 
  } 
  else
  {
    digitalWrite(R1, LOW);
    digitalWrite(R2, LOW);
    digitalWrite(R3, HIGH);
    digitalWrite(R4, HIGH);

    digitalWrite(pinLedOn, LOW);
    digitalWrite(pinLedOff, HIGH);
  }
}

void actuadores_rnwork(int d) {
  //Calculamos la distancia en cm
  d = 0.01723 * readUltrasonicDistance(pinGatillo, pinEco);
  //Mostramos la distancia
  Serial.println(d);

  relay(d);
  delay(500);
} 