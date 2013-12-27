#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <termios.h>
#include <sys/types.h>
#include <sys/time.h>
#include <errno.h>

int main(int argc,char *argv[]) {
  int nfc_reader;
/*  int read_fp; */
  int writing=0;
  int reading=0;
  int i=0;
  int n=1;
  int b_read =0;
  int limit = -1;
  struct timeval timeout;
  unsigned char reddit[500];
  unsigned char puffer[6];
  unsigned char bcc=0;
  fd_set set;
  timeout.tv_sec=5;
  timeout.tv_usec=0;
  nfc_reader = open("/dev/ttyUSB0",O_RDWR|O_NOCTTY|O_NONBLOCK);
  while(1){
/**************************/
/* INITIALIZE FD READ SET */
  FD_ZERO (&set);
  FD_SET (nfc_reader,&set);
/************************/
      if((reading=read(nfc_reader,reddit,37))>0){
        for(i=0;i<reading;i++)
          printf("Reading...%d....Read...%d\n",reading,reddit[i]);
        if(reading>=3 && b_read==0) limit=reddit[2]+5;
        b_read+=reading;
      }
  }
  close(nfc_reader);
  return 0;
}

