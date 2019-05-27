#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>


int main(int argc, char const* argv[]){

char buf[512], buffFinal[1024]; 
int n; 

mkfifo("ordenar", 0666); 

while(1){
	int fd= open("ordenar", O_RDONLY); 
	if((n=read(fd, buf, sizeof(buf)))<=0){
		break; 
	}
	if(fork()==0){//sou filho
		buf[n]='\0'; 
		sprintf(buffFinal, "%s.sorted", buf);
		int fd1=open(buf, O_RDONLY);
		dup2(fd1,0);
		close(fd1); 
		int fd2= open(buffFinal,  O_CREAT|O_TRUNC|O_WRONLY);
		dup2(fd2,1); 
		close(fd2); 
		execlp("sort","sort", buf, NULL); 
		_exit(1);

	}

	close(fd); 
}

return 0; 

}

//Um utilizador que queira ordenar um ficheiro chamado "nome.txt" presente na diretoria local, deve
// escrever o nome do mesmo para um pipe com o nome "ordenar". 
//V2. Faça um programa que leia repetidamente desse pipe e produza ficheiros ordenados com a extensão adicional 'sorted'. 
// Neste exemplo seria gerado o ficheiro "nome.txt.sorted"

// ------------------------------servidor------------------------------------
void escreveFicheiro (char * nomeFicheiro){
	pid_t x; 
	char * novoNome = (char *) malloc (1024); 
	sprintf(novoNome, "%s.sorted", nomeFicheiro);
	x=fork(); 
	if(x==0){
		execlp("sort","sort","-o", novoNome, nomeFicheiro, NULL); 
		perror("Erro no execlp"); 
		_exit(1);
	}
	else{
		wait(0); 
	}
}

int main(nt argc, char const* argv[]){
	char * nomeFicheiro = (char*) malloc(1024); 
	int ordena; 
	ordena= mkfifo("ordenar", 0666); 
	int ler, lidos; 
	while(1){
		if(ordena<0){
			perror("Erro no pipe"); 
			_exit(1); 
		}
		ler= open("ordenar", O_RDONLY);
		if ((lidos=read(ler, nomeFicheiro, sizeof(nomeFicheiro)))>0){
			escreveFicheiro(nomeFicheiro); 
		}
	}
	close(ler); 
	return 0; 
}

//-------------------------Cliente--------------------------------------------
int main(nt argc, char const* argv[]){
	char * nomeFicheiro; 
	int ler; 
	int lidos; 
	ler = open("ordenar", O_WRONLY); 
	write(ler, argv[1], sizeof(argv[1])); 
	close(ler); 
	return 0; 
}





