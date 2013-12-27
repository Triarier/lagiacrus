#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <termios.h>
#include <sys/types.h>
#include <sys/time.h>
#include <errno.h>

int do_it(unsigned char* reddit,int nfc_reader){
  int bla=0;
  while(1){
    if((bla=read(nfc_reader,reddit,37))>0)
      printf("READING... %i\nREDDIT..%d..%d\n",bla,reddit[1],reddit[36]);
  } 

}
void byebye(int bla){ 
  
  fprintf(stderr,"\nNothing to read anymore\n"); 
  exit(EXIT_FAILURE); 
  }

int main(int argc,char *argv[]) {
  int nfc_reader;
/*  int read_fp; */
  int writing=0;
  int reading=0;
  int i=0;
  int n=1;
  int b_read =0;
  int limit = -1;
  struct termios tconfig;
  struct timeval timeout;
  unsigned char reddit[500];
  unsigned char puffer[6];
  unsigned char bcc=0;
  fd_set set;
  timeout.tv_sec=5;
  timeout.tv_usec=0;
  tconfig.c_cc[VTIME] = 3;
  tconfig.c_cc[VMIN] = 5;
  nfc_reader = open("/dev/ttyUSB0",O_RDWR|O_NOCTTY|O_NONBLOCK);
   
/*  read_fp = open("/dev/ttyUSB0",O_RDONLY|O_NOCTTY|O_NONBLOCK); */
/**************************/
/* INITIALIZE FD READ SET */
/*  FD_ZERO (&set);
  FD_SET (read_fp,&set);
  fprintf(stderr,"Select....%d\n", select(FD_SETSIZE,&set,NULL,NULL,&timeout)); */
/************************/
  tconfig.c_iflag &= ~(IGNBRK|ISTRIP|IXON);
  tconfig.c_cflag |= CS8;
  tconfig.c_oflag &= ~(OPOST); 
  if(cfsetispeed(&tconfig, B115200) < 0 || cfsetospeed(&tconfig, B115200) < 0) 
    fprintf(stderr,"ERROR %s:%d\n",__FILE__,__LINE__);
  if(tcsetattr(nfc_reader, TCSAFLUSH, &tconfig) < 0)
    fprintf(stderr,"ERROR %s:%d\n",__FILE__,__LINE__);
  puffer[0] = 2;
  puffer[1] = 255;
  puffer[2] = 1;
  puffer[3] = 'b'; 
/*  puffer[3] = 5; */
  for(i=1;i<puffer[2]+3;i++) 
    bcc ^= puffer[i];
  printf("%i\n....%d\n",puffer[2],i);
  puffer[4] = bcc;
  puffer[5] = 3;
  printf("nana...%c\n",(bcc-100));
  if (nfc_reader!=-1) printf("SUCCES\n");
  else printf("FAILURE\n");
  writing = write(nfc_reader,puffer,6);
  fprintf(stderr,"Select....%d\n", select(FD_SETSIZE,&set,NULL,NULL,&timeout));
  printf("%i\n",writing);
  printf("Reading now......\n");
  while(1){
/**************************/
/* INITIALIZE FD READ SET */
  FD_ZERO (&set);
  FD_SET (nfc_reader,&set);
/************************/
    if( select(FD_SETSIZE,&set,NULL,NULL,&timeout)>0){
      fprintf(stderr,"Something\n");
      if((reading=read(nfc_reader,reddit,37))>0){
        for(i=0;i<reading;i++)
          printf("Reading...%d....Read...%d\n",reading,reddit[i]);
        if(reading>=3 && b_read==0) limit=reddit[2]+5;
        b_read+=reading;
      }
    }
    else
      byebye(b_read);
    
  }
  close(nfc_reader);
  return 0;
}

