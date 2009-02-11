#ifdef linux

int __isnanf(float);
#ifndef NAN
#define NAN sqrt(-1)
#endif
#endif

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>
#include <signal.h>
#include "pfm.h"
#ifdef WORDS_BIGENDIAN
#define BYTEORDER PFM_BigEndian
#else
#define BYTEORDER PFM_LittleEndian
#endif

#include <limits.h>

typedef unsigned short int uint16;
typedef signed short int sint16;
typedef unsigned char grey;

static double pfmGeoWest_,pfmGeoNorth_,pfmGeoEast_,pfmGeoSouth_;
static int pfmGeoSet_=0;
static int pfmGeoRead_=0;
static const char* pfmComment_=0;
static char* pfmCommentRead_=0;
static FILE* fp_=0;
static int pid_=0;
static char fifoname_[256];
static int compressed_=0;

static void read_buffer(char* buffer, int maxlen, FILE *fp)
{
  do {
    fgets(buffer,maxlen,fp);
    if (buffer[0]=='#' && buffer[1]=='g' && buffer[2]=='e' && buffer[3]=='o') {
      pfmGeoRead_=(sscanf(buffer,"%*s %lf %lf %lf %lf",&pfmGeoWest_,&pfmGeoNorth_,&pfmGeoEast_,&pfmGeoSouth_)==4);
#ifdef DEBUG
      if (pfmGeoRead_) printf("Read geo %f %f %f %f\n",pfmGeoWest_,pfmGeoNorth_,pfmGeoEast_,pfmGeoSouth_);
      else printf("Read geo failed\n");
#endif
    }
    if (buffer[0]=='#' && buffer[1]=='#' && buffer[2]=='c' && buffer[3]=='o') {
      fgets(buffer,maxlen,fp);
      if (buffer[0]=='#') {
        buffer[maxlen-1]=0;
        buffer[strlen(buffer)-1]=0;
        if (pfmCommentRead_) free(pfmCommentRead_);
        pfmCommentRead_=strdup(&buffer[1]);
      }
    }
  } while (buffer[0]=='#' || buffer[0]==0 || buffer[0]==10 || buffer[0]==13);
}

static FILE* start_decompression(FILE* fp, const char* command)
{
  char cmd[1024];
  char buffer[1025];
  int num_char;
  FILE *fifofp;
  sprintf(fifoname_,"/tmp/pfm.%d",getpid());
  mkfifo(fifoname_,0700);
  sprintf(cmd,command,fifoname_);
#ifdef DEBUG
  printf("Starting decompression %s\n",cmd);
#endif
  fp_=fp;
  fp=popen(cmd,"r");
  pid_=fork();
  switch (pid_) {
    case 0:
      fifofp=fopen(fifoname_,"w");
      while (!feof(fp_)) {
        num_char=fread(buffer,sizeof(char),1024,fp_);
        fwrite(buffer,sizeof(char),num_char,fifofp);
      }
      fclose(fp_);
      exit(0);
      break;
    case -1: 
			fprintf(stderr,"decompression failed: %s\n",command);
      break;
    default:
      break;
  }
  return fp;
}

static FILE *check_compression(char *buffer, FILE *fp)
{
  long filepos;
  filepos=ftell(fp);
  read_buffer(buffer,1023,fp);
  compressed_=0;
#ifdef DEBUG
  printf("magic: %d%d%d\n",buffer[0],buffer[1],buffer[2]);
#endif
  if (buffer[0]=='B' && buffer[1]=='Z' && buffer[2]=='h') {
    compressed_=1;
    fseek(fp,filepos,SEEK_SET);
    fp=start_decompression(fp,"bzip2 -dc %s");
    read_buffer(buffer,1023,fp);
  }
  else if (buffer[0]==31 && (unsigned char)buffer[1]==139) {
    compressed_=1;
    fseek(fp,filepos,SEEK_SET);
    fp=start_decompression(fp,"gzip -dc <%s");
    read_buffer(buffer,1023,fp);
  }
  return fp;
}

static FILE* stop_decompression(FILE *fp)
{
  if (!compressed_) return fp;
  kill(pid_,SIGTERM);
  waitpid(pid_,0,0);
  pid_=0;
  pclose(fp);
  unlink(fifoname_);
  return fp_;
}

static void swapfloat(float *p_float, int n, int byteorder)
{
  union FloatRec {
    float f;
    unsigned char b[4];
  } *p,tmp;

  if (byteorder == BYTEORDER) return;
  /*  printf("Swapping\n");*/
  p=(union FloatRec*)p_float;
  while (n) {
    tmp.b[0]=p->b[3];
    tmp.b[1]=p->b[2];
    tmp.b[2]=p->b[1];
    tmp.b[3]=p->b[0];
    *p=tmp;
    n--;
    p++;
  }
}

static void swapdouble(double *p_double, int n, int byteorder)
{
  union DoubleRec {
    double d;
    unsigned char b[8];
  } *p,tmp;

  if (byteorder == BYTEORDER) return;
  /*  printf("Swapping\n");*/
  p=(union DoubleRec*)p_double;
  while (n) {
    tmp.b[0]=p->b[7];
    tmp.b[1]=p->b[6];
    tmp.b[2]=p->b[5];
    tmp.b[3]=p->b[4];
    tmp.b[4]=p->b[3];
    tmp.b[5]=p->b[2];
    tmp.b[6]=p->b[1];
    tmp.b[7]=p->b[0];
    *p=tmp;
    n--;
    p++;
  }
}

static void swapint(int *p_int,int n,int byteorder)
{
  union IntRec {
    int i;
    unsigned char b[4];
  } *p,tmp;

  if (byteorder == BYTEORDER) return;
  /*  printf("Swapping\n");*/
  p=(union IntRec*)p_int;
  while (n) {
    tmp.b[0]=p->b[3];
    tmp.b[1]=p->b[2];
    tmp.b[2]=p->b[1];
    tmp.b[3]=p->b[0];
    *p=tmp;
    n--;
    p++;
  }
}

static void swapint16(uint16 *p_int,int n,int byteorder)
{
  union IntRec {
    uint16 i;
    unsigned char b[2];
  } *p,tmp;

  if (byteorder == BYTEORDER) return;
  /*  printf("Swapping\n");*/
  p=(union IntRec*)p_int;
  while (n) {
    tmp.b[0]=p->b[1];
    tmp.b[1]=p->b[0];
    *p=tmp;
    n--;
    p++;
  }
}

static void* double2double(void *dest, void *src, int n_el,
        PFMConvert *func, float minval, float maxval)
{
  int i;
  if (func)
    for (i=0; i<n_el; i++)
      ((double*)dest)[i]=(*func)(((double*)src)[i],minval,maxval);
  else
    memcpy(dest,src,n_el*sizeof(double));
  return dest;
}


static void* double2float(void *b_float, void* b_double, int n_el,
      PFMConvert *func, float minval, float maxval)
{
  int i;
  if (func)
    for (i=0; i<n_el; i++)
      ((float*)b_float)[i]=(*func)(((double*)b_double)[i],minval,maxval);
  else
    for (i=0; i<n_el; i++)
      ((float*)b_float)[i]=((double*)b_double)[i];
  return b_float;
}

