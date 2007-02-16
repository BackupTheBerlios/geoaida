/*
 * $RCSfile: tri2pfm.c,v $
 * $Source: /data/cvs/geoaidalib/geoaida/pfm_tools/tri2pfm.c,v $
 * $Revision: 1.1 $
 * $Date: 2000/12/13 12:45:53 $
 * $Author: bueckner $
 * $Locker:  $
 */
#include <stdio.h>
#include <stdlib.h>
#include "pfm.h"

void Usage()
{
  fprintf(stderr,
	  "Triangle points to PFM\n"
	  "Usage:\n"
	  "  tri2pfm <infile> <outfile> <xdim> <ydim>  <heightoffset> <heigtfactor>\n");
  exit(0);
}

//===================================================
// Berechnet die Hoehenwerte der Dreiecksflaechen:
//===================================================
void SwapPoints(float *x1, float *y1, float *z1, float *x2, float *y2, float *z2)
{
  float t;
  t=*x1; *x1=*x2; *x2=t;
  t=*y1; *y1=*y2; *y2=t;
  t=*z1; *z1=*z2; *z2=t;
}

void CalcPlain(float *pic, int xsize, int ysize,
	  float x1, float y1, float z1,
	  float x2, float y2, float z2,
	  float x3, float y3, float z3,
	  float minval, float maxval)
{
  /*float dmx1, dmy1, dmx2, dmy2, dmx3, dmy3;*/
  float xmin, ymin, xmax, ymax;
  int xmini, ymini, xmaxi, ymaxi;
  int x,y;
  double a,b,c;
  double dx21,dy21,dz21,dx31,dy31,dz31,dx32,dy32;
  double n2;
  float u,l;

#ifdef DEBUG
  printf("t:(%7.3f %7.3f %7.3f) (%7.3f %7.3f %7.3f) (%7.3f %7.3f %7.3f)\n",
		 x1,y1,z1,
		 x2,y2,z2,
		 x3,y3,z3);
#endif
  
  if (y1<y2)
    SwapPoints(&x1,&y1,&z1,&x2,&y2,&z2);
  else 
    if (y1==y2)
      if (x1<x2)
	  SwapPoints(&x1,&y1,&z1,&x2,&y2,&z2);
  if (y1<y3)
    SwapPoints(&x1,&y1,&z1,&x3,&y3,&z3);
  else 
    if (y1==y3)
      if (x1<x3)
	SwapPoints(&x1,&y1,&z1,&x3,&y3,&z3);
  if (y2>y3)
    SwapPoints(&x2,&y2,&z2,&x3,&y3,&z3);
  else 
    if (y2==y3)
      if (x2<x3)
	SwapPoints(&x2,&y2,&z2,&x3,&y3,&z3);
  
#define max(x1,x2) ((x1>x2) ? (x1) : (x2))
#define min(x1,x2) ((x1<x2) ? (x1) : (x2))
  xmin=min(x1,min(x2,x3));
  ymin=min(y1,min(y2,y3));
  xmax=max(x1,max(x2,x3));
  ymax=max(y1,max(y2,y3));
#undef max
#undef min

  xmini=(int)xmin;
  ymini=(int)ymin;
  xmaxi=-((int)-xmax);
  ymaxi=-((int)-ymax);

  dx21=x2-x1;
  dy21=y2-y1;
  dz21=z2-z1;
  dx31=x3-x1;
  dy31=y3-y1;
  dz31=z3-z1;
  dx32=x3-x2;
  dy32=y3-y2;

  /* z=a*x+b*y+c */
  a=(dz31*dy21-dz21*dy31)/(dx31*dy21-dx21*dy31);
  b=(dz31-dx31*a)/dy31;
  c=z3-x3*a-y3*b;

  
#ifdef DEBUG
  printf("t:(%7.3f %7.3f %7.3f) (%7.3f %7.3f %7.3f) (%7.3f %7.3f %7.3f)\n",
		 x1,y1,z1,
		 x2,y2,z2,
		 x3,y3,z3);
#endif

  n2=dx31*dy21-dx21*dy31;
#ifdef DEBUG
  printf("t:(%7.3f %7.3f %7.3f) (%7.3f %7.3f %7.3f) (%7.3f %7.3f %7.3f)\n",
	 x1,y1,z1,
	 x2,y2,z2,
	 x3,y3,z3);
  printf("t:min=(%4d %4d) max=(%4d %4d)\n",
	 xmini,ymini,xmaxi,ymaxi);
  printf("v2(%6.1lf, %6.1lf) v3(%6.1lf, %6.1lf)\n",
	 dx21,dy21,dx31,dy31);
  printf(" d21(%7.3lf, %7.3lf, %7.3lf) d31(%7.3lf, %7.3lf, %7.3lf) d32(%7.3lf, %7.3lf, %7.3lf)\n",
	 dx21,dy21,dz21,dx31,dy31,dz31,dx32,dy32,0);
  printf(" a=%7.3lf, b=%7.3lf, c=%7.3lf\n",a,b,c);
#endif  
  for (y=ymini; y<=ymaxi; y++)
    for (x=xmini; x<=xmaxi; x++)
      {
	u=((x-x1)*dy21-dx21*(y-y1))/n2;
	if (dx21==0.0)
	  l=(y-y1-u*dy31)/dy21;
	else
	  l=(x-x1-u*dx31)/dx21;
#ifdef DEBUG
      	printf("(%4d,%4d): u=%6.3lf, l=%6.3lf: ",x,y,u,l);
#endif
	if ((l>=0.0) && (u>=0.0) && ((u+l)<=1.0))
	  pic[x+y*xsize]=a*x+b*y+c;
      }
#ifdef DEBUG
  pic[(int)(x1+(dx31+dx21)/4)+(int)(y1+(dy31+dy21)/4)*xsize]=1000;
#endif  

}

