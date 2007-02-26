#include <stdio.h>
#include "PFMArray.h"
#include "PFMDiff.h"
//#include "PFMSqrDiff.h"
#include "PFMGradAbs.h"
#include "PFMGradAngle.h"

void Usage()
{
  printf("Usage:\n"
	 "  maxdiff <source> <destination-abs> <destination-angle>\n");
}


int main(int argc, char **argv)
{
  if (argc<4)
    {
      Usage();
      return 0;
    }

  PFMArray *pic=new PFMArray(argv[1]);
  if (!pic->valid())
    {
      printf("Picture %s not read!\n",argv[1]);
    }
  PFMGradAbs *pic1=new PFMGradAbs(*pic);
  PFMGradAngle *pic2=new PFMGradAngle(*pic);
  pic1->write(argv[2]);
  pic2->write(argv[3]);
  
  delete pic;
  delete pic1;
  delete pic2;
}
