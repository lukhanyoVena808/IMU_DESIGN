// ============================================================================
// Name        : main.c
// Author      : Nicolas Favre
// Description : SUPSI/DTI - I2A (2017/2018), LZ77 Compression Algorithm
// ============================================================================

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

// windowSize = Size of dictionary
// bufferSize = Size of lookahead buffer
// Important: windowSize < 255 & windowSize > bufferSize!
#define windowSize 60
#define bufferSize 40
#define arraySize bufferSize + windowSize
typedef struct stream_t stream_t, *stream;
struct stream_t {
	/* get function is supposed to return a byte value (0-255),
		or -1 to signify end of input */
	int (*get)(stream);
	/* put function does output, one byte at a time */
	int (*put)(stream, int);
};

/* next two structs inherit from stream_t */
typedef struct {
	int (*get)(stream);
	int (*put)(stream, int);
	char *string;
	int pos;
} string_stream;

typedef struct {
	int (*get)(stream);
	int (*put)(stream, int);
	FILE *fp;
} file_stream;

/* methods for above streams */
int sget(stream in)
{
	int c;
	string_stream* s = (string_stream*) in;
	c = (unsigned char)(s->string[s->pos]);
	if (c == '\0') return -1;
	s->pos++;
	return c;
}

int sput(stream out, int c)
{
	string_stream* s = (string_stream*) out;
	s->string[s->pos++] = (c == -1) ? '\0' : c;
	if (c == -1) s->pos = 0;
	return 0;
}

int file_put(stream out, int c)
{
	file_stream *f = (file_stream*) out;
	return fputc(c, f->fp);
}

/* helper function */
void output(stream out, unsigned char* buf, int len)
{
	int i;
	out->put(out, 128 + len);
	for (i = 0; i < len; i++)
		out->put(out, buf[i]);
}


typedef enum { false, true } bool;

// ============================================================================

// This method searches for a match from str[] in window[] of strLen length.
// Returns the position of the match starting from the beginning of window[],
// or -1 if no match is found.
// Is invoked during every iteration of the compression algorithm.
int findMatch(unsigned char window[], unsigned char str[], int strLen) {
    int j, k, pos = -1;

    for (int i = 0; i <= windowSize - strLen; i++) {
        pos = k = i;

        for (j = 0; j < strLen; j++) {
            if (str[j] == window[k])
                k++;
            else
                break;
        }
        if (j == strLen)
            return pos;
    }

    return -1;
}

// ============================================================================

// This method contains the logic of the compression algorithm.
// Is invoked when "-c" option is specified in launch command, followed by file name.
int compress(stream in, stream out) {

    unsigned char buf[256];
	int (*get)(stream) = in->get;
	int (*put)(stream, int) = out->put;

    bool last = false;
    int inputLength = 0;
    int outputLength = 0;
    int endOffset = 0;
    int pos = -1;
    int i, size, shift, c_in;
    size_t bytesRead = (size_t) -1;
    unsigned char c;
    unsigned char array[arraySize];
    unsigned char window[windowSize];
    unsigned char buffer[bufferSize];
    unsigned char loadBuffer[bufferSize];
    unsigned char str[bufferSize];




    inputLength = sizeof(in);

    fprintf(stdout, "Input file size: %d bytes", inputLength);

    // If file is empty, return alert
    if (inputLength == 0)
        return 3;

    // If file length is smaller than arraySize, not worth processing
    if (inputLength < arraySize)
        return 2;

    // Load array with initial bytes
    // fread(array, 1, arraySize, fileInput);

    // Write the first bytes to output file
    // fwrite(array, 1, windowSize, fileOutput);
    // output(out, array, windowSize);

    // LZ77 logic beginning
    while (true) {
        if ((c_in = get(in)) == EOF)
            last = true;
        else
            c = (unsigned char) c_in;

        // Load window (dictionary)
        for (int k = 0; k < windowSize; k++)
            window[k] = array[k];

        // Load buffer (lookahead)
        for (int k = windowSize, j = 0; k < arraySize; k++, j++) {
            buffer[j] = array[k];
            str[j] = array[k];
        }

        // Search for longest match in window
        if (endOffset != 0) {
            size = bufferSize - endOffset;
            if (endOffset == bufferSize)
                break;
        }
        else {
            size = bufferSize;
        }

        pos = -1;
        for (i = size; i > 0; i--) {
            pos = findMatch(window, str, i);
            if (pos != -1)
                break;
        }

        // No match found
        // Write only one byte instead of two
        // 255 -> offset = 0, match = 0
        if (pos == -1) {
            // fputc(255, fileOutput);
            put(out,255);
            put(out, buf[0]);
            // fputc(buffer[0], fileOutput);
            shift = 1;
        }
        // Found match
        // offset = windowSize - position of match
        // i = number of match bytes
        // endOffset = number of bytes in lookahead buffer not to be considered (EOF)
        else {
            // fputc(windowSize - pos, fileOutput);
            put(out, windowSize - pos);
            // fputc(i, fileOutput);
            put(out, i);
            if (i == bufferSize) {
                shift = bufferSize + 1;
                if (!last)
                    // fputc(c, fileOutput);
                    put(out, c);
                else
                    endOffset = 1;
            }
            else {
                if (i + endOffset != bufferSize)
                    // fputc(buffer[i], fileOutput);
                    put(out, buffer[i]);
                else
                    break;
                shift = i + 1;
            }
        }

        // Shift buffers
        for (int j = 0; j < arraySize - shift; j++)
            array[j] = array[j + shift];
        if (!last)
            array[arraySize - shift] = c;

        if (shift == 1 && last)
            endOffset++;

        // If (shift != 1) -> read more bytes from file
        // if (shift != 1) {
        //     // Load loadBuffer with new bytes
        //     bytesRead = fread(loadBuffer, 1, (size_t) shift - 1, fileInput);

        //     // Load array with new bytes
        //     // Shift bytes in array, then splitted into window[] and buffer[] during next iteration
        //     for (int k = 0, l = arraySize - shift + 1; k < shift - 1; k++, l++)
        //         array[l] = loadBuffer[k];

        //     if (last) {
        //         endOffset += shift;
        //         continue;
        //     }

        //     if (bytesRead < shift - 1)
        //         endOffset = shift - 1 - bytesRead;
        // }
    }

    // Get fileOutput length
    // fseek(fileOutput, 0, SEEK_END);
    outputLength = sizeof(out);
    // fseek(fileOutput, 0, SEEK_SET);

    fprintf(stdout, "\nOutput file size: %d bytes\n", outputLength);

    // Close I/O files
    // fclose(fileInput);
    // fclose(fileOutput);

    return 1;
}

