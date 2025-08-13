/*
===============================================================================
  Semester 4 Mini Project - IoT Based IDEA Lab Automation

  Under Supervision - Mr. Mayank Deep Khare Sir
  By
    Sumesh Sarkar  - 2201331550123 - CSE (IoT) A
    Anshul Chauhan - 2201331550026 - CSE (IoT) A
    Chetan Chauhan - 2201331550044 - CSE (IoT) A
    Shivam Tiwari  - 2201331550119 - CSE (IoT) A
===============================================================================
*/


//FirebaseESP8266.h must be included before ESP8266WiFi.h
#include "FirebaseESP8266.h"	// Install Firebase ESP8266 library
#include <DHT.h>		// Install DHT11 Library and Adafruit Unified Sensor Library

#include <ESP8266WiFi.h>


#define FIREBASE_HOST "<your-project-path>.firebaseio.com" //Without http:// or https:// schemes
#define FIREBASE_AUTH "<your-secret-auth-token>"
#define WIFI_SSID "<your-wifi-ssid>"         //wifi name (should be switched to 4G or below)
#define WIFI_PASSWORD "<your-wifi-password>"

//Define FirebaseESP8266 data object
FirebaseData firebaseData;
// FirebaseJson json;

// define the GPIO connected with Relays
#define LRelay1  D5 //GPIO 14
#define LRelay2  D0 //GPIO 16
#define LRelay3  D1 //GPIO 5

#define FRelay0  D2 //GPIO 0
#define FRelay1  D7 //GPIO 13
#define FRelay2  D6 //GPIO 15
#define FRelay3  D9 //GPIO 12
#define FRelay4  D10 //GPIO 1

#define wifiLed D4
#define pir1 A0 //GPIO 3
// #define pir2 D6 //GPIO 12

#define DHTPIN D3		//Connect Data pin of DHT to GPIO 4
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

short led1 = 0, led2 = 0, led3 = 0, fan0 = 0, fan1 = 0, fan2 = 0, fan3 = 0, fan4 = 0;
short pir1val = 0, pir2val = 0, pir1trig = 0, automode = 0, fanmode = 0, prevalfan = 0, flag = 0;   //triggers
float hval = 0, tval = 0; //variable to store humidity & temperature value


void setup() {
  //start dht readings
  dht.begin();

  //initialize and set all the relays as output
  pinMode(LRelay1, OUTPUT); digitalWrite(LRelay1, HIGH);
  pinMode(LRelay2, OUTPUT); digitalWrite(LRelay2, HIGH);
  pinMode(LRelay3, OUTPUT); digitalWrite(LRelay3, HIGH);

  pinMode(FRelay0, OUTPUT); digitalWrite(FRelay0, HIGH);
  pinMode(FRelay1, OUTPUT); digitalWrite(FRelay1, HIGH);
  pinMode(FRelay2, OUTPUT); digitalWrite(FRelay2, HIGH);
  pinMode(FRelay3, OUTPUT); digitalWrite(FRelay3, HIGH);
  pinMode(FRelay4, OUTPUT); digitalWrite(FRelay4, HIGH);

  pinMode(wifiLed, OUTPUT); digitalWrite(wifiLed, HIGH);
  pinMode(pir1,INPUT);
  // pinMode(pir2,INPUT);
  
  //establish connection with wifi
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(300);
  }
  digitalWrite(wifiLed, LOW);

  //establish connection with Firebase Real Time Database
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  Firebase.reconnectWiFi(true);
  delay(1000);
}


