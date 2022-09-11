import serial
import subprocess
import time
import os

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

f = open("huffman_time_excution.txt","w")

for i in range(1,11):
    filename = f"./data/data_set{i}.txt"
    file_stats = os.stat(filename).st_size
    start_time = time.time()
    print("\nCompressing......")
    subprocess.call("huffman -e ./data/data.txt ./encoded/encoded.txt")
    print("\n")
    print("\nEncrypting data.....")
    subprocess.call("../Encrypt_Decrypt/fenc e '!ace' ./encoded/encoded.txt out.txt")
    time.sleep(2)
    print("\nDecrypting data.....")
    subprocess.call("../Encrypt_Decrypt/fenc d '!ace' out.txt final.txt")
    print("\n\nDecompressing......")
    subprocess.call("huffman -d final.txt ./output/now.txt")
    print("\n")
    print("\n---Project Time Execution: %s seconds ---" % (time.time() - start_time))
print("\nProcess completed!")


