#include<stdio.h> 


//servidor tem de estar a ler de um pipe a palavra enviada pelo cliente

int main(int argc, char const* argv[]){
	int fd,n;
	char buffer[16384];
	char identificador[1024];
	char palavra[7]; 

	mkfifo("servidor", 0666); 

	//o servidor tem de estar sempre à escuta
	while (1){
		fd=open("servidor",O_RDONLY);
		n=readln(fd,identificador,sizeof(identificador)); //temos a palavra enviada pelo cliente 
		identificador[n]='\0'; 
		if(fork()==0){
			strncpy(palavra, identificador, 6);
			n=captcha(palavra, buffer); 
			mkfifo(identificador,0666); 
			fd=open(identificador,O_CREAT|O_WRONLY); 
			//escrever 
			write(fd,buffer,n); 
			//fechar 
			close(fd); 
			_exit(1); 
		}
	}
	close(fd); 
	return 0; 
}


void create_captcha_file(const char* palavra){
	char buffer[16384];
	char identificador[6+sizeof(getpid())+1]; 
	strncpy(identificador, palavra+getpid(),sizeof(identificador)-1); 

	mkfifo("servidor", 0666); 
	int fd= open("servidor",O_WRONLY|O_CREAT); 
	write(fd,identificador, sizeof(identificador)); 
	close(fd); 
	mkfifo(identificador,0666); 
	fd=open(identificador,O_RDONLY); 
	int n= readln(fd, buffer, sizeof(buffer)); 
	close(fd); 
	char nome[512]; 
	snprintf(nome,6,"%s.png",palavra); 
	fd=open(nome,O_CREAT|O_WRONLY); 
	write(fd,buffer,n); 
	close(fd); 

}

