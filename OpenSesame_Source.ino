#include <Arduino.h>
//Librerias
#include <LoRa.h>
#if defined(ESP32)
  #include <WiFi.h>
#elif defined(ESP8266)
  #include <ESP8266WiFi.h>
#endif
#include <Firebase_ESP_Client.h>
#include <Wire.h>
//aaron pata dura

// Provide the token generation process info.
#include "addons/TokenHelper.h"
// Provide the RTDB payload printing info and other helper functions.
#include "addons/RTDBHelper.h"

// Insert your network credentials
// #define WIFI_SSID "SBG6700AC-01B96"
// #define WIFI_PASSWORD "2b0ca2f4d4"

#define WIFI_SSID "Casa 2.4GHz"
#define WIFI_PASSWORD "0141372386"

// Insert Firebase project API Keyon
#define API_KEY "AIzaSyCqSbAK8TwGf8wG0t61dRgyy0yYGLoDyrI"

// Insert Authorized Email and Corresponding Password
#define USER_EMAIL "choqueaaron@gmail.com"
#define USER_PASSWORD "UtenianosOS"

// Insert RTDB URLefine the RTDB URL
#define DATABASE_URL "https://esp32led-250ba-default-rtdb.firebaseio.com/"

//LoRa Pin defines
#define SCK 18   //GPIO18  -- SX1278's SCK
#define MISO 19  //GPIO19 -- SX1278's MISO
#define MOSI 23  //GPIO23 -- SX1278's MOSI
#define SS 05    //GPIO5 -- SX1278's CS
#define RST 14   //GPIO14 -- SX1278's RESET
#define DI0 02   //GPIO2  -- SX1278's IRQ(Interrupt Request)

//GPIO defines
#define LEDTX 16  //GPIO 19 -- LED  - VAMOS A PONERLE16/17
#define LEDRX 17

// Define Firebase objects
FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;

// Variable to save USER UID
String uid;

// Variables to save database paths
String databasePath;
String ledPath;

//const int led = 2;//led

// Timer variables (send new readings every three minutes)
unsigned long sendDataPrevMillis = 0;
unsigned long timerDelay = 2000;

string status_Led = "OFF";



//Local Variables
bool SENDER = false;           //Conmutador entre recepción y tranmisión
unsigned long t0 = 0;         //Tiempo de inicio para el TimeOut

<<<<<<< HEAD
//cambio

int DISTANCIA = 0;
=======
/**************************************
TODO:
 - CAMBIAR LED A TX P16 Y RX P17
 - IMPLEMENTAR GITHUB
 - HACER UN CRC
 - HACER ANDAR SENSORES
 - SERVIDOR
>>>>>>> c1bec31 (Se fusionó LoRa con FireBase)

 Los dos últimos son medios generales, a medida que nos vayamos metiendo pondriamos TODOs más específicos
**************************************/

// Initialize WiFi
void initWiFi() {
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to WiFi ..");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print('.');
    delay(1000);
  }
  Serial.println(WiFi.localIP());
  Serial.println();
}

// Write float values to the database
void sendFloat(String path, float value){
  if (Firebase.RTDB.setFloat(&fbdo, path.c_str(), value)){
    Serial.print("Writing value: ");
    Serial.print (value);
    Serial.print(" on the following path: ");
    Serial.println(path);
    Serial.println("PASSED");
    Serial.println("PATH: " + fbdo.dataPath());
    Serial.println("TYPE: " + fbdo.dataType());
  }
  else {
    Serial.println("FAILED");
    Serial.println("REASON: " + fbdo.errorReason());
  }
}
void readData(String path){
  String readIncoming ="";
  if (Firebase.RTDB.getString(&fbdo, path.c_str())){
    Serial.println("PATH: " + fbdo.dataPath());
    Serial.println("TYPE: " + fbdo.dataType());
    if(fbdo.dataType()=="string"){
      readIncoming =fbdo.stringData();
       Serial.println("DATA: " + readIncoming);
      if(readIncoming=="ON"){
        digitalWrite(LEDRX, HIGH);   // turn the LED on (HIGH is the voltage level)
        status_Led="ON";
        }
      else{
         digitalWrite(LEDRX, LOW);   // turn the LED off (LOW is the voltage level)
         status_Led="OFF";
      }
    }
  }
  else {
    Serial.println("FAILED");
    Serial.println("REASON: " + fbdo.errorReason());
  }
}

void setup(){
  //Configura la conexión serial en 115200 baudios
  Serial.begin(115200);                           
  while (!Serial);

  //Configura los pins del modulo RF85 - Si no hay exito queda en un loop y manda un mensaje de error
  LoRa.setPins(SS, RST, DI0);                           
  if (!LoRa.begin(915E6)) {
    Serial.println("Error iniciando el módulo LoRa");
    while (1);
  } else Serial.println("Módulo LoRa iniciado correctamente"); //Éxito!

  //Configuración de GPIOs 
  pinMode(LEDTX, OUTPUT); //LED testigo
  pinMode(LEDRX, OUTPUT);
  digitalWrite(LEDTX, LOW); 
  digitalWrite(LEDRX, LOW); 

  //initBME();
  initWiFi();

  // Assign the api key (required)
  config.api_key = API_KEY;

  // Assign the user sign in credentials
  auth.user.email = USER_EMAIL;
  auth.user.password = USER_PASSWORD;

  // Assign the RTDB URL (required)
  config.database_url = DATABASE_URL;

  Firebase.reconnectWiFi(true);
  fbdo.setResponseSize(4096);

  // Assign the callback function for the long running token generation task */
  config.token_status_callback = tokenStatusCallback; //see addons/TokenHelper.h

  // Assign the maximum retry of token generation
  config.max_token_generation_retry = 5;

  // Initialize the library with the Firebase authen and config
  Firebase.begin(&config, &auth);

  // Getting the user UID might take a few seconds
  Serial.println("Getting User UID");
  while ((auth.token.uid) == "") {
    Serial.print('.');
    delay(1000);
  }
  // Print user UID
  uid = auth.token.uid.c_str();
  Serial.print("User UID: ");
  Serial.println(uid);

  // Update database path
  databasePath = "/UsersData/" + uid;

  // Update database path for sensor readings
  ledPath = databasePath + "/led"; // --> UsersData/<user_uid>/led
}

void loop(){
  // Send new readings to database
  if (Firebase.ready() && (millis() - sendDataPrevMillis > timerDelay || sendDataPrevMillis == 0)){
    sendDataPrevMillis = millis();

    readData(ledPath);
  }


/******** MODO TRANMISIÓN ********/
  if (SENDER) {

    if (status_Led=="ON"){

      digitalWrite(LEDTX, HIGH);                      // Prende led testigo
      // Comienzo de la tranmisión 
      LoRa.beginPacket();
      LoRa.print("ON");
      LoRa.endPacket();
      Serial.println("Mensaje Enviado");
      // Fin de la tranmisión      
      digitalWrite(LEDTX, LOW);
    }

    else if (status_Led=="OFF"){

      digitalWrite(LEDTX, HIGH);                      // Prende led testigo
      // Comienzo de la tranmisión 
      LoRa.beginPacket();
      LoRa.print("OFF");
      LoRa.endPacket();
      Serial.println("Mensaje Enviado");
      // Fin de la tranmisión   
      digitalWrite(LEDTX, LOW);   
    }

    //SENDER = false;
    //t0 = millis();                              //Toma el tiempo incial de recepción

  } 

