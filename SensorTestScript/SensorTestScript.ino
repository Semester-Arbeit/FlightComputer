#include <mbed.h>
#include <Arduino_PortentaBreakout.h>
#include "Sensors.h"
#include "DataLogger.h"

Sensors flightSensors = Sensors();
DataLogger flightDataLogger;

double sensPitch, sensRoll, sensYaw, xAcc, yAcc, zAcc, xGyro, yGyro, zGyro, latitude, longitude, altitude;


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
  flightDataLogger.println("Time,sensPitch,sensRoll,sensYaw,xGyro,yGyro,ZGyro,xAcc,yAcc,zAcc,latitude,longitude,altitude");
  delay(500);

  for (int i = 0; i < 10000; i++)
  {
    double* currentSensorData = flightSensors.getAttitude();
    sensPitch = currentSensorData[0];
    sensRoll = currentSensorData[1];
    sensYaw = currentSensorData[2];
    currentSensorData = flightSensors.getGyro();
    xGyro = currentSensorData[0];
    yGyro = currentSensorData[1];
    zGyro = currentSensorData[2];

    currentSensorData = flightSensors.getAcc();
    xAcc = currentSensorData[0];
    yAcc = currentSensorData[1];
    zAcc = currentSensorData[2];

    double* currentPos = flightSensors.getAlt();
    latitude = currentPos[0];
    longitude = currentPos[1];
    altitude = currentPos[2];

    String PrintLine = String(millis()) + "," + String(sensPitch) + "," + String(sensRoll) + "," + String(sensYaw) + "," + String(xGyro) + "," + String(yGyro) + "," + String(zGyro) + "," + String(xAcc) + "," + String(yAcc) + "," + String(zAcc) + "," + String(latitude) + "," + String(longitude) + "," + String(altitude);
    flightDataLogger.println(PrintLine);
  }

  flightDataLogger.close();
  digitalWrite(LEDR, HIGH);
  digitalWrite(LEDG, HIGH);
  digitalWrite(LEDB, HIGH);
  for ( int i = 0; i < 10; i++)
  {
    digitalWrite(LEDG, LOW);
    delay(250);
    digitalWrite(LEDG, HIGH);
    delay(250);
  }
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
