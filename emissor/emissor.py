from ctypes import sizeof
import time
from turtle import delay
import numpy
import random
import tempfile

def geradorBinario():
    Bytes = numpy.fromfile("logo.png", dtype = "uint8")
    print(len(Bytes.tolist()))
    Bits = numpy.unpackbits(Bytes)
    # print(Bytes[0],Bytes[1])
    # print(Bits[0],Bits[1])
    # Bytes = numpy.packbits(Bits)
    # print(Bytes[0],Bytes[1])
    # numpy.ndarray.tofile(Bytes,file = "xis.jpg")
    return Bits

def codificador(m):
    g = [[1,1,0,1,0,0,0],[0,1,1,0,1,0,0],[1,1,1,0,0,1,0],[1,0,1,0,0,0,1]]
    c = g
    for i in range(0,4):
        if m[i] == 0:
            c[i] = [0,0,0,0,0,0,0]
        else:
            c[i] = g[i]

    code = [0,0,0,0,0,0,0]
    #print(c)
    for i in range(0,7):
        code[i] = c[0][i] ^ c[1][i] ^ c[2][i] ^ c[3][i]
    #print(code)
    return code

def canal(code):
    res = [0,0,0,0,0,0,0]
    for i in range(0,7):
        num = random.randint(1,100)
        if num > limit:
            e = 0
        else:
            e = 1
        res[i] = code[i] ^ e

    #print(res)
    return res

def descodificador(code):
    h = numpy.array([[1,0,0,1,0,1,1],[0,1,0,1,1,1,0],[0,0,1,0,1,1,1]])
    hT = h.transpose().tolist()
    #print(hT)
    c = hT
    for i in range(0,7):
        if code[i] == 0:
            c[i] = [0,0,0]
        else:
            c[i] = hT[i]

    res = [0,0,0]
    #print(c)
    for i in range(0,3):
        res[i] = c[0][i] ^ c[1][i] ^ c[2][i] ^ c[3][i] ^ c[4][i] ^ c[5][i] ^ c[6][i]
    #print(res)
    return res

def corrector():
    e = [0,0,0,0,0,0,1]
    my_dict[str(descodificador(e))] = e 
    e = [0,0,0,0,0,1,0]
    my_dict[str(descodificador(e))] = e 
    e = [0,0,0,0,1,0,0]
    my_dict[str(descodificador(e))] = e 
    e = [0,0,0,1,0,0,0]
    my_dict[str(descodificador(e))] = e 
    e = [0,0,1,0,0,0,0]
    my_dict[str(descodificador(e))] = e 
    e = [0,1,0,0,0,0,0]
    my_dict[str(descodificador(e))] = e 
    e = [1,0,0,0,0,0,0]
    my_dict[str(descodificador(e))] = e 


my_dict = {}
# corrector()
# limit = 10
# code = codificador()
# code = canal(code)
# s = descodificador(code)
 
# if s != [0,0,0]:
#     e = my_dict[str(s)]
#     for i in range(0,7):
#         code[i] = e[i] ^ code[i]

# print(code)


limit = 1
corrector()
bits = geradorBinario()
print(bits)
bits = bits.tolist()
berbit=bits

# bits = [1,0,0,1,0,1,0,1,1,1,0,1,0,0,0,1]
# print(bits)
n = 4
output=[bits[i:i + n] for i in range(0, len(bits), n)]
bits2 = []
for msg in output:
    code = codificador(msg)
    #print("message = ",msg)
    codeCod = code
    code = canal(code)
    codeCha = code
    s = descodificador(code)
    
    if s != [0,0,0]:
        e = my_dict[str(s)]
        print("Entrei!")
        
        if codeCod != codeCha:
            print("Entrei 2!")

        for i in range(0,7):
            code[i] = e[i] ^ code[i]
        
        if code != codeCod:
            print("EI! -> ",codeCod,codeCha)
    
    #print("final   = ",code[3:])
    #print()

    #time.sleep(0.5)
    bits2 = numpy.append(bits2,code[3:])
    #print("bits ->",bits2)


bits2 = numpy.round(bits2.transpose()).astype(int)
#print(bits2.tolist())

bytes2 = numpy.packbits(bits2)

bytes2 = bytes2.tolist()

print(len(berbit))
    

with open("Primeiro.png", "wb") as f:
    f.write(bytes(bytes2))

#numpy.ndarray.tofile(bytes2,file = "xis.png")