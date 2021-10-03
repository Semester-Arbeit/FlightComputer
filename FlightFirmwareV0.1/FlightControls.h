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
//            bool setAilerons(double pitch, double yaw, double roll) -> Sets the Pitch, yaw and roll angle of the ailerons.
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
      ESC.write(0);
    }

    void enterTransportMode()
    {
      for (int i = 0; i < 4; i++)
      {
        servoFlightControls[i].write(servoOffset[i] - 90);
      }
      ESC.write(0);
    }

    bool setThrotle(double power)
    {
      if (power < 0)
      {
        ESC.write(0);
      }
      else if (power > 100)
      {
        ESC.write(100);
      }
      else
      {
        ESC.write(power);
      }
    }

    bool setAilerons(double pitch, double yaw, double roll)
    {
      // Pitch, Yaw, Roll setup as seen from the top of the Vehicle.
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

      double pitchYn = yaw + pitch;
      double pitchYp = yaw - pitch;

      double rollXp = yaw + pitch;
      double rollXn = yaw - pitch;

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

      servoFlightControls[0].write(servoOffset[0] - pitchYn);
      servoFlightControls[0].write(servoOffset[0] - pitchYp);
      servoFlightControls[0].write(servoOffset[0] - rollXp);
      servoFlightControls[0].write(servoOffset[0] - rollXn);
      return !maxReached;
    }

    void setServoNullPos(int ServoIndex, double changeValue)
    {
      if (escPower == 0 && ServoIndex < 4)
      {
        servoOffset[ServoIndex] = servoOffset[ServoIndex] + changeValue;
        servoFlightControls[ServoIndex].write(servoOffset[ServoIndex]);
      }
    }

    void restAilerons()
    {
      setAilerons(0, 0, 0);
    }

    void testAilerons()
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
    }

  private:

    const double defaultServoOffset = 90;
    double servoOffset[4] = {defaultServoOffset, defaultServoOffset, defaultServoOffset, defaultServoOffset};

    const double stallAngle = 45;
    const double squareStallAngle = stallAngle * stallAngle; //used to decide if the courrent Angle is greater than the stall angle.

    double escPower = 0;

    breakoutPin MotorPin = PWM0;
    breakoutPin servoPins[4] = {PWM1, PWM2, PWM3, PWM4};

    Servo servoFlightControls[4];
    Servo ESC;

};

#endif /*FLIGHTCONTROLS_H_*/
