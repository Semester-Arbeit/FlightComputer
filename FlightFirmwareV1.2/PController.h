#ifndef PCONTROLLER_H_
#define PCONTROLLER_H_



//The Class implements a very simple P Controler
//---------------------------------------------------------------------------
//Version: V0.1
//Author: Tobias Rothlin
//---------------------------------------------------------------------------
//Methods:
//


class PController
{
  public:
    PController()
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

    String getKValue()
    {
      return String(*Kp);
    }

    

  private:
    double * Kp;
    double * outputValue;
    double * sensorValue;
    double * targetValue;
};

#endif /*PCONTROLLER_H_*/
