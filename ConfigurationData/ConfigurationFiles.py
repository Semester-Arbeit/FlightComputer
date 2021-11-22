
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




kvalues = { 'PitchKi':1,'PitchKp':1,'XPosKi':0,'XPosKp':0,
            'RollKi':0,'RollKp':0,'YPosKi':0,'YPosKp':0,
            'YawKi':0,'YawKp':0,'AltKi':0,'AltKp':0 }

generateKvalues(kvalues)