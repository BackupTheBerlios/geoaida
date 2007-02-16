#include "Array2DFilter.h"

void Usage()
{
  printf("Usage:\n"
	 "  filterpfm <source> <filter> <destination>\n");
}



int main(int argc, char **argv)
{
#if 1
  if (argc<4)
    {
      Usage();
      return 0;
    }
  Array2D<float> *src=new Array2D<float>(argv[1]);
  Array2D<float> *matrix=new Array2D<float>(argv[2]);
  Array2D<float> *dst=new Array2DFilter<float>(*src,*matrix);
  dst->write(argv[3]);
#endif
#if 0
  Array2D<float> *src=new Array2D<float>("/project/geoaida/projects/ravensburg/ausschnitt/vis_ausschnitt.ppm");
  Array2D<float> *matrix=new Array2D<float>("/project/geoaida/projects/ravensburg/ausschnitt/grad_3_x.pfm");
  Array2D<float> *dst=new Array2DFilter<float>(*src,*matrix);
  dst->write("test.pfm");
#endif
  //  Array2DFilter<float>
}
