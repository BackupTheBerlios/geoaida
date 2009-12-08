/***************************************************************************
*                                                                         *
*   This program is free software; you can redistribute it and/or modify  *
*   it under the terms of the GNU General Public License as published by  *
*   the Free Software Foundation; either version 2 of the License, or     *
*   (at your option) any later version.                                   *
*                                                                         *
***************************************************************************/

////////////////////////////////////////////////////////////////////////////////
///
/// \mainpage   Create Region description from label image
/// \author     Torsten Büschenfeld (bfeld@tnt.uni-hannover.de)
///
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
///
/// \file       main.cpp
/// \brief      Main program
///
/// \date       2009-12-07
/// \author     Torsten Büschenfeld (bfeld@tnt.uni-hannover.de)
///
////////////////////////////////////////////////////////////////////////////////

//--- Standard header --------------------------------------------------------//
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

//--- Program header ---------------------------------------------------------//
#include "garegionsplitter_multiclass.h"

//--- Misc header ------------------------------------------------------------//

using namespace std;

//--- Major glocal instances -------------------------------------------------//

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Prints out usage message if command was called incorrectly
///
////////////////////////////////////////////////////////////////////////////////
void usage()
{
    std::cout << std::endl;
    std::cout << "USAGE: label2regiondescr" << std::endl;
    std::cout << "         Input label image" << std::endl;
    std::cout << "         Output label image" << std::endl;
    std::cout << "         Class name mapping" << std::endl;
    std::cout << "           - file containing" << std::endl;
    std::cout << "               label1, name1" << std::endl;
    std::cout << "               label2, name2" << std::endl;
    std::cout << "               ..." << std::endl;
    std::cout << "         Region description output" << std::endl;
    std::cout << "         Region creation mode" << std::endl;
    std::cout << "           - 0: normal mode, creating context regions." << std::endl;
    std::cout << "           - 1: simple mode, creating one region for each label." << std::endl;
    std::cout << std::endl;
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Main function
///
/// This is the entrance point for program startup.
///
/// \param  argc number of given arguments
/// \param  argv array, storing the arguments
/// \return exit code
///
////////////////////////////////////////////////////////////////////////////////
int main(int argc, char *argv[])
{
    std::string     strClassMapperFile;
    std::string     strInputLabelImage;
    std::string     strOutputLabelImage;
    std::string     strRegionDescrOutput;
    ClassnameMapper Mapper;
    int             nMode;
    
    const int MODE_CONTEXT=0;
    const int MODE_SIMPLE =1;
    
    if (argc != 6)
    {
        usage();
    }
    else
    {
        strInputLabelImage   = argv[1];
        strOutputLabelImage  = argv[2];
        strRegionDescrOutput = argv[4];
        strClassMapperFile   = argv[3];
        nMode                = atoi(argv[5]);
        
        std::cout << "label2regiondescr: parameters: " << std::endl;
        std::cout << "  Input label image:         " << strInputLabelImage << std::endl;
        std::cout << "  Output label image:        " << strOutputLabelImage << std::endl;
        std::cout << "  Class mapper file:         " << strClassMapperFile << std::endl;
        std::cout << "  Region description output: " << strRegionDescrOutput << std::endl;
        std::cout << "  Mode:                      " << nMode << std::endl;
        
        // Read class mapping
        ifstream in(strClassMapperFile.c_str());
      
        if (!in)
        {
            std::cerr << "label2regiondescr: ERROR, can't open class mapper file " << strClassMapperFile << std::endl;
            exit(EXIT_FAILURE);
        }
        std::cout << "label2regiondescr: Class mapping:" << std::endl;
        while (!in.eof())
        {
            int         nClassId;
            std::string strClass;
            
            if (in >> nClassId)
            {
                std::cout << "  " << nClassId << " ";
                // Class name requires previous class id, therefore query is nested here
                if (in >> strClass)
                {
                    std::cout << strClass << std::endl;
                    Mapper[nClassId] = strClass;
                }
                else
                {
                    std::cout << "???" << std::endl;
                    std::cerr << "label2regiondescr: ERROR, syntax error in class mapper file. "
                                 "Class name without class id." << std::endl;
                    return EXIT_FAILURE;
                }
            }
        }
        
        Ga::Image LabelImage;
        if (!LabelImage.read(strInputLabelImage.c_str()))
        {
            std::cerr << "label2regiondescr: ERROR, can't read input image "
                      << strInputLabelImage << std::endl;
            return EXIT_FAILURE;
        }
        
        // Everything's prepared, now do the labelling
        switch (nMode)
        {
            case MODE_CONTEXT:
                relabelOutput(LabelImage, Mapper, strOutputLabelImage, strRegionDescrOutput);
                break;
            case MODE_SIMPLE:
                simpleOutput(LabelImage, Mapper, strOutputLabelImage, strRegionDescrOutput);
                break;
            default:
                std::cerr << "label2regiondescr: ERROR, unknown mode: " << nMode << std::endl;
                usage();
                return EXIT_FAILURE;
        }
    }
    
    return EXIT_SUCCESS;
}
