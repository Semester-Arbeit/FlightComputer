#include <mbed.h>
#include <Arduino_PortentaBreakout.h>
#include "Sensors.h"
#include "DataLogger.h"

Sensors flightSensors = Sensors();
DataLogger flightDataLogger;

double sensPitch, sensRoll, sensYaw;
float latitude, longitude, altitude;
int sat;


void setup() {
  Serial.begin(9600);
  pinMode(LEDR, OUTPUT);
  pinMode(LEDG, OUTPUT);
  pinMode(LEDB, OUTPUT);
  digitalWrite(LEDR, HIGH);
  digitalWrite(LEDG, HIGH);
  digitalWrite(LEDB, HIGH);

  blinkTaskLED(2);
  Serial.println("Init Sensors");
  if (flightSensors.init())
  {
    Serial.println("Sensors Status: Okey");
    digitalWrite(LEDG, LOW);
  }
  else
  {
    Serial.println("Sensors Status: Failed");
    digitalWrite(LEDR, LOW);
  }

  blinkTaskLED(4);
  Serial.println("Init Flight Data Logger");
  if (flightDataLogger.init())
  {
    Serial.println("Flight Data Logger Status: Okey");
    digitalWrite(LEDG, LOW);
  }
  else
  {
    Serial.println("Flight Data Logger Status: False");
    digitalWrite(LEDR, LOW);
  }
  flightDataLogger.open("FlightLog.csv");
  flightDataLogger.println("Time,sensPitch,sensRoll,sensYaw,latitude,longitude,altitude,sat");
  delay(500);

  for (int i = 0; i < 10000; i++)
  {
    double* currentSensorData = flightSensors.getAttitude();
    sensPitch = currentSensorData[0];
    sensRoll = currentSensorData[1];
    sensYaw = currentSensorData[2];
    float* currentPos = flightSensors.getPos();
    latitude = currentPos[0];
    longitude = currentPos[1];
    altitude = currentPos[2];
    sat = flightSensors.getNumberOfSatellites();
    String PrintLine = String(millis()) + "," + String(sensPitch) + "," + String(sensRoll) + "," + String(sensYaw) + "," + String(latitude) + "," + String(longitude) + "," + String(altitude) + "," + String(sat);
    flightDataLogger.println(PrintLine);
    Serial.println(PrintLine);
  }

  flightDataLogger.close();
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
