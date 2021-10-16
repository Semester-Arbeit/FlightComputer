#include <Servo.h>
#include <mbed.h>
#include <Arduino_PortentaBreakout.h>
#include <WiFi.h>
#include <WiFiUdp.h>
#include "FlightControls.h"
#include "Sensors.h"
#include "Config.h"
#include "ControlSystem.h"
#include "DataLogger.h"

Config configurationData = Config();
FlightControls flightSystem = FlightControls(&configurationData);
Sensors flightSensors = Sensors();


double targetAtt[3] = {0, 0, 0};
double targetGyro[3] = {0, 0, 0};
double targetPos[3] = {0, 0, 0};
double targetSpeed[3] = {0, 0, 0};

double outputPitch, outputRoll, outputYaw, outputPower;

ControlSystem flightControlSystem;

DataLogger flightDataLogger;
DataLogger flightErrorLog;

SDMMCBlockDevice block_device;
mbed::FATFileSystem fs("fs");
FILE *fp;

//UDP Connection
char ssid[] = "Alpha";    // SSID
char pass[] = "00000000";    // Pasword
int keyIndex = 0;             // your network key Index number (needed only for WEP)

int status = WL_IDLE_STATUS;

unsigned int localPort = 2390;      // local port to listen on

char packetBuffer[350]; //buffer to hold incoming packet
char  ReplyBuffer[200];       // a string to send back
char  Terminator[] = "E";       // a string to send back

WiFiUDP Udp;


void setup() {
  Serial.begin(9600);
  pinMode(LEDR, OUTPUT);
  pinMode(LEDG, OUTPUT);
  pinMode(LEDB, OUTPUT);
  digitalWrite(LEDR, HIGH);
  digitalWrite(LEDG, HIGH);
  digitalWrite(LEDB, HIGH);

  blinkTaskLED(10);
  //LoadDataFromSD
  if (fs.mount(&block_device) == 0)
  {
    fp = fopen("fs/ConfigFiles/KValues.csv", "r");
    char c = fgetc(fp);
    double newKValues[12];
    int indexInNewKValues = 0;
    String currentNumber = "";
    while (c != '\n')
    {
      Serial.print(c);
      if (c == ',')
      {
        newKValues[indexInNewKValues] = currentNumber.toDouble();
        currentNumber = "";
        indexInNewKValues++;
      }
      else
      {
        currentNumber += c;
      }
      c = fgetc(fp);
    }
    fclose(fp);
    newKValues[indexInNewKValues] = currentNumber.toDouble();
    configurationData.setControlSystemValues(newKValues);
    double** data = configurationData.getKValuesForController();
    Serial.println("---Parsed Data---");
    for (int i = 0; i < 3; i++)
    {
      for (int m = 0; m < 3; m++)
      {
        Serial.print(i);
        Serial.print(",");
        Serial.print(m);
        Serial.print(":   ");
        Serial.println(data[i][m]);
      }
    }
    Serial.println("PID Configuration Updated");

    fp = fopen("fs/ConfigFiles/AltitudeProfile.csv", "r");
    c = fgetc(fp);
    currentNumber = "";
    while (c != '\n')
    {
      Serial.print(c);
      if (c == ',')
      {
        configurationData.setNextKeyFrame(currentNumber.toDouble());
        currentNumber = "";
      }
      else
      {
        currentNumber += c;
      }
      c = fgetc(fp);
    }
    fclose(fp);
    configurationData.setNextKeyFrame(currentNumber.toDouble());
    Serial.println("---Parsed Data---");
    double* altitudeData = configurationData.getAllKeyFrames();
    for (int i = 0; i < configurationData.getNumberOfKeyFrames(); i++)
    {
      Serial.print(i);
      Serial.print(":   ");
      Serial.println(altitudeData[i]);
    }
  }
  else
  {
    Serial.println("Warning:  Not the correct UPD Commend Received!!");
    Serial.println("          Using default Values vor K");
  }


  //Initalise Sensors
  blinkTaskLED(1);
  Serial.println("Init Sensors");
  if (flightSensors.init())
  {
    Serial.println("Sensors Status: Okey");
    digitalWrite(LEDG, LOW);
  }
  else
  {
    Serial.println("Sensors Status: Failed");
    digitalWrite(LEDR, LOW);
  }

  //Initalise WIFI
  blinkTaskLED(2);
  Serial.println("Init WiFi");
  status = WiFi.beginAP(ssid, pass);
  Udp.begin(localPort);
  Serial.println("WiFi Status: Okey");
  delay(500);


  //Initalise Flight Controls
  blinkTaskLED(3);
  Serial.println("Init Flight Controls");
  flightSystem.testAilerons();
  digitalWrite(LEDG, LOW);
  Serial.println("Flight Controls Status: Okey");
  delay(500);

  //Initalise Flight Data Logger
  blinkTaskLED(4);
  Serial.println("Init Flight Data Logger");
  if (flightDataLogger.init())
  {
    Serial.println("Flight Data Logger Status: Okey");
    digitalWrite(LEDG, LOW);
  }
  else
  {
    Serial.println("Flight Data Logger Status: False");
    digitalWrite(LEDR, LOW);
  }
  flightDataLogger.open("FlightLog.csv");
  flightDataLogger.println("Time,sensPitch,sensRoll,sensYaw,xGyro,yGyro,ZGyro,xAcc,yAcc,zAcc,correctionPitch,correctionRoll,correctionYaw");
  delay(500);

  

  //Initalise Control System
  blinkTaskLED(5);
  Serial.println("Init Control System");
  flightControlSystem.init(flightSensors.getAttitude(), flightSensors.getGyro(), flightSensors.getPos(), flightSensors.getSpeed(), targetAtt, targetGyro, targetPos, targetSpeed, &outputPitch, &outputRoll, &outputYaw, &outputPower, configurationData.getKValuesForController()[0], configurationData.getKValuesForController()[1], configurationData.getKValuesForController()[2], configurationData.getKValuesForController()[3]);
  Serial.println(flightControlSystem.getStatus());
  flightSensors.getAttitude();
  flightControlSystem.updateValues();
  flightSystem.setAilerons(outputPitch, outputRoll, outputYaw);
}