static void* double2sint(void *b_int, void* b_double, int n_el,
      PFMConvert *func, float minval, float maxval)
{
  int i;
  if (func)
    for (i=0; i<n_el; i++)
      ((int*)b_int)[i]=(*func)(((double*)b_double)[i],minval,maxval);
  else
    for (i=0; i<n_el; i++)
      ((int*)b_int)[i]=((double*)b_double)[i];
  return b_int;
}


static void* double2uint16(void *b_int, void* b_double, int n_el,
        PFMConvert *func, float minval, float maxval)
{
  int i;
  if (func)
    for (i=0; i<n_el; i++)
      ((uint16*)b_int)[i]=(*func)(((double*)b_double)[i],minval,maxval);
  else
    for (i=0; i<n_el; i++)
      ((uint16*)b_int)[i]=((double*)b_double)[i];
  return b_int;
}

static void* float2float(void *dest, void *src, int n_el,
        PFMConvert *func, float minval, float maxval)
{
  int i;
  if (func)
    for (i=0; i<n_el; i++)
      ((float*)dest)[i]=(*func)(((float*)src)[i],minval,maxval);
  else
    memcpy(dest,src,n_el*sizeof(float));
  return dest;
}

static void* float2sint(void *b_int, void* b_float, int n_el,
      PFMConvert *func, float minval, float maxval)
{
  int i;
  if (func)
    for (i=0; i<n_el; i++)
      ((int*)b_int)[i]=(*func)(((float*)b_float)[i],minval,maxval);
  else
    for (i=0; i<n_el; i++)
      ((int*)b_int)[i]=((float*)b_float)[i];
  return b_int;
}

static void* float2uint16(void *b_int, void* b_float, int n_el,
        PFMConvert *func, float minval, float maxval)
{
  int i;
  if (func)
    for (i=0; i<n_el; i++)
      ((uint16*)b_int)[i]=(*func)(((float*)b_float)[i],minval,maxval);
  else
    for (i=0; i<n_el; i++)
      ((uint16*)b_int)[i]=((float*)b_float)[i];
  return b_int;
}

static void* float2double(void *b_double, void* b_float, int n_el,
        PFMConvert *func, float minval, float maxval)
{
  int i;
  if (func)
    for (i=0; i<n_el; i++)
      ((double*)b_double)[i]=(*func)(((float*)b_float)[i],minval,maxval);
  else
    for (i=0; i<n_el; i++)
      ((double*)b_double)[i]=((float*)b_float)[i];
  return b_double;
}

static void* sint2float(void *b_float, void* b_int, int n_el,
      PFMConvert *func, float minval, float maxval)
{
  int i;
  if (func)
    for (i=0; i<n_el; i++)
      ((float*)b_float)[i]=(*func)(((int*)b_int)[i],minval,maxval);
  else
    for (i=0; i<n_el; i++)
      ((float*)b_float)[i]=((int*)b_int)[i];
  return b_float;
}

static void* sint2double(void *b_double, void* b_int, int n_el,
      PFMConvert *func, float minval, float maxval)
{
  int i;
  if (func)
    for (i=0; i<n_el; i++)
      ((double*)b_double)[i]=(*func)(((int*)b_int)[i],minval,maxval);
  else
    for (i=0; i<n_el; i++)
      ((double*)b_double)[i]=((int*)b_int)[i];
  return b_double;
}

static void* sint2sint(void *dest, void* src, int n_el,
        PFMConvert *func, float minval, float maxval)
{
  int i;
  if (func)
    for (i=0; i<n_el; i++)
      ((int*)dest)[i]=(*func)(((int*)src)[i],minval,maxval);
  else
    memcpy(dest,src,n_el*sizeof(int));
  return dest;
}

static void* sint2uint16(void *b_int, void* b_float, int n_el,
       PFMConvert *func, float minval, float maxval)
{
  int i;
  if (func)
    for (i=0; i<n_el; i++)
      ((uint16*)b_float)[i]=(*func)(((int*)b_int)[i],minval,maxval);
  else
    for (i=0; i<n_el; i++)
      ((uint16*)b_float)[i]=((int*)b_int)[i];
  return b_float;
}

static void* uint162sint(void *b_int, void* b_float, int n_el,
      PFMConvert *func, float minval, float maxval)
{
  int i;
  if (func)
    for (i=0; i<n_el; i++)
      ((int*)b_int)[i]=(*func)(((uint16*)b_float)[i],minval,maxval);
  else
    for (i=0; i<n_el; i++)
      ((int*)b_int)[i]=((uint16*)b_float)[i];
  return b_int;
}

static void* uint162float(void* b_float, void *b_int, int n_el,
        PFMConvert *func, float minval, float maxval)
{
  int i;
  if (func)
    for (i=0; i<n_el; i++)
      ((float*)b_float)[i]=(*func)(((uint16*)b_int)[i],minval,maxval);
  else
    for (i=0; i<n_el; i++) {
      ((float*)b_float)[i]=((uint16*)b_int)[i];
      /*      printf("%8d=%f\n",((uint16*)b_int)[i],((float*)b_float)[i]);*/
    }
  return b_float;
}

static void* uint162double(void* b_double, void *b_int, int n_el,
        PFMConvert *func, float minval, float maxval)
{
  int i;
  if (func)
    for (i=0; i<n_el; i++)
      ((float*)b_double)[i]=(*func)(((uint16*)b_int)[i],minval,maxval);
  else
    for (i=0; i<n_el; i++) {
      ((float*)b_double)[i]=((uint16*)b_int)[i];
      /*      printf("%8d=%f\n",((uint16*)b_int)[i],((float*)b_float)[i]);*/
    }
  return b_double;
}

static void* uint162uint16(void *dest, void* src, int n_el,
         PFMConvert *func, float minval, float maxval)
{
  int i;
  if (func)
    for (i=0; i<n_el; i++)
      ((uint16*)dest)[i]=(*func)(((uint16*)src)[i],minval,maxval);
  else
    memcpy(dest,src,n_el*sizeof(uint16));
  return dest;
}

static void* pgm2float(void *b_float, void* b_grey, int n_el,
                      PFMConvert *func, float minval, float maxval)
{
  int i;
  if (func)
    for (i=0; i<n_el; i++)
      ((float*)b_float)[i]=(*func)(((grey*)b_grey)[i],minval,maxval);
  else
    for (i=0; i<n_el; i++)
      ((float*)b_float)[i]=((grey*)b_grey)[i];
  return b_float;
}

static void* pgm2double(void *b_double, void* b_grey, int n_el,
                      PFMConvert *func, float minval, float maxval)
{
  int i;
  if (func)
    for (i=0; i<n_el; i++)
      ((double*)b_double)[i]=(*func)(((grey*)b_grey)[i],minval,maxval);
  else
    for (i=0; i<n_el; i++)
      ((double*)b_double)[i]=((grey*)b_grey)[i];
  return b_double;
}

static void* pgm2sint(void *b_int, void* b_grey, int n_el,
                      PFMConvert *func, float minval, float maxval)
{
  int i;
  if (func)
    for (i=0; i<n_el; i++)
      ((int*)b_int)[i]=(*func)(((grey*)b_grey)[i],minval,maxval);
  else
    for (i=0; i<n_el; i++)
      ((int*)b_int)[i]=((grey*)b_grey)[i];
  return b_int;
}

