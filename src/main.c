#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <termios.h>
#include <sys/types.h>
#include <sys/time.h>
#include <errno.h>
#include "funct.h"

int main(int argc,char *argv[]) {
 
  int i =0;
  unsigned char* yoshi; 
  unsigned char* reddit;
  char *test;
  char hugo = '0';
  test = (char *) malloc(sizeof(char)*100);
  yoshi = (unsigned char*) malloc(sizeof(unsigned char)*100);
  
/* Here the testing begins */
  while(hugo!='n'){ 
    fprintf(stdout,"Bitte Command eingeben: \n");
    fflush(stdin);
    
    test = fgets(test,100,stdin);
    /* Converting char into unsigned char */
    strtouchar(test,yoshi);

    fprintf(stderr,"HALLO?(%s:%d)\n",__FILE__,__LINE__);
    /* Sending the command and returning the response */
    reddit = getserial(yoshi);
    /* Output the return */
    fprintf(stdout,"Resultat\n");
    for(i=0;reddit[i]!='\0';i++) 
      fprintf(stdout, "%x\t",reddit[i]);
    fprintf(stdout,"\n");
    for(i=0;reddit[i]!='\0';i++) 
      fprintf(stdout, "%c\t",reddit[i]);
    fprintf(stdout,"\n");
    fprintf(stdout,"Nochmal?(y/n)\n");
    fflush(stdin);
    test = fgets(test,100,stdin);
    hugo = test[0];
  }
  fprintf(stdout,"\nFinish\n");
  return 0;
}


