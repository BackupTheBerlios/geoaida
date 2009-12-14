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
/// \mainpage	Project SVM_Operator
/// \author		Torsten Büschenfeld (bfeld@tnt.uni-hannover.de)
///
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
///
/// \file		main.cpp
/// \brief		Main program
///
/// \date		2009-06-29
/// \author		Torsten Büschenfeld (bfeld@tnt.uni-hannover.de)
///
////////////////////////////////////////////////////////////////////////////////

//--- Standard header --------------------------------------------------------//
#include <cstdlib>
#include <string>

//--- Program header ---------------------------------------------------------//
#include "feature_extractor.h"
#include "log.h"
#include "otb_svmop_common.h"
#include "svm_classifier.h"

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
    METHOD_ENTRY("usage()");
    INFO(

    std::cout << "svm_class" << std::endl;
    std::cout << "          number of input channels" << std::endl;
    std::cout << "          input channel 1" << std::endl;
    std::cout << "          ..." << std::endl;
    std::cout << "          input channel n" << std::endl;
    std::cout << "          Path to SVM configuration" << std::endl;
    std::cout << "          SVM model" << std::endl;
    std::cout << "          SVM scaling" << std::endl;
    std::cout << "          output file name" << std::endl;

    );
    METHOD_EXIT("usage()");
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Main function
///
/// This is the entrance point for program startup.
///
/// \param	argc number of given arguments
/// \param	argv array, storing the arguments
/// \return	exit code
///
////////////////////////////////////////////////////////////////////////////////
int main(int argc, char *argv[])
{
    METHOD_ENTRY("main");

    Log.setBreak(120u);

    FeatureExtractor    Extractor;
    SVMClassifier       Classifier;
    int                 nNumberOfChannels;
    std::vector<std::string> ArgvList;

    for (int i=1; i<argc; ++i)
    {
        ArgvList.push_back(argv[i]);
    }
    if (ArgvList.size() > 0)
    {
        nNumberOfChannels = atoi(ArgvList[0].c_str());
        if (ArgvList.size() == nNumberOfChannels+5)
        {
            //--- Start the classification process -------------------------------//
            for (int i=1; i<nNumberOfChannels+1; ++i)
            {
                Extractor.addInputChannel(ArgvList[i]);
            }
            Extractor.setNumberOfPyramidLevels(4);
            Extractor.setPyramidDecimationRate(1.75);
            if (!Extractor.extract()) return EXIT_FAILURE;
            if (!Classifier.loadModel(ArgvList[nNumberOfChannels+1]+ "/" +
                                      ArgvList[nNumberOfChannels+2])) return EXIT_FAILURE;
            if (!Classifier.loadScaling(ArgvList[nNumberOfChannels+1]+ "/" +
                                        ArgvList[nNumberOfChannels+3])) return EXIT_FAILURE;
            Classifier.setLabelImageSize(Extractor.getImageSize());
            Extractor.clearChannels(); // Free some memory!
            Classifier.setFeatures(Extractor.getFeatures());
            Classifier.scaleFeatures();
            Classifier.classify();
            Classifier.saveClassificationResult(ArgvList[nNumberOfChannels+4]);
        }
        else
        {
            ERROR_MSG("Main", "Wrong number of parameters.", LOG_DOMAIN_NONE);
            usage();
            METHOD_EXIT("Main");
            return EXIT_FAILURE;
        }
    }
    else
    {
        ERROR_MSG("Main", "Wrong number of parameters.", LOG_DOMAIN_NONE);
        usage();
        METHOD_EXIT("Main");
        return EXIT_FAILURE;
    }

    METHOD_EXIT("main");
    return EXIT_SUCCESS;
}