static void* pgm2uint16(void *b_int16, void* b_grey, int n_el,
                      PFMConvert *func, float minval, float maxval)
{
  int i;
  if (func)
    for (i=0; i<n_el; i++)
      ((uint16*)b_int16)[i]=(*func)(((grey*)b_grey)[i],minval,maxval);
  else
    for (i=0; i<n_el; i++)
      ((uint16*)b_int16)[i]=((grey*)b_grey)[i];
  return b_int16;
}

static void* pgm2pgm(void *dest, void* src, int n_el,
         PFMConvert *func, float minval, float maxval)
{
  int i;
  if (func)
    for (i=0; i<n_el; i++)
      ((grey*)dest)[i]=(*func)(((grey*)src)[i],minval,maxval);
  else
    memcpy(dest,src,n_el*sizeof(grey));
  return dest;
}

static float pfm_mult;
static float pfm_offset;

static float StretchConvert(float v, float min_val, float max_val)
{
  return v*pfm_mult+pfm_offset;
}

typedef void *ConvertFunc(void*, void*, int, PFMConvert*, float, float);

static void pfm_read_ppm(FILE *fp, int cols, int rows, PFM3Byte *ppmbuffer)
{
  unsigned char buffer[cols*3*sizeof(unsigned char)];
  int r,c;
  unsigned char *p_r, *p_g, *p_b;
  unsigned char *p_buf;
  p_r=ppmbuffer->r;
  p_g=ppmbuffer->g;
  p_b=ppmbuffer->b;
  for (r=0; r<rows; r++) {
    fread(buffer,sizeof(unsigned char)*3,cols,fp);
    p_buf=buffer;
    for (c=0; c<cols; c++) {
      *p_r++=*p_buf++;
      *p_g++=*p_buf++;
      *p_b++=*p_buf++;
    }
  }
}

