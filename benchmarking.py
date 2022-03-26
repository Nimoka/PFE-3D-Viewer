import subprocess as sub
import matplotlib.pyplot as plt
from pathlib import Path
import os
import glob
import csv
import sys


fileNames =[]
filecount =0
rowcount =0
args = ""
csvFilePath = './out/fps.csv'
plyFilePath = './data/models/'
fileNb = len(glob.glob(plyFilePath + '*.ply'))
pointLights = [x*50 for x in range(1,6)]

if((len(sys.argv))>1):
    for arg in sys.argv[1:]:
        args += arg+ " "

if os.path.exists(csvFilePath):
    os.remove(csvFilePath)



for file in glob.glob(plyFilePath + '*.ply'):
    if(len(sys.argv)==2):
        for pl in pointLights:
            tmpArgs = args
            args += " --pl "+str(pl)
            if filecount%fileNb ==0 and filecount ==0:
                fpsFile = open(csvFilePath, 'a')
                fpsFile.write('')
                fpsFile.close()
            elif filecount%fileNb ==0 :
                fpsFile = open(csvFilePath, 'a')
                fpsFile.write('\n')
                fpsFile.close()
            else :
                fpsFile = open(csvFilePath, 'a')
                fpsFile.write(', ')
                fpsFile.close()

            proc = sub.Popen("./build/3DViewer -b " + args +" -i " + file)
            filecount+=1
            args = tmpArgs
            proc.wait()
    else:  
        if filecount%fileNb !=0 :  
            fpsFile = open(csvFilePath, 'a')
            fpsFile.write(', ')
            fpsFile.close()
        proc = sub.Popen("./build/3DViewer -b " + args +" -i " + file)
        filecount+=1
        proc.wait()

    fileNames.append(Path(file).stem)

file = open (csvFilePath)
csvreader = csv.reader(file)
rows = []
for row in csvreader:
    rows.append(row)
    rowcount+= 1


indexCsvFile =0;
if rowcount ==1 :
    plt.bar(fileNames, rows[0])
else :
    for row in rows :       
        plt.plot(pointLights,row, label = fileNames[indexCsvFile])
        plt.legend()
        indexCsvFile +=1
    plt.xlabel('point lights')
    plt.ylabel('FPS')
plt.show()