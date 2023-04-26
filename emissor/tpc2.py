from ctypes import sizeof

from turtle import delay

import numpy

import random


ficheiro = "logo.txt"
destinoWith = "Primeiro.png"

destinoWithout = "Segundo.png"


maximum = 100

limit = 0  # (0 - maximum) Bit Error Rate


# Estatisticas

detectedErrors = 0

notDetectedErros = 0

notCorrectedErrors = 0

errorBits = 0

counter = 0

nBytes = 0

nBits = 0


def geradorBinario():

    global nBits

    global nBytes

    ficheiro = "logo.png"

    Bytes = numpy.fromfile(ficheiro, dtype="uint8")

    nBytes = len(Bytes.tolist())

    Bits = numpy.unpackbits(Bytes)

    nBits = len(Bits.tolist())

    return Bits


def codificador(m):

    g = [[1, 1, 0, 1, 0, 0, 0], [0, 1, 1, 0, 1, 0, 0],
         [1, 1, 1, 0, 0, 1, 0], [1, 0, 1, 0, 0, 0, 1]]

    c = g

    for i in range(0, 4):

        if m[i] == 0:

            c[i] = [0, 0, 0, 0, 0, 0, 0]

        else:

            c[i] = g[i]

    code = [0, 0, 0, 0, 0, 0, 0]

    for i in range(0, 7):

        code[i] = c[0][i] ^ c[1][i] ^ c[2][i] ^ c[3][i]

    return code


def canal(code):

    res = [0, 0, 0, 0, 0, 0, 0]

    global errorBits

    global counter

    for i in range(0, 7):

        counter = counter + 1

        num = random.randint(1, maximum)

        if num > limit:

            e = 0

        else:

            errorBits = errorBits + 1

            e = 1

        res[i] = code[i] ^ e

    return res


def descodificador(code):

    h = numpy.array(
        [[1, 0, 0, 1, 0, 1, 1], [0, 1, 0, 1, 1, 1, 0], [0, 0, 1, 0, 1, 1, 1]])

    hT = h.transpose().tolist()

    c = hT

    for i in range(0, 7):

        if code[i] == 0:

            c[i] = [0, 0, 0]

        else:

            c[i] = hT[i]

    res = [0, 0, 0]

    for i in range(0, 3):

        res[i] = c[0][i] ^ c[1][i] ^ c[2][i] ^ c[3][i] ^ c[4][i] ^ c[5][i] ^ c[6][i]

    return res


def corrector():

    e = [0, 0, 0, 0, 0, 0, 1]

    my_dict[str(descodificador(e))] = e

    e = [0, 0, 0, 0, 0, 1, 0]

    my_dict[str(descodificador(e))] = e

    e = [0, 0, 0, 0, 1, 0, 0]

    my_dict[str(descodificador(e))] = e

    e = [0, 0, 0, 1, 0, 0, 0]

    my_dict[str(descodificador(e))] = e

    e = [0, 0, 1, 0, 0, 0, 0]

    my_dict[str(descodificador(e))] = e

    e = [0, 1, 0, 0, 0, 0, 0]

    my_dict[str(descodificador(e))] = e

    e = [1, 0, 0, 0, 0, 0, 0]

    my_dict[str(descodificador(e))] = e


my_dict = {}

corrector()


bits = geradorBinario()

bits = bits.tolist()


n = 4

output = [bits[i:i + n] for i in range(0, len(bits), n)]

bitsWith = []

bitsWithout = []

for msg in output:

    code = codificador(msg)

    codeCod = code

    code = canal(code)

    codeCha = code

    s = descodificador(code)

    bitsWithout = numpy.append(bitsWithout, code[3:])

    if s != [0, 0, 0]:

        detectedErrors = detectedErrors + 1

        e = my_dict[str(s)]

        for i in range(0, 7):

            code[i] = e[i] ^ code[i]

        if code != codeCod:

            notCorrectedErrors = notCorrectedErrors + 1

    elif codeCha != codeCod:

        notDetectedErros = notDetectedErros + 1

    bitsWith = numpy.append(bitsWith, code[3:])


bitsWith = numpy.round(bitsWith.transpose()).astype(int)


bytesWith = numpy.packbits(bitsWith)


bytesWith = bytesWith.tolist()


with open(destinoWith, "wb") as f:

    f.write(bytes(bytesWith))


bitsWithout = numpy.round(bitsWithout.transpose()).astype(int)


bytesWithout = numpy.packbits(bitsWithout)


bytesWithout = bytesWithout.tolist()


with open(destinoWithout, "wb") as f:

    f.write(bytes(bytesWithout))


print("--//--")

print("Taxa de Erro ---------> (", limit, "/ 100 )")

print("--//--")

print("Bits no Ficheiro ----->", nBits)

print("Bits transmitidos ---->", counter)

print("Bits Errados --------->", errorBits)

print("--//--")

print("Bytes no Ficheiro ---->", nBytes)

print("--//--")

print("Erros detetados ------>", detectedErrors)

print("Erros não corrigidos ->", notCorrectedErrors)

print("Erros não detetados -->", notDetectedErros)
