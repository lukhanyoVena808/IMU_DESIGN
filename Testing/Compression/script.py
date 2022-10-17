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
filename1 = "compressed.txt"
fil = open(filename1,"w",encoding ="utf-8")
ser = serial.Serial()
ser.timeout = 1
ser.port = 'COM16'
ser.baudrate =38400
ser.open()

#getting data from stm
for i in range(10):
        line = ser.readline().strip()
        line = line.decode('utf-8','ignore')
        if (len(line)>5):
            data.append(line)   

# print(data)
fil.write("\n".join(data))
fil.close()