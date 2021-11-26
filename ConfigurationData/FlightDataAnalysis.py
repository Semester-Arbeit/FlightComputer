import sys

def analyseFlightData(filepath):
    Data = {}
    cycleTimeCutOff = 10500

    NumberOfOverTimes = 0
    MaxCycleTime = 0
    MinCycleTime = sys.maxsize
    AverageTime = 0

    AverageLineLenght = 0
    MaxLineLenght = 0
    MinLineLenght = sys.maxsize

    ErrorsFoundInCell = 0
    CellsFound = 0
    ListOfErrorCells = []



    with open(filepath, 'r') as csvFile:
        headerList =  csvFile.readline().split(',')[:20]
        lineLists = [[] for i in range(len(headerList))]
        for line in csvFile.read().split('\n'):
            for i,value in zip(range(len(line.split(',')[:20])),line.split(',')[:20]):
                lineLists[i].append(value)

        for i, header in zip(range(len(headerList)),headerList):
            Data.update({header:lineLists[i]})

    for i in range(len(Data['Time'][:-1])):
        try:
            next = int(Data['Time'][i+1])
            current = int(Data['Time'][i])
            res = next - current
            AverageTime += res
            if res > MaxCycleTime:
                MaxCycleTime = res

            if res < MinCycleTime:
                MinCycleTime = res

            if (res > cycleTimeCutOff):
                NumberOfOverTimes += 1
        except:
            print("!!Error!!")


    with open(filepath, 'r') as csvFile:
        file = csvFile.read().split('\n')
        for line in file:
            AverageLineLenght += len(line)
            if len(line) > MaxLineLenght:
                MaxLineLenght = len(line)
            if len(line) < MinLineLenght:
                MinLineLenght = len(line)
        AverageLineLenght /= len(file)

    for key in [*Data]:
        for i,element in zip(range(len(Data[key])),Data[key]):
            try:
                CellsFound += 1
                float(element)
            except:
                ErrorsFoundInCell += 1
                ListOfErrorCells.append([key.replace('\n',''),i,element])
    #Report

    print("-------------------------Status Report----------------------------")
    print("Overview:")
    print(f"            Number of entries           : {len(Data[[*Data][0]]):3d} (5000)")
    print(f"            Number of parameters        : {len([*Data]):3d} (20)")
    print()
    print(f"            Average line Lenght         : {round(AverageLineLenght):3d}")
    print(f"            Max line Lenght             : {MaxLineLenght:3d}")
    print(f"            Min line Lenght             : {MinLineLenght:3d}")
    print()
    print("Time Report:")
    print(f"            Average                     : {(AverageTime/len(Data[[*Data][0]]))/1000:<7.3f}ms -> {1/((AverageTime/len(Data[[*Data][0]]))/1000000)} hz -> Flight Time: {((AverageTime/len(Data[[*Data][0]]))/1000000)*5000}s")
    print(f"            MaxCycle                    : {MaxCycleTime:<7}us -> {MaxCycleTime/1000:.2f}ms")
    print(f"            MinCycle                    : {MinCycleTime:<7}us -> {MinCycleTime/1000:.2f}ms")
    print(f"            NumberOfOver                : {NumberOfOverTimes} > {cycleTimeCutOff}")
    print()
    print("Error:")
    print(f"            Number of corrupted cells   : {ErrorsFoundInCell}/{CellsFound}")
    for i, error in zip(range(len(ListOfErrorCells)),ListOfErrorCells):

        if(i%5 == 0):
            print()
            print(f"                                         ", end="")

        print(f"{error[0]:}\t[{error[1]:04d}]:\t\033[4m{error[2]:}\033[0m \t | ", end="")

    print()
    print("------------------------------Done--------------------------------")
    return

filepath = "/Volumes/NO NAME/LogFiles/FlightLog.csv"
#filepath = "/Users/tobiasrothlin/Desktop/FlightLog 2.csv"
analyseFlightData(filepath)
