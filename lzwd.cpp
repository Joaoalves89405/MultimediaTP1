#include <unistd.h>
#include <fcntl.h>

#include <iostream> 
#include <string>
#include <unordered_map>

#define BUFFER		1000
#define MAX_STRING_12	1<<12	//valor máximo para 12 bits
#define MAX_STRING_16	1<<16	//valor maximo para 16 bits
#define FIRST_VALUE	256




int output(unsigned char *out, int out_ind, uint16_t value){
	out[out_ind] = value >> 8;
	out_ind++;
	out[out_ind] = value;
	out_ind++;
	return out_ind;	
}



int lzwd(unsigned char *in, unsigned char *out, int size_in){

	using namespace std;

	unordered_map<string, uint16_t> dicionario;
	string pj, pk, pm, temp;

	int inindex = 0;
	int outindex = 0;
	bool incremento = false;

	int value = FIRST_VALUE;

	while(inindex < size_in){	     
	     if(pj.empty()){
	    	pj = in[inindex];
		  	inindex++;
	     }


	     pk = temp = in[inindex];
	     inindex++;

	     while(dicionario.find(temp) != dicionario.end() && inindex < size_in){
	          pk = temp;
	          temp += in[inindex];
	          inindex++;
		  incremento = true;	  
	     }

	     if(incremento){
	          incremento = false;
		  inindex--;
	     }

	     pm = pj + pk;

	     dicionario[pm] = value;
	     value++;

	     outindex = output(out, outindex, dicionario[pj]);

	     if(value == MAX_STRING_16){
	          dicionario.clear();
		  value = FIRST_VALUE;
		  pj.clear();
		  pk.clear();
	     }

	     pj = pk;
	     pk.clear();
	     temp.clear();
	
	}

	for(auto i = dicionario.begin(); i != dicionario.end(); i++){
	     cout << i->second << ": " << i->first << endl;
	}

	return outindex;

}


int main(int argc, char const *argv[]){
	int fd_in = open("test.file", O_RDONLY);
	int fd_out = open("lzwd.lzwd", O_WRONLY);

	printf("Trabalho realizado por: Ines Marques e Miguel Moreira\n");
	printf("Ficheiro de entrada: test.file\nFicheiro de saída: lzwd.lzwd\n\n");

	unsigned char buffer[BUFFER], compressed_buffer[BUFFER];
	int size_of_buffer = 0, size_of_compressed_buffer = 0;

	while(size_of_buffer = read(fd_in, buffer, BUFFER)){
		size_of_compressed_buffer = lzwd(buffer, compressed_buffer, size_of_buffer);
		write(fd_out, compressed_buffer, size_of_compressed_buffer);
	}

	return 0; 
}

