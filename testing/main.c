#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "xor_algo.h"

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

int main(int argc, char* argv[]){

    int original = 2500;
    int compressed = original*(6/8);

    uint8_t buffer[compressed];
    // strcpy(buffer,  argv[1]);
    // printf("Normal: %s\n", argv[1]);
    aXor(buffer, argv[1]);
    printf("Crypted : %s\n\n", buffer);
    dXor(buffer, buffer);
    printf("%s\n", buffer);

    // unsigned CompressedSize = sizeof(buffer)*6/8; 
	// unsigned char CompressedBytes[CompressedSize]; 
	// char DecompressedSize[sizeof(original)];
    // Decompression(DecompressedSize, buffer, compressed);
	// DecompressedSize[original] = 0; 
    //   printf("Decompresed: %s\n", DecompressedSize);



    return 0;
}