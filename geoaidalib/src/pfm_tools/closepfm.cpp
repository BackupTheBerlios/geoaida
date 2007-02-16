/***************************************************************************
                          closepfm.cpp  -  description
                             -------------------
    begin                : Thu Feb  8 10:21:00 MET 2001
    copyright            : (C) 2000 by jürgen bückner
    email                : bueckner@tnt.uni-hannover.de
 ***************************************************************************/

#include "Array2DClose.h"

void Usage(char* prg)
{
  fprintf(stderr,"Usage:\n"
	 "  %s [-l] <source> <destination>\n "
	 "     close iterative all NAN values in PFM-file 'source'.\n"
   "     result is written in PFM-file 'destination'."
	 "     use -l for label images\n\n", prg);
  exit(1);
}

int main(int argc, char **argv)
{
  if (argc<3)
    {
      Usage(argv[0]);
      return 0;
    }
  
	int arg = 1;

	if (!strcmp(argv[1], "-l"))	
		arg++;

  Array2D<float> *dst=new Array2DClose<float>(argv[arg], (arg == 2) ? 1 : 0 );
  dst->write(argv[arg+1]);
}