void *pfm_readpfm_type(FILE *fp,
           int *cols,
           int *rows,
           float *minval,
           float *maxval,
           int storageType,
           PFMConvert *convFunc)
{
/*  int compressed;*/
  void *ret_val;
  char buffer[1024];
  char     *data=0;
  float    *floatbuffer=0;
  double   *doublebuffer=0;
  int      *intbuffer=0;
  uint16   *int16buffer=0;
  grey     *greybuffer=0;
  PFM3Byte *ppmbuffer=0;
  int       byte_order=BYTEORDER;
  int r,c;
  int type;
  int storage_size[]={
          sizeof(float),
          sizeof(signed int),
          sizeof(unsigned int),
          sizeof(sint16),
          sizeof(uint16),
          sizeof(grey),
          sizeof(unsigned char),
          sizeof(unsigned char),
          sizeof(double)
          };
  ConvertFunc* double2[]={
       double2float,
       double2sint,
       double2sint,
       double2uint16,
       double2uint16,
       0,0,0,
       double2double,
       0};
  ConvertFunc* float2[]={
       float2float,
       float2sint,
       float2sint,
       float2uint16,
       float2uint16,
       0,0,0,
       float2double,
       0};
  ConvertFunc* uint162[]={
        uint162float,
        uint162sint,
        uint162sint,
        uint162uint16,
        uint162uint16,
        0,0,0,
        uint162double,
        0};
  ConvertFunc* sint2[]={
      sint2float,
      sint2sint,
      sint2sint,
      sint2uint16,
      sint2uint16,
      0,0,0,
      sint2double,
      0};
  ConvertFunc* pgm2[]={
      pgm2float,
      pgm2sint,
      pgm2sint,
      pgm2uint16,
      pgm2uint16,
      pgm2pgm,
      0,0,
      pgm2double,
      0};
  ConvertFunc* pbm2[]={
  0,0,0,0,0,0,0,0,0,0
  };

  memset(buffer, 0, 1024);
  pfmGeoRead_=0;
  if (pfmCommentRead_) free(pfmCommentRead_);
  pfmCommentRead_=0;
  pfmGeoSet_=0;
  fp=check_compression(buffer,fp);
  
#ifdef DEBUG
  printf("pfm_readpfm: %s\n",buffer);
#endif
  switch (buffer[0]) {
  case 'F':
    switch (buffer[1]) {
    case '0':
      type=PFM_FLOAT_ASCII;
      break;
    case '1':
      type=PFM_INT_ASCII;
      break;
    case '2':
      type=PFM_INT16_ASCII;
      break;
    case '3':
      type=PFM_DOUBLE_ASCII;
      break;
    case '4':
      type=PFM_FLOAT_BIN;
      break;
    case '5':
      type=PFM_INT_BIN;
      break;
    case '6':
      type=PFM_INT16_BIN;
      break;
    case '7':
      type=PFM_DOUBLE_BIN;
      break;
    default:
      fprintf(stderr,"pfm_readpfm: Wrong fileformat!\n");
      ret_val=0;
      goto exit_read_pfm;
    }
    break;
  case 'P':
    switch (buffer[1]) {
    case '1':
      type=PFM_PBM_ASCII;
      break;
    case '2':
      type=PFM_PGM_ASCII;
      break;
    case '3':
      type=PFM_PPM_ASCII;
      break;
    case '4':
      type=PFM_PBM_BIN;
      break;
    case '5':
      type=PFM_PGM_BIN;
      break;
    case '6':
      type=PFM_PPM_BIN;
      break;
    default:
      fprintf(stderr,"pfm_readpfm: Wrong fileformat!\n");
      ret_val=0;
      goto exit_read_pfm;
    }
    break;
  default:
    {
      fprintf(stderr,"pfm_readpfm: Wrong fileformat!\n");
      ret_val=0;
      goto exit_read_pfm;
    }
  }
  if ((type>=PFM_FLOAT_BIN) && (type<=PFM_DOUBLE_BIN))
    {
      read_buffer(buffer,1023,fp);
#ifdef DEBUG
      printf("pfm_readpfm: %s\n",buffer);
#endif
      if (buffer[0] == 'L')  {
        byte_order=PFM_LittleEndian;
      }
      else {
        byte_order=PFM_BigEndian;
      }
    }
  read_buffer(buffer,1023,fp);
#ifdef DEBUG
  printf("pfm_readpfm: %s\n",buffer);
#endif
  sscanf(buffer,"%d %d",cols,rows);
  read_buffer(buffer,1023,fp);
#ifdef DEBUG
  printf("pfm_readpfm: %s\n",buffer);
#endif
  pfm_mult=0.0;
  pfm_offset=0.0;
  if (sscanf(buffer,"%f %f %f %f",minval,maxval,&pfm_mult,&pfm_offset)<2) {
    *maxval=*minval;
    *minval=0.0;
  }
  if (type==PFM_PGM_BIN && *maxval>255) {
    type=PFM_INT16_BIN;
    byte_order=PFM_BigEndian;
  }
  if (type==PFM_PGM_ASCII && *maxval>255 && *maxval<=65535) {
    type=PFM_INT16_ASCII;
    byte_order=PFM_BigEndian;
  }
  if (pfm_mult!=0.0) convFunc=StretchConvert;
#ifdef DEBUG
  printf("pfm_readpfm: cols=%d rows=%d min=%f max=%f byte_order=%s\n",
   *cols,*rows,*minval,*maxval,(byte_order==PFM_BigEndian ? "BE" : "LE"));
#endif
  if (type==PFM_PPM_BIN) {
    ppmbuffer=(PFM3Byte*)malloc(sizeof(PFM3Byte));
    ppmbuffer->r=(unsigned char*)calloc(*cols*(*rows),storage_size[storageType]);
    ppmbuffer->g=(unsigned char*)calloc(*cols*(*rows),storage_size[storageType]);
    ppmbuffer->b=(unsigned char*)calloc(*cols*(*rows),storage_size[storageType]);
    if (ppmbuffer && ppmbuffer->r && ppmbuffer->g && ppmbuffer->b)
      data=(void*)ppmbuffer;
  }
  else data =(void*)calloc(*cols*(*rows),storage_size[storageType]);

  if (!data)
    {
      fprintf(stderr,"pfm_readpfm: Out of memory while allocating input buffer!\n");
      ret_val=0;
      goto exit_read_pfm;
    }
  switch (type)
    {
    case PFM_FLOAT_ASCII:
      if (!float2[storageType]) {
        fprintf(stderr,"pfm_readpfm: conversion from float to type %d not implemented\n",storageType);
        ret_val=0;
        goto exit_read_pfm;
      }
      floatbuffer=(float*)calloc(*cols,sizeof(float));
      if (!floatbuffer)  {
        fprintf(stderr,"pfm_readpfm: Out of memory (temporary float buffer)!\n");
        ret_val=0;
        goto exit_read_pfm;
      }
      for (r=0; r<*rows; r++)  {
        for (c=0; c<*cols; c++) {
          fscanf(fp,"%f",&(floatbuffer[c]));
        }
        float2[storageType](&data[r*(*cols)*storage_size[storageType]],
                            floatbuffer,
                            *cols,
                            convFunc,
                            *minval,
                            *maxval);
      }
      free(floatbuffer);
      break;
    case PFM_FLOAT_BIN:
      if (!float2[storageType]) {
        fprintf(stderr,"pfm_readpfm: conversion from float to type %d not implemented\n",storageType);
        ret_val=0;
        goto exit_read_pfm;
      }
      floatbuffer=(float*)calloc(*cols,sizeof(float));
      if (!floatbuffer)  {
        fprintf(stderr,"pfm_readpfm: Out of memory (temporary float buffer)!\n");
        ret_val=0;
        goto exit_read_pfm;
      }
      for (r=0; r<*rows; r++) {
        fread(floatbuffer,sizeof(float),*cols,fp);
        swapfloat(floatbuffer,*cols,byte_order);
        float2[storageType](&data[r*(*cols)*storage_size[storageType]],
                            floatbuffer,
                            *cols,
                            convFunc,
                            *minval,
                            *maxval);
  
      }
      free(floatbuffer);
      break;      
    case PFM_DOUBLE_ASCII:
      if (!double2[storageType]) {
        fprintf(stderr,"pfm_readpfm: conversion from double to type %d not implemented\n",storageType);
        ret_val=0;
        goto exit_read_pfm;
      }
      doublebuffer=(double*)calloc(*cols,sizeof(double));
      if (!doublebuffer)  {
        fprintf(stderr,"pfm_readpfm: Out of memory (temporary double buffer)!\n");
        ret_val=0;
        goto exit_read_pfm;
      }
      for (r=0; r<*rows; r++)  {
        for (c=0; c<*cols; c++) {
          fscanf(fp,"%lf",&(doublebuffer[c]));
        }
        double2[storageType](&data[r*(*cols)*storage_size[storageType]],
                            doublebuffer,
                            *cols,
                            convFunc,
                            *minval,
                            *maxval);
      }
      free(doublebuffer);
      break;
    case PFM_DOUBLE_BIN:
      if (!double2[storageType]) {
        fprintf(stderr,"pfm_readpfm: conversion from double to type %d not implemented\n",storageType);
        ret_val=0;
        goto exit_read_pfm;
      }
      doublebuffer=(double*)calloc(*cols,sizeof(double));
      if (!doublebuffer)  {
        fprintf(stderr,"pfm_readpfm: Out of memory (temporary double buffer)!\n");
        ret_val=0;
        goto exit_read_pfm;
      }
      for (r=0; r<*rows; r++) {
        fread(doublebuffer,sizeof(double),*cols,fp);
        swapdouble(doublebuffer,*cols,byte_order);
        double2[storageType](&data[r*(*cols)*storage_size[storageType]],
                            doublebuffer,
                            *cols,
                            convFunc,
                            *minval,
                            *maxval);
  
      }
      free(doublebuffer);
      break;
    case PFM_INT_ASCII:
      
      if (!sint2[storageType]) {
        fprintf(stderr,"pfm_readpfm: conversion from int to type %d not implemented\n",storageType);
        ret_val=0;
        goto exit_read_pfm;
      }
      intbuffer =(int*)calloc(*cols,sizeof(int));
      if (!intbuffer)  {
        fprintf(stderr,"pfm_readpfm: Out of memory (temporary int buffer)!\n");
        ret_val=0;
        goto exit_read_pfm;
      }
      for (r=0; r<*rows; r++)  {
        for (c=0; c<*cols; c++) {
          fscanf(fp,"%d",&(intbuffer[c]));
        }
        sint2[storageType](&data[r*(*cols)*storage_size[storageType]],
                            intbuffer,
                            *cols,
                            convFunc,
                            *minval,
                            *maxval);

      }
      free(intbuffer);
      break;
    case PFM_INT_BIN:
      if (!sint2[storageType]) {
        fprintf(stderr,"pfm_readpfm: conversion from int to type %d not implemented\n",storageType);
        ret_val=0;
        goto exit_read_pfm;
      }
      intbuffer =(int*)calloc(*cols,sizeof(int));
      if (!intbuffer)  {
        fprintf(stderr,"pfm_readpfm: Out of memory (temporary int buffer)!\n");
        ret_val=0;
        goto exit_read_pfm;
      }
      for (r=0; r<*rows; r++)  {
        fread(intbuffer,sizeof(int),*cols,fp);
        swapint(intbuffer,*cols,byte_order);
        sint2[storageType](&data[r*(*cols)*storage_size[storageType]],
                          intbuffer,
                          *cols,
                          convFunc,
                          *minval,
                          *maxval);
      }
      free(intbuffer);
      break;
    case PFM_INT16_ASCII:
      if (!uint162[storageType]) {
        fprintf(stderr,"pfm_readpfm: conversion from int16 to type %d not implemented\n",storageType);
        ret_val=0;
        goto exit_read_pfm;
      }
      int16buffer =(uint16*)calloc(*cols,sizeof(uint16));
      if (!int16buffer)  {
        fprintf(stderr,"pfm_readpfm: Out of memory (temporary int16 buffer)!\n");
        ret_val=0;
        goto exit_read_pfm;
      }
      for (r=0; r<*rows; r++)  {
        for (c=0; c<*cols; c++) {
          fscanf(fp,"%hd",&(int16buffer[c])); /* "%d" -> "%hd" (T. Wiebesiek, 31.3.2005) */
        }
        uint162[storageType](&data[r*(*cols)*storage_size[storageType]],
                            int16buffer,
                            *cols,
                            convFunc,
                            *minval,
                            *maxval);

      }
      free(int16buffer);
      break;
    case PFM_INT16_BIN:
      if (!uint162[storageType]) {
        fprintf(stderr,"pfm_readpfm: conversion from int16 to type %d not implemented\n",storageType);
        ret_val=0;
        goto exit_read_pfm;
      }
      int16buffer =(uint16*)calloc(*cols,sizeof(uint16));
      if (!int16buffer)  {
        fprintf(stderr,"pfm_readpfm: Out of memory (temporary int16 buffer)!\n");
        ret_val=0;
        goto exit_read_pfm;
      }
      for (r=0; r<*rows; r++) {
        fread(int16buffer,sizeof(uint16),*cols,fp);
        swapint16(int16buffer,*cols,byte_order);
        uint162[storageType](&data[r*(*cols)*storage_size[storageType]],
                            int16buffer,
                            *cols,
                            convFunc,
                            *minval,
                            *maxval);
      }
      free(int16buffer);
      break;
    case PFM_PGM_ASCII:
      fprintf(stderr,"pfm_readpfm: PGM ASCII not implemented!\n");
      ret_val=0;
      goto exit_read_pfm;
      break;
    case PFM_PGM_BIN:
      if (!pgm2[storageType]) {
        fprintf(stderr,"pfm_readpfm: conversion from pgm to type %d not implemented\n",storageType);
        ret_val=0;
        goto exit_read_pfm;
      }
      greybuffer =(grey*)calloc(*cols,sizeof(grey));
      if (!greybuffer)  {
        fprintf(stderr,"pfm_readpfm: Out of memory (temporary pgm buffer)!\n");
        ret_val=0;
        goto exit_read_pfm;
      }
      for (r=0; r<*rows; r++) {
        fread(greybuffer,sizeof(grey),*cols,fp);
        pgm2[storageType](&data[r*(*cols)*storage_size[storageType]],
                            greybuffer,
                            *cols,
                            convFunc,
                            *minval,
                            *maxval);
      }
      free(greybuffer);
      break;
    case PFM_PBM_ASCII:
      fprintf(stderr,"pfm_readpfm: PBM ASCII not implented!\n");
      ret_val=0;
      goto exit_read_pfm;
      break;
    case PFM_PBM_BIN:
      fprintf(stderr,"pfm_readpfm: PBM BIN not implented!\n");
      ret_val=0;
      goto exit_read_pfm;
      if (!pbm2[storageType]) {
        fprintf(stderr,"pfm_readpfm: conversion from pgm to type %d not implemented\n",storageType);
        ret_val=0;
        goto exit_read_pfm;
      }
      greybuffer =(grey*)calloc(*cols,sizeof(grey));
      if (!greybuffer)  {
        fprintf(stderr,"pfm_readpfm: Out of memory (temporary pgm buffer)!\n");
        ret_val=0;
        goto exit_read_pfm;
      }
      for (r=0; r<*rows; r++) {
        fread(greybuffer,sizeof(grey),*cols,fp);
        pgm2[storageType](&data[r*(*cols)*storage_size[storageType]],
                            greybuffer,
                            *cols,
                            convFunc,
                            *minval,
                            *maxval);
      }
      free(greybuffer);
      break;
    case PFM_PPM_BIN:
      pfm_read_ppm(fp,*cols,*rows,ppmbuffer);
#if 0
      if (!pgm2[storageType]) {
        fprintf(stderr,"pfm_readpfm: conversion from pgm to type %d not implemented\n",storageType);
        ret_val=0;
        goto exit_read_pfm;
      }
#endif      
      break;
    }
exit_read_pfm:
  if (compressed_) fp=stop_decompression(fp);
  return data;
}

