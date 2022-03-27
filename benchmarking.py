import subprocess as sub
import matplotlib.pyplot as plt
from pathlib import Path
from sys import platform
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

def load_subProcess(args, file):
    if platform == "win32":
        proc = sub.Popen("./build/3DViewer -b " + args +" -i " + file)
    else:
        proc = sub.Popen("./build/3DViewer -b " + args +" -i " + file, shell = True)
    proc.wait()


if((len(sys.argv))>1):
    for arg in sys.argv[1:]:
        args += arg+ " "
        
if (not os.path.exists("./out/")):
    os.mkdir("./out/")

if os.path.exists(csvFilePath):
    os.remove(csvFilePath)




for file in glob.glob(plyFilePath + '*.ply'):
    if(len(sys.argv)==2):
        for pl in pointLights:
            tmpArgs = args
            args += " --pl "+str(pl)

            fpsFile = open(csvFilePath, 'a')
            if filecount ==0:
                pass
            elif filecount% len(pointLights) ==0 :
                fpsFile.write('\n')
            else:
                fpsFile.write(', ')
            fpsFile.close()

            load_subProcess(args, file)
            filecount+=1
            args = tmpArgs
          
    else:  
        if filecount%fileNb !=0 :  
            fpsFile = open(csvFilePath, 'a')
            fpsFile.write(', ')
            fpsFile.close()
        load_subProcess(args, file)
        filecount+=1

    fileNames.append(Path(file).stem)

file = open (csvFilePath)
csvreader = csv.reader(file)
rows = []
for row in csvreader:
    row = [float(x) for x in row]
    rows.append(row)
    rowcount+= 1


indexCsvFile =0
if rowcount ==1 :
    plt.bar(fileNames, rows[0])
    plt.title('FPS simple')
else :
    for row in rows :       
        plt.plot(pointLights,row, label = fileNames[indexCsvFile])
        plt.legend()
        indexCsvFile +=1
    plt.xlabel('point lights')
    plt.ylabel('FPS')
plt.show()