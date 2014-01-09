#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <termios.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include "nfclib.h"

void (**nfc_what_happens_if)(nfc_reader* x) = NULL;
int what_happens_len;
int what_happens_max_len;

int nfc_set_cmd(nfc_reader* x,char* str){
 
  int str_len = strlen(str);
  int i = 0;
  if(!str_len) 
    return 1;
  for(;i<str_len;i++) 
    x->command[i]=(uint8_t) str[i];
  x->command_len = i;
  return 0;
}


void dance_the_expanding_dance(){

  int i;
  void(**temp)(nfc_reader* x) = malloc((what_happens_max_len+2) * sizeof(void (*)(nfc_reader*)));
  for(i=0;i<what_happens_len;i++){
    temp[i] = nfc_what_happens_if[i];
  }
  free(nfc_what_happens_if);
  fprintf(stdout,"YES, WE DANCED!\n");
  nfc_what_happens_if=temp;
}

void* nfc_reader_on_tag(nfc_reader* x,void (*y)(nfc_reader*)){
  if(nfc_what_happens_if == NULL){
    what_happens_len = 0;
    what_happens_max_len = 2;
    nfc_what_happens_if = malloc(2 * sizeof(void (*)(nfc_reader*)));
  }
  if(what_happens_len==what_happens_max_len){
    dance_the_expanding_dance();
  }
  nfc_what_happens_if[(what_happens_len)++] = y;
  return NULL;
}

nfc_reader* nfc_reader_init(const char* name) {
  nfc_reader* xptr;
  nfc_reader x;
  int len = strlen(name);
  int i;
  struct termios tconfig;
fprintf(stdout,"Korrekt\n");
fprintf(stdout,"%s\n",name);
  x.nfc_port = (char*) malloc(sizeof(char)*(len+1));
  for(i = 0;i<len;i++) 
    x.nfc_port[i] = name[i];
  x.nfc_port[i] = '\0';
  x.fh = open(x.nfc_port,O_RDWR|O_NOCTTY|O_NONBLOCK);
  if(x.fh==-1) 
    byebye("Could not open a filehandler for writing\n");
  tconfig.c_cc[VTIME] = 3;
  tconfig.c_cc[VMIN] = 5;
  /* Tconfig setup */ 
  tconfig.c_iflag &= ~(IGNBRK|ISTRIP|IXON);
/*  tconfig.c_lflag &= ~(ECHO | ECHONL | ICANON | IEXTEN | ISIG); */
  tconfig.c_lflag &= ~(ECHO | ECHONL | ICANON | IEXTEN | ISIG); 
  tconfig.c_cflag &= ~(CSIZE | PARENB);
  tconfig.c_cflag |= CS8;
  tconfig.c_oflag &= ~(OPOST); 
  if(cfsetispeed(&tconfig, B115200) < 0 || cfsetospeed(&tconfig, B115200) < 0) 
    fprintf(stderr,"ERROR %s:%d\n",__FILE__,__LINE__);
  if(tcsetattr(x.fh, TCSANOW, &tconfig) < 0)
    fprintf(stderr,"ERROR %s:%d\n",__FILE__,__LINE__);
  
  x.reddit = (unsigned char*) malloc(sizeof(unsigned char)*100);
  x.reddit_len = 0;
  x.command_len = 0;
  xptr = malloc(sizeof(nfc_reader)*1);
  *xptr = x;
  return xptr;
}
void* nfc_reader_do(void* y){
    nfc_reader* x= y;
    int write_pos = 0;
    int i;
    int j=0;
    int limit = -1;
    int len = x->command_len;
    int read_bytes;
    int b_read =0;
    unsigned char bcc = 0;
    unsigned char ucmd [50];
    fd_set set;
    int written_bytes;
    struct timeval timeout;
    timeout.tv_sec=1;
    timeout.tv_usec=0;
    for(i=0;i<len;i++)
      fprintf(stderr,"%d\n",(x->command)[i]);
    for(i=0;i<len;i++){
      if(isalpha((x->command)[i])){
        ucmd[j++] = (x->command)[i];
      }
      if(isdigit((x->command)[i])){
        int temp = (x->command)[i++] - '0';
        while(i<len && isdigit((x->command)[i])){
          temp *= 10;
          temp += (x->command)[i++] - '0';
        }
        if(temp>255) 
          byebye("Adresse zu groß");
        ucmd[j++] = temp;
        if(i==len)
          break;
      }
    }
    /*CLEAR */
    written_bytes = write(x->fh,"\0",1);
    ucmd[j] = '\0';
    x->write_puffer[write_pos++] = 2;
    x->write_puffer[write_pos++] = 255;
    x->write_puffer[write_pos++] = ustrlen(ucmd);
    for(i=0;i<ustrlen(ucmd);i++)
      x->write_puffer[write_pos++] = ucmd[i];
    for(i=1;i<(len)+3;i++) 
      bcc ^= x->write_puffer[i];
    x->write_puffer[write_pos++] = bcc;
    x->write_puffer[write_pos++] = 3;
    for(i=0;i<write_pos;i++)
      fprintf(stderr,"written: %d\n",(x->write_puffer)[i]);
    written_bytes = write(x->fh,x->write_puffer,write_pos);
    if(written_bytes != write_pos){
      fprintf(stderr,"Error while writing\n");
      return y;
    }
    j=0;
    while(1){
      /* INITIALIZE FD READ SET */
      FD_ZERO (&set);
      FD_SET (x->fh,&set);
      
      if(select(FD_SETSIZE,&set,NULL,NULL,&timeout)>0){
        if((read_bytes=read(x->fh,x->read_puffer,37))>0){
          for(i=0;i<read_bytes;i++,j++)
            (x->reddit)[j] = (x->read_puffer)[i];
          
          if(j>2 && limit==-1) limit=(x->reddit)[2]+5;
          b_read+=read_bytes;
        }
        if(b_read==limit) {
          x->reddit_len = b_read;
          break;
        }
        if(limit!=-1 && b_read>limit){
          fprintf(stdout,"Error. Read too many bytes\n");
          x->reddit_len = 0;
          break;
        }
      }
      else{
        int ii= 0;
        fprintf(stderr,"Read Bytes: %d\n",b_read);
        fprintf(stderr,"Expected: %d\n",limit);
        for(ii=0;ii<b_read;ii++)
          fprintf(stderr,"Read: %d\n",(x->reddit)[ii]);
        fprintf(stdout,"Could not catch the correct length of the answer\n");
        break;
      }
    }
    return y;  /*Bessere Returns; */
}