float *pfm_readpfm(FILE *fp,
       int *cols,
       int *rows,
       float *minval,
       float *maxval)
{
  return pfm_readpfm_type(fp,cols,rows,minval,maxval,PFM_FLOAT,0);
}

static void pfm_write_ppm_part(FILE *fp,
                         unsigned char*p_r,
                         unsigned char*p_g,
                         unsigned char*p_b,
                         int num_items)
{
  unsigned char buffer[num_items*3*sizeof(unsigned char)];
  unsigned char *p_buf;
  int i;
  p_buf=buffer;
  for (i=0; i<num_items; i++) {
    *p_buf++=*p_r++;
    *p_buf++=*p_g++;
    *p_buf++=*p_b++;
  }
  fwrite(buffer,sizeof(unsigned char)*3,num_items,fp);
}

static void pfm_write_ppm(FILE *fp, const PFM3Byte *data,
                          int cols, int rows,
                          float minval, float maxval,
                          int x1, int y1, int x2, int y2,
                          int dx, int dy)
{
  unsigned char emptybuffer[dx*sizeof(unsigned char)*3];
  unsigned char buffer[dx*sizeof(unsigned char)*3];
  unsigned char* p_r=data->r;
  unsigned char* p_g=data->g;
  unsigned char* p_b=data->b;
  int x,y;

  fprintf(fp,"P6 # 8 bit unsigned integer triple (PPM)\n");
  if (pfmGeoSet_) fprintf(fp,"#geo %f %f %f %f\n",pfmGeoWest_,pfmGeoNorth_,pfmGeoEast_,pfmGeoSouth_);
  pfmGeoSet_=0;
  if (pfmComment_) fprintf(fp,"#comment\n#%s\n#end\n",pfmComment_);
  pfmComment_=0;
  fprintf(fp,"%d\t%d\n",dx,dy);
  fprintf(fp,"%d\n",(int)255);
  /*  fprintf(fp,"%d\n",(int)maxval); */
  for (x=0; x<dx*3; x++) buffer[x]=0;
  for (y=y1; y<=y2; y++)
    if (y<0 || y>rows-1 || x2<0 || x1>=cols)
      fwrite(emptybuffer,sizeof(unsigned char)*3,dx,fp);
    else if (x1<0) {/* y>0 y<rows */
      fwrite(emptybuffer,sizeof(unsigned char)*3,-x1,fp);
      if (x2<=cols)
        pfm_write_ppm_part(fp,&p_r[y*cols],&p_g[y*cols],&p_b[y*cols],x2+1);
      if (x2>cols){
        pfm_write_ppm_part(fp,&p_r[y*cols],&p_g[y*cols],&p_b[y*cols],cols);
        fwrite(emptybuffer,sizeof(unsigned char)*3,x2-cols+1,fp);
      }
      }
      else if (x2>=cols) { /* y>0 y<rows; not: x1>cols x1<0 => must 0<=x1<cols*/
        pfm_write_ppm_part(fp,&p_r[y*cols+x1],&p_g[y*cols+x1],&p_b[y*cols+x1],cols-x1);
        fwrite(emptybuffer,sizeof(unsigned char*)*3,x2-cols+1,fp);
      }
      else {/* x1>=0 x2<cols y>0 y<rows  */
        pfm_write_ppm_part(fp,&p_r[y*cols+x1],&p_g[y*cols+x1],&p_b[y*cols+x1],x2-x1+1);
      }
}

