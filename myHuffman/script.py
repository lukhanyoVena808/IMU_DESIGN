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
    subprocess.call(f"huffman -e {filename} ./encoded/encoded.txt")
    compressed = os.stat("./encoded/encoded.txt").st_size
    print("\n")
    print("\nEncrypting data.....")
    subprocess.call("../Encrypt_Decrypt/fenc e '!ace' ./encoded/encoded.txt out.txt")
    time.sleep(2)
    print("\nDecrypting data.....")
    subprocess.call("../Encrypt_Decrypt/fenc d '!ace' out.txt decrypt.txt")
    print("\n\nDecompressing......")
    subprocess.call("huffman -d decrypt.txt ./output/final.txt")
    print("\n")
    time_exe = (time.time() - start_time)
    ratio =(float)(compressed/(file_stats+0.0))*100
    f.write(f"{file_stats}, {time_exe}, {ratio}\n")
    print("\n---Project Time Execution: %s seconds ---" % (time_exe))
print("\nProcess completed!")


