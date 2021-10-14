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


double sensPitch = 0;
double sensRoll = 0;
double sensYaw = 0;

double correctionPitch = 0;
double correctionRoll = 0;
double correctionYaw = 0;

double tragetPitch = 0;
double targetRoll = 0;
double targetYaw = 0;

ControlSystem flightControlSystem;

DataLogger flightDataLogger;
DataLogger flightErrorLog;

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
  while (!getNewUDPPackets())
  {
    digitalWrite(LEDG, LOW);
    delay(250);
    digitalWrite(LEDG, HIGH);
    delay(250);
  }
  double newKValues[9];
  if ( packetBuffer[0] == 'P')
  {
    while (!getNewUDPPackets())
    {
      delay(1);
    }

    int i = 0;
    int m = 0;
    String currentNumber = "";
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
  }
  else
  {
    Serial.println("Warning:  Not the correct UPD Commend Received!!");
    Serial.println("          Using default Values vor K");
  }

  //KeyFrameConfiguration
  while (!getNewUDPPackets())
  {
    digitalWrite(LEDG, LOW);
    delay(100);
    digitalWrite(LEDG, HIGH);
    delay(100);
  }
  if ( packetBuffer[0] == 'F')
  {
    int i = 0;
    String currentNumber = "";
    for ( int m = 0; m < 100; m++)
    {
      digitalWrite(LEDG, HIGH);
      while (!getNewUDPPackets())
      {
      }
      digitalWrite(LEDG, LOW);
      while (packetBuffer[i] != '\n')
      {
        if (packetBuffer[i] == ',')
        {
          configurationData.setNextKeyFrame(currentNumber.toDouble());
          currentNumber = "";
        }
        else
        {
          currentNumber += packetBuffer[i];
        }
        i++;
      }

      i = 0;
      configurationData.setNextKeyFrame(currentNumber.toDouble());
    }


    Serial.println("Flight Parameters Updated");
    Serial.println("FileDumpCompleted");
  }
  else
  {
    Serial.println("Warning:  Not the correct UPD Commend Received!!");
  }

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
  flightControlSystem.init(configurationData.getKValuesForController(), &sensPitch, &sensRoll, &sensYaw, &correctionPitch, &correctionRoll, &correctionYaw, &tragetPitch, &targetRoll, &targetYaw);
  Serial.println(flightControlSystem.getStatus());
  double* currentSensorData = flightSensors.getAttitude();
  sensPitch = currentSensorData[0];
  sensRoll = currentSensorData[1];
  sensYaw = currentSensorData[2];
  flightControlSystem.updateValues();
  flightSystem.setAilerons(correctionPitch, correctionRoll, correctionYaw);
}

void loop() {
  if (getNewUDPPackets())
  {
    //Flight Mode
    if (packetBuffer[0] == 'L')
    {
      unsigned long lastExecutionTime = millis();
      for(int i = 0; i < 5000; i++)
      {
        while(millis()-lastExecutionTime < 10)
        {
        }
        lastExecutionTime = millis();
        if(getNewUDPPackets())
        {
          if(packetBuffer[0] == 'A')
          {
            break;
          }
        }
        //DoFlightStuff
        Serial.println(millis());
      }
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
