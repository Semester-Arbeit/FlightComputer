#ifndef CONTROLSYSTEM_H_
#define CONTROLSYSTEM_H_



//The Class implements a very simple P Controler
//---------------------------------------------------------------------------
//Version: V0.1
//Author: Tobias Rothlin
//---------------------------------------------------------------------------
//Methods:
//            


class P
{
  public:
    P()
    {

    }
    void init(double* output, double* sensor, double* target, double* Kp)
    {
      this -> Kp = Kp;
      this -> outputValue = output;
      this -> sensorValue = sensor;
      this -> targetValue = target;
    }

    void updateValues()
    {
      *outputValue = (*targetValue - *sensorValue) * *Kp;
    }


    String getStatus()
    {
      String status = "K = " + String(*Kp) + "\n";
      return status;
    }

  private:
    double * Kp;
    double * outputValue;
    double * sensorValue;
    double * targetValue;
};

#endif /*CONTROLSYSTEM_H_*/
