/* #include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdbool.h>

#define BUFFER 1000
#define MAX_STRING_12 (1 << 12) // valor máximo para 12 bits
#define MAX_STRING_16 (1 << 16) // valor máximo para 16 bits
#define FIRST_VALUE 256

int output(unsigned char *out, int out_ind, uint16_t value) {
    out[out_ind] = value >> 8;
    out_ind++;
    out[out_ind] = value;
    out_ind++;
    return out_ind;
}

int lzwd(unsigned char *in, unsigned char *out, int size_in) {
    unsigned char *pj = NULL;
a    unsigned char *pk = NULL;
    unsigned char *pm = NULL;
    unsigned char *temp = NULL;

    int inindex = 0;
    int outindex = 0;
    bool incremento = false;

    uint16_t value = FIRST_VALUE;

    unsigned char dictionary[MAX_STRING_16][256];
    int dictSize = 0;

    while (inindex < size_in) {
        if (pj == NULL) {
            pj = &in[inindex];
            inindex++;
        }

        pk = temp = &in[inindex];
        inindex++;

        while (dictSize > 0 && strncmp((char*)temp, (char*)dictionary[dictSize-1], strlen((char*)temp)) == 0 && inindex < size_in) {
            pk = temp;
            int pk_len = strlen((char*)pk);
            temp = (unsigned char*)malloc(pk_len + 2);
            strncpy((char*)temp, (char*)pk, pk_len);
            temp[pk_len] = in[inindex];
            temp[pk_len + 1] = '\0';
            inindex++;
            incremento = true;
        }

        if (incremento) {
            incremento = false;
            inindex--;
        }

        pm = (unsigned char*)malloc(strlen((char*)pj) + strlen((char*)pk) + 1);
        strcpy((char*)pm, (char*)pj);
        strcat((char*)pm, (char*)pk);

        strcpy((char*)dictionary[dictSize], (char*)pm);
        dictSize++;
        value++;

        outindex = output(out, outindex, dictionary[dictSize-2][0] * 256 + dictionary[dictSize-2][1]);

        if (value == MAX_STRING_16) {
            dictSize = 0;
            value = FIRST_VALUE;
            free(pj);
            free(pk);
        }

        free(pj);
        pj = pk;
        pk = NULL;
        free(temp);
        temp = NULL;
    }

    for (int i = 0; i < dictSize; i++) {
        printf("%d: %s\n", dictionary[i][0] * 256 + dictionary[i][1], dictionary[i]);
    }

    return outindex;
}

int main(int argc, char const *argv[]) {
    int fd_in = open("input.txt", O_RDONLY);
    int fd_out = open("output.txt", O_WRONLY);

    printf("Trabalho realizado por: Ines Marques e Miguel Moreira\n");
    printf("Ficheiro de entrada: test.file\nFicheiro de saída: lzwd.lzwd\n\n");

    unsigned char buffer[BUFFER], compressed_buffer[BUFFER];
    int size_of_buffer = 0, size_of_compressed_buffer = 0;

    while ((size_of_buffer = read(fd_in, buffer, BUFFER))) {
        size_of_compressed_buffer = lzwd(buffer, compressed_buffer, size_of_buffer);
        write(fd_out, compressed_buffer, size_of_compressed_buffer);
    }

    return 0;
} */

#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdbool.h>

#define BUFFER 1000
#define MAX_STRING_12 (1 << 12) // valor máximo para 12 bits
#define MAX_STRING_16 (1 << 16) // valor máximo para 16 bits
#define FIRST_VALUE 256

int output(unsigned char *out, int out_ind, uint16_t value) {
    out[out_ind] = value >> 8;
    out_ind++;
    out[out_ind] = value;
    out_ind++;
    return out_ind;
}

int lzwd(unsigned char *in, unsigned char *out, int size_in) {
    unsigned char *pj = NULL;
    unsigned char *pk = NULL;
    unsigned char *pm = NULL;
    unsigned char *temp = NULL;

    int inindex = 0;
    int outindex = 0;
    bool incremento = false;

    uint16_t value = FIRST_VALUE;

    unsigned char dictionary[MAX_STRING_16][256];
    int dictSize = 0;

    while (inindex < size_in) {
        if (pj == NULL) {
            pj = &in[inindex];
            inindex++;
        }

        pk = temp = &in[inindex];
        inindex++;

        while (dictSize > 0 && strncmp((char*)temp, (char*)dictionary[dictSize-1], strlen((char*)temp)) == 0 && inindex < size_in) {
            pk = temp;
            int pk_len = strlen((char*)pk);
            temp = (unsigned char*)malloc(pk_len + 2);
            strncpy((char*)temp, (char*)pk, pk_len);
            temp[pk_len] = in[inindex];
            temp[pk_len + 1] = '\0';
            inindex++;
            incremento = true;
        }

        if (incremento) {
            incremento = false;
            inindex--;
        }

        pm = (unsigned char*)malloc(strlen((char*)pj) + strlen((char*)pk) + 1);
        strcpy((char*)pm, (char*)pj);
        strcat((char*)pm, (char*)pk);

        strcpy((char*)dictionary[dictSize], (char*)pm);
        dictSize++;
        value++;

        outindex = output(out, outindex, dictionary[dictSize-2][0] * 256 + dictionary[dictSize-2][1]);

        if (value == MAX_STRING_16 || dictSize == MAX_STRING_16) { // Verificar se o dicionário atingiu o tamanho máximo
            dictSize = 0;
            value = FIRST_VALUE;
            free(pj);
            free(pk);
        }

        free(pj);
        pj = pk;
        pk = NULL;
        free(temp);
        temp = NULL;
        free(pm);
    }

    for (int i = 0; i < dictSize; i++) {
        printf("%d: %s\n", dictionary[i][0] * 256 + dictionary[i][1], dictionary[i]);
    }

    return outindex;
}

int main(int argc, char const *argv[]) {
    int fd_in = open("input.txt", O_RDONLY);
    int fd_out = open("output.txt", O_WRONLY);

    printf("Trabalho realizado por: Ines Marques e Miguel Moreira\n");
    printf("Ficheiro de entrada: input.txt\nFicheiro de saída: output.txt\n\n");

    unsigned char buffer[BUFFER], compressed_buffer[BUFFER];
    int size_of_buffer = 0, size_of_compressed_buffer = 0;

    while ((size_of_buffer = read(fd_in, buffer, BUFFER))) {
        size_of_compressed_buffer = lzwd(buffer, compressed_buffer, size_of_buffer);
        write(fd_out, compressed_buffer, size_of_compressed_buffer);
    }

    return 0;
}
