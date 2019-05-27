#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>
#include <fcntl.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>
#include <signal.h>

/*
Escreva um programa, invocado da forma controlador <p> <c>, para processamento de linhas de texto
produzidas concorrentemente por p processos, instâncias de um produtor, por um de c processos, instâncias
de um programa consumidor. Em cada momento, todas as linhas produzidas devem ser processadas por um mesmo
consimidor. Com o tempo deve ser feita uma escolha rotativa do consumidor corrente, devendo ser escolhido o consumidor seguinte q
quando o processo controlador receber o sinal SIGUSR1. 
*/

int atual=0, c;

//rotativo: 
void handler(int sinal){
	atual=(atual+1)%c; 
}

int main(int argc, char const *argv[]){
	char * buf = (char*) malloc (sizeof(buf)); 
	int i, pd[2], p,c,j; 
	p=atoi(argv[1]); 
	c=atoi(argv[2]); 
	int cd[c][2]; //consumidor 
	pipe(pd); 
	for(i=0;i<p;i++){//produtor
		if(fork()==0){
			close(pd[0]); 
			dup2(pd[1],1); 
			close(pd[1]); 
			execlp("produtor", "produtor", NULL); 
			_exit(1); 
		}
	}
	close(pd[1]); 
	for(j=0; j<c; j++){
		pipe(cd[j]); //tantos pipes quantos consumidores
		if(fork()==0){
			close(cd[j][1]); //fecho a escrita
			dup2(cd[j][0]);
			execlp("consumidor", "consumidor", NULL); 
			_exit(1); 
		}
	close(cd[j][0]); // fechar a leitura pq pai vai escrever  
	}

	signal(SIGUSR1, handler); 
	while((n=readln(pd[0]), buf, sizeof(buf))>0){
		write(cd[atual][1], buf,n); 

	}

	return 0;
} 