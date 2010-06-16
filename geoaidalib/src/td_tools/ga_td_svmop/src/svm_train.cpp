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
/// \author		Torsten Bueschenfeld (bfeld@tnt.uni-hannover.de)
///
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
///
/// \file		main.cpp
/// \brief		Main program
///
/// \date		2009-06-29
/// \author		Torsten Bueschenfeld (bfeld@tnt.uni-hannover.de)
///
////////////////////////////////////////////////////////////////////////////////

//--- Standard header --------------------------------------------------------//
#include <cstdlib>
#include <string>
#include <dirent.h>

//--- Program header ---------------------------------------------------------//
#include "configurator.h"
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
    METHOD_ENTRY("usage");
    INFO(

    std::cout << std::endl;
    std::cout << "svm_train" << std::endl;
    std::cout << "          [int     ] number of input channels" << std::endl;
    std::cout << "          [filename] input channel 1" << std::endl;
    std::cout << "          ..." << std::endl;
    std::cout << "          [filename] input channel n" << std::endl;
    std::cout << "          [filename] label image" << std::endl;
    std::cout << "          [int     ] number of classes" << std::endl;
    std::cout << "          [filename] feature extraction parameters" << std::endl;
    std::cout << "          [filename] SVM model" << std::endl;
    std::cout << "          [filename] SVM scaling" << std::endl;
    std::cout << "          [string  ] prefix for reclassification result filenames" << std::endl;
    std::cout << std::endl;

    );
    METHOD_EXIT("usage");
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
    METHOD_ENTRY("main")

    Log.setBreak(120u);

    Configurator        Config;
    FeatureExtractor    Extractor;
    SVMClassifier       Classifier(Extractor.getFeatures());
    int                 nNumberOfChannels;
    std::vector<std::string> ArgvList;

    for (int i=1; i<argc; ++i)
    {
        ArgvList.push_back(argv[i]);
    }
    if (ArgvList.size() > 0)
    {
        nNumberOfChannels = atoi(ArgvList[0].c_str());
        if (ArgvList.size() == nNumberOfChannels+7)
        {
            if (!Config.loadConfig(ArgvList[nNumberOfChannels+3].c_str()))
                return EXIT_FAILURE;
            Log.setColourScheme(Log.stringToColourScheme(Config.getStr("LogColourScheme")));
            
            //--- Load training images and label image -----------------------//
            for (int i=1; i<nNumberOfChannels+1; ++i)
            {
                Extractor.addInputChannel(ArgvList[i]);
            }

            //--- Start the training process ---------------------------------//
            Extractor.loadLabelImage(ArgvList[nNumberOfChannels+1]);
            Extractor.setConfigurator(&Config);
            Classifier.setConfigurator(&Config);
            if (!Extractor.extract(FEATURE_EXTRACTOR_USE_LABELS))
                return EXIT_FAILURE;
            Classifier.setLabelImageSize(Extractor.getImageSize());
            Classifier.setLabels(Extractor.getLabels());
//             Classifier.setFeatures(Extractor.getFeatures());
            Classifier.scaleFeatures(SVM_CLASSIFIER_CALCULATE_EXTREMA);
            Classifier.setNumberOfClasses(atoi(ArgvList[nNumberOfChannels+2].c_str()));
            Classifier.train();
            Classifier.saveModel(ArgvList[nNumberOfChannels+4]);
            Classifier.saveScaling(ArgvList[nNumberOfChannels+5]);

            //--- Reclassification -------------------------------------------//
            if (Config.getStr("TrainingDoReclassification") == "true")
            {
                if (!Extractor.extract())
                    return EXIT_FAILURE;
                Extractor.clearChannels(); // Free some memory!
                Classifier.loadModel(ArgvList[nNumberOfChannels+4]);
                if (!Classifier.loadScaling(ArgvList[nNumberOfChannels+5]))
                    return EXIT_FAILURE;
//                 Classifier.setFeatures(Extractor.getFeatures());
                Classifier.scaleFeatures();
                Classifier.classify();
                
                if (Config.getStr("CalculateDistanceMaps") == "true") Classifier.calculateDistanceMaps();
                if (Config.getStr("CalculateUncertainty")  == "true") Classifier.calculateUncertainty();
//                 Classifier.createLabelImageFromProbabilities();
//                 Classifier.applyUncertaintyOnLabelImage();
                Classifier.saveClassificationResult(ArgvList[nNumberOfChannels+6]);
            }
            
        }
        else
        {
            ERROR_MSG("Main", "Wrong number of parameters.")
            usage();
            METHOD_EXIT("main");
            return EXIT_FAILURE;
        }
    }
    else
    {
        ERROR_MSG("Main", "Wrong number of parameters.")
        usage();
        METHOD_EXIT("main")
        return EXIT_FAILURE;
    }

    METHOD_EXIT("main")
    return EXIT_SUCCESS;
}
