#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <termios.h>
#include <sys/types.h>
#include <sys/time.h>
#include <errno.h>
#include <ctype.h> 

void byebye(int bla){ 
  
  fprintf(stderr,"\nNothing to read anymore\n"); 
  exit(EXIT_FAILURE); 
}

unsigned char* getserial(unsigned char* yoshi){
  int nfc_reader;
  int writing=0;
  int reading=0;
  int limit = -1;
  int i=0;
  int j=0;
  int len=0;
  int puffer_pos =0;
  int b_read =0;
  struct termios tconfig;
  struct timeval timeout;
  unsigned char* ret;
  unsigned char reddit[38];
  unsigned char puffer[50];
  unsigned char bcc=0;
  fd_set set;
  ret = (unsigned char*) malloc(sizeof(unsigned char)*500);
  timeout.tv_sec=5;
  timeout.tv_usec=0;
  tconfig.c_cc[VTIME] = 3;
  tconfig.c_cc[VMIN] = 5;
  nfc_reader = open("/dev/ttyUSB0",O_RDWR|O_NOCTTY|O_NONBLOCK);
  /* Tconfig setup */ 
  tconfig.c_iflag &= ~(IGNBRK|ISTRIP|IXON);
  tconfig.c_cflag |= CS8;
  tconfig.c_oflag &= ~(OPOST); 
  /* Set Baudrate */
  if(cfsetispeed(&tconfig, B115200) < 0 || cfsetospeed(&tconfig, B115200) < 0) 
    fprintf(stderr,"ERROR %s:%d\n",__FILE__,__LINE__);
  /* Set Config */
  if(tcsetattr(nfc_reader, TCSAFLUSH, &tconfig) < 0)
    fprintf(stderr,"ERROR %s:%d\n",__FILE__,__LINE__);

/* Create Message */
  for(len=0;yoshi[len]!='\0';len++);
  printf("It's okay...%d\n",len);
  puffer[puffer_pos++] = 2;
  puffer[puffer_pos++] = 255;
  puffer[puffer_pos++] = (unsigned char) len;
  for(i=0;i<len;i++)
    puffer[puffer_pos++] = yoshi[i];
 
/*  
  puffer[puffer_pos++]= 'v';
  puffer[puffer_pos++]= 'h';
*/  
  for(i=1;i<len+3;i++) 
    bcc ^= puffer[i];
  puffer[puffer_pos++] = bcc;
  puffer[puffer_pos++] = 3;
  for(i=0;i<puffer_pos;i++)
    printf("Writing...%d\n",puffer[i]);
  printf("It's okay...%d\n",puffer_pos);
  
  if (nfc_reader!=-1) printf("SUCCES\n");
  else printf("FAILURE\n");
  writing = write(nfc_reader,puffer,puffer_pos);
  printf("%i\n",writing);
  printf("Reading now......\n");
  while(1){
    /**************************/
    /* INITIALIZE FD READ SET */
    FD_ZERO (&set);
    FD_SET (nfc_reader,&set);
    /************************/
    if(select(FD_SETSIZE,&set,NULL,NULL,&timeout)>0){
      fprintf(stderr,"Something\n");
      if((reading=read(nfc_reader,reddit,37))>0){
        for(i=0;i<reading;i++,j++){
          printf("Reading...%d....Read...%d\n",reading,reddit[i]);
          ret[j] = reddit[i];
        }
        if(j>2 && limit==-1) limit=ret[2]+5;
        b_read+=reading;
      }
      if(b_read==limit) break;
    }
    else{
      fprintf(stdout,"Could not catch the correct length of the answer\n");
      break;
    }
  }
  close(nfc_reader);
  /*Chechking response */
  bcc = 0;
  for(i=1;i<ret[2]+3;i++) 
    bcc ^= ret[i];
  if(ret[ret[2]+3] == bcc){
    fprintf(stdout,"Succesful command.\n");
    return ret;
  }
  else{
    fprintf(stdout,"Something went wrong.\n");
    return NULL;
  }
}

void strtouchar(char* str,unsigned char* uchar){
  
  int i = 0;
  int j = 0;
  int temp=0;
  int numberflag = 0;
  
  for(i=0;str[i]!='\n';){
    if(isspace(str[i]) && str[i]!='\n'){
      i++;;
      continue;
    }
    for(temp=0;str[i]>=48 && str[i] <=  57;i++){
      numberflag=1;
      fprintf(stderr,"ASCII-ZAHL: %d\n",str[i]);
      temp *= 10;
      temp += str[i] - '0';
    }
    if(numberflag) 
      uchar[j++] = (unsigned char) temp;
    else 
      uchar[j++] = (unsigned char) str[i++];
    numberflag = 0;
  }
  fprintf(stderr,"0 IST GLEICH ...%d\n",'0'-'0');
  fprintf(stderr,"HALLO?(%s:%d)\n",__FILE__,__LINE__);
  uchar[j] = '\0';

}



