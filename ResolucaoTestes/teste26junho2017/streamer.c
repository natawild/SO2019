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
Escreva um programa invocado da forma streamer <p1> <p2> ... <pn>, que encadeia as saídas e entradas
standard dos programas p1 a pn. No caso de streamer receber um sinal SIGUSR1, os programas deverão ser
terminados, assegurando que qualquer informação lida por p1 é antes processada e impressa por pn. 
--pai lê e filho escreve--  
*/

int npids=0; 
int pid[1024]; 


void handler(int sinal){
	for(int i =0; i<npids; i++){
		kill(pid[i],SIGKILL); 
	}
}


int main(int argc, char const *argv[]){
	npids = argc-1; 
	int pd[npids][2], i, exitVal; 
	signal(SIGUSR1, handler); 


	for(i=0; i < npids -1; i++){
		pipe(pd[i]); 
		if((pid[i]=fork()) == 0){
			dup2(pd[i][0], 0); 
			dup2(pd[i++][1], 1); 
			close(pd[0]); 
			close(pd[1]); 
			execlp(argv[i+1], argv[i+1], NULL); 
			_exit(i); 
		}

		wait(&status); 
	}

	int pn; 
	if(WIFEXITED(status)){
		pn=WEXITSTATUS(status); 
		n= read(pd[pn+1][0]; buff, sizeof(buff))
		write(1, buff, n);
	}

	for(i=0;i<npids;i++){
		close(pd[i][1]); 
		close(pd[i][0]); 
	}

	return 0;
}