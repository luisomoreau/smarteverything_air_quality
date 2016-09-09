// ---------------------------------------------------------------------
// This file is an example on how to use the the MQ135 sensor to measure
// quality and send the data through Sigfox network
//
// Created: 08.09.2016
// Author: Louis Moreau
// Code: https://github.com/luisomoreau/smarteverything_air_quality
// ---------------------------------------------------------------------

// ---------------------------------------------------------------------
// Define
// ---------------------------------------------------------------------

#define SENSOR A0 //analog input pin
#define DEBUG true
#define DELAY 600000 // Set the delay to 10 minutes (10 min x 60 seconds x 1000 milliseconds)

// ---------------------------------------------------------------------
// Include
// ---------------------------------------------------------------------

#include <Wire.h>
#include <Arduino.h>

// ---------------------------------------------------------------------
// Global variables
// ---------------------------------------------------------------------

int sensorValue = 0;
uint16_t message[6];

// ---------------------------------------------------------------------
// Public functions
// ---------------------------------------------------------------------

void setup() {

  pinMode(SENSOR, INPUT);
  SigFox.begin(19200);
  
  if(DEBUG){
    SerialUSB.begin(9600);
  }
  delay(100);
  setConfigurationMode();
  delay(1000);
  getID();

}

void loop() {
  /*
   * This part need a small explaination:
   * As the analogRead function will give us a result
   * betwwen 0 and 1023, we need to use two byte to store the value,
   * thus we store the value on the second byte (message[1])
   */
  sensorValue = analogRead(SENSOR);
  message[1] = sensorValue;
  if(DEBUG){
    SerialUSB.print("Sensor value: ");
    SerialUSB.println(sensorValue);
  }
  delay(100);
  resetLEDs();
  sendSigfox(message);
  resetLEDs();
  delay(DELAY);
}


//Send a sigfox message
void sendSigfox(uint16_t msg[]){
  String status = "";
  char output;
  if(DEBUG){
    for(int i= 0;i<sizeof(msg);i++){
      SerialUSB.print(String(msg[i], HEX));
    }
    SerialUSB.println();
    for(int i= 0;i<sizeof(msg);i++){
      SerialUSB.print(String(msg[i], DEC));
    }
  }
  
  SigFox.print("AT$SF=");
  for(int i= 0;i<sizeof(msg);i++){
    SigFox.print(String(msg[i], HEX));
  }
  
  SigFox.print("\r");
  while (!SigFox.available()){
     blink();
  }
  while(SigFox.available()){
    output = (char)SigFox.read();
    status += output;
    delay(10);
  }
  if(DEBUG){
    SerialUSB.println();
    SerialUSB.print("Status \t");
    SerialUSB.println(status);
  }
  
 if (status == "OK\r"){
  //Success :)
  ledGreenLight(HIGH);
  }
 else{
  ledRedLight(HIGH);
  }
}

//This function gives you the device ID
String getID(){
  setConfigurationMode();
  resetLEDs();
  String id = "";
  char output;
  
  SigFox.print("AT$ID\r");
  while (!SigFox.available()){
     blink();
  }
  
  while(SigFox.available()){
    output = (char)SigFox.read();
    id += output;
    delay(10);
  }
  
  if(DEBUG){
    SerialUSB.println("Sigfox Device ID: ");
    SerialUSB.println(id);
  }
  
  return id;
}


//To access certain information, you may need to enter in configuration mode
void setConfigurationMode(){
  resetLEDs();
  String status = "";
  char output;
  if(DEBUG){
    SerialUSB.println("Configuration Mode");
  }
  
  delay(1000);
  SigFox.print("+++");
  while (!SigFox.available()){
     blink();
  }
  while(SigFox.available()){
    output = (char)SigFox.read();
    status += output;
    delay(10);
  }
  SerialUSB.println(status);
   if (status == "OK\r"){
    //Success :)
    ledBlueLight(HIGH);
  }
  else{
    ledRedLight(HIGH);
  }
}


//Reset all leds
void resetLEDs(){
  ledRedLight(LOW);
  ledGreenLight(LOW);
  ledBlueLight(LOW);
}

//blink function
void blink(){
  ledBlueLight(HIGH);
  delay(100);
  ledBlueLight(LOW);
  delay(200);
}

