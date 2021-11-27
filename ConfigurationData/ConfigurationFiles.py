import plotly.express as px
import numpy as np
import pandas

def generateKvalues(DictOfValues):
    with open("KValues.csv", 'w') as kvalues:
        kvalues.write(str(DictOfValues['PitchKi'])+ ',')
        kvalues.write(str(DictOfValues['RollKi'] )+ ',')
        kvalues.write(str(DictOfValues['YawKi']) + ',')
        kvalues.write(str(DictOfValues['PitchKp']) + ',')
        kvalues.write(str(DictOfValues['RollKp']) + ',')
        kvalues.write(str(DictOfValues['YawKp']) + ',')
        kvalues.write(str(DictOfValues['XPosKi']) + ',')
        kvalues.write(str(DictOfValues['YPosKi']) + ',')
        kvalues.write(str(DictOfValues['AltKi']) + ',')
        kvalues.write(str(DictOfValues['XPosKp']) + ',')
        kvalues.write(str(DictOfValues['YPosKp']) + ',')
        kvalues.write(str(DictOfValues['AltKp'])+'\n')


def generateAltMap(maxAlt, acccendSteepness, decentSteepness, groundClerance):
    timeBase = np.linspace(0,4999,5000)
    timeBaseAccend = np.linspace(0,2499,2500)
    timeBaseDecend = np.linspace(2499, 0, 2500)

    p1 = 5
    a1 = 1/acccendSteepness

    p2 = 5
    a2 = 1/decentSteepness

    altValuesAccend = np.round(groundClerance+(maxAlt * 1/(1+np.exp(-((timeBaseAccend/a1)-p1)))),2)
    altValuesDecend = np.round(groundClerance+(maxAlt * 1/(1+np.exp(-((timeBaseDecend/a2)-p2)))), 2)
    altValue = np.hstack((altValuesAccend,altValuesDecend))

    fig = px.scatter(x= timeBase,y = altValue)
    fig.show()
    with open("AltitudeProfile.csv", 'w') as altFile:
        for i, alt in zip(range(len(altValue[:-1])), altValue[:-1]):
            print(i,alt, sep=":")
            altFile.write(str(alt) + ',')
        altFile.write(str(altValue[-1]) + '\n')








kvalues = { 'PitchKi':1,'PitchKp':1,'XPosKi':0,'XPosKp':0,
            'RollKi':0,'RollKp':0,'YPosKi':0,'YPosKp':0,
            'YawKi':0,'YawKp':0,'AltKi':0,'AltKp':0 }

generateKvalues(kvalues)
generateAltMap(0.5,0.02,0.005,0.55)