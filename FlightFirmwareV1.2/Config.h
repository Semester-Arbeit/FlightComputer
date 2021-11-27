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
      kAtt[0] = arrayOfValues[0];
      kAtt[1] = arrayOfValues[1];
      kAtt[2] = arrayOfValues[2];
      
      kGyro[0] = arrayOfValues[3];
      kGyro[1] = arrayOfValues[4];
      kGyro[2] = arrayOfValues[5];
      
      kPos[0] = arrayOfValues[6];
      kPos[1] = arrayOfValues[7];
      kPos[2] = arrayOfValues[8];

      kSpeed[0] = arrayOfValues[9];
      kSpeed[1] = arrayOfValues[10];
      kSpeed[2] = arrayOfValues[11];
      
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
    const double defaultServoOffset = 45;
    double servoOffset[4] = {defaultServoOffset + 0, defaultServoOffset + 0, defaultServoOffset + 0, defaultServoOffset + 0};


    //Control System: Kp,Ki
    double kAtt[3] ={0,0,0};
    double kGyro[3] ={0,0,0};
    double kPos[3] ={0,0,0};
    double kSpeed[3] = {0,0,0};

    double* KValuesForController[4] = {kAtt,kGyro,kPos,kSpeed};
     
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
