import serial
import subprocess
import time

num_batchs = 0 #chunks of data to be read
data = []
# fil = open("./data/data.txt","a")
# ser = serial.Serial()
# ser.timeout = 100
# ser.port = 'COM16'
# ser.baudrate =38400
# ser.open()

# #getting data from stm
# while num_batchs!=4:
#     for i in range(10):
#         line = ser.readline().decode('UTF-8','ignore')
#         print(line) # COM16
#         data.append(line)
        
#     fil.write(" ".join(data))
#     if num_batchs!=3:
#         print("\nReading next data batch:\n")
#     time.sleep(1)
#     num_batchs+=1

# print("\nCompressing......")
# subprocess.call("huffman -e ./data/data.txt ./encoded/encoded.txt")
# print("\n")
# print("\nEncrypting data.....")
# subprocess.call("../fenc/fenc e '!ace' ./encoded/encoded.txt out.txt")
# print("\nDecrypting data.....")
# time.sleep(2)
# subprocess.call("../fenc/fenc d '!ace' out.txt final.txt")
print("\n\nDecompressing......")
subprocess.call("huffman -d final.txt ./output/now.txt")
print("\nProcess completed!")


