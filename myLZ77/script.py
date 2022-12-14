from fileinput import filename
from tokenize import Double
import serial
import subprocess
import time
import os

num_batchs = 0 #chunks of data to be read
data = []
filename = "./data/data.txt"
fil = open(filename,"a")
ser = serial.Serial()
ser.timeout = 100
ser.port = 'COM16'
ser.baudrate =38400
ser.open()

#getting data from stm
while num_batchs!=4:
    for i in range(25):
        line = ser.readline().decode('UTF-8','ignore')
        print(line) # COM16
        data.append(line)
        
    fil.write(" ".join(data))
    if num_batchs!=3:
        print("\nReading next data batch:\n")
    time.sleep(1)
    num_batchs+=1

start_time = time.time()
print("\nCompressing......")
subprocess.call(f"main -c {filename}")
compressed = os.stat("./encoded/output.lz77").st_size
print("\n")
print("\nEncrypting data.....")
subprocess.call("../Encrypt_Decrypt/fenc e '!ace' ./encoded/output.lz77 out.txt")
time.sleep(2)
print("\nDecrypting data.....")
subprocess.call("../Encrypt_Decrypt/fenc d '!ace' out.txt decrypted.txt")
print("\n\nDecompressing......")
subprocess.call("main -d decrypted.txt ./output/final_out.txt")
print("\n")
time_exe = (time.time() - start_time)

print("\n---Project Time Execution: %s seconds ---" % (time_exe))
print("\nProcess completed!")


