#include <mbed.h>
#include <Arduino_PortentaBreakout.h>
#include "Sensors.h"

Sensors flightSensors = Sensors();

void setup() {
  Serial.begin(9600);
  Serial.println(flightSensors.init());
}

void loop() {
  double* att = flightSensors.getAttitude();
  delay(5000);
  flightSensors.updateLocation();
  delay(1000);
  double* pos = flightSensors.getAlt();

  Serial.print("Pitch:");
  Serial.print(att[0]);
  Serial.print("  Roll:");
  Serial.print(att[1]);
  Serial.print("  Yaw:");
  Serial.print(att[2]);

  Serial.print("  X:");
  Serial.print(pos[0]);
  Serial.print("  Y:");
  Serial.print(pos[1]);
  Serial.print("  Alt:");
  Serial.println(pos[2]);
  
}
