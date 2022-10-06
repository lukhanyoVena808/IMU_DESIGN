#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#include <errno.h>


void Compression(unsigned char *sizeOut, const char *Message_size) {  
    
	unsigned long long Buffer = 0;  
	char Bits = 0;
	
	while (*Message_size != 0) {
		Buffer |= (unsigned long long)(*Message_size++) << Bits;
		Bits += 7;
		if (Bits == 7 * 8) { 
			while (Bits > 0) {
				*sizeOut++ = Buffer;
				Buffer >>= 8;
				Bits -= 8;
			}
			Bits = 0;
			Buffer = 0;
		}
	}
	while (Bits > 0) {
		*sizeOut++ = Buffer;
		Buffer >>= 8;
		Bits -= 8;
	}
}


void Decompression (char *Out_size, const unsigned char *Compressed_size, unsigned CompressedLength) {

	unsigned long long Buffer = 0;
	char Bits = 0;
	while (CompressedLength) {
		while (CompressedLength && Bits < 7 * 8) {
			Buffer |= (unsigned long long)*Compressed_size++ << Bits;
			Bits += 8;
			--CompressedLength;
		}
		char cbits = Bits;
		while (cbits > 0) {
			*Out_size++ = Buffer & 0x7F;
			Buffer >>= 7;
			if(Bits<8){
				cbits =0;
			}
			else{
			Bits -= 8;
			cbits = Bits;
			}
		}
		Bits = 0;
		Buffer = 0;
	}
}


int main(int argc, char* argv[]) {
    
    
    unsigned char Message[900];

	

	for (int j=1;j<argc;j++){
		strcat(Message, argv[j]);

		if (j!=argc-1){
			strcat(Message, " ");
		}

	}
	
	unsigned CompressedSize = sizeof(1200)*6/8;
    // printf("Old Size: %d\n",sizeof(Message));
    // printf("new size: %d\n",CompressedSize);
    // printf("Ratio: %f\n",(double)CompressedSize/(double)sizeof(Message)*100);
	// unsigned char CompressedBytes[CompressedSize]; 
	char DecompressedSize[sizeof(1200)];
	
	
	// Decompression(DecompressedSize, CompressedBytes, 2048);
	Decompression(DecompressedSize, Message, CompressedSize);
	DecompressedSize[sizeof(Message)] = 0; 
	printf("\nUncompressed version: %s\n", Message);
	printf("\nDecompressed version: %s\n", DecompressedSize);


	return 0;
}