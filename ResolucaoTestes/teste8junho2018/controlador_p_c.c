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
Escreva um programa, invocado na forma controlador p c, para processamento de linha de texto produzidas concorrentemente 
por p processos, instâncias de um programa produtos, por c processos, instâncias de um programa consumidor, ambos 
invocados sem argumentos. Assuma que cada linha produzida começa por um digito d, e faça com que o resto da linha seja 
processada pelo consumidor d, quando 0<=d<c, ou descartada caso contrário. 
*/


int readln(int fd, char *buf, int size){
  int n, i = 0;
  while((n = read(0, buf+i, 1)) && i < size){
    if(buf[i] == '\0')
      return i+1;
    i++;
  }
  return i;
}

int main(int argc, char const *argv[]){

	char buf[1024]; 
	char valor[2]; 
	int p,c,i,n,d; 
	p=atoi(argv[1]); 
	c=atoi(argv[2]);
	int pd[2], cd[c][2]; 
	pipe(pd); 
	for(i=0;i<p;i++){
		if(fork()==0){
			close(pd[0]);
			dup2(pd[1],1); //escrever
			close(pd[1]); 
			execlp("produtor", "produtor", NULL);
			_exit(1); 

		}
	}
	close(pd[1]); 
	for(i=0; i<c; i++){
		pipe(cd[i]); 
		if(fork()==0){
			close(cd[i][1]); //fecha a escrita
			dup2(cd[i][0],0); //redireciona para leitura
			close(cd[i][0]); 
			execlp("consumidor","consumidor", NULL); 
			_exit(1); 
		}
	}
	while((n=readln(pd[0],buf,sizeof(buf)))>0){//lê do pipe para ctrl
		valor[0]=buf[0]; 
		valor[1]='\0'; 
		d=atoi(valor); 
		if(d<0 && d<c){
			write(cd[d][1], buf+1, n-1); //escreve no pipe cd 
		}
	}

	return 0;
}