int pfm_writepfm_region_type(FILE *fp, const void *data,
            int cols, int rows,
            float minval, float maxval,
            int x1, int y1, int x2, int y2,
            int storageType)
{
  int x,y,xi,yi;
  float    *floatbuffer=0;
  double   *doublebuffer=0;
  int      *intbuffer=0;
  uint16   *int16buffer=0;
  grey     *greybuffer=0;
  int dx = x2-x1+1;
  int dy = y2-y1+1;
  float   *fpx;
  double  *dpx;  
  int     *ipx;
  uint16  *i16px;
  grey *gpx;
  if (dx<0 || dy<0) return 0;
  
  switch (storageType) {
  case PFM_FLOAT:
    floatbuffer=(float*)data;
    fpx= calloc(dx,sizeof(float));
     if(!fpx) return 0;
    if (minval>maxval) {
      minval=1/0.0; maxval=-1/0.0;
      y=y1; yi=y2;
      if(y1<0) y=0;
      if(y2>rows) yi=rows;
      for (; y<=yi; y++) {
        x=x1; xi=x2;
        if(x1<0) x=0;
        if(x2>cols) xi=cols;  
        for (; x<=xi; x++)  {
          if (isnan(floatbuffer[y*cols+x])) continue;
          if (floatbuffer[y*cols+x]<minval) minval=floatbuffer[y*cols+x];
          if (floatbuffer[y*cols+x]>maxval) maxval=floatbuffer[y*cols+x];
        }
      }
    }
    fprintf(fp,"F4 # 4 byte float\n");
    if (pfmGeoSet_) fprintf(fp,"#geo %f %f %f %f\n",pfmGeoWest_,pfmGeoNorth_,pfmGeoEast_,pfmGeoSouth_);
    pfmGeoSet_=0;
    if (pfmComment_) fprintf(fp,"#comment\n#%s\n#end\n",pfmComment_);
    pfmComment_=0;
#ifdef WORDS_BIGENDIAN
    fprintf(fp,"B # Big endian\n");
#else
    fprintf(fp,"L # Little endian\n");
#endif
    fprintf(fp,"%d\t%d\n",dx,dy);
    fprintf(fp,"%f\t%f\n",minval,maxval);
    for (x=0; x<dx; x++) fpx[x]=minval;
    for (y=y1; y<=y2; y++)
      if (y<0 || y>rows-1 || x2<0 || x1>=cols)
        fwrite(fpx,sizeof(float),dx,fp);
      else if (x1<0) {/* y>0 y<rows */
        fwrite(fpx,sizeof(float),-x1,fp);
        if (x2<=cols) fwrite(&floatbuffer[y*cols],sizeof(float),x2+1,fp);
        if (x2>cols){
          fwrite(&floatbuffer[y*cols],sizeof(float),cols,fp);
          fwrite(fpx,sizeof(float),x2-cols+1,fp);
        }
      }
      else if (x2>=cols) { /* y>0 y<rows; not: x1>cols x1<0 => must 0<=x1<cols*/
        fwrite(&floatbuffer[y*cols+x1],sizeof(float),cols-x1,fp);
        fwrite(fpx,sizeof(float),x2-cols+1,fp);
      }
      else {/* x1>=0 x2<cols y>0 y<rows  */
        fwrite(&floatbuffer[y*cols+x1],sizeof(float),x2-x1+1,fp);
      }
    free(fpx);
    break;
    

    
  case PFM_DOUBLE:
    doublebuffer=(double*)data;
    dpx = calloc(dx,sizeof(double));
    if(!dpx) return 0;
    if (minval>maxval) {
      minval=1/0.0; maxval=-1/0.0;
      y=y1; yi=y2;
      if(y1<0) y=0;
      if(y2>rows) yi=rows;
      for (; y<=yi; y++) {
        x=x1; xi=x2;
        if(x1<0) x=0;
        if(x2>cols) xi=cols;  
        for (; x<=xi; x++)  {
          if (isnan(doublebuffer[y*cols+x])) continue;
          if (doublebuffer[y*cols+x]<minval) minval=doublebuffer[y*cols+x];
          if (doublebuffer[y*cols+x]>maxval) maxval=doublebuffer[y*cols+x];
        }
      }
    }
    fprintf(fp,"F7 # 8 byte double\n");
    if (pfmGeoSet_) fprintf(fp,"#geo %f %f %f %f\n",pfmGeoWest_,pfmGeoNorth_,pfmGeoEast_,pfmGeoSouth_);
    pfmGeoSet_=0;
    if (pfmComment_) fprintf(fp,"#comment\n#%s\n#end\n",pfmComment_);
    pfmComment_=0;
#ifdef WORDS_BIGENDIAN
    fprintf(fp,"B # Big endian\n");
#else
    fprintf(fp,"L # Little endian\n");
#endif
    fprintf(fp,"%d\t%d\n",dx,dy);
    fprintf(fp,"%f\t%f\n",minval,maxval);
    for (x=0; x<dx; x++) dpx[x]=minval;
    for (y=y1; y<=y2; y++)
      if (y<0 || y>rows-1 || x2<0 || x1>=cols)
        fwrite(dpx,sizeof(double),dx,fp);
      else if (x1<0) {/* y>0 y<rows */
        fwrite(dpx,sizeof(double),-x1,fp);
        if (x2<=cols) fwrite(&doublebuffer[y*cols],sizeof(double),x2+1,fp);
        if (x2>cols){
          fwrite(&doublebuffer[y*cols],sizeof(double),cols,fp);
          fwrite(dpx,sizeof(double),x2-cols+1,fp);
        }
      }
      else if (x2>=cols) { /* y>0 y<rows; not: x1>cols x1<0 => must 0<=x1<cols*/
        fwrite(&doublebuffer[y*cols+x1],sizeof(double),cols-x1,fp);
        fwrite(dpx,sizeof(double),x2-cols+1,fp);
      }
      else {/* x1>=0 x2<cols y>0 y<rows  */
        fwrite(&doublebuffer[y*cols+x1],sizeof(double),x2-x1+1,fp);
      }
    free(dpx);
    break;
        
    
    
  case PFM_SINT:
  case PFM_UINT:
    intbuffer=(int*)data;
    ipx = calloc(dx,sizeof(int));
    if(!ipx) return 0;
    if (minval>maxval) {
      minval=INT_MAX; maxval=INT_MIN;
      y=y1; yi=y2;
      if(y1<0) y=0;
      if(y2>rows)yi=rows;
      for (; y < yi; y++) {
        x=x1; xi=x2;
        if(x1<0) x=0;
        if(x2>cols) xi=cols;  
        for (; x < xi; x++)  {
          if (intbuffer[y*cols+x]<minval) minval=intbuffer[y*cols+x];
          if (intbuffer[y*cols+x]>maxval) maxval=intbuffer[y*cols+x];
        }
      }
    }
    fprintf(fp,"F5 # 32 bit integer\n");
    if (pfmGeoSet_) fprintf(fp,"#geo %f %f %f %f\n",pfmGeoWest_,pfmGeoNorth_,pfmGeoEast_,pfmGeoSouth_);
    pfmGeoSet_=0;
    if (pfmComment_) fprintf(fp,"#comment\n#%s\n#end\n",pfmComment_);
    pfmComment_=0;
#ifdef WORDS_BIGENDIAN
    fprintf(fp,"B # Big endian\n");
#else
    fprintf(fp,"L # Little endian\n");
#endif
    fprintf(fp,"%d\t%d\n",dx,dy);
    fprintf(fp,"%f\t%f\n",minval,maxval);
    for (x=0; x<dx; x++) ipx[x]=minval;
    for (y=y1; y<=y2; y++)
      if (y<0 || y>rows-1 || x2<0 || x1>=cols)
        fwrite(ipx,sizeof(int),dx,fp);
      else if (x1<0) {/* y>0 y<rows */
        fwrite(ipx,sizeof(int),-x1,fp);
        if (x2<=cols) fwrite(&intbuffer[y*cols],sizeof(int),x2+1,fp);
        if (x2>cols) {
          fwrite(&intbuffer[y*cols],sizeof(int),cols,fp);
          fwrite(ipx,sizeof(int),x2-cols+1,fp);
        }
      }
      else if (x2>=cols) { /* y>0 y<rows; not: x1>cols x1<0 => must 0<=x1<cols*/
        fwrite(&intbuffer[y*cols+x1],sizeof(int),cols-x1,fp);
        fwrite(ipx,sizeof(int),x2-cols+1,fp);
      }
      else {/* x1>=0 x2<cols y>0 y<rows  */
        fwrite(&intbuffer[y*cols+x1],sizeof(int),x2-x1+1,fp);
      }
    free(ipx);
    break;
  case PFM_SINT16:
  case PFM_UINT16:
    int16buffer=(uint16*)data;
    i16px = calloc(dx,sizeof(uint16));
    if(!i16px) return 0;
    if (minval>maxval) {
      minval=INT_MAX; maxval=INT_MIN;
      y=y1; yi=y2;
      if (y1<0) y=0;
      if (y2>rows) yi=rows;
      for (; y<=yi; y++) {
        x=x1; xi=x2;
        if(x1<0) x=0;
        if(x2>cols)xi=cols;  
        for (; x<=xi; x++)  {
          if (int16buffer[y*cols+x]<minval) minval=int16buffer[y*cols+x];
          if (int16buffer[y*cols+x]>maxval) maxval=int16buffer[y*cols+x];
        }
      }
    }
    fprintf(fp,"F6 # 16 bit unsigned integer\n");
    if (pfmGeoSet_) fprintf(fp,"#geo %f %f %f %f\n",pfmGeoWest_,pfmGeoNorth_,pfmGeoEast_,pfmGeoSouth_);
    pfmGeoSet_=0;
    if (pfmComment_) fprintf(fp,"#comment\n#%s\n#end\n",pfmComment_);
    pfmComment_=0;
#ifdef WORDS_BIGENDIAN
    fprintf(fp,"B # Big endian\n");
#else
    fprintf(fp,"L # Little endian\n");
#endif
    fprintf(fp,"%d\t%d\n",x2-x1+1,y2-y1+1);
    fprintf(fp,"%f\t%f\n",minval,maxval);
    for (x=0; x<dx; x++) i16px[x]=minval;
    for (y=y1; y<=y2; y++)
      if (y<0 || y>rows-1 || x2<0 || x1>=cols)
        fwrite(i16px,sizeof(uint16),dx,fp);
      else if (x1<0) {/* y>0 y<rows */
        fwrite(i16px,sizeof(uint16),-x1,fp);
        if (x2<=cols) fwrite(&int16buffer[y*cols],sizeof(uint16),x2+1,fp);
        if (x2>cols){
          fwrite(&int16buffer[y*cols],sizeof(uint16),cols,fp);
          fwrite(i16px,sizeof(uint16),x2-cols+1,fp);
        }
      }
      else if (x2>=cols) { /* y>0 y<rows; not: x1>cols x1<0 => must 0<=x1<cols*/
        fwrite(&int16buffer[y*cols+x1],sizeof(uint16),cols-x1,fp);
        fwrite(i16px,sizeof(uint16),x2-cols+1,fp);
      }
      else {/* x1>=0 x2<cols y>0 y<rows  */
        fwrite(&int16buffer[y*cols+x1],sizeof(uint16),x2-x1+1,fp);
      }
    free(i16px);
    break;
  case PFM_BYTE:
    greybuffer=(grey*)data;
    gpx = calloc(dx,sizeof(grey));
    if (minval>maxval) {
      minval=INT_MAX; maxval=INT_MIN;
      y=y1; yi=y2;
      if (y1<0) y=0;
      if (y2>rows) yi=rows;
      if(!gpx) return 0;
      for (; y<=yi; y++) {
        x=x1; xi=x2;
        if(x1<0) x=0;
        if(x2>cols) xi=cols;  
        for (; x<=xi; x++)  {
          if (greybuffer[y*cols+x]<minval) minval=greybuffer[y*cols+x];
          if (greybuffer[y*cols+x]>maxval) maxval=greybuffer[y*cols+x];
        }
      }
    }
    fprintf(fp,"P5 # 8 bit unsigned integer (PGM)\n");
    if (pfmGeoSet_) fprintf(fp,"#geo %f %f %f %f\n",pfmGeoWest_,pfmGeoNorth_,pfmGeoEast_,pfmGeoSouth_);
    pfmGeoSet_=0;
    if (pfmComment_) fprintf(fp,"#comment\n#%s\n#end\n",pfmComment_);
    pfmComment_=0;
    fprintf(fp,"%d\t%d\n",x2-x1+1,y2-y1+1);
    fprintf(fp,"%d\n",(int)255);
  /*  fprintf(fp,"%d\n",(int)maxval); */
    for (x=0; x<dx; x++) gpx[x]=minval;
    for (y=y1; y<=y2; y++)
      if (y<0 || y>rows-1 || x2<0 || x1>=cols)
        fwrite(gpx,sizeof(grey),dx,fp);
      else if (x1<0) {/* y>0 y<rows */
        fwrite(gpx,sizeof(grey),-x1,fp);
        if (x2<=cols) fwrite(&greybuffer[y*cols],sizeof(grey),x2+1,fp);
        if (x2>cols){
          fwrite(&greybuffer[y*cols],sizeof(grey),cols,fp);
          fwrite(gpx,sizeof(grey),x2-cols+1,fp);
        }
      }
      else if (x2>=cols) { /* y>0 y<rows; not: x1>cols x1<0 => must 0<=x1<cols*/
        fwrite(&greybuffer[y*cols+x1],sizeof(grey),cols-x1,fp);
        fwrite(gpx,sizeof(grey),x2-cols+1,fp);
      }
      else {/* x1>=0 x2<cols y>0 y<rows  */
        fwrite(&greybuffer[y*cols+x1],sizeof(grey),x2-x1+1,fp);
      }
    free(gpx);
    break;
  case PFM_3BYTE:
    pfm_write_ppm(fp, (const PFM3Byte*)data, cols, rows, minval, maxval,
                       x1, y1, x2, y2, dx, dy);
    break;
  }
  return 1;
}

