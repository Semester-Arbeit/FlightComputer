#include <Servo.h>
#include <mbed.h>
#include <Arduino_PortentaBreakout.h>
#include "FlightControls.h"
#include "DataLogger.h"
#include "Config.h"

Config configurationData = Config();
FlightControls flightSystem = FlightControls(&configurationData);


void setup() {
  double power = 60;
  double pitch = 40;

  
  Serial.begin(9600);
  flightSystem.startMotor();
  Serial.println("Starting Motor");
  delay(2000);
  Serial.print("Power:");
  Serial.print(power);
  Serial.print("Pitch:");
  Serial.println(pitch);
  flightSystem.setThrotle(power);
  flightSystem.setAilerons(pitch, 0, 0);
  delay(25000);
  flightSystem.stopMotor();
}

void loop() {


}