void loop() {
  if (getNewUDPPackets())
  {
    //Flight Mode
    if (packetBuffer[0] == 'L')
    {
      unsigned long lastExecutionTime = millis();
      for (int i = 0; i < 5000; i++)
      {
        while (millis() - lastExecutionTime < 10)
        {
        }
        lastExecutionTime = millis();
        if (getNewUDPPackets())
        {
          if (packetBuffer[0] == 'A')
          {
            break;
          }
        }
        //DoFlightStuff
        Serial.println(millis());
      }
    }

    if (packetBuffer[0] == 'P')
    {
      while (!getNewUDPPackets())
      {
        delay(1);
      }

      int i = 0;
      int m = 0;
      String currentNumber = "";
      double newKValues[12];
      while (packetBuffer[i] != '\n')
      {
        if (packetBuffer[i] == ',')
        {
          newKValues[m] = currentNumber.toDouble();
          currentNumber = "";
          m++;
        }
        else
        {
          currentNumber += packetBuffer[i];
        }
        i++;
      }
      newKValues[m] = currentNumber.toDouble();
      configurationData.setControlSystemValues(newKValues);
      double** data = configurationData.getKValuesForController();
      Serial.println("---Parsed Data---");
      for (int i = 0; i < 3; i++)
      {
        for (int m = 0; m < 3; m++)
        {
          Serial.print(i);
          Serial.print(",");
          Serial.print(m);
          Serial.print(":   ");
          Serial.println(data[i][m]);
        }

      }

      Serial.println("PID Configuration Updated");
      Serial.println(flightControlSystem.getStatus());
    }
    else
    {
      Serial.println("Warning:  Not the correct UPD Commend Received!!");
      Serial.println("          Using default Values vor K");
    }
  }
}


bool getNewUDPPackets()
{
  int packetSize = Udp.parsePacket();
  if (packetSize != 0)
  {
    int len = Udp.read(packetBuffer, 350);
    if (len > 0) packetBuffer[len] = '\n';
    return true;
  }
  else
  {
    return false;
  }
}

void blinkTaskLED(int blinks)
{
  digitalWrite(LEDR, HIGH);
  digitalWrite(LEDG, HIGH);
  digitalWrite(LEDB, HIGH);
  for ( int i = 0; i < blinks; i++)
  {
    digitalWrite(LEDB, LOW);
    delay(250);
    digitalWrite(LEDB, HIGH);
    delay(250);
  }
}

void sendUdpData(char * dataToSend)
{
  Udp.beginPacket(Udp.remoteIP(), Udp.remotePort());
  Udp.write(dataToSend);
  Udp.endPacket();
}