int pfm_writepfm_region(FILE *fp, const float *data,
       int cols, int rows,
       float minval, float maxval,
       int x1, int y1, int x2, int y2)
{
  return pfm_writepfm_region_type(fp, data,
          cols, rows,
          minval, maxval,
          x1, y1, x2, y2, PFM_FLOAT);
}

int pfm_writepfm_type(FILE *fp, const void *data,
           int cols, int rows,
           float minval, float maxval,
           int storageType)
{
  return pfm_writepfm_region_type(fp,data,cols,rows,minval,maxval,
         0,0,cols-1,rows-1,storageType);
}

int pfm_writepfm(FILE *fp, const float *data,
      int cols, int rows,
      float minval, float maxval)
{
  return pfm_writepfm_region(fp,data,cols,rows,minval,maxval,0,0,cols-1,rows-1);
}


int pfm_readpfm_size(FILE *fp,
          int *cols, int *rows)
{
  char buffer[1024];
  pfmGeoRead_=0;
  if (pfmCommentRead_) free(pfmCommentRead_);
  pfmCommentRead_=0;
  pfmGeoSet_=0;
  pfmComment_=0;
  fp=check_compression(buffer,fp);
#ifdef DEBUG
  printf("pfm_readpfm_size: %s\n",buffer);
#endif
  if (!((buffer[0]=='F' && buffer[1]>='0' && buffer[1]<='7')
    || (buffer[0]=='P' && buffer[1]>='1' && buffer[1]<='6'))) {
    fprintf(stderr,"pfm_readpfm: Wrong fileformat! (F)\n");
    fp=stop_decompression(fp);
    return 0;
  }

  read_buffer(buffer,1023,fp);
  fgets(buffer,1023,fp);
  sscanf(buffer,"%d %d",cols,rows);
  fp=stop_decompression(fp);
  return 1;
}    

