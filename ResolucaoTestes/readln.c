
int readln(int fd, char * buffer, int size){
	int r, i =0; 
	while(r=read(fd, buffer +1,1) && i<size){
		if(buffer[i]='\n'){
			return i+1; 
		}
	}
	return i; 
}