#define ToPic(p)       \
  {                        \
    p.x=(p.x+coffset)*cfactor; \
    p.y=(p.y+roffset)*rfactor; \
    p.z=(p.z+hoffset)*hfactor; \
  }

int main(int argc, char **argv)
{
  FILE *infile, *outfile;
  int cols, rows;
  int c;
  float cfactor, rfactor;
  float coffset,roffset;
  float cmin, cmax, rmin, rmax;
  float hfactor,hoffset;
  float *pic, minval, maxval;
  struct {
      float x,y,z;
  } p1,p2,p3;

  if ( argc != 7) Usage();

  sscanf(argv[3],"%d",&cols);
  sscanf(argv[4],"%d",&rows);
  sscanf(argv[5],"%f",&hoffset);
  sscanf(argv[6],"%f",&hfactor);
  infile=fopen(argv[1],"r");
  if (!infile) {
      fprintf(stderr,"Can't open file %s for reading!\n",argv[1]);
      exit(0);
    }
  outfile=fopen(argv[2],"w");
  if (!outfile) {
      fprintf(stderr,"Can't open file %s for writing!\n",argv[2]);
      exit(0);
    }

  cmin=1e30; cmax=-1e30; 
  rmin=1e30; rmax=-1e30; 
  minval=1e30; maxval=-1e30;
  while (!feof(infile)) {
    if (fscanf(infile,"%f %f %f\n",&p1.x,&p1.y,&p1.z)==3) {
#ifdef DEBUG
	  printf("s:(%6.1f %6.1f %6.1f)\n",
		 p1.x,p1.y,p1.z);
#endif
	  if (p1.x>cmax) cmax=p1.x;
	  if (p1.x<cmin) cmin=p1.x;
	  if (p1.y>rmax) rmax=p1.y;
	  if (p1.y<rmin) rmin=p1.y;
	  if (p1.z>maxval) maxval=p1.z;
	  if (p1.z<minval) minval=p1.z;
	  }
  }
  fclose(infile);
  coffset=-cmin;
  roffset=-rmin;
  cfactor=(cols-1)/(cmax-cmin);
  rfactor=(rows-1)/(rmax-rmin);
  fprintf(stderr,
	  "min=(%6.1f,%6.1f,%6.1f), max=(%6.1f,%6.1f,%6.1f), "
	  "offset=(%6.1f,%6.1f), factor=(%6.2f,%6.2f) hoffs=%f hfac=%f\n",
	  cmin,rmin,minval,cmax,rmax,maxval,coffset,roffset,
	  cfactor,rfactor,hoffset,hfactor);
  infile=fopen(argv[1],"r");
  pic=(float*)calloc(cols*rows,sizeof(float));
  while (!feof(infile)) {
      if (fscanf(infile,"%f %f %f\n%f %f %f\n%f %f %f\n",
		 &p1.x,&p1.y,&p1.z,
		 &p2.x,&p2.y,&p2.z,
		 &p3.x,&p3.y,&p3.z)==9)
	{
#ifdef DEBUG
	  printf("s:(%6.1f %6.1f %6.1f) (%6.1f %6.1f %6.1f) (%6.1f %6.1f %6.1f)\n",
		 p1.x,p1.y,p1.z,
		 p2.x,p2.y,p2.z,
		 p3.x,p3.y,p3.z);
#endif
          ToPic(p1);
          ToPic(p2);
          ToPic(p3);
#ifdef DEBUG
	  printf("d:(%6.1f %6.1f %6.1f) (%6.1f %6.1f %6.1f) (%6.1f %6.1f %6.1f)\n",
		 p1.x,p1.y,p1.z,
		 p2.x,p2.y,p2.z,
		 p3.x,p3.y,p3.z);
#endif
	  CalcPlain(pic,  cols, rows,
		    p1.x,p1.y,p1.z,
		    p2.x,p2.y,p2.z,
		    p3.x,p3.y,p3.z,
		    minval,maxval);
	}
     }
  fclose(infile);
  maxval=-1e30; minval=1e30;
  for (c=0; c<rows*cols; c++) {
    if (pic[c]>maxval)
	    maxval=pic[c];
    if (pic[c]<minval)
	    minval=pic[c];
  }
  fprintf(stderr,"minval=%6.1f, maxval=%6.1f\n",minval,maxval);
#ifdef DEBUG
  for (r=0; r<rows; r++) {
      printf("%3d: ",r);
      for (c=0; c<cols; c++)
	printf("%6.1f ",pic[c+r*cols]);
      printf("\n");
    }
#endif
  pfm_writepfm(outfile,pic,cols,rows,minval,maxval);
  fclose(outfile);
  return 1;
}


