/*
Considere um programa CRTL que monitoriza a atividade de um conjunto de processos a executar no mesmo computador. 
Cada processo monitorizado escreve, de 3 em 3 segundos "OK" para o seu standard output enquanto se encontra ativo. 
O programa CTRL revebe como argumentos um número arbitrário de programas (CTRL p1 p2  p3 ... pn) que deverá colocar
em execução e monitorizar. Sempre que um processo deixe de estar ativo (não for recebido "OK" durante 3 segundos) o 
processo deve ser termonado e o tempo aproximado em que esteve ativo reporatdo para o standard output. Logo que todos
os processos tenham sido terminados CTRL termina a sua execução. 
*/

void handler(int sinal){
	kill(pid,SIGKILL); 
	_exit(1);
}

int pid; 
int main(int argc, char const *argv[]){

	int i, pd[2], pid,n, iteracoesAlarm=0; 
	char buffer[1024]; 

	for(i=0;i!=argc;i++){
		if(fork()==0){//cria os A
			pipe(pd); 
			if(pid=fork()==0){//Cria os P
				dup2(pd[1],1); 
				close(pd[0]); //fechar a leitura do pipe
				close(pd[1]); //fechar a escrita
				execlp(argv[i+1], argv[i+1], NULL); 
				_exit(1); 
			}
		//Os A leem, por isso fechamos a escrita
		close(pd[1]); 
		signal(SIGCHILD,handler); 
		while(1){
			alarm(3); //adormece até um maximo de 3 segundos 
			n=readln(pd[0],buffer,sizeof(buffer)); 
			if(n<=0){
				kill(pid, SIGKILL);
				printf("O filho com o pid %d executou durante aproximadamente %d segundos", pid, 3*iteracoesAlarm); 
			}
			else{// se tiver coisas escritas validar se é OK
				if(strcmp(buffer,'OK')!=0){
					kill(pid,SIGKILL); 
					printf("O filho com o pid %d executou durante aproximadamente %d segundos", pid, 3*iteracoesAlarm); 
					_exit(1); 
				}
			}
			iteracoesAlarm++; 
		}
		}
	}
	for(i=0;i<argc;i++){
		wait(NULL); 
	}
	return 0;
}