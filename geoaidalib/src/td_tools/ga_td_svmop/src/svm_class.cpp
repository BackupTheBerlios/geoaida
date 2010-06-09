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

    std::cout << "svm_class" << std::endl;
    std::cout << "          [int     ] number of input channels" << std::endl;
    std::cout << "          [filename] input channel 1" << std::endl;
    std::cout << "          ..." << std::endl;
    std::cout << "          [filename] input channel n" << std::endl;
    std::cout << "          [string  ] path to SVM configuration" << std::endl;
    std::cout << "          [filename] feature extraction parameters" << std::endl;
    std::cout << "          [filename] SVM model" << std::endl;
    std::cout << "          [filename] SVM scaling" << std::endl;
    std::cout << "          [string  ] prefix classification result filenames" << std::endl;

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
        if (ArgvList.size() == nNumberOfChannels+6)
        {
            if (!Config.loadConfig(ArgvList[nNumberOfChannels+1]+ "/" +
                                   ArgvList[nNumberOfChannels+2])) return EXIT_FAILURE;
            Log.setColourScheme(Log.stringToColourScheme(Config.getStr("LogColourScheme")));
            
            //--- Start the classification process -------------------------------//
            for (int i=1; i<nNumberOfChannels+1; ++i)
            {
                Extractor.addInputChannel(ArgvList[i]);
            }
            
            Extractor.setConfigurator(&Config);
            Classifier.setConfigurator(&Config);
            if (!Extractor.extract()) return EXIT_FAILURE;
            if (!Classifier.loadModel(ArgvList[nNumberOfChannels+1]+ "/" +
                                      ArgvList[nNumberOfChannels+3])) return EXIT_FAILURE;
            if (!Classifier.loadScaling(ArgvList[nNumberOfChannels+1]+ "/" +
                                        ArgvList[nNumberOfChannels+4])) return EXIT_FAILURE;
            Classifier.setLabelImageSize(Extractor.getImageSize());
            if (Config.getStr("DoRefinementTraining") != "true")
            {
                Extractor.clearChannels(); // Free some memory!
            }
//             Classifier.setFeatures(Extractor.getFeatures());
            Classifier.scaleFeatures();
            Classifier.classify();
            if (Config.getStr("CalculateDistanceMaps") == "true") Classifier.calculateDistanceMaps();
            if (Config.getStr("CalculateUncertainty")  == "true") Classifier.calculateUncertainty();
//             Classifier.createLabelImageFromProbabilities();
            Classifier.saveClassificationResult(ArgvList[nNumberOfChannels+5]);
//             Classifier.applyUncertaintyOnLabelImage();

//             if (Config.getStr("DoRefinementTraining") == "true")
//             {
//                 //--- Start the training process -----------------------------//
//                 Extractor.setLabelImage(Classifier.getLabelImage());
//                 if (!Extractor.extract(FEATURE_EXTRACTOR_USE_LABELS))
//                     return EXIT_FAILURE;
//                 Classifier.setLabelImageSize(Extractor.getImageSize());
//                 Classifier.setLabels(Extractor.getLabels());
//                 Classifier.setFeatures(Extractor.getFeatures());
//                 Classifier.scaleFeatures(SVM_CLASSIFIER_CALCULATE_EXTREMA);
//                 Classifier.train();
//                 Classifier.saveModel(ArgvList[nNumberOfChannels+1]+ "/" +
//                                      ArgvList[nNumberOfChannels+3]+ "_rt");
//                 Classifier.saveScaling(ArgvList[nNumberOfChannels+1]+ "/" +
//                                      ArgvList[nNumberOfChannels+4]+ "_rt");
//             }
//             if (Config.getStr("DoRefinementClassification") == "true")
//             {
//                 //--- Start the refinement process ---------------------------//
//                 if (!Extractor.extract()) return EXIT_FAILURE;
//                 Extractor.clearChannels(); // Free some memory!
//                 Classifier.setFeatures(Extractor.getFeatures());
//                 Classifier.scaleFeatures();
//                 Classifier.classify();
//                 if (Config.getStr("CalculateDistanceMaps") == "true") Classifier.calculateDistanceMaps();
//                 if (Config.getStr("CalculateUncertainty")  == "true") Classifier.calculateUncertainty();
//                 Classifier.createLabelImageFromProbabilities();
//                 Classifier.saveClassificationResult(ArgvList[nNumberOfChannels+5]+"_rt");
//             }
//             if (Config.getStr("DoProbabilityJoining") == "true")
//             {
//                 Classifier.joinProbabilities(ArgvList[nNumberOfChannels+5],
//                                              ArgvList[nNumberOfChannels+1]+ "/" +
//                                              ArgvList[nNumberOfChannels+3],
//                                              ArgvList[nNumberOfChannels+5]+ "_rt",
//                                              ArgvList[nNumberOfChannels+1]+ "/" +
//                                              ArgvList[nNumberOfChannels+3]+ "_rt");
//                 if (Config.getStr("CalculateUncertainty")  == "true") Classifier.calculateUncertainty();
//                 Classifier.saveClassificationResult(ArgvList[nNumberOfChannels+5]+"_jnt");
//             }
        }
        else
        {
            ERROR_MSG("Main", "Wrong number of parameters.")
            usage();
            METHOD_EXIT("main")
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
