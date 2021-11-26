import os
import sys
from time import sleep
import matplotlib.pyplot as px
import numpy as np

data_packet = {}

class complementaryFilter:

    def __init__(self, alpha):
        self.last_executionTime = None
        self.alpha = alpha
        self.speed_est_integrated = 0
        self.speed_est_differentiator = 0
        self.speed_est = 0
        self.last_pos_value = 0
        self.peviusBuffer =[0  for i in range(20)]

    def complementaryFilter(self,time, a, p, staticAccOffset = -0.178):
        if (self.last_executionTime == None):
            self.last_executionTime = time
            self.last_pos_value = p
            return 0

        delta_t = ((time - self.last_executionTime)/1000000)

        self.speed_est_integrated += (a - staticAccOffset) * delta_t
        self.speed_est_differentiator = (p - self.last_pos_value)/delta_t
        self.peviusBuffer = self.peviusBuffer[1:]
        self.peviusBuffer.append(self.speed_est_differentiator)
        average = 0
        for number in self.peviusBuffer:
            average += number
        average /= len(self.peviusBuffer)

        self.speed_est = (1-self.alpha) * self.speed_est_integrated + (self.alpha) * average

        self.last_pos_value = p
        self.last_executionTime = time

        return self.speed_est

with open("/Volumes/NO NAME/LogFiles/FlightLog.csv", 'r') as csv_file:
    headers = [row.replace("\n", "") for row in csv_file.readline().split(',')]
    file_lines = csv_file.read().split('\n')
    data_columns = [[]for i in headers]
    for line in file_lines:
        for i, data in zip(range(len(line.split(',')[:-3])),line.split(',')[:-3]):
            data_columns[i].append(float(data))

    for header,data_column in zip(headers,data_columns):
        data_packet.update({header:data_column})


myFilter = complementaryFilter(0.5)
print([*data_packet])
speedList = []
error = []
numberOfSmpels = 5000
average_alt_offset = 0
average_zAcc_offset = 0

for t,a,p,s in zip(data_packet['Time'][:numberOfSmpels],data_packet['zAcc'][:numberOfSmpels],data_packet['alt'][:numberOfSmpels],data_packet['zSpeed'][:numberOfSmpels]):
    speed = myFilter.complementaryFilter(t,a,p)
    average_alt_offset += p
    average_zAcc_offset += a
    speedList.append(speed)
    error.append(s)
    #sleep(0.001)

print(average_alt_offset/numberOfSmpels)
print(average_zAcc_offset/numberOfSmpels)

#px.plot([time/1000000 for time in data_packet['Time'][:numberOfSmpels]],speedList)
px.plot([time/1000000 for time in data_packet['Time'][:numberOfSmpels]],error)
#px.plot([time/1000000 for time in data_packet['Time'][:numberOfSmpels]],data_packet['zAcc'][:numberOfSmpels])
#px.plot([time/1000000 for time in data_packet['Time'][:numberOfSmpels]],data_packet['alt'][:numberOfSmpels])

px.legend(["Calculated", "Error", "Sens zAcc", "Sens alt"])
px.show()