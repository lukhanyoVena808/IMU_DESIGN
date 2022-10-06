#include <string.h>
#include "huffman.h"
#include "simple_file_buffer.h"
#include <time.h>

//const char *input_file, const char *output_file
int file_encode() {

    struct buffer_ops *in, *out;

    in = create_file_buffer_ops(input_file, "rb");
    // if (!in) {
    //     LOGE("Create input file ops failed\n");
    //     return 1;
    // }
    // out = create_file_buffer_ops(output_file, "wb");
    // if (!out) {
    //     LOGE("Create input file ops failed\n");
    //     goto OUT;
    // }
    strcpy(in->data , "George Orwell");
    strcpy(out->data , "");

    encode(in, out);
    // printf(in->data);
//     desotry_file_buffer_ops(out);
// OUT:
//     desotry_file_buffer_ops(in);
    return 0;
}

int file_decode(const char *input_file, const char *output_file) {

    struct buffer_ops *in, *out;

    in = create_file_buffer_ops(input_file, "rb");
    if (!in) {
        LOGE("Create input file ops failed\n");
        return 1;
    }
    out = create_file_buffer_ops(output_file, "wb");
    if (!out) {
        LOGE("Create input file ops failed\n");
        goto OUT;
    }
    decode(in, out);
    desotry_file_buffer_ops(out);
OUT:
    desotry_file_buffer_ops(in);
    return 0;
}

int main(void) {
    file_encode();
    // char *input, *output;
    // if (argc < 3) {
    //     LOGE("Miss args.\n");
    //     LOGE("Usage: -d/-e input [output]\n");
    //     return 1;
    // }
    // input = argv[2];
    // output = NULL;
    // if (argc > 3) {
    //     output = argv[3];
    // }
    // if (!strcmp(argv[1], "-e")) {
    //     clock_t begin = clock();
    //     file_encode(input, output);
    //     // Print execution time
    //     clock_t end = clock();
    //     printf("\nExecution time: ");
    //     printf("%f", ((double) (end - begin) / CLOCKS_PER_SEC));
    //     printf(" [seconds]");

    // } else if (!strcmp(argv[1], "-d")) {
    //     clock_t begin = clock();
    //     file_decode(input, output);
    //     // Print execution time
    //     clock_t end = clock();
    //     printf("\nExecution time: ");
    //     printf("%f", ((double) (end - begin) / CLOCKS_PER_SEC));
    //     printf(" [seconds]");
        
    // } else {
    //     LOGE("Unknown action.\n");
    // }
    return 0;
}
