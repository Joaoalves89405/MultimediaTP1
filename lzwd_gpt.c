#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_DICT_SIZE 65536
#define MAX_PATTERN_SIZE 1024

typedef struct
{
    int pattern[MAX_PATTERN_SIZE]; // AABB
    int length;                    // 4
    int pattern_number;            // P257
} Pattern;

Pattern dict[MAX_DICT_SIZE];
int dict_size = 0;

void init_dict()
{
    dict_size = 256;
    for (int i = 0; i < dict_size; i++)
    {
        dict[i].pattern[0] = -1;
        dict[i].length = 0;
        dict[i].pattern_number = i;
    }
}

/* int find_pattern(int *prefix, int prefix_len, char suffix)
{
    for (int i = 0; i < dict_size; i++)
    {
        if (dict[i].length != prefix_len + 1)
        {
            continue;
        }
        if (memcmp(dict[i].prefix, prefix, prefix_len * sizeof(int)) != 0)
        {
            continue;
        }
        if (dict[i].suffix[prefix_len] != suffix)
        {
            continue;
        }
        return i;
    }
    return -1;
} */

void print_dict()
{
    char buffer[1024];
    for (int i = 255; i < dict_size; i++)
    {
        Pattern pattern = dict[i];
        int pattern_len = pattern.length;
        int buffer_pos = 0;
        buffer_pos += sprintf(&buffer[buffer_pos], "P%d = ", pattern.pattern_number);
        for (int j = 0; j < pattern_len; j++)
        {
            buffer_pos += sprintf(&buffer[buffer_pos], "%d ", pattern.pattern[j]);
        }
        buffer[buffer_pos++] = '\0';
        printf("%s\n", buffer);
    }
}

void add_pattern(int *aux, int aux_len)
{
    memcpy(dict[dict_size].pattern, aux, aux_len * sizeof(int));
    dict[dict_size].length = aux_len;
    dict[dict_size].pattern_number = dict_size;
    dict_size++;
}

void compress(FILE *in_file, FILE *out_file)
{
    init_dict();
    int prefix[MAX_PATTERN_SIZE];
    int suffix[MAX_PATTERN_SIZE];
    int after_suffix[MAX_PATTERN_SIZE];
    int aux[MAX_PATTERN_SIZE];
    prefix[0] = fgetc(in_file);
    int prefix_len = 1;
    int offset;
    while (prefix[0] != EOF)
    {
        suffix[0] = fgetc(in_file);
        offset = 0;
        if (suffix[0] == EOF)
        {
            break;
        }
        // after_suffix[0]=fgetc(in_file);
        // offset--;
        memcpy(aux, prefix, prefix_len * sizeof(int));
        memcpy(aux + prefix_len, suffix, sizeof(int));
        int aux_len = prefix_len + 1; // incrementar conforme find_pattern
        prefix[0] = suffix[0];
        add_pattern(aux, aux_len);
        print_dict();

        // for (int i=0; i < 5; i++){ //checkar se o padrao aux existe no dict
        //     if(find_pattern(*aux)==-1)
        //          add_pattern(aux, aux_len);
        //         //adiciona novo pattern
        //         break;
        //     } //se encontrar icrementa ate nao conhecer
        //     after_suffix[0]=fgetc(in_file);
        //     offset--;
        // }

        // seek(in_file, offset, SEEK_CUR); //offset tem de ser dinamico, POR CADA FGETC NO AFTER_SUFFIX DEC O OFFSET
        //  for (int i = 0; i < aux_len ; i++) {
        //      printf("Aux: %d \n", aux[i]);
        //  }

        // char suffix = fgetc(in_file);
        // if (suffix == EOF)
        // {
        //     int code = find_pattern(prefix, prefix_len, -1);
        //     fwrite(&code, sizeof(code), 1, out_file);
        //     break;
        // }
        // prefix[prefix_len] = suffix;
        // int pattern = find_pattern(prefix, prefix_len, suffix);
        // if (pattern != -1)
        // {
        //     memcpy(prefix, dict[pattern].prefix, dict[pattern].length * sizeof(int));
        //     prefix_len = dict[pattern].length;
        // }
        // else
        // {
        //     int code = find_pattern(prefix, prefix_len, -1);
        //     fwrite(&code, sizeof(code), 1, out_file);
        //     if (dict_size >= MAX_DICT_SIZE)
        //     {
        //         fprintf(stderr, "Error: dictionary overflow\n");
        //         exit(1);
        //     }
        //     add_pattern(prefix, prefix_len, suffix);
        //     printf("%d %d \n",prefix[0],suffix);
        //     prefix[0] = suffix;
        //     prefix_len = 1;
        //     printf(" Print %d \n", prefix[0]);
        // }
        // if (dict_size == 512) // check if dictionary size is 512
        // {
        //     init_dict(); // reset the dictionary
        //     prefix[0] = fgetc(in_file);
        //     prefix_len = 1;
        // }
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
    // print_dict();
    // printf("Compression rate: %.2f%%\n", (float)compressed_size / orig_size * 100);

    return 0;
}
