#include <stdio.h>
#include <string.h>
#include <stdlib.h>

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

void Compression(unsigned char *sizeOut, char *Message_size) {

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
	char cbits = Bits;
	while (cbits > 0) {
		*sizeOut++ = Buffer;
		Buffer >>= 8;
		if(Bits<8){
			cbits =0;
		}
		else{
		Bits -= 8;
		cbits = Bits;
		}
	}
}


int main(void){

	int lines = 962249;
	int sets = 15;
	int chunks = lines/sets;
	int incr = chunks;
	FILE *pt;
	for (int i=0; i<sets;i++){
		char *str = malloc(10);
		sprintf(str, "dataset%d.txt",i);
		pt = fopen(str, "r");
		char buffer[chunks*80];
		fread(buffer, 80, chunks, pt);
		printf("%s\n",buffer);
		chunks = chunks + incr;
	}
	return 0;
}