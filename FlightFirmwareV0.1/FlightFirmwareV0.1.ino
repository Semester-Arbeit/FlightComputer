#include <Servo.h>
#include <mbed.h>
#include <Arduino_PortentaBreakout.h>
#include "FlightControls.h"
#include "Sensors.h"


void setup() {
  Serial.begin(9600);
  pinMode(LEDR, OUTPUT);
  pinMode(LEDG, OUTPUT);
  pinMode(LEDB, OUTPUT);
  digitalWrite(LEDR, HIGH);
  digitalWrite(LEDG, HIGH);
  digitalWrite(LEDB, HIGH);

  //Initalise Flight Controls
  blinkTaskLED(1);
  FlightControls flightControlSystem = FlightControls();
  flightControlSystem.testAilerons();
  digitalWrite(LEDG, LOW);
  delay(500);

  //Initalise Sensors
  blinkTaskLED(2);
  Sensors flightSensors = Sensors();
  if (flightSensors.init())
  {
    digitalWrite(LEDG, LOW);
  }
  else
  {
    digitalWrite(LEDR, LOW);
  }
  delay(500);

  //Initalise Control System
  blinkTaskLED(3);
  
  delay(500);

  //Initalise Flight Data Logger
  blinkTaskLED(4);

  delay(500);

  //Initalise Wifi Network
  blinkTaskLED(5);

  delay(500);



}

void loop() {

}

void blinkTaskLED(int blinks)
{
  digitalWrite(LEDR, HIGH);
  digitalWrite(LEDG, HIGH);
  digitalWrite(LEDB, HIGH);
  for ( int i = 0; i < blinks; i++)
  {
    digitalWrite(LEDB, LOW);
    delay(250);
    digitalWrite(LEDB, HIGH);
    delay(250);
  }
}
