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

  if (Serial.available())
  {
    char c = Serial.read();
    if ( c == '0')
    {
      i = 0;
      Serial.println(i);
    }
    else if ( c == '1')
    {
      i = 1;
      Serial.println(i);
    }
    else if ( c == '2')
    {
      i = 2;
      Serial.println(i);
    }
    else if ( c == '3')
    {
      i = 3;
      Serial.println(i);
    }
    else if ( c == '+')
    {
      double* offsets = flightControlSystem.setServoNullPos(i, 0.5);
      Serial.print(i);
      Serial.print(":     ");
      Serial.println(offsets[i]);
    }
    else if ( c == '-')
    {
      double* offsets = flightControlSystem.setServoNullPos(i, -0.5);
      Serial.print(i);
      Serial.print(":     ");
      Serial.println(offsets[i]);
    }

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
