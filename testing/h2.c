#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <errno.h>

// void Compression(unsigned char *sizeOut, const char *Message_size) {

// 	unsigned long long Buffer = 0;
// 	char Bits = 0;

// 	while (*Message_size != 0) {
// 		Buffer |= (unsigned long long)(*Message_size++) << Bits;
// 		Bits += 7;
// 		if (Bits == 7 * 8) {
// 			while (Bits > 0) {
// 				*sizeOut++ = Buffer;
// 				Buffer >>= 8;
// 				Bits -= 8;
// 			}
// 			Bits = 0;
// 			Buffer = 0;
// 		}
// 	}
// 	char cbits = Bits;
// 	while (cbits > 0) {
// 		*sizeOut++ = Buffer;
// 		Buffer >>= 8;
// 		if(Bits<8){
// 			cbits =0;
// 		}
// 		else{
// 		Bits -= 8;
// 		cbits = Bits;
// 		}
// 	}
// }

void Decompression (char *Out_size, const unsigned char *Compressed_size, unsigned CompressedLength) {
    
	unsigned long long Buffer = 0;
	char Bits = 0;
	while (CompressedLength) {
		while (CompressedLength && Bits < 7 * 8) {
			Buffer |= (unsigned long long)*Compressed_size++ << Bits;
			Bits += 8;
			--CompressedLength;
		}
		while (Bits > 0) {
			*Out_size++ = Buffer & 0x7F;
			Buffer >>= 7;
			Bits -= 7;
		}
		
		Bits = 0;
		Buffer = 0;
	}
}

// void Decompression (char *Out_size, const unsigned char *Compressed_size, unsigned CompressedLength) {

// 	unsigned long long Buffer = 0;
// 	char Bits = 0;
// 	while (CompressedLength) {
// 		while (CompressedLength && Bits < 7 * 8) {
// 			Buffer |= (unsigned long long)*Compressed_size++ << Bits;
// 			Bits += 8;
// 			--CompressedLength;
// 		}
// 		char cbits = Bits;
// 		while (cbits > 0) {
// 			*Out_size++ = Buffer & 0x7F;
// 			Buffer >>= 7;
// 			if(Bits<8){
// 				cbits =0;
// 			}
// 			else{
// 			Bits -= 8;
// 			cbits = Bits;
// 			}
// 		}

// 		Bits = 0;
// 		Buffer = 0;
// 	}
// }

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


int main(void) {
    
    
    
    
    int letters;
    
    
    
    
    char Message[1000] = "-0.745465, 39568.37487, 3483.67348, -0.745465, 39568.37487, 3483.67348";
    // strcat(Message, "Hello Lux.....Hi");

    
	
	unsigned CompressedSize = sizeof(Message)*6/8; 
	unsigned char CompressedBytes[CompressedSize]; 
	char DecompressedSize[sizeof(Message)];
	
	
	
	printf("Message: %s\n", Message);
	printf("char number of message: %d\n", strlen(Message));
	Compression(CompressedBytes, (char *)Message);
	
	// printf("\nCompressed version: ");
	// for (int Byte = 0; Byte < CompressedSize; ++Byte) {
	// 	printf("%02X ", CompressedBytes[Byte]);
	// }
     printf("\nMessage after: %s\n", CompressedBytes);
	
	
	Decompression(DecompressedSize, CompressedBytes, CompressedSize);
	DecompressedSize[sizeof(Message)] = 0; 
	
	printf("\nDecompressed version: %s\n", (char *)DecompressedSize);
	printf("char number of message: %d\n", strlen(DecompressedSize));
	
	// if (strcmp(Message, DecompressedSize) == 0) {
	// 	printf("\nCompression done.\n");
	// } else {
	// 	printf("\nCompression crushed!\n");
	// }
	return 0;
}