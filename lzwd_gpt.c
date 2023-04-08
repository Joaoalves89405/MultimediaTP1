#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_DICT_SIZE 65536

typedef struct
{
    int prefix;
    int suffix;
} Pattern;

Pattern dict[MAX_DICT_SIZE];
int dict_size = 0;

void init_dict()
{
    dict_size = 256;
    for (int i = 0; i < dict_size; i++)
    {
        dict[i].prefix = -1;
        dict[i].suffix = i;
    }
}

int find_pattern(int prefix, char suffix)
{
    for (int i = 0; i < dict_size; i++)
    {
        if (dict[i].prefix == prefix && dict[i].suffix == suffix)
        {
            return i;
        }
    }
    return -1;
}

void add_pattern(int prefix, char suffix)
{
    dict[dict_size].prefix = prefix;
    dict[dict_size].suffix = suffix;
    dict_size++;
}

void compress(FILE *in_file, FILE *out_file)
{
    init_dict();
    int prefix = fgetc(in_file);
    while (prefix != EOF)
    {
        int suffix = fgetc(in_file);
        if (suffix == EOF)
        {
            int code = find_pattern(prefix, -1);
            fwrite(&code, sizeof(code), 1, out_file);
            break;
        }
        int pattern = find_pattern(prefix, suffix);
        if (pattern != -1)
        {
            prefix = pattern;
        }
        else
        {
            int code = find_pattern(prefix, -1);
            fwrite(&code, sizeof(code), 1, out_file);
            if (dict_size >= MAX_DICT_SIZE)
            {
                fprintf(stderr, "Error: dictionary overflow\n");
                exit(1);
            }
            add_pattern(prefix, suffix);
            prefix = suffix;
        }
        if (dict_size == 512) // check if dictionary size is 512
        {
            init_dict(); // reset the dictionary
        }
    }
}

void print_dict()
{
    char buffer[1024];
    for (int i = 256; i < dict_size; i++)
    {
        int prefix = dict[i].prefix;
        char suffix = dict[i].suffix;
        int buffer_pos = 0;
        buffer[buffer_pos++] = suffix;
        while (prefix >= 256)
        {
            buffer[buffer_pos++] = '[';
            buffer_pos += sprintf(&buffer[buffer_pos], "%d", prefix);
            buffer[buffer_pos++] = ']';
            prefix = dict[prefix].prefix;
        }
        buffer[buffer_pos++] = prefix;
        buffer[buffer_pos++] = '[';
        buffer_pos += sprintf(&buffer[buffer_pos], "%d", prefix);
        buffer[buffer_pos++] = ']';
        buffer[buffer_pos++] = '\0';
        printf("%s ", buffer);
    }
    printf("\n");
}

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        fprintf(stderr, "Usage: %s input_file output_file\n", argv[0]);
        return 1;
    }
    FILE *in_file = fopen(argv[1], "rb");
    if (in_file == NULL)
    {
        fprintf(stderr, "Error: cannot open input file %s\n", argv[1]);
        return 1;
    }
    FILE *out_file = fopen(argv[2], "wb");
    if (out_file == NULL)
    {
        fprintf(stderr, "Error: cannot open output file %s\n", argv[2]);
        return 1;
    }
    compress(in_file, out_file);
    fclose(in_file);
    fclose(out_file);

    // Calculate compression rate
    FILE *orig_file = fopen(argv[1], "rb");
    fseek(orig_file, 0, SEEK_END);
    long orig_size = ftell(orig_file);
    fclose(orig_file);

    FILE *compressed_file = fopen(argv[2], "rb");
    fseek(compressed_file, 0, SEEK_END);
    long compressed_size = ftell(compressed_file);
    fclose(compressed_file);
    print_dict();
    printf("Compression rate: %.2f%%\n", (float)compressed_size / orig_size * 100);

    return 0;
}
