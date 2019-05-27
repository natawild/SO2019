/*
Dado um arquivo de impressoes digitais previamente recolhidas de várias pessoas, pretende-se identificar de que 
pessoa é uma nova impressão digital. Suponha a exitência de um programa 'matcher', que recebe como argumentos 
os nomes de dois ficheiros das duas impressões digitais a comparar, e escreve no standard output os digitos
de um inteiro, de 0 (não condizem) a 255(correspondecia perfeita). Implemente um programa que recebe como argumentos
os nomes de ficheiros das impressões em arquivo e da nova impressão a comparar, faça comparação concorrente, e escreva
o nome do ficheiro da impressão em arquivo que melhor corresponde (assuma que não há empates). O programa deve 
imprimir imediatamente o resultado e parar todo o processamento em curso caso seja encontrada uma correspondencia 
perfeita. 
*/

int main(int argc, char const *argv[]){

	int i,n,valor,status,ultimo,fd[argc-2][2], indice,melhor=0; 
	char buf[16]; 
	ultimo=argc-1; 
	for(i=0;i<argc-1;i++){
		pipe(fd); 
		if(fork()==0){//se sou filho 
			close(fd[i][0]); 
			dup2(fd[i][1],1); 
			close(fd[i][1]); 
			execlp("matcher","matcher", argv[i+1], argv[ultimo], NULL); 
			_exit(1); 
		}
		close(fd[i][1]); //pai vai ler, fecho a escrita
	}
	for(i=0; i<argc-2; i++){
		wait(NULL); 
	}
	n=read(fd[i][0],buf, sizeof(buf)); 
	buf[n]='\0'; 
	valor=atoi(buf); 
	if(valor>mehor){
		if(valor==255){
			write(1,argv[i+1], strlen(argv[i+1])); 
			_exit(0); 
		}
		else{
			melhor=valor; 
			indice=i+1; 
		}
	}

	write(1,argv[indice], strlen(argv[indice])); 
	return 0;
}