// ============================================================================

// This method contains the logic of the inverse algorithm, used to decompress.
// Is invoked when "-d" option is specified in launch command.
int decompress(char* inputPath,char* outPath ) {
    FILE *fileInput;
    FILE *fileOutput;
    int shift, offset, match, c_in;
    bool done = false;
    unsigned char c;
    unsigned char window[windowSize];
    unsigned char writeBuffer[windowSize];
    unsigned char readBuffer[2];

    // Open I/O files
    fileInput = fopen(inputPath, "rb");
    fileOutput = fopen(outPath, "wb");

    if (!fileInput) {
        fprintf(stderr, "Unable to open fileInput %s", "output.lz77");
        return 0;
    }

    // Load array with initial bytes and write to file
    fread(window, 1, windowSize, fileInput);
    fwrite(window, 1, windowSize, fileOutput);

    // Inverse algorithm beginning
    while (true) {
        // Read file by couples/triads to reconstruct original file
        size_t bytesRead = fread(readBuffer, 1, 2, fileInput);

        if (bytesRead >= 2) {
            offset = (int) readBuffer[0];
            match = (int) readBuffer[1];

            // If first byte of readBuffer is 255 -> offset = 0, match = 0
            if (offset == 255) {
                offset = 0;
                c = (unsigned char) match;
                match = 0;
                shift = match + 1;
            }
            else {
                shift = match + 1;
                c_in = fgetc(fileInput);
                if (c_in == EOF)
                    done = true;
                else
                    c = (unsigned char) c_in;
            }

            // Load and write occurrence to file
            for (int i = 0, j = windowSize - offset; i < match; i++, j++)
                writeBuffer[i] = window[j];
            fwrite(writeBuffer, 1, (size_t) match, fileOutput);

            if (!done)
                fputc(c, fileOutput);

            // Shift window
            for (int i = 0; i < windowSize - shift; i++)
                window[i] = window[i + shift];

            for (int i = 0, j = windowSize - shift; i < match; i++, j++)
                window[j] = writeBuffer[i];
            window[windowSize - 1] = c;
        }
        else {
            break;
        }
    }

    // Close I/O files
    fclose(fileInput);
    fclose(fileOutput);

    return 1;
}

// ============================================================================

// This method is the controller, reads user inputs.
// Is invoked on program launch.
int main() {
    clock_t begin = clock();
    
    char buf[256];
	string_stream str_in = { sget, 0,
		"0.093999997	12	0.999899983	0.0037	-0.0003	0	-0.0018	-0.0024	4.488900185	-0.061000001	-0.061000001	0	0	0.0006	0.0073	0	0.0006	-0.0073	0.0006	0.0073	0.999899983	30.37709999 0.093999997	12	0.999899983	0.0037	-0.0003	0	-0.0018	-0.0024	4.488900185	-0.061000001	-0.061000001	0	0	0.0006	0.0073	0	0.0006	-0.0073	0.0006	0.0073	0.999899983	30.37709999", 0};
	string_stream str_out = { sget, sput, buf, 0 };
	file_stream file = { 0, file_put, stdout };

    compress((stream)&str_in, (stream)&str_out);
    // printf("\n\nInput: %s",&str_in);
    // printf("\n\nOuput: %s",&str_out);
     fprintf(stdout, "\nOutput file size: %d bytes\n",(stream)&str_in);
	fprintf(stdout, "\nOutput file size: %s bytes\n",(stream)&str_out);
    // Print execution time
    clock_t end = clock();
    printf("\n\nExecution time: ");
    printf("%f", ((double) (end - begin) / CLOCKS_PER_SEC));
    printf(" [seconds]");

    return 0;
}

// ============================================================================