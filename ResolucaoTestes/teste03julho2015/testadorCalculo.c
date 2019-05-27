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
Testar um programa calculo (ja existente) que repetidamente lê uma linha do standad input e produz outra
no standar output. Implemente um programa testador que deverá receber no seu standard input numa sequencia
de pares de strings (input e output esperado). 
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

int main(int argc, char const* argv[]){
	int pid, n1, n2,n3; 
	int pd1[2], pd2[2]; 
	char buf1[512], buf2[512], buf3[512]; 
	//criação de 2 pipes
	pipe(pd1); 
	pipe(pd2);

	while(1){
		pipe(pd1); 
		pipe(pd2);
		if((pid=fork())==0){
			dup2(pd1[0],0); 
			dup2(pd2[1],1);
			close(pd1[1]); 
			close(pd1[0]); 
			close(pd2[1]);
			close(pd2[0]); 
			execlp("calculo", "calculo", NULL); 
			_exit(1); 
		}
		close(pd1[0]);
		close(pd2[1]); 

		while((n1= readln(0, buf1, sizeof(buf1))>0)&&(n2=readln(0,buf2,sizeof(buf2))>0)){
			write(pd1[1], buf1, n1); 
			n3= readln(pd2[0], buf3, sizeof(buf3)); //lê do pipe resultado 
			if(n3<=0){
				printf("Nao tem resultado\n"); 
				break;
			}
			else {
				buf2[n2]='\0'; //resultado a verificar 
				buf3[n3]='\0'; // resultado verdadeiro
				if(strcmp(buf2,buf3)==0){
					printf("resultado certo\n"); 
					break;
				}
				else {
					printf("resultado errado\n"); 
					break;
				}
			}
		}

		close(pd1[1]); 
		close(pd2[0]); 
	}

	
	return 0; 


}