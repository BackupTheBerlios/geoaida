using namespace std;

#include "Array2DMedian.h"

void Usage()
{
  printf("Usage:\n"
	 "  medianpfm <source> <destination> <filtersizex> <filtersizey> [<pickElementNo>]\n");
}



int main(int argc, char **argv)
{
  if (argc<5)
    {
      Usage();
      return 0;
    }
  int fsizex, fsizey, pick;
  sscanf(argv[3],"%d",&fsizex);
  sscanf(argv[4],"%d",&fsizey);
  if (argc>=6)
    sscanf(argv[5],"%d",&pick);
  else
    pick=-1;
  Array2D<float> *src=new Array2D<float>(argv[1]);
  Array2D<float> *dst=new Array2DMedian<float>(*src,fsizex,fsizey,pick);
  dst->write(argv[2]);
}
