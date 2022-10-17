#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "../../Xor_Encryption/xor_algo.h"

/// @brief Test the Encryption Xor-Algorithm
/// @param argc 
/// @param argv 
/// @return 
int main(int argc, char const *argv[])
{
    FILE* ptr;
    FILE* fp;
    int count = 80;
    char Message[count];
 
    // Opening file in reading mode
    ptr = fopen("encrypted_cleaned.txt", "r");

    // Opening file in writing mode
    fp = fopen("decrypted_raw.txt", "w");

    
    if (NULL == ptr) {
        printf("file can't be opened \n");
    }

     while (fgets(Message, 80, ptr) != NULL) {
	    printf("\nEncrypted: %s\n", (char *)Message);
	    dXor((char *)Message, (char *)Message);
	    printf("\nDecrypted: %s\n", (char *)Message);
        fputs(Message, fp);
    }


    fclose(ptr);
    fclose(fp);
    return 0;
}