void loop() {
  //check wifi is connected or not
  if (WiFi.status() != WL_CONNECTED) { 
    digitalWrite(wifiLed, HIGH); //Turn off WiFi LED
  } else{ 
    digitalWrite(wifiLed, LOW); //Turn on WiFi LED
  }

  if(Firebase.get(firebaseData,"/DB_IoT_Classroom/autoMode")) {
    if (firebaseData.dataType() == "string") {
      automode = firebaseData.stringData().toInt();
    }
  }
  delay(10);

  if(automode == 1) {
    int val = analogRead(pir1);
    if(val < 500) { pir1val = 0; }
    else { pir1val = 1; }
    // pir1val = digitalRead(pir1);
    // pir2val = digitalRead(pir2);
    if((pir1val == 1 || pir2val == 1) && pir1trig == 0) {
      pir1trig = 1;
      led1=1, led2=1, led3=1;
      dbLedUpdate();
    }
    else if((pir1val == 0 || pir2val == 0) && pir1trig == 1) {
      pir1trig = 0;
      flag = 0;
      led1=0, led2=0, led3=0, fan0=0, fan1=0, fan2=0, fan3=0, fan4=0;
      dbLedUpdate();
      prevalfan = 0;
      dbFanUpdate();
    }
  }

  else {
    if(Firebase.get(firebaseData,"/DB_IoT_Classroom/led1")) {
      if (firebaseData.dataType() == "string") {
        led1 = firebaseData.stringData().toInt();
      }
    }
    delay(10);

    if(Firebase.get(firebaseData,"/DB_IoT_Classroom/led2")) {
      if (firebaseData.dataType() == "string") {
        led2 = firebaseData.stringData().toInt();
      }
    }
    delay(10);

    if(Firebase.get(firebaseData,"/DB_IoT_Classroom/led3")) {
      if (firebaseData.dataType() == "string") {
        led3 = firebaseData.stringData().toInt();
      }
    }
  }

  tempUpdate();

  if(Firebase.get(firebaseData,"/DB_IoT_Classroom/fanMode")) {
    if (firebaseData.dataType() == "string") {
      fanmode = firebaseData.stringData().toInt();
    }
  }
  delay(10);

  if(fanmode == 1 && (pir1trig == 1 || automode == 0)) {
    tempFan();
  }
  else {
    if(Firebase.get(firebaseData,"/DB_IoT_Classroom/fan0")) {
      if (firebaseData.dataType() == "string") {
        fan0 = firebaseData.stringData().toInt();
      }
    }
    delay(10);

    if(Firebase.get(firebaseData,"/DB_IoT_Classroom/fan1")) {
      if (firebaseData.dataType() == "string") {
        fan1 = firebaseData.stringData().toInt();
      }
    }
    delay(10);

    if(Firebase.get(firebaseData,"/DB_IoT_Classroom/fan2")) {
      if (firebaseData.dataType() == "string") {
        fan2 = firebaseData.stringData().toInt();
      }
    }
    delay(10);

    if(Firebase.get(firebaseData,"/DB_IoT_Classroom/fan3")) {
      if (firebaseData.dataType() == "string") {
        fan3 = firebaseData.stringData().toInt();
      }
    }
    delay(10);

    if(Firebase.get(firebaseData,"/DB_IoT_Classroom/fan4")) {
      if (firebaseData.dataType() == "string") {
        fan4 = firebaseData.stringData().toInt();
      }
    }
    delay(10);
  }

  delay(10);
  Relays();
}


void tempUpdate(){
  hval = dht.readHumidity();
  tval = dht.readTemperature();

  if (isnan(hval) || isnan(tval)) {
    hval = 0;
    tval = 0;
  }

  if (Firebase.setFloat(firebaseData, "/DB_IoT_Classroom/temperature", tval)) {
  } else {
  }
  delay(10);

  if (Firebase.setFloat(firebaseData, "/DB_IoT_Classroom/humidity", hval)) {
  } else {
  }
  delay(10);

  delay(100);
}


void tempFan(){
  prevalfan = flag;

  if(tval < 20.0) {
    flag = 1;
    fan0=0, fan1=0, fan2=0, fan3=0, fan4=0;
  }

  if(tval > 20.0 && tval < 30.0) {
    flag = 2;
    fan0=1, fan1=0, fan2=1, fan3=0, fan4=1;
  }

  if(tval > 30.0) {
    flag = 3;
    fan0=1, fan1=1, fan2=1, fan3=1, fan4=1;
  }

  if(prevalfan != flag) {
    dbFanUpdate();
  }
}


void dbLedUpdate(){
  if(Firebase.setString(firebaseData, "/DB_IoT_Classroom/pir", String(pir1trig))){
  } else{
  }
  delay(10);

  if(Firebase.setString(firebaseData, "/DB_IoT_Classroom/led1", String(led1))){
  } else{
  }
  delay(10);

  if(Firebase.setString(firebaseData, "/DB_IoT_Classroom/led2", String(led2))){
  } else{
  }
  delay(10);

  if(Firebase.setString(firebaseData, "/DB_IoT_Classroom/led3", String(led3))){
  } else{
  }
  delay(10);
}


void dbFanUpdate(){
  if(Firebase.setString(firebaseData, "/DB_IoT_Classroom/fan0", String(fan0))){
  } else{
  }
  delay(10);

  if(Firebase.setString(firebaseData, "/DB_IoT_Classroom/fan1", String(fan1))){
  } else{
  }
  delay(10);

  if(Firebase.setString(firebaseData, "/DB_IoT_Classroom/fan2", String(fan2))){
  } else{
  }
  delay(10);

  if(Firebase.setString(firebaseData, "/DB_IoT_Classroom/fan3", String(fan3))){
  } else{
  }
  delay(10);

  if(Firebase.setString(firebaseData, "/DB_IoT_Classroom/fan4", String(fan4))){
  } else{
  }
  delay(10);
}


void Relays(){  
  digitalWrite(LRelay1, !led1);  
  digitalWrite(LRelay2, !led2);
  digitalWrite(LRelay3, !led3);

  digitalWrite(FRelay0, !fan0);
  digitalWrite(FRelay1, !fan1);
  digitalWrite(FRelay2, !fan2);
  digitalWrite(FRelay3, !fan3);
  digitalWrite(FRelay4, !fan4);

  delay(100);
}

