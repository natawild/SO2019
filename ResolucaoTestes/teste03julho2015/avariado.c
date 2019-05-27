/*
Considere um programa 'avariado' que por vezes bloqueia durante a execução, sem conseguir realizar uma função 
e sem terminar. Sabemos que, enquanto funciona corretamente o avariado envia o sinal SIGUSR1 ao processo pai
pelo menos uma vez por segundo. Escreva um programa remendo que re(executa) o avariado as vezes necessárias
até que ele conclua com sucesso. Não permita que o 'avariado' fique bloqueado por mais de 5 segundos de cada
vez. 
*/


void handler(int sinal){
	switch (sinal){
		case SIGALARM: 
			tempo++; 
			alarm(1); 
			if (tempo==5){
				kill(pid, SIGKILL); 
				tempo=0; 
			}
			break; 
		case SIGUSR1: 
			tempo=0; 
			alarm(1);
			break; 
		default: 
	}
}


pid_t pid; 
int tempo=0; 
int main(int argc, char const *argv[]){
	int status; 
	signal(SIGALARM, handler); 
	signal(SIGUSR1, handler); 
	do{
		alarm(1); 
		if(pid=fork()==0){
			execlp("avariado", "avariado", NULL); 
			_exit(1); 
		}
		wait(&status); 
	}while(!IFEXITED(status)||WEXITSTATUS(status)); 
	return 0;
}