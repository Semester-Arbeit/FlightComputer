//#include "LaserRangeFinder.h"
#include <mbed.h>
#include <Arduino_PortentaBreakout.h>

//LaserRangeFinder myLaser = LaserRangeFinder();


void setup() {
  Serial.begin(115200);
  Serial1.begin(9600);
  //myLaser.init();
  delay(2000);

}

void loop() {
  delay(1);
  //Serial.println(myLaser.getDistance());
  if (Serial.available()) {
    char c = Serial.read();
    Serial.write(c);
    Serial1.write(c);
  }
  if (Serial1.available()) {
    char c = Serial1.read();
    Serial.write(c);
  }
}
