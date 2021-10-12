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

    DataLogger()
      : fs("fs")
    {

    }

    bool init()
    {

      int err = fs.mount(&block_device);
      mkdir("fs/LogFiles", 0777);
      if (err == 0)
      {
        return true;
      }
      else
      {
        return false;
      }
    }

    void open(String fileName)
    {
      String("fs/LogFiles/" + fileName).toCharArray(this->fileName, 100);
      fp = fopen(this->fileName, "w");
    }

    void println(String line)
    {
      String(line + "\r\n").toCharArray(this->lineInFile, 100);
      fprintf(fp, this->lineInFile);
    }

    void close()
    {
      fclose(fp);
    }



  private:
    FILE *fp;
    char fileName[100];
    char lineInFile[100];
    SDMMCBlockDevice block_device;
    mbed::FATFileSystem fs;
};

#endif /*DATALOGGER_H_*/
