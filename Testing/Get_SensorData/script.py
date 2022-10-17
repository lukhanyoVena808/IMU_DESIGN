from array import array
from asyncore import write
from fileinput import filename
from tokenize import Double
from turtle import fd
import serial
import codecs
import subprocess
import time
import os

num_batchs = 0 #chunks of data to be read
data = []
line = ""
filename1 = "data.txt"
filename2 = "clean_data.txt"
# fil = open(filename1,"w",encoding="utf-8")
# ser = serial.Serial()
# ser.timeout = 1
# ser.port = 'COM16'
# ser.baudrate =38400
# ser.open()
# count = 0
# # getting data from stm
# while(count<10000):
#     # Wait until there is data waiting in the serial buffer
#     if(ser.in_waiting > 0):
#         line = ser.readline().strip().decode('utf-8','ignore')
#         print(line)# COM16
#         if len(line)> 10:

#             data.append(line)
#     fil.write("\n".join(data))
#     count+=1

# fil.close()

# print("cleaning....")
# cleaner = []
# fdata = open(filename1,"r",encoding="utf-8")
# for i in fdata:
#     i = i.strip()
#     if(len(i)<=64 and len(i)>10):
#         cleaner.append(i)

# fdata.close()
# fdata2 = open(filename2,"w",encoding="utf-8")
# fdata2.write("\n".join(cleaner))
# fdata2.close()
# print("done....")


#Disect Data into chunks
fdata = open(filename2,"r",encoding="utf-8")
arr = fdata.readlines()
numLines = len(arr)
datasets = 15
chunk = (int)(numLines/datasets)
incr = chunk
print(len(arr))
# for i in range(datasets):
#     dset = open(f"dataset{i}.txt","w",encoding="utf-8")
#     for line in range(chunk):
#             dset.write(arr[line])
#     dset.close()
#     chunk+=incr


# fdata.close()
   
