#ifndef CONFIG_H_
#define CONFIG_H_


//The Class to handle all configuration Parameters
//---------------------------------------------------------------------------
//Version: V0.1
//Author: Tobias Rothlin
//---------------------------------------------------------------------------
//Methods:
//            Config() -> Initalies the defaut values


class Config
{
  public:

    Config()
    {

    }

    double* getServoOffsets()
    {
      return servoOffset;
    }

    void setServoOffset(int index, double offset)
    {
      servoOffset[index] = offset;
    }

    double** getKValuesForController()
    {
      return KValuesForController;
    }


    bool setControlSystemValues(double* arrayOfValues)
    {
      //Check if Len is okey!
      pitchK[0] = arrayOfValues[0];
      pitchK[1] = arrayOfValues[1];
      pitchK[2] = arrayOfValues[2];
      rollK[0] = arrayOfValues[3];
      rollK[1] = arrayOfValues[4];
      rollK[2] = arrayOfValues[5];
      yawK[0] = arrayOfValues[6];
      yawK[1] = arrayOfValues[7];
      yawK[2] = arrayOfValues[8];
      return true;
    }

    bool setNextKeyFrame(double altitudeValue)
    {
      if(numberOfKeyFrames != indexInKeyFrameArray)
      {
        altitude[indexInKeyFrameArray] = altitudeValue;
        indexInKeyFrameArray++;
        return true;
      }
      else
      {
        return false;
      }
    }

    double* getAllKeyFrames()
    {
      return altitude;
    }

    int getNumberOfKeyFrames()
    {
      return numberOfKeyFrames;
    }



  private:

    //FLight Controls values:
    const double defaultServoOffset = 90;
    double servoOffset[4] = {defaultServoOffset + 2, defaultServoOffset + 2.5, defaultServoOffset + 8.5, defaultServoOffset + 10};


    //Control System: Kp,Ki,Kd
    double pitchK[3] = { -3, 0, 0};
    double rollK[3] = {1, 0, 0};
    double yawK[3] = {1, 0, 0};

    double* KValuesForController[3] = {pitchK, rollK, yawK};

    //FlightParameters:
    enum FlightParams{
      numberOfKeyFrames = 5000
    };

    int indexInKeyFrameArray = 0;

    double altitude[numberOfKeyFrames];
    
    //Wifi Settings:


    //LoggingSettings:


};

#endif /*CONFIG_H_*/
