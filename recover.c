#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define BLOCK_SIZE 52

//Defining the byte variable
typedef uint8_t BYTE;

int main(int argc, char *argv[])
{
    //Check that the argument count is 2 (Check that the user provides 2 arguments)
    if (argc != 2)
    {
        printf("Usage ./recover IMAGE\n");
        return 1;
    }

    //Open the file for reading
    FILE *input_file = fopen(argv[1], "r");

    //The check if the input_file is valid
    //Safety check, if input_file has no value (is NULL), the return the error message and end the program
    if (input_file == NULL)
    {
        printf("Could not open file\n");
        return 2;
    }

    //Store the blocks of 512 bytes (the block size of the file system) in an array
    unsigned char buffer[512];

    //Track the number of images that are being generated
    int count_image = 0;

    //File pointer for the recovered images (the output file)
    FILE *output_file = NULL;

    //Allocate memory for the file name
    //Char filename[0]
    char *filename = malloc(8 * sizeof(char));

    //Read the blocks of 512 bytes
    while (fread(buffer, sizeof(char), 512, input_file))
    {
        //Check if the bytes indicate the start of a JPEG
        if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && (buffer[3] & 0xf0) == 0xe0)
        {
            //Write the filename of the JPEG
            //The system will print 3 digits, with zero infront of the 3 digits
            sprintf(filename, "%03i.jpg", count_image);

            //Open output file for writing
            output_file = fopen(filename, "w");

            //Count the number of images that were found
            count_image++;
        }
        //Check if the output has been used for a valid input
        if (output_file != NULL)
        {
            fwrite(buffer, sizeof(char), 512, output_file);
        }
    }
    free(filename);
    fclose(output_file);
    fclose(input_file);

    return 0;
}

