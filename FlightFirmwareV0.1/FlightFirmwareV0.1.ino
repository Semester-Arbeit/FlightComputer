#include <Servo.h>
#include <Arduino_PortentaBreakout.h>


breakoutPin Motor = PWM0;
breakoutPin servos[] = {PWM1,PWM2,PWM3,PWM4};



void setup() {
 pinMode(pwmPins[0], OUTPUT);
 pinMode(pwmPins[1], OUTPUT);
 pinMode(pwmPins[2], OUTPUT);
 pinMode(pwmPins[3], OUTPUT);
 pinMode(Motor, OUTPUT);

}

void loop() {
  // put your main code here, to run repeatedly:

}