int nfc_reader_poll(nfc_reader* x){
  int rc;
#if 0  
  struct termios tconfig;
  tconfig.c_cc[VTIME] = 3;
  tconfig.c_cc[VMIN] = 5;
  /* Tconfig setup */ 
  tconfig.c_iflag &= ~(IGNBRK|ISTRIP|IXON);
  tconfig.c_cflag |= CS8;
  tconfig.c_lflag &= ~(ECHO | ECHONL | ICANON | IEXTEN | ISIG);
  tconfig.c_oflag &= ~(OPOST); 
  if(cfsetispeed(&tconfig, B115200) < 0 || cfsetospeed(&tconfig, B115200) < 0) 
    fprintf(stderr,"ERROR %s:%d\n",__FILE__,__LINE__);
  if(tcsetattr(x->fh, TCSANOW, &tconfig) < 0)
    fprintf(stderr,"ERROR %s:%d\n",__FILE__,__LINE__);
#endif
  rc = pthread_create(&(x->polling),NULL,&nfc_reader_read,x);
  if( rc != 0)
    byebye("Problem bei Threaderzeugung\n");
  pthread_join(x->polling,NULL); 
fprintf(stderr,"FILE...(%s:%d)\n",__FILE__,__LINE__);
  return 0;
}

int nfc_reader_stop_poll(nfc_reader* x){
  int ret = pthread_cancel(x->polling);
  return ret;
}



void* nfc_reader_read(void *y){
  nfc_reader *x = y;
  int read_bytes;
  fd_set set;
  struct timeval timeout;
  int i,j=0;
  int temp =0;
  int limit=-1;
  int b_read = 0;
#if 0
  struct termios tconfig;
  tconfig.c_cc[VTIME] = 3;
  tconfig.c_cc[VMIN] = 5;
  /* Tconfig setup */ 
  tconfig.c_iflag &= ~(IGNBRK|ISTRIP|IXON);
  tconfig.c_lflag &= ~(ECHO | ECHONL | ICANON | IEXTEN | ISIG);
  tconfig.c_cflag |= CS8;
  tconfig.c_oflag &= ~(OPOST); 
  if(cfsetispeed(&tconfig, B115200) < 0 || cfsetospeed(&tconfig, B115200) < 0) 
    fprintf(stderr,"ERROR %s:%d\n",__FILE__,__LINE__);
  if(tcsetattr(x->fh, TCSANOW, &tconfig) < 0)
    fprintf(stderr,"ERROR %s:%d\n",__FILE__,__LINE__);
#endif
  pthread_setcancelstate(PTHREAD_CANCEL_ENABLE,NULL);
  while(1){
    /* INITIALIZE FD READ SET */
    timeout.tv_sec=1;
    timeout.tv_usec=0;
    FD_ZERO (&set);
    FD_SET (x->fh,&set);
    if(select(FD_SETSIZE,&set,NULL,NULL,&timeout)>0){ 
      if((read_bytes=read(x->fh,x->read_puffer,3))>0){
        for(i=0;i<read_bytes;i++,j++)
          (x->reddit)[j] = x->read_puffer[i];
        b_read += read_bytes;
        if(limit==-1 && b_read>3)
          limit = (x->reddit)[2]+5;
      }
      if(limit==b_read){
        x->reddit_len=b_read;
        if(nfc_what_happens_if!=NULL)
          nfc_what_happens_if[(++temp % what_happens_len)](x);
        j=0;
        b_read =0;
        limit=-1;
      }
      if(limit!=-1 && b_read>limit){
        fprintf(stdout,"Error. Read too many bytes\n");
        limit=-1;
        b_read=0;
        j=0;
      }
    }
    else{
      if(limit!=-1){
        limit = -1;
        j=0;
        b_read =0;
        fprintf(stdout,"Selstam hier zu sein\n");
      }
    }
  }
}


int nfc_reader_destroy(nfc_reader* x){
  if(x!=NULL){
    if(x->reddit != NULL)
      free(x->reddit);
    if(x->nfc_port != NULL)
      free(x->nfc_port);
    if(nfc_what_happens_if != NULL)
    free(nfc_what_happens_if);
    free(x);
  }
  nfc_what_happens_if = NULL;
  return 0;
}
void byebye(char *str){
  fprintf(stderr,"Error\nErrormsg: %s\n",str);
  exit(EXIT_FAILURE);
}

int ustrlen(unsigned char* str){
  int ret;
  for(ret=0;str[ret]!='\0';ret++);
  return ret;

}

