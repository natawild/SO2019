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