int pfm_readpfm_header(FILE *fp,
      int *cols, int *rows,
      float *minval, float *maxval,
      int *storageType)
{
  long filepos;
  char buffer[1024];
  int cols_, rows_;
  float minval_, maxval_;
  int type;
  int byte_order;
  pfmGeoRead_=0;
  if (pfmCommentRead_) free(pfmCommentRead_);
  pfmCommentRead_=0;
  pfmGeoSet_=0;
  pfmComment_=0;
  filepos=ftell(fp);
  fp=check_compression(buffer,fp);
#ifdef DEBUG
  printf("pfm_readpfm_header: %s\n",buffer);
#endif
  switch (buffer[0]) {
  case 'F':
    switch (buffer[1]) {
    case '0':
      type=PFM_FLOAT_ASCII;
      break;
    case '1':
      type=PFM_INT_ASCII;
      break;
    case '2':
      type=PFM_INT16_ASCII;
      break;
    case '3':
      type=PFM_DOUBLE_ASCII;
      break;
    case '4':
      type=PFM_FLOAT_BIN;
      break;
    case '5':
      type=PFM_INT_BIN;
      break;
    case '6':
      type=PFM_INT16_BIN;
      break;
    case '7':
      type=PFM_DOUBLE_BIN;
      break;
    default:
      fprintf(stderr,"pfm_readpfm: Wrong fileformat!\n");
      fp=stop_decompression(fp);
      fseek(fp,filepos,SEEK_SET);
      return 0;
    }
    break;
  case 'P':
    switch (buffer[1]) {
    case '1':
      type=PFM_PBM_ASCII;
      break;
    case '2':
      type=PFM_PGM_ASCII;
      break;
    case '3':
      type=PFM_PPM_ASCII;
      break;
    case '4':
      type=PFM_PBM_BIN;
#if 0      
      fprintf(stderr,"pfm_readpfm: PBM not supported by libpfm, use libpbm!\n");
#endif      
      fp=stop_decompression(fp);
       fseek(fp,filepos,SEEK_SET);
      return 0;
      break;
    case '5':
      type=PFM_PGM_BIN;
      break;
    case '6':
      type=PFM_PPM_BIN;
#if 0      
      fprintf(stderr,"pfm_readpfm: PPM not supported by libpfm, use libppm!\n");
      fp=stop_decompression(fp);
       fseek(fp,filepos,SEEK_SET);
      return 0;
#endif      
      break;
    default:
      fprintf(stderr,"pfm_readpfm: Wrong fileformat!\n");
      fp=stop_decompression(fp);
      fseek(fp,filepos,SEEK_SET);
      return 0;
    }
    break;
  default:
    fprintf(stderr,"pfm_readpfm: Wrong fileformat!\n");
    fp=stop_decompression(fp);
     fseek(fp,filepos,SEEK_SET);
    return 0;
  }
  if ((type>PFM_SINT16_ASCII) && (type<PFM_PGM_ASCII)) {
     read_buffer(buffer,1023,fp);
     if (buffer[0] == 'L') {
      byte_order=PFM_LittleEndian;
    }
     else {
      byte_order=PFM_BigEndian;
    }
  }
  read_buffer(buffer,1023,fp);
  sscanf(buffer,"%d %d",&cols_,&rows_);
  read_buffer(buffer,1023,fp);
  pfm_mult=1.0;
  pfm_offset=0.0;
  if (sscanf(buffer,"%f %f %f %f",&minval_,&maxval_,&pfm_mult,&pfm_offset)<2) {
    maxval_=minval_;
    minval_=0;
  }
  if (type==PFM_PGM_BIN && maxval_>255) type=PFM_INT16_BIN;
  if (type==PFM_PGM_ASCII && maxval_>255 && maxval_<=65535) type=PFM_INT16_ASCII;

  if (storageType) {
    switch (type) {
    case PFM_FLOAT_ASCII:
    case PFM_FLOAT_BIN:
      *storageType=PFM_FLOAT;
      break;
    case PFM_DOUBLE_ASCII:
    case PFM_DOUBLE_BIN:
      *storageType=PFM_DOUBLE;
      break;
    case PFM_INT_ASCII:
    case PFM_INT_BIN:
      *storageType=PFM_SINT;
      break;
    case PFM_INT16_ASCII:
    case PFM_INT16_BIN:
      *storageType=PFM_UINT16;
      break;
    case PFM_SINT16_ASCII:
    case PFM_SINT16_BIN:
      *storageType=PFM_SINT16;
      break;
    case PFM_PGM_ASCII:
    case PFM_PGM_BIN:
      *storageType=PFM_BYTE;
      break;
    case PFM_PBM_ASCII:
    case PFM_PBM_BIN:
      *storageType=PFM_BIT;
      break;
    case PFM_PPM_ASCII:
    case PFM_PPM_BIN:
      *storageType=PFM_3BYTE;
      break;
    }
#ifdef DEBUG
    printf("storageType=%d\n",*storageType);
#endif
  }
  if (minval) *minval=minval_;
  if (maxval) *maxval=maxval_;
  if (cols) *cols=cols_;
  if (rows) *rows=rows_;
#ifdef DEBUG
  printf("cols=%d, rows=%d, minval=%f, maxval=%f\n",cols_,rows_,minval_,maxval_);
#endif
  fp=stop_decompression(fp);
  fseek(fp,filepos,SEEK_SET);
  return 1;
}

void pfm_geo_set(double geoWest, double geoNorth, double geoEast, double geoSouth)
{
  pfmGeoWest_=geoWest;
  pfmGeoNorth_=geoNorth;
  pfmGeoEast_=geoEast;
  pfmGeoSouth_=geoSouth;
  pfmGeoSet_=1;
  pfmGeoRead_=0;
}

int pfm_geo_get(double* geoWest, double* geoNorth, double* geoEast, double* geoSouth)
{
  if (pfmGeoRead_) {
    *geoWest=pfmGeoWest_;
    *geoNorth=pfmGeoNorth_;
    *geoEast=pfmGeoEast_;
    *geoSouth=pfmGeoSouth_;
    return 1;
  }
  else return 0;
}


void pfm_comment_set(const char* comment)
{
  pfmComment_=comment;
}

const char* pfm_comment_get()
{
  return pfmCommentRead_;
}

