/***************************************************************************
                          gi_canny.cpp  -  description
                             -------------------
    begin                : Thu Feb 28 2007
    copyright            : (C) 2007 by Julian Raschke
    email                : raschke@tnt.uni-hannover.de
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include <gaimage.h>
#include <gacanny.h>
using namespace Ga;

int main(int argc, char **argv)
{
    if (argc < 5) {
        printf("Usage:\n  %s <input-file> <output-file> <low-threshold> <high-threshold>\n", argv[0]);
        return EXIT_FAILURE;
    }
    
    const char* inputfile = argv[1];
    const char* outputfile = argv[2];
    double lowTh = atof(argv[3]);
    double highTh = atof(argv[4]);
    
    Image input;
    input.read(inputfile);
    if (input.isEmpty()) {
        fprintf(stderr,"Can't open inputfile %s\n", inputfile);
        return -1;
    }

    canny(input, lowTh, highTh).write(outputfile, 0);
}
