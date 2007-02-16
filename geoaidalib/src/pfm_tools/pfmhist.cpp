#include "gaimage.h"

using namespace Ga;

void Usage()
{
  printf("Usage:\n"
	 "  pfmhist <source> <destination> <min> <max> <step> [<x1> <y1> <x2> <y2>]\n");
}



int main(int argc, char **argv)
{
  if (argc<6)
    {
      Usage();
      return 0;
    }
  float min, max, step;
  int x1, y1, x2, y2;
  sscanf(argv[3],"%f",&min);
  sscanf(argv[4],"%f",&max);
  sscanf(argv[5],"%f",&step);
  Image src;
  src.read(argv[1]);
  if (argc>6) {
    sscanf(argv[6],"%d",&x1);
    sscanf(argv[7],"%d",&y1);
    sscanf(argv[8],"%d",&x2);
    sscanf(argv[9],"%d",&y2);
  }
  else {
    x1=0;
    y1=0;
    x2=src.sizeX()-1;
    y2=src.sizeY()-1;
  }
  if (max<min) exit(0);
  int x,y;
  int num_classes=abs(int((max-min)/step));
  double factor=(num_classes)/(max-min);
  int hist[num_classes];
  {
    int cno; 
    for (cno=0; cno<num_classes; cno++)
      hist[cno]=0;
  }
  int error=0;
  for (y=y1; y<=y2; y++)
    for (x=x1; x<=x2; x++) {
      float val=src.getFloat(x,y);
      if (isnan(val)) { error++; continue;}
      if (val<min) { error++; continue;}
      if (val>max) { error++; continue;}
      int cno=int((val-min)*factor);
      if ((cno<0) || (cno>=num_classes)) { error++; continue;}
      hist[cno]++;
    }
  {
    FILE *fp=fopen(argv[2],"wt");
    if (!fp) 
      {
	fprintf(stderr,"Can't open outputfile %s\n",argv[2]);
	Usage();
	return -1;
      }
    fprintf(fp,"# Histogramm filename min max step x1 y1 x2 y2\n");
    fprintf(fp,"# %s %f %f %f %d %d %d %d\n",
	    argv[1],min,max,step,x1,y1,x2,y2);
    fprintf(fp,"# %d not classified\n",error);
    int cno;
    int num_points=(x2-x1+1)*(y2-y1+1);
    for (cno=0; cno<num_classes; cno++)
      {
	fprintf(fp,"%7d\t%20f\t%7d\t%20f\n",
		cno,min+cno*step+step/2,
		hist[cno],float(hist[cno])/num_points);
      }
    fclose(fp);
  }
}
