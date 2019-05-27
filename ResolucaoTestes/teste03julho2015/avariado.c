#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>
#include <fcntl.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <string.h>

/*
Considere um programa 'avariado' que por vezes bloqueia durante a execução, sem conseguir realizar uma função 
e sem terminar. Sabemos que, enquanto funciona corretamente o avariado envia o sinal SIGUSR1 ao processo pai
pelo menos uma vez por segundo. Escreva um programa remendo que re(executa) o avariado as vezes necessárias
até que ele conclua com sucesso. Não permita que o 'avariado' fique bloqueado por mais de 5 segundos de cada
vez. 
*/


pid_t pid; 
void handler(int sinal){
	switch (sinal){
		case SIGALRM: 
			kill(pid, SIGKILL); 
			break; 
		case SIGUSR1: 
			alarm(5);
			break; 
		default:
			break;  
	}
}


int main(int argc, char const *argv[]){
	int status; 
	signal(SIGALRM, handler); 
	signal(SIGUSR1, handler); 
	do{
		alarm(1); 
		if((pid=fork())==0){
			execlp("avariado","avariado",NULL); 
			_exit(1); 
		}
		wait(&status); 
	}while(!WIFEXITED(status)||!WEXITSTATUS(status)); 
	return 0;
}