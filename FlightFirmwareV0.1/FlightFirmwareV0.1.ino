#include <Servo.h>
#include <mbed.h>
#include <Arduino_PortentaBreakout.h>
#include <WiFi.h>
#include <WiFiUdp.h>
#include "FlightControls.h"
#include "Sensors.h"
#include "Config.h"
#include "ControlSystem.h"
#include "FlightPlaner.h"
#include "DataLogger.h"




Config configurationData = Config();
FlightControls flightSystem = FlightControls(&configurationData);
Sensors flightSensors = Sensors();

FlightPlaner planer = FlightPlaner(&configurationData);

double sensPitch = 0;
double sensRoll = 0;
double sensYaw = 0;

double correctionPitch = 0;
double correctionRoll = 0;
double correctionYaw = 0;

double tragetPitch = 0;
double targetRoll = 0;
double targetYaw = 0;

ControlSystem flightControlSystem = ControlSystem(configurationData.getKValuesForController(), &sensPitch, &sensRoll, &sensYaw, &correctionPitch, &correctionRoll, &correctionYaw, &tragetPitch, &targetRoll, &targetYaw);

DataLogger flightDataLogger = DataLogger("FlightLog.csv");




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

  flightSystem.testAilerons();
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
  double* currentSensorData = flightSensors.getAttitude();
  sensPitch = currentSensorData[0];
  sensRoll = currentSensorData[1];
  sensYaw = currentSensorData[2];
  flightControlSystem.updateValues();
  flightSystem.setAilerons(correctionPitch, correctionRoll, correctionYaw);
  delay(500);

  //Initalise Flight Data Logger
  blinkTaskLED(4);
  if (flightDataLogger.init())
  {
    digitalWrite(LEDG, LOW);
  }
  else
  {
    digitalWrite(LEDR, LOW);
  }
  flightDataLogger.start("Time,sensPitch,sensRoll,sensYaw,correctionPitch,correctionRoll,correctionYaw");
  delay(500);

  //Initalise Wifi Network
  blinkTaskLED(5);

  delay(500);
  flightSystem.startMotor();
  delay(2500);
  flightSystem.setThrotle(80);


  for ( int i = 0; i < 20000; i++)
  {
    double* currentSensorData = flightSensors.getAttitude();
    sensPitch = currentSensorData[0];
    sensRoll = currentSensorData[1];
    sensYaw = currentSensorData[2];
    flightControlSystem.updateValues();
    flightSystem.setAilerons(correctionPitch, correctionRoll, correctionYaw);
    String currentTelemetryData = String(millis()) + ",";
    currentTelemetryData += String(sensPitch) + ",";
    currentTelemetryData += String(sensRoll) + ",";
    currentTelemetryData += String(sensYaw) + ",";
    currentTelemetryData += String(correctionPitch) + ",";
    currentTelemetryData += String(correctionRoll) + ",";
    currentTelemetryData += String(correctionYaw) +"\r\n";
  }
  flightDataLogger.end();
  flightSystem.setThrotle(0);
  flightSystem.stopMotor();
  digitalWrite(LEDG, LOW);
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
