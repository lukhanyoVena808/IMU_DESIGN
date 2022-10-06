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
filename = "data.txt"
fil =  codecs.open(filename,"a","utf-8")
ser = serial.Serial()
ser.timeout = 10
ser.port = 'COM16'
ser.baudrate =38400
ser.open()

#getting data from stm
while num_batchs!=2:
    for i in range(5):
        line = ser.readline().decode('utf-8','ignore')
        print(line)# COM16
        # data.append(line)
        # fil.write(line)
        # fil.write("\n")
    if num_batchs!=2:
        print("\nReading next data batch:\n")
    time.sleep(1)
    num_batchs+=1
fil.close()
# print("\nDecrypting data.....")
# subprocess.call("../Encrypt_Decrypt/fenc d '!ace' out.txt decrypted.txt")
# print("\n\nDecompressing......")
print(f"Compressed: ",line)
subprocess.call(f"main {line}")

# print("\n")
# time_exe = (time.time() - start_time)

# print("\n---Project Time Execution: %s seconds ---" % (time_exe))
# print("\nProcess completed!")


