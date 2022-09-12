from fileinput import filename
from tokenize import Double
import subprocess
import time
import os

# f = open("lz77_time_excution.txt","w")

for i in range(1,11):
    filename = f"./data/data_set{i}.txt"
    # file_stats = os.stat(filename).st_size
    # print(filename,"\n")
    # print(file_stats,"\n")
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
    subprocess.call(f"main -d decrypted.txt ./output/final{i}.txt")
    print("\n")
    time_exe = (time.time() - start_time)
    # ratio =(float)(compressed/(file_stats+0.0))*100
    # f.write(f"{file_stats}, {time_exe}, {ratio}\n")
    print("\n---Project Time Execution: %s seconds ---" % (time_exe))
print("\nProcess completed!")
