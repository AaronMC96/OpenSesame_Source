/* Libraries */
#include <LoRa.h>	//Externo	
#include "LoRa_OS.h" //Libreria interna OS

/*IMPORTANTE, INICIALIZAR Serial.begin(115200) YA QUE NINGUNA FUNCION DE CONFIG LO ESTÁ HACIENDO; */
/*
	void lora_Setup(void)
		
*/
void lora_Setup(void)
{
 //Configura los pins del modulo RF85 - Si no hay exito queda en un loop y manda un mensaje de error
  LoRa.setPins(SS, RST, DI0);                           
  if (!LoRa.begin(915E6)) {
    Serial.println("Error iniciando el módulo LoRa");
    while (1);
  } else Serial.println("Módulo LoRa iniciado correctamente"); //Éxito!

  //Configuración de GPIOs 
  pinMode(LEDTX, OUTPUT); //LED Transmisión
  pinMode(LEDRX, OUTPUT); //LED Recepción
}
/*
	void lora_Loop (void) : 
		NO RECIBE ARGUMENTOS XQ TRABAJA CON LA VARIABLE LOCAL AL .H ""SENDER""
	La función pasa de tranmisión a recepción y viceversa en cuestión de tiempo, si no
	se esta enviando/recibiendo.
	
	Modificando unas pocas lineas de código se puede controlar el estado de la función externamente
		1. Agregando un parametro booleano a la entrada tipo 'SENDER'
		2. Eliminando las definiciones de 'SENDER'. Pj: SENDER = tuher.
	esto lo efectua ...lora_Noloop(...)
*/

void lora_Loop(void)
{
	/******** MODO TRANMISIÓN ********/
  if (SENDER) {

    digitalWrite(LEDTX, HIGH);                      // Prende led transmisión

    // Comienzo de la tranmisión 
    LoRa.beginPacket();
    LoRa.print("Tuchitachi");					//Mensaje a TRANSMITIR
    LoRa.endPacket();
    Serial.println("Mensaje Enviado");
    // Fin de la tranmisión

    SENDER = false;
    t0 = millis();                              //Toma el tiempo incial de recepción
	
	digitalWrite(LEDTX, LOW);                     //Apaga el led transmisión 


  } 
/******** MODO RECEPCIÓN *******/
else { 
    
    digitalWrite(LEDRX, HIGH);                     //Apaga el led recepción 
    
    if (LoRa.parsePacket()) {                   //Si hay un paquete para leer

// Lee el String mientras que haya bytes para leer
      String mensajeRecibido = "";
      while (LoRa.available()) {
        mensajeRecibido += (char)LoRa.read();
      }

      // Imprime el mensaje recibido
      Serial.print("Mensaje recibido: ");
      Serial.print(mensajeRecibido);				//mensaje RECIBIDO
      Serial.print(" con RSSI ");
      Serial.println(LoRa.packetRssi());

      SENDER = true;
	  digitalWrite(LEDRX, LOW);					//Apaga el led recepción
    }
//Si no se recibio nada, espera un TimeOut y pasa al MODO TRANMISIÓN
    if (millis() - t0 >= 1000) {
      
      SENDER = true;
      Serial.println("Timeout ");

      t0 = millis();                  // Reinicia el tiempo de inicio
    }
  }
  delay(100);     
}
/*
	unsigned long lora_Noloop(bool SENDER, long tF)
		Remplaza a la función lora_Loop cuando se desea utilizar espeficificamente un modo de 
	comunicación (Transmisor/Receptor).
		Toma como parametros el estado (que toma el lugar de SENDER global):
			SENDER = true 	  -    TRANSMISIÓN
			SENDER = false    -    RECEPCIÓN
		y el tiempo de timeOut para recepción deseado tF.
	Devuelve el tiempo total
	
	Lo demás funciona igual que la función anterior.
*/
u long lora_Noloop(bool SENDER, long tF)
{
	
	unsigned long t0 = millis();
	
	/******** MODO TRANMISIÓN ********/
  if (SENDER) {
	  				
    digitalWrite(LEDTX, HIGH);                      // Prende led transmisión

    // Comienzo de la tranmisión 
    LoRa.beginPacket();
    LoRa.print("Tuchitachi");					//Mensaje a TRANSMITIR
    LoRa.endPacket();
    Serial.println("Mensaje Enviado");
    
	// Fin de la tranmisión
	digitalWrite(LEDTX, LOW);                     //Apaga el led transmisión 
	t0 = millis() - t0;                              //Toma el tiempo total de recepción

  } 
/******** MODO RECEPCIÓN *******/
else { 
    
    digitalWrite(LEDRX, HIGH);                     //Apaga el led recepción 
    
    if (LoRa.parsePacket()) {                   //Si hay un paquete para leer

// Lee el String mientras que haya bytes para leer
      String mensajeRecibido = "";
      while (LoRa.available()) {
        mensajeRecibido += (char)LoRa.read();
      }

      // Imprime el mensaje recibido
      Serial.print("Mensaje recibido: ");
      Serial.print(mensajeRecibido);				//mensaje RECIBIDO
      Serial.print(" con RSSI ");
      Serial.println(LoRa.packetRssi());

      
	  digitalWrite(LEDRX, LOW);					//Apaga el led recepción
	  t0 = millis() - t0;
	  
    }
//Si no se recibio nada, espera un TimeOut y sale
    if (t0 >= tF) Serial.println("Timeout ");
  }     
  return t0;
}
