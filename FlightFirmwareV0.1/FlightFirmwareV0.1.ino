#include <Servo.h>
#include <mbed.h>
#include <Arduino_PortentaBreakout.h>
#include "FlightControls.h"


void setup() {
  Serial.begin(9600);
  pinMode(LEDR, OUTPUT);
  pinMode(LEDG, OUTPUT);
  pinMode(LEDB, OUTPUT);
  digitalWrite(LEDR, HIGH);
  digitalWrite(LEDG, HIGH);
  digitalWrite(LEDB, HIGH);

  //Initalise Flight Controls
  FlightControls flightControlSystem = FlightControls();
  flightControlSystem.testAilerons();
  digitalWrite(LEDR, LOW); //Flight Control Initalised

  //Initalise Sensors
  digitalWrite(LEDG, LOW); //Seonsors Initalised

  //Initalise Control System
  digitalWrite(LEDB, LOW); //Control System Initalised

  //Initalise Wifi Network
  digitalWrite(LEDR, HIGH);
  digitalWrite(LEDB, HIGH); //Wifi Network System Initalised /Ready to Use

  for( int i= 0; i<10; i++)
  {
    digitalWrite(LEDG, LOW);
    delay(250);
    digitalWrite(LEDG, HIGH);
    delay(250);
  }

}

void loop() {

}
