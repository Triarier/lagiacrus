#ifndef NFCLIB_H
#define NFCLIB_H

#include <pthread.h>
#include <stdint.h>

struct nfc_struct;

typedef struct nfc_struct{
  char* nfc_port;  /* Port of NFC_Reader, e.g '/dev/ttyUSB0' , gets initialize with nfc_reader_init(); */
  int fh;
  pthread_t polling;
  uint8_t read_puffer[100];
  uint8_t write_puffer[100];
  uint8_t* reddit;
  int reddit_len;
  uint8_t command[100];
  int command_len;
  int what_happens_len;
  int what_happens_max_len;
  void (**nfc_what_happens_if)(struct nfc_struct* x);
} nfc_reader ;


void* nfc_reader_on_tag(nfc_reader* x,void (*y)(nfc_reader*));

nfc_reader* nfc_reader_init(const char* name);
int nfc_reader_stop_poll(nfc_reader*);
int nfc_reader_poll( nfc_reader*);
int nfc_reader_destroy(nfc_reader* x);
void* nfc_reader_read(void* y);
void byebye(char *str); 
void* nfc_reader_do(void* y);
void set_cmd(nfc_reader *x,char * y);
int ustrlen(unsigned char* str);
int nfc_set_cmd(nfc_reader* x,char* str);
void* nfc_reader_1_read(void *y);
#endif /* NFCLIB_H */
