#include <stdio.h>
#include <stdlib.h>

void Usage(const char* prg) 
{
   printf("Usage:\n"
	  "  %s <infile> <outfile> <west> <north> <east> <south>\n",prg);
   exit(1);
}

int main(int argc, char** argv)
{
#define BUF_SIZE 10*1024*1024
  double gW,gN,gE,gS;
  FILE *ifp,*ofp;
  unsigned char *buffer;
  int count, num_bytes;
  if (argc<7) {
    Usage(argv[0]);
  }
   
  sscanf(argv[3],"%lf",&gW);
  sscanf(argv[4],"%lf",&gN);
  sscanf(argv[5],"%lf",&gE);
  sscanf(argv[6],"%lf",&gS);
  ifp=fopen(argv[1],"r");
  if (!ifp) {
    fprintf(stderr,"Can't input %s\n",argv[1]);
    exit(1);
  }
  buffer=(unsigned char*)malloc(sizeof(char)*(BUF_SIZE+1));
  fgets(buffer,BUF_SIZE,ifp);
  if (buffer[0]!='F' && buffer[0]!='P') {
     fprintf(stderr,"Wrong fileformat %s\n",argv[1]);
     fclose(ifp);
     exit(1);
  }
  ofp=fopen(argv[2],"w");
  if (!ifp) {
    fprintf(stderr,"Can't output %s\n",argv[2]);
    exit(1);
  }
  fputs(buffer,ofp);
  printf("Writing geo %f %f %f %f\n",gW,gN,gE,gS);
  fprintf(ofp,"#geo %f %f %f %f\n",gW,gN,gE,gS);
   
  fgets(buffer,BUF_SIZE,ifp);
  if (buffer[0]=='#' && buffer[1]=='g' && buffer[2]=='e' && buffer[3]=='o')
    printf("Found old %sRemoving\n",buffer);
  else
    fputs(buffer,ofp);
  printf("Copying data %d MB blocks\n",BUF_SIZE/1024/1024);
  count=0;
  while (!feof(ifp)) {
    printf("\r%4d",++count);
    fflush(stdout);
    num_bytes=fread(buffer,sizeof(char),BUF_SIZE,ifp);
    fwrite(buffer,sizeof(char),num_bytes,ofp);
  }
  printf("\n");
  fclose(ifp);
  fclose(ofp);
}
