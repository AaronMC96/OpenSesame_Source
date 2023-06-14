/* Libraries */
#include <LoRa.h>	//Externo	
#include "LoRa_OS.h" //Libreria interna OS

/* LoRa PIN Defines */
#define SCK 18   //GPIO18  -- SX1278's SCK
#define MISO 19  //GPIO19 -- SX1278's MISO
#define MOSI 23  //GPIO23 -- SX1278's MOSI
#define SS 05    //GPIO5 -- SX1278's CS
#define RST 14   //GPIO14 -- SX1278's RESET
#define DI0 02   //GPIO2  -- SX1278's IRQ(Interrupt Request)	

/* Local Variables */ 
bool SENDER = false;
unsigned long t0 = 0;

/* Funciones Globables */
void lora_Setup(void);
void lora_Loop(void);
u long lora_NoLoop(bool SENDER, long tF);