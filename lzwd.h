#define DICTIONARY_SIZE 4096
#define DICTIONARY_LENGTH 10

#ifndef HEADER_FILE
#define HEADER_FILE

struct timeval start, end;

int checkIfExistsInDictionary(char *cod, int size_cod, int dictionary[DICTIONARY_SIZE][DICTIONARY_LENGTH])
{
    int line = -1;
    int t = 1;
    // printf("COD-> %s  , SIZE_COD -> %d\n",cod,size_cod);

    for (int i = 0; i < DICTIONARY_SIZE; i++)
    {
        if (*(cod + t - 1) == dictionary[i][t])
        {
            t++;
            for (int j = t; j < size_cod + 1; j++)
            {
                if (*(cod + j - 1) == dictionary[i][j])
                {
                    t++;
                }
            }
            if (t == size_cod + 1 && dictionary[i][t] == -1)
            {
                line = i;
                break;
            }
            t = 1;
        }
    }
    return line;
}

void lzwd(FILE *inputFile, const char *name, int type)
{

    unsigned long block_size = 65535; // Tamanho dos blocos
    int dictionary[DICTIONARY_SIZE][DICTIONARY_LENGTH];
    for (int i = 0; i < DICTIONARY_SIZE; i++)
    { // Colocar os primeiros 256 símbolos no dicionário e o resto a -1
        for (int j = 0; j < DICTIONARY_LENGTH; j++)
        {
            if (i < 256 && (j == 0 || j == 1))
            {
                dictionary[i][j] = i;
            }
            else
            {
                dictionary[i][j] = -1;
            }
        }
    }
    unsigned char *temp_buffer; // Buffers com conteúdo do ficheiro
    unsigned long lastBlockSize = 0;

    size_t read = 0;
    int n_blocks = 0;
    temp_buffer = malloc(sizeof(unsigned char) * block_size);
    unsigned char *pj;
    unsigned char *pk;
    int pm[256];
    pj = malloc(sizeof(unsigned char) * 64); //  Variável Pj para guardar primeiro padrão
    pk = malloc(sizeof(unsigned char) * 64); //  Variável Pk para guardar segundo padrão
    int tries4pk = 0;
    int tries4pm = 0;
    int pointerDict = 256;

    FILE *outputFile;
    outputFile = fopen("output.txt", "wb");
    unsigned long outputFileSize = 0;

    double secs = 0;
    gettimeofday(&start, NULL);
    while (!feof(inputFile))
    {
        n_blocks++;
        read = fread(temp_buffer, sizeof(unsigned char), block_size, inputFile); // Leitura do ficheiro por blocos para memória
        unsigned long posicao_no_buffer = 0;                                     // Processamento de blocos normais
        short arrayToOutput[block_size];
        int n_simbolos = 0;
        while (posicao_no_buffer < read - 1)
        {
            for (size_t triesPj = 0; triesPj < 64; triesPj++)
            {
                *(pj + triesPj) = *(temp_buffer + posicao_no_buffer + triesPj);
                printf("PJ -> %s\n", pj);
                int line = checkIfExistsInDictionary(pj, triesPj + 1, dictionary);
                if (line == -1)
                {
                    posicao_no_buffer = posicao_no_buffer + triesPj;
                    for (int i = 0; i < triesPj; i++)
                    {
                        pm[i] = *(pj + i);
                    }
                    tries4pk = triesPj;
                    triesPj = 64;
                }
                else
                {
                    arrayToOutput[n_simbolos] = line;
                }
            }
            n_simbolos++;
            printf("Posição PJ : %ld\n", posicao_no_buffer);
            // sleep(2);

            if (type == 1)
            { // LZW
                *(pk) = *(temp_buffer + posicao_no_buffer);
                // printf("PK -> %s\n", pk);
                //  posicao_no_buffer = posicao_no_buffer + triesPk;
                tries4pm = tries4pk;
                pm[tries4pm] = *(pk);
                /*for (size_t i = 0; i < tries4pm + 1; i++)
                {
                    printf("%d ",pm[i]);
                }
                printf("\n");*/
            }
            else if (type == 2)
            { // LZWD
                for (size_t triesPk = 0; triesPk < 64; triesPk++)
                {
                    *(pk + triesPk) = *(temp_buffer + posicao_no_buffer + triesPk);
                    printf("PK -> %s\n", pk);
                    int line = checkIfExistsInDictionary(pk, triesPk + 1, dictionary);
                    if (line == -1)
                    {
                        // posicao_no_buffer = posicao_no_buffer + triesPk;
                        tries4pm = tries4pk + triesPk;
                        for (int i = tries4pk; i < tries4pm; i++)
                        {
                            pm[i] = *(pk + i - tries4pk);
                        }
                        triesPk = 64;
                    }
                }
            }

            if (posicao_no_buffer < read)
            {
                if (pointerDict > DICTIONARY_SIZE - 1)
                {
                    for (int i = 0; i < DICTIONARY_SIZE; i++)
                    { // Colocar os primeiros 256 símbolos no dicionário e o resto a -1
                        for (int j = 0; j < DICTIONARY_LENGTH; j++)
                        {
                            if (i < 256 && (j == 0 || j == 1))
                            {
                                dictionary[i][j] = i;
                            }
                            else
                            {
                                dictionary[i][j] = -1;
                            }
                        }
                    }
                    pointerDict = 256;
                }
                dictionary[pointerDict][0] = pointerDict;
                if (type == 1)
                {
                    for (int i = 0; i < tries4pm + 1; i++)
                    {
                        dictionary[pointerDict][i + 1] = pm[i];
                    }
                }
                else if (type == 2)
                {
                    for (int i = 0; i < tries4pm; i++)
                    {
                        dictionary[pointerDict][i + 1] = pm[i];
                    }
                }
                // printf("\n");
                pointerDict++;
            }

            // sleep(2);
            memset(pj, 0, 64);
            memset(pk, 0, 64);
            if (read != block_size)
            {
                lastBlockSize = read;
            }
        }
        for (size_t i = 0; i < n_simbolos; i++)
        {
            const void *ptr = &arrayToOutput[i];
            fwrite(ptr, 2, 1, outputFile);
            outputFileSize = outputFileSize + 2;
        }
        memset(arrayToOutput, 0, block_size);
        n_simbolos = 0;
    }
    gettimeofday(&end, NULL);
    secs = (double)(end.tv_usec - start.tv_usec) / 1000000 + (double)(end.tv_sec - start.tv_sec);
    unsigned long inputFileSize = ((n_blocks - 1) * block_size) + lastBlockSize;
    printf("***********************************************************************\n");
    printf("Authors: - Rui Freitas, pg47639@alunos.uminho.pt\n");
    printf("         - Tiago Ferreira, pg47692@alunos.uminho.pt\n");
    printf("***********************************************************************\n");
    if (type == 1)
    {
        printf("******************************* LZW ***********************************\n***********************************************************************\n");
    }
    else if (type == 2)
    {
        printf("******************************* LZWD ***********************************\n***********************************************************************\n");
    }
    int hours, minutes, seconds, day, month, year;
    time_t now;
    time(&now);
    printf("Date: %s", ctime(&now));
    printf("Input file: %s with %ld bytes\n", name, inputFileSize);
    printf("Output file: output.txt with %ld bytes\n", outputFileSize);
    printf("%d blocks with %ld bytes and last block with %ld bytes\n", n_blocks - 1, block_size, lastBlockSize);
    float compression_rate = (((float)inputFileSize - (float)outputFileSize) / (float)inputFileSize) * 100;
    printf("Compression rate: %.2f %c\n", compression_rate, '%');
    printf("Execution time: %.2f seconds\n", secs);
    printf("***********************************************************************\n");
    free(temp_buffer);
    fclose(outputFile);
    fclose(inputFile);

    /*for(int i=0;i<300;i++){
        printf("%d   %d     %d      %d      %d\n",dictionary[i][0],dictionary[i][1],dictionary[i][2],dictionary[i][3],dictionary[i][4]);
    }*/
}