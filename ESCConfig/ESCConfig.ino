#include <Servo.h>
#include <mbed.h>
#include <Arduino_PortentaBreakout.h>


breakoutPin MotorPin = PWM0;
Servo ESC;


void setup() {
  Serial.begin(9600); 
  ESC.attach(MotorPin);
  ESC.write(0);
  while(!Serial.available())
  {
    delay(20);
    Serial.print(".");
  };
  for(int i = 0; i<90;i++)
  {
    ESC.write(i);
    delay(500);
   Serial.println(i);
  }
  
  
}

void loop() {

}
