from fileinput import filename
from tokenize import Double
import serial
import codecs
import subprocess
import time
import os

num_batchs = 0 #chunks of data to be read
data = []
line = ""
filename1 = "CompAndEnc_raw.txt"
filename2 = "CompAndEnc_cleaned.txt"


fil =  open(filename1,"w",encoding="utf-8")
ser = serial.Serial()
ser.timeout = 1
ser.port = 'COM16'
ser.baudrate =38400
ser.xonxoff=1
ser.open()

#getting data from stm
# if(ser.in_waiting > 0):
for i in range(17):
        line = ser.readline().decode('utf-8','ignore')
        line = line.replace("\\", "\\\\")
        print(f"{line}")# COM16
        data.append(line)
fil.write("\n".join(data))
fil.close()

#clean data from stm32
print("cleaning....")
cleaner = []
fdata = open(filename1,"r",encoding="utf-8")
for i in fdata:
    i = i.strip()
    if(len(i)>10):
        cleaner.append(i)
fdata.close()
fdata2 = open(filename2,"w",encoding="utf-8")
fdata2.write("\n".join(cleaner))
print("done....")




