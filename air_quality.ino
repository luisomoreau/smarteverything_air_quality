// ---------------------------------------------------------------------
// This file is an example on how to use the the MQ135 sensor to measure
// quality and send the data through Sigfox network
//
// Created: 08.09.2016
// Author: Louis Moreau
// Code: https://github.com.luisomoreau
// ---------------------------------------------------------------------

// ---------------------------------------------------------------------
// Define
// ---------------------------------------------------------------------

#define SENSOR A0 //analog input pin
#define LED 13
#define DEBUG true

// ---------------------------------------------------------------------
// Include
// ---------------------------------------------------------------------

#include <SmeSFX.h>

// ---------------------------------------------------------------------
// Global variables
// ---------------------------------------------------------------------

int sensorValue = 0;

// ---------------------------------------------------------------------
// Public functions
// ---------------------------------------------------------------------

void setup() {

  pinMode(LED, OUTPUT);
  pinMode(SENSOR, INPUT);
  sfxAntenna.begin();
  
  if(DEBUG){
    SerialUSB.begin(9600);
  }

}

void loop() {
  digitalWrite(LED, HIGH);
  sensorValue = analogRead(SENSOR);
  
  if(DEBUG){
    SerialUSB.print("Sensor value: ");
    SerialUSB.println(sensorValue);
  }
  delay(1000);
  digitalWrite(LED, LOW);
  delay(1000);
}
