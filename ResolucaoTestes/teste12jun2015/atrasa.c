#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <string.h>

/*
ssize_t readln(int fildes, void *buf, size_t nbyte){
	int r,bytesRead=0;
	while (r=read(fildes,buf+bytesRead,1) && ((char*)buf)[bytesRead]!='\n' && bytesRead<nbyte) {
		bytesRead++;
	}
	if(((char*)buf)[bytesRead]=='\n') bytesRead++;
	return bytesRead;
}
*/


ssize_t readln(int fildes, void *buf, size_t nbyte) {

	size_t i = 0;
	ssize_t n = 1;
	char c = ' ';
	
	/* Enquanto está a ler algo mas que seja menos de nbyte caracteres, e não seja o '\n' */
	while((i < nbyte) && (n > 0) && (c != '\n')) {
		//Lê um caractere
		n = read(fildes, &c, 1);
		// Se não for o '\n' adiciona-o ao buffer
		if(n && (c != '\n')) {
			((char*) buf)[i] = c;
			i++;
		}
	}

	// Adição de EOF == 0 com verificação no caso de chegar ao limite de leitura (N);
	if(i < nbyte)
		((char*) buf)[i] = 0;
	else {
		// passou o limite (i == 100). buf[99] = EOF.
		i--;
		((char*) buf)[i] = 0;
	}

	// se deu erro na leitura retorna esse mesmo erro
	if(n < 0)
		return n;
	// no caso de apanhar a linha só com o '\n'
	if((n == 0) && (i == 0))
		return (-1);
	return i;
}



int atraso;

void aumenta (int signum){
	atraso++;
}

void diminui (int signum){
	if(atraso>0){
		atraso--;
	}
}

void vazio (int signum){

}


int main (int argc, char* argv[]) {
	char buf[1024]; 
	int n; 
	atraso = atoi(argv[1]);

	signal(SIGALRM, vazio);
	signal(SIGUSR1, aumenta);
	signal(SIGUSR2, diminui);


	char *linha;
	int jaPassou=0; 
	while((n=readln(0,buf,sizeof(buf)))>0){
		if(buf[n]=='\n'){
			if(jaPassou==1){
				char *linhaAux=malloc(sizeof(linha+n)); 
				strcpy(linhaAux,linha); 
				strcat(linhaAux,buf); 
				linha=linhaAux;
			}
			else{
				linha=buf; 
			}

			alarm(atraso); 
			pause(); 
			write(1,linha,sizeof(linha)); 
			jaPassou=0;
		}
		else {//se não é \n 
			char *linhaAux=malloc(sizeof(linha+n)); 
			strcpy(linhaAux,linha); 
			strcat(linhaAux,buf); 
			linha=linhaAux;
			jaPassou=1; 
		}
	}
	return 0; 

}


//Versão 2 -- menos correta, porque não lê linha a linha 

#define MAX_SIZE 512

int tempo; 

int main(int argc, char * argv){
	int tamanho; 
	char buffer[512]; 
	tempo = atoi(argv[1]); 

	signal(SIGUSR1, aumenta); 
	signal(SIGUSR2, diminui); 
	signal(SIGALRM, vazio); 

	while(tamanho=read(0,buffer,MAX_SIZE)){
		buffer[tamanho]=0;
		alarm(tempo);
		pause();
		write(1, buffer, tamanho); 
	}

	return 0; 
}






	