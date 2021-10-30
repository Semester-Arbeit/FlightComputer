import matplotlib.pyplot as plt
import numpy as np

powerSettings = [0,10,20,30,40,50,60,70,80,90,100]

print([i*1.8 for i in powerSettings ])
pwmDutyCycle_measurement = [464,674,872,1064,1280,1488,1680,1880,2088,2280,2488]
pwmDutyCycle_afterCorrection = [1002,1080,1176,1280,1384,1490,1584,1688,1784,1888,1992]
ones = [1,1,1,1,1,1,1,1,1,1,1]

pwmDutyCycle_should =[]
for i in range(len(powerSettings)):
    pwmDutyCycle_should.append(10* powerSettings[i] + 1000);

M = np.array(np.column_stack([powerSettings,ones]))
f = np.array(pwmDutyCycle_measurement)

res = np.linalg.lstsq(M,f,rcond=None)

pwmDutyCycle_current = []
for i in range(len(powerSettings)):
    pwmDutyCycle_current.append(res[0][0]* (powerSettings[i] * 0.5+ 26.075) + res[0][1]);

error = 0;

for i in range(len(powerSettings)):
    error += pwmDutyCycle_current[i] - pwmDutyCycle_should[i]

for output in pwmDutyCycle_current:
    print(output,"us")

print(error)
plt.plot(powerSettings,pwmDutyCycle_current)
plt.plot(powerSettings,pwmDutyCycle_should)
plt.show()