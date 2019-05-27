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
Considere um programa controlador que deverá executar repetidamente um conjunto de programas especificados na sua linha de 
comando, até um destes terminar com o código de daída de 0. Os programas deverão ser todos executados concorrentemente, em 
tentativas sucessivas. Sempre que numa tentativa nenum dos processos terminar como o código de saída de 0, a próxima 
tentativa tem como limite de tempo (em segundos) a soma dos códigos de saída. Findo esse limite de tempo, todos os processos, 
incluindo o controlador, devem ser terminados. A primeira tentaiva não tem limite de tempo. Implemente o programa controlador 
recorrendo às primitivas de processos e sinais estudadas. 
*/

int pid[1024]; 
int npids=0; 


void handler(int sinal){
	for(int i=0; i<npids; i++){
		kill(pid[i],SIGKILL); 
		_exit(1); 
	}
}

int main(int argc, char const *argv[]){
	int i, status, fim=0, contador=0; 
	npids=argc-1; 
	signal(SIGALRM,handler); 
	while(!fim){
		alarm(contador); 
		for(i=0; i<npids;i++){
			if((pid[i]=fork())==0){//sou filho
				execlp(argv[i+1], argv[i+1], NULL); 
				_exit(1); 
			}
		}
		for(i=0; i<npids;i++){
			wait(&status); 
			if(WIFEXITED(status)){
				if(WEXITSTATUS(status)==0){
					fim=0; 
				}
				else{
					contador+=WEXITSTATUS(status); 
				}
			}
		}

	}
	return 0;
}