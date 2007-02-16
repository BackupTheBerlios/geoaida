using namespace std;
#include "Array2DMax.h"

void Usage()
{
  printf("Usage:\n"
	 "  maxpfm <source> <destination> <filtersizex> <filtersizey>\n");
}



int main(int argc, char **argv)
{
  if (argc<5)
    {
      Usage();
      return 0;
    }
  int fsizex, fsizey;
  sscanf(argv[3],"%d",&fsizex);
  sscanf(argv[4],"%d",&fsizey);
  Array2D<float> *src=new Array2D<float>(argv[1]);
  Array2D<float> *dst=new Array2DMax<float>(*src,fsizex,fsizey);
  dst->write(argv[2]);
}
