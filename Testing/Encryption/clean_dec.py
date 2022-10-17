
filename3 = "decrypted_raw.txt"
filename4 = "decrypted_cleaned.txt"


#Clean Decryption Data
print("Cleaning Decrypted Data...")
cleaner = []
fdata = open(filename3,"r")
for i in fdata:
    i = i.strip()
    if(len(i)>10):
        cleaner.append(i)
fdata.close()
fdata2 = open(filename4,"w")
fdata2.write("\n".join(cleaner))
print("Done...")