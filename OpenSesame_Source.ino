#include <Arduino.h>
 /*
Incluye librerias arduino
*/

//Código para funcionamiento de Relé y Sensor combinado


#define DMAX 10
/*Pines*/
#define pinLedOn   16
#define pinLedOff  17
#define pinEco     26
#define pinGatillo 27

#define R1  22
#define R2  21
#define R3  33
#define R4  25

int DISTANCIA = 0;

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

void relay(int d)
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

void setup() {
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

void loop() {
  //Calculamos la distancia en cm
  DISTANCIA = 0.01723 * readUltrasonicDistance(pinGatillo, pinEco);
  //Mostramos la distancia
  Serial.println(DISTANCIA);

  relay(DISTANCIA);
  delay(10);
} 
