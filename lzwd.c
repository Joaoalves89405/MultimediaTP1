#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/time.h>
#include <time.h>
#include "lzwd.h"

int main(int argc, char const *argv[])
{
    FILE *inputFile;
    if (argc != 3)
    {
        printf("Wrong number of arguments, specify the input file.\n");
        exit(1);
    }
    if ((inputFile = fopen(argv[2], "rb")) == NULL)
    {
        printf("Error - File cannot be opened.\n");
        exit(1);
    }
    const char *s1 = argv[1];
    unsigned char *s2 = "lzw";
    int compareStatus1 = 0;
    int compareStatus2 = 0;

    /* This is a loop that compares the two strings. */
    for (int i = 0; i < 1000; i++)
    {
        while ((*s1 != '\0' && *s2 != '\0') && *s1 == *s2)
        {
            s1++;
            s2++;
        }
        // Compare the mismatched character or last
        compareStatus1 = (*s1 == *s2) ? 0 : (*s1 > *s2) ? 1
                                                        : -1;
    }
    const char *sb1 = argv[1];
    unsigned char *sb2 = "lzwd";
    /* This is a loop that compares the two strings. */
    for (int i = 0; i < 1000; i++)
    {
        while ((*sb1 != '\0' && *sb2 != '\0') && *sb1 == *sb2)
        {
            sb1++;
            sb2++;
        }
        // Compare the mismatched character or last
        compareStatus2 = (*sb1 == *sb2) ? 0 : (*sb1 > *sb2) ? 1
                                                            : -1;
    }
    printf("Compare Status 2: %d\n", compareStatus2);
    if (compareStatus1 == 0 || compareStatus2 == 0)
    {
        ;
    }
    else
    {
        exit(1);
    }

    int type = 0;

    if (compareStatus1 == 0)
    {
        type = 1;
    }
    if (compareStatus2 == 0)
    {
        type = 2;
    }
    lzwd(inputFile, argv[2], type);

    return 0;
}