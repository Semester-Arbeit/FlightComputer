#ifndef DATALOGGER_H_
#define DATALOGGER_H_

#include "SDMMCBlockDevice.h"
#include "FATFileSystem.h"

//The Class to handle the data logging
//---------------------------------------------------------------------------
//Version: V0.1
//Autor: Tobias Rothlin
//---------------------------------------------------------------------------
//Methods:
//

class DataLogger
{
  public:

    DataLogger(String fileName)
    {
      this->fileName = fileName;
    }

    bool init()
    {
      fs.mount(&block_device);
    }

    void start(String header)
    {
      fp = fopen("/fs/" + header, "a");
      fprintf(fp, header);
    }

    void append(String Line)
    {
      fprintf(fp, Line);
    }

    void end()
    {
      fclose(fp);
    }



  private:
    SDMMCBlockDevice block_device;
    mbed::FATFileSystem fs("fs");
    FILE *fp;
};

#endif /*DATALOGGER_H_*/
