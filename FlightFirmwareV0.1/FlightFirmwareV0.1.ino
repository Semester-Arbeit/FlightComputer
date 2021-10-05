#include <Servo.h>
#include <mbed.h>
#include <Arduino_PortentaBreakout.h>
#include "FlightControls.h"
#include "Sensors.h"

#include "Config.h"

Config configurationData = Config();
FlightControls flightControlSystem = FlightControls(&configurationData);
Sensors flightSensors = Sensors();

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

  flightControlSystem.testAilerons();
  digitalWrite(LEDG, LOW);
  delay(500);

  //Initalise Sensors
  blinkTaskLED(2);

  if (flightSensors.init())
  {
    digitalWrite(LEDG, LOW);
  }
  else
  {
    digitalWrite(LEDR, LOW);
  }
  //Initalise Control System
  blinkTaskLED(3);

  delay(500);

  //Initalise Flight Data Logger
  blinkTaskLED(4);

  delay(500);

  //Initalise Wifi Network
  blinkTaskLED(5);

  delay(500);

  flightControlSystem.startMotor();
  delay(2500);
  flightControlSystem.setThrotle(80);

}

void loop() {
  double* currentAttitude = flightSensors.getAttitude();
  Serial.print(millis());
  Serial.print(":       Pitch:");
  Serial.print(currentAttitude[0]);
  Serial.print(":       Roll:");
  Serial.print(currentAttitude[1]);
  Serial.print(":       Yaw:");
  Serial.println(currentAttitude[2]);
  flightControlSystem.setAilerons(currentAttitude[0],currentAttitude[1],currentAttitude[2]);
  delay(1);
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
