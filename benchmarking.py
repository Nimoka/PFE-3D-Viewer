import subprocess as sub
import matplotlib.pyplot as plt
from pathlib import Path
import glob
import time
import os
import signal
import sys

names=[]
FPS =[]
args = ""

if((len(sys.argv))>1):
    for arg in sys.argv[1:]:
        args += arg+ " "

for file in glob.glob("./data/models/*.ply"): 
    count =0
    sum =0
    proc = sub.Popen("./build/3DViewer -b " + args +" -i " + file , shell=True, preexec_fn=os.setsid, stdout=sub.PIPE)
    for line in iter(proc.stdout.readline, b''):
        count += 1
        sum += float(line)
    #names.append(os.path.basename(file))
    names.append(Path(file).stem)
    FPS.append(sum/count)
    os.killpg(os.getpgid(proc.pid), signal.SIGTERM)


plt.bar(names, FPS)
plt.title('FPS forward')
plt.show()