#ifndef FLIGHTCONTROLS_H_
#define FLIGHTCONTROLS_H_

#include <Servo.h>
#include <mbed.h>
#include <Arduino_PortentaBreakout.h>

//The Class to interface between the Firmware and Hardware FLight Controls.
//---------------------------------------------------------------------------
//Version: V0.1
//Autor: Tobias Rothlin
//---------------------------------------------------------------------------
//Methods:
//            FlightControls() -> Constructor initalises the 5 PWM signals and sets the start values: ESC -> 0
//                                                                                                    Servos -> Custom offset (default 90)
//
//            void enterTransportMode() -> Turns all ailerons to a vertival positio so the do not break during transport.
//
//            bool setAilerons(double pitch, double roll, double yaw) -> Sets the Pitch, roll and yaw angle of the ailerons.
//
//            bool setThrotle(double power) -> sets the Throtle power ( 0 <= power <= 100)
//
//            void setServoNullPos(int ServoIndex, double changeValue) -> changes the offsets of the Servo at ServoIndex)

class FlightControls
{
  public:
    FlightControls()
    {
      for (int i = 0; i < 4; i++)
      {
        servoFlightControls[i] = Servo();
        servoFlightControls[i].attach(servoPins[i]);
        servoFlightControls[i].write(servoOffset[i]);
      }
      ESC.attach(MotorPin);
      MotorEnable.attach(MotorEnablePin);
      ESC.write(0);
      MotorEnable.write(0);
    }

    void enterTransportMode()
    {
      for (int i = 0; i < 4; i++)
      {
        servoFlightControls[i].write(servoOffset[i] - 90);
      }
      ESC.write(0);
    }

    void startMotor()
    {
      MotorEnable.write(180);
      for ( int i = 0; i < 30; i++)
      {
        ESC.write(i);
        delay(10);
      }
    }

    void stopMotor()
    {
      MotorEnable.write(0);
    }

    void setThrotle(double power)
    {
      if (power < 0)
      {
        ESC.write(0);
      }
      else if (power > 100)
      {
        ESC.write(1.8 * 100);
      }
      else
      {
        ESC.write(1.8 * power);
      }
    }

    bool setAilerons(double pitch, double roll, double yaw)
    {
      // Pitch, Roll, Yaw setup as seen from the top of the Vehicle.
      //
      //                      rollXp
      //                 , - ~ ~ ~ - ,                    ^ x
      //             , '               ' ,                |
      //           ,                       ,              |
      //          ,                         ,             |
      //         ,                           ,            |
      // pitchYn,                             ,  pitchYP  |
      //         ,                           ,            |________________>  y
      //          ,                         ,
      //           ,                       ,
      //             ,                  , '
      //               ' - , _ _ _ ,  '
      //                    rollXn
      //
      // the yaw value is added to all 4 Servos values.
      // if the final servo Value is out of bounds the function will prioritise the yaw value and neglegt the overflow and return a false value!

      double pitchYn = yaw - pitch;
      double pitchYp = yaw + pitch;

      double rollXp = yaw + roll;
      double rollXn = yaw - roll;

      bool maxReached = false;

      if (pitchYn * pitchYn > squareStallAngle)
      {
        maxReached = true;
        if (pitchYn > 0)
        {
          pitchYn = stallAngle;
        }
        else
        {
          pitchYn = -stallAngle;
        }
      }

      if (pitchYp * pitchYp > squareStallAngle)
      {
        maxReached = true;
        if (pitchYp > 0)
        {
          pitchYp = stallAngle;
        }
        else
        {
          pitchYp = -stallAngle;
        }
      }

      if (rollXp * rollXp > squareStallAngle)
      {
        maxReached = true;
        if (rollXp > 0)
        {
          rollXp = stallAngle;
        }
        else
        {
          rollXp = -stallAngle;
        }
      }

      if (rollXn * rollXn > squareStallAngle)
      {
        maxReached = true;
        if (rollXn > 0)
        {
          rollXn = stallAngle;
        }
        else
        {
          rollXn = -stallAngle;
        }
      }

      servoFlightControls[0].write(servoOffset[0] - rollXn);
      servoFlightControls[1].write(servoOffset[1] - pitchYp);
      servoFlightControls[2].write(servoOffset[2] - rollXp);
      servoFlightControls[3].write(servoOffset[3] - pitchYn);
      return !maxReached;
    }

    double* setServoNullPos(int ServoIndex, double changeValue)
    {
      if (escPower == 0 && ServoIndex < 4)
      {
        servoOffset[ServoIndex] = servoOffset[ServoIndex] + changeValue;
        servoFlightControls[ServoIndex].write(servoOffset[ServoIndex]);
      }
      return servoOffset;
    }

    void restAilerons()
    {
      setAilerons(0, 0, 0);
    }

    bool testAilerons()
    {
      setAilerons(20, 0, 0);
      delay(500);
      setAilerons(-20, 0, 0);
      delay(500);
      setAilerons(0, 20, 0);
      delay(500);
      setAilerons(0, -20, 0);
      delay(500);
      setAilerons(0, 0, 20);
      delay(500);

      setAilerons(20, 0, 20);
      delay(250);
      setAilerons(-20, 0, -20);
      delay(250);
      setAilerons(20, 0, -20);
      delay(250);
      setAilerons(-20, 0, 20);

      delay(250);
      setAilerons(0, 20, 20);
      delay(250);
      setAilerons(0, -20, -20);
      delay(250);
      setAilerons(0, -20, 20);
      delay(250);
      setAilerons(0, 20, -20);
      delay(250);

      restAilerons();
      return true;
    }

  private:

    const double defaultServoOffset = 90;
    double servoOffset[4] = {defaultServoOffset +2, defaultServoOffset+2.5, defaultServoOffset+ 8.5, defaultServoOffset + 2};

    const double stallAngle = 45;
    const double squareStallAngle = stallAngle * stallAngle; //used to decide if the courrent Angle is greater than the stall angle.

    double escPower = 0;

    breakoutPin MotorPin = PWM5;
    breakoutPin MotorEnablePin = PWM0;
    breakoutPin servoPins[4] = {PWM1, PWM2, PWM3, PWM4};

    Servo servoFlightControls[4];
    Servo ESC;
    Servo MotorEnable;

};

#endif /*FLIGHTCONTROLS_H_*/
