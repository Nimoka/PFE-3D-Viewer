import subprocess as sub
import matplotlib.pyplot as plt
from pathlib import Path
import glob
import time
import os
import signal
import sys
import psutil

names=[]
FPS =[]
args = ""
pointLights = [x*50 for x in range(1,6)]

if((len(sys.argv))>1):
    for arg in sys.argv[1:]:
        args += arg+ " "

for file in glob.glob("./data/models/*.ply"): 
    count =0
    sum =0
    if(len(sys.argv)==2):
        for pl in pointLights:
            tmpArgs = args
            args += " --pl "+str(pl)
            #print(args + file)
            proc = sub.Popen("./build/3DViewer -b " + args +" -i " + file , shell=True, preexec_fn=os.setsid, stdout=sub.PIPE)
            for line in iter(proc.stdout.readline, b''):
                count += 1
                sum += float(line)
            args = tmpArgs
            FPS.append(sum/count)
            os.killpg(os.getpgid(proc.pid), signal.SIGTERM)
    else:
        proc = sub.Popen("./build/3DViewer -b " + args +" -i " + file , shell=True, preexec_fn=os.setsid, stdout=sub.PIPE)
        for line in iter(proc.stdout.readline, b''):
            count += 1
            sum += float(line)
        FPS.append(sum/count)
        os.killpg(os.getpgid(proc.pid), signal.SIGTERM)

    names.append(Path(file).stem)


if(len(sys.argv)==2):
    plotCount = 0
    for i in range(len(glob.glob("./data/models/*.ply"))):
        plt.plot(pointLights, FPS[plotCount:plotCount + len(pointLights)], label = names[i])
        plt.legend()
        plotCount+= len(pointLights)
    plt.xlabel('point lights')
    plt.ylabel('FPS')

else:
    plt.bar(names, FPS)
    plt.title('FPS forward')

plt.show()