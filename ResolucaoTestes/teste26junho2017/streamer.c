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
standard dos programas p1 a pn. No casp de streamer receber um sinal SIGUSR1, os programas deverão ser
terminados, assegurando que qualquer informção lida por p1 é antes processada e impressa por pn. 
pai lê e filho escreve 
*/

int npids=0; 
int pid[1024]; 


void handler(int sinal){
	for(int i =0; i<npids; i++){
		kill(pid[i],SIGKILL); 
	}
}



int main(int argc, char const *argv[])
{
	int pd[2]; 
	npids=argc-1; 
	signal(SIGUSR1,handler); 
	pipe(pd); 
	for(i=0; i<npids; i++){
		if((pid[i]=fork())==0){
			close(pd[0]); 
			dup2(pd[1],1); 
			execlp(argv[i+1], argv[i+1], NULL); 
			_exit(1); 
		}
	/*pai*/	
	dup2(pd[0],0); 
	close(pd[1]); 
	close(pd[0]); 
	}

	for(i=0;i<npids;i++){
		wait(NULL); 
	}
	return 0;
}