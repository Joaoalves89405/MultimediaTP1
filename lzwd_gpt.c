#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_DICT_SIZE 65536
#define MAX_PATTERN_LEN 1024

typedef struct
{
    int prefix[MAX_PATTERN_LEN];
    int suffix[MAX_PATTERN_LEN];
    int len;
} Pattern;

Pattern dict[MAX_DICT_SIZE];
int dict_size = 0;

void init_dict()
{
    dict_size = 256;
    for (int i = 0; i < dict_size; i++)
    {
        dict[i].len = 1;
        dict[i].prefix[0] = -1;
        dict[i].suffix[0] = i;
    }
}

int find_pattern(int *prefix, int len, int suffix)
{
    for (int i = 0; i < dict_size; i++)
    {
        if (dict[i].len == len && dict[i].suffix[len - 1] == suffix)
        {
            int j;
            for (j = 0; j < len; j++)
            {
                if (dict[i].prefix[j] != prefix[j])
                {
                    break;
                }
            }
            if (j == len)
            {
                return i;
            }
        }
    }
    return -1;
}

void add_pattern(int *prefix, int len, int suffix)
{
    dict[dict_size].len = len;
    for (int i = 0; i < len; i++)
    {
        dict[dict_size].prefix[i] = prefix[i];
        dict[dict_size].suffix[i] = suffix;
    }
    dict_size++;
}

void compress(FILE *in_file, FILE *out_file)
{
    init_dict();
    int prefix[MAX_PATTERN_LEN] = {-1};
    int prefix_len = 0;
    int suffix = fgetc(in_file);
    while (suffix != EOF)
    {
        int new_prefix[MAX_PATTERN_LEN];
        int new_prefix_len = 0;
        new_prefix[new_prefix_len++] = prefix[prefix_len - 1];
        new_prefix[new_prefix_len++] = suffix;
        int pattern = find_pattern(prefix, prefix_len, suffix);
        if (pattern != -1)
        {
            prefix[prefix_len++] = suffix;
        }
        else
        {
            // printf("prefix[0]: %d, prefix[1]:%d, suffix:%d\n", prefix[0], prefix[1], suffix);
            int code = find_pattern(prefix, prefix_len - 1, suffix);
            fwrite(&code, sizeof(code), 1, out_file);
            if (dict_size >= MAX_DICT_SIZE)
            {
                fprintf(stderr, "Error: dictionary overflow\n");
                exit(1);
            }
            add_pattern(prefix, prefix_len, suffix);
            prefix[prefix_len++] = suffix;
            prefix_len = 2;
            prefix[0] = -1;
        }
        if (dict_size == 512) // check if dictionary size is 512
        {
            init_dict(); // reset the dictionary
        }
        prefix[0] = new_prefix[0];
        prefix[1] = new_prefix[1];
        prefix_len = 2;
        suffix = fgetc(in_file);
    }
    int code = find_pattern(prefix, prefix_len - 1, -1);
    fwrite(&code, sizeof(code), 1, out_file);
}

void print_dict()
{
    char buffer[1024];
    int buffer_pos = 0;
    for (int i = 256; i < dict_size; i++)
    {
        buffer_pos = 0;
        for (int j = 0; j < dict[i].len; j++)
        {
            if (dict[i].suffix[j] < 256)
            {
                buffer[buffer_pos++] = dict[i].suffix[j];
            }
            else
            {
                buffer_pos += sprintf(&buffer[buffer_pos], "[%d]", dict[i].suffix[j]);
            }
        }
        buffer[buffer_pos++] = ' ';
        for (int j = 0; j < dict[i].len - 1; j++)
        {
            if (dict[i].prefix[j] < 256)
            {
                buffer[buffer_pos++] = dict[i].prefix[j];
            }
            else
            {
                buffer_pos += sprintf(&buffer[buffer_pos], "[%d]", dict[i].prefix[j]);
            }
        }
        buffer[buffer_pos++] = '\0';
        printf("%d: %s\n", i, buffer);
    }
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

    // Calculate compression rate
    fseek(in_file, 0, SEEK_END);
    long orig_size = ftell(in_file);
    fseek(out_file, 0, SEEK_END);
    long compressed_size = ftell(out_file);
    fclose(in_file);
    fclose(out_file);
    print_dict();
    printf("Compression rate: %.2f%%\n", (float)compressed_size / orig_size * 100);

    return 0;
}
