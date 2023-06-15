/*  Libraries */
#include <Arduino.h>
#include <Firebase_ESP_Client.h>
#include <Wire.h>
#include "addons/TokenHelper.h" // Provide the token generation process info.
#include "addons/RTDBHelper.h" // Provide the RTDB payload printing info and other helper functions.
#include "firebase.h"

#if defined(ESP32)
  #include <WiFi.h>
#elif defined(ESP8266)
  #include <ESP8266WiFi.h>
#endif





/* Defines */
#define API_KEY "AIzaSyCqSbAK8TwGf8wG0t61dRgyy0yYGLoDyrI" // Insert Firebase project API Key
#define USER_EMAIL "choqueaaron@gmail.com" //Authorized email and password
#define USER_PASSWORD "UtenianosOS"
#define DATABASE_URL "https://esp32led-250ba-default-rtdb.firebaseio.com/" // Insert RTDB URLefine the RTDB URL


/* Functions */
void initWiFi(void);
void sendFloat(String path, float value);
void readData(String path);
void firebase_Setup(void);
void firebase_Loop(void);
