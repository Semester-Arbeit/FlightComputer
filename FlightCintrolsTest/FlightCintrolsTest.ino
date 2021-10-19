#include <Servo.h>
#include <mbed.h>
#include <Arduino_PortentaBreakout.h>
#include "FlightControls.h"

int i = 0;

FlightControls flightControlSystem = FlightControls();

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
}

void loop() {
  flightControlSystem.startMotor();


}
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
