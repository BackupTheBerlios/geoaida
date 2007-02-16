#include "Array2DAbs.h"

void Usage()
{
  printf("Usage:\n"
	 "  abspfm <source> <destination>\n");
}



int main(int argc, char **argv)
{
  if (argc<3)
    {
      Usage();
      return 0;
    }
  Array2D<float> *src=new Array2D<float>(argv[1]);
  Array2D<float> *dst=new Array2DAbs<float>(*src);
  dst->write(argv[2]);
}
