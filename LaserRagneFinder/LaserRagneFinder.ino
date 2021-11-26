#include "LaserRangeFinder.h"
#include <mbed.h>
#include <Arduino_PortentaBreakout.h>

LaserRangeFinder myLaser = LaserRangeFinder();


void setup() {
  Serial.begin(115200);
  myLaser.init();
  delay(2000);

}

void loop() {
  delay(10);
  Serial.println(myLaser.getDistance());
}
