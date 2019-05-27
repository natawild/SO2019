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
Pretende-se fazer uma pesquisa concorrente para determinar a exitencia de determinado padrão 
em linhas de texto, recebidas no stdin. Assuma a existencia de dois programas 'flitro' e 'existe' que 
enacadeados, permitem fazer a pesquisa. O primeiro descarta oas casos mais óbvios que lhe chegam ao seu stdin, deixando passar os 
restantes para o seu stout. O segundo termina com o valor de exit 1 caso receba uma linha com o padrão no seu stdin, ou com 0, caso isso 
não aconteça, até EOF. Escreva um programa que receba um número n como argumento, e faça a pesquisa concorrente utilizando n pares de processos
a executar 'filtro' e 'existe'. O programa deverá imprimir o resultado, logo que possivel, e terminar os processos que já não estejam a fazer
processamento relevante. 
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

int main(int argc, char const *argv[]) {
  int r, status;
  int n = atoi(argv[1]);
  char buffer[1024];
  int fd[n][2];
  int fd2[n][2];
  int pid[n];

  for(int i = 0; i<n; i++){
    pipe(fd[i]);
    pipe(fd2[i]);

    if((pid[i] = fork()) == 0){
      dup2(fd[i][0], 0);
      dup2(fd2[i][1], 1);
      close(fd[i][1]);
      close(fd2[i][0]);
      execlp("filtro", "filtro", NULL);
      _exit(1);
    }
    close(fd[i][0]);
    close(fd2[i][1]);

    if(!fork()){
      dup2(fd2[i][0], 0);
      close(fd2[i][1]);
      execlp("existe", "existe", NULL);
      _exit(1);
    }
    close(fd2[i][0]);
  }
  
    if(!fork()){
      int d = 0;
      while((r = readln(0, buffer, 1024))){
        write(fd[d][1], buffer, r);
        d = (d+1)%n;
      }
    }

  for(int i = 0; i<n; i++){
    wait(&status);
    if(WIFEXITED(status))
      if(WEXITSTATUS(status) == 1){
        for(int i = 0; i<n; i++){
          kill(pid[i], SIGKILL);
          _exit(0);
        }
      }
  }
  return 0;
}