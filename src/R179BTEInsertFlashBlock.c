/*
 ============================================================================
 Name        : R179BTEInsertFlashBlock.c
 Author      : David Smail
 Version     :
 Copyright   : SiGenix - All Rights Reserved
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const unsigned BUFFER_SIZE = 200;

static unsigned char GenerateChecksum(const char *str);


int main(void) {


    FILE *fpInput = fopen("r179.hex","r");
    FILE *fpOutput = fopen("r179table.hex","w");
    unsigned loopIndex = 0;
    char fileBuffer[200];

    puts("Flash Walking 1's and 0's Table Insertion tool... Version 1.0\n");

    if (fpInput == NULL)
    {
        puts("File \"r179.hex\" doen't exist... Exiting program");
        return (EXIT_FAILURE);
    }
    if (fpOutput == NULL)
    {
        puts("File \"r179table.hex\" can't be created... Exiting program");
        return (EXIT_FAILURE);
    }


    const char *insertTable[] =    {"020000040011",                                 /* Base Address 0x120000 */
                                     "1000000000000001000200040008001000200040",     /* Data */
                                     "1000100000800100020004000800100020004000",     /* Data */
                                     "100020008000FFFFFFFEFFFDFFFBFFF7FFEFFFDF",     /* Data */
                                     "10003000FFBFFF7FFEFFFDFFFBFFF7FFEFFFDFFF",     /* Data */
                                     "04004000BFFF7FFF", };                          /* Data */

    fseek( fpInput, 0, SEEK_SET );
    fseek( fpOutput, 0, SEEK_SET );

    /* Place the table at the very top of the hex file */
    for (loopIndex = 0; loopIndex < 6; loopIndex++)
    {
        fprintf (fpOutput, ":%s%02X\n", insertTable[loopIndex], GenerateChecksum(insertTable[loopIndex]));
    }

    /* Now append the entire original hex file by */
    while (fgets(fileBuffer, BUFFER_SIZE, fpInput) != NULL)
    {
        fprintf (fpOutput, "%s", fileBuffer);
    }

    fclose(fpInput);
    fclose(fpOutput);

	return (EXIT_SUCCESS);
}

static unsigned char GenerateChecksum(const char *str)
{
    unsigned index = 0;
    unsigned strLength = strlen(str);
    unsigned char csum = 0;

    while (index < strLength)
    {
        char hexString[3];
        unsigned char val;
        /* Copy the next 2 nibbles */
        strncpy(hexString, &str[index], 2);
        /* terminate the string since strncpy doesn't */
        hexString[2] = '\0';
        /* Convert string to hexadecimal byte */
        val = (unsigned char)strtol(hexString, NULL, 16);
        /* Accumulate and increment the index to the next 2 nibbles */
        csum += val;
        index += 2;
    }

    /* Create the 2's complement by inverting all bits and adding 1 */
    csum = csum ^ 0xFF;
    csum++;
    return (csum);
}
