
# Print iterations progress
def printProgressBar (iteration, total, prefix = '', suffix = '', decimals = 1, length = 100, fill = '█', printEnd = "\r"):
    """
    Call in a loop to create terminal progress bar
    @params:
        iteration   - Required  : current iteration (Int)
        total       - Required  : total iterations (Int)
        prefix      - Optional  : prefix string (Str)
        suffix      - Optional  : suffix string (Str)
        decimals    - Optional  : positive number of decimals in percent complete (Int)
        length      - Optional  : character length of bar (Int)
        fill        - Optional  : bar fill character (Str)
        printEnd    - Optional  : end character (e.g. "\r", "\r\n") (Str)
    """
    percent = ("{0:." + str(decimals) + "f}").format(100 * (iteration / float(total)))
    filledLength = int(length * iteration // total)
    bar = fill * filledLength + '-' * (length - filledLength)
    print(f'\r{prefix} |{bar}| {percent}% {suffix}', end = printEnd)
    # Print New Line on Complete
    if iteration == total:
        print()





FlightProfileNumberOfDataPoints= 5000;
# Initial call to print 0% progress
printProgressBar(0, FlightProfileNumberOfDataPoints, prefix = 'Progress:', suffix = 'Complete', length = 50)
with open("AltitudeProfile.csv", 'w') as file:
    file.write(f"{0:04}"+".11")
    for i in range(FlightProfileNumberOfDataPoints-1):
        file.write("," + f"{(i+1):04}"+".11")
        printProgressBar(i + 1, FlightProfileNumberOfDataPoints, prefix = 'Progress:', suffix = 'Complete', length = 50)
    file.write("\n")

print()
KValuesNumberOfDataPoints = 12;
printProgressBar(0, KValuesNumberOfDataPoints, prefix='Progress:',suffix='Complete', length=50)
with open("KValues.csv", 'w') as file:
    file.write("9"+f"{0:03}" + ".11")
    for i in range(KValuesNumberOfDataPoints - 1):
        file.write(","+"9" + f"{(i + 1):03}" + ".11")
        printProgressBar(i + 1, KValuesNumberOfDataPoints, prefix='Progress:',suffix='Complete', length=50)
    file.write("\n")