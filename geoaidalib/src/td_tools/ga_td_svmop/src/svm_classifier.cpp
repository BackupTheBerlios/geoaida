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
/// \file		svm_classifier.cpp
/// \brief		Implementation of interface "SVMClassifier"
///
/// \date		2009-07-29
/// \author		Torsten Bueschenfeld (bfeld@tnt.uni-hannover.de)
///
////////////////////////////////////////////////////////////////////////////////

#include "svm_classifier.h"

#include <fstream>

DEBUG(
    uint SVMClassifier::m_unNoFVFiles = 0;
);

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Constructor
///
///////////////////////////////////////////////////////////////////////////////
SVMClassifier::SVMClassifier() : 	m_unNumberOfClasses(SVM_CLASSIFIER_DEFAULT_NUMBER_OF_CLASSES),
                                    m_fMin( 1e300),
                                    m_fMax(-1e300),
                                    m_bGotClassificationResult(false),
                                    m_bGotFeatures(false),
                                    m_bGotLabels(false),
                                    m_bGotModel(false),
                                    m_bGotScaling(false),
                                    m_bGotSize(false)
{
    METHOD_ENTRY("SVMClassifier::SVMClassifier()");
    CTOR_CALL("SVMClassifier::SVMClassifier()");

    METHOD_EXIT("SVMClassifier::SVMClassifier()");
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Destructor
///
///////////////////////////////////////////////////////////////////////////////
SVMClassifier::~SVMClassifier()
{
    METHOD_ENTRY("SVMClassifier::~SVMClassifier()");
    DTOR_CALL("SVMClassifier::~SVMClassifier()");

    METHOD_EXIT("SVMClassifier::~SVMClassifier()");
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Returns the label image
///
/// \return Pointer to label image
///
///////////////////////////////////////////////////////////////////////////////
LabelImageType::Pointer SVMClassifier::getClassificationResult() const
{
    METHOD_ENTRY("SVMClassifier::getClassificationResult()");
    
    if (!m_bGotClassificationResult)
    {
        WARNING_MSG("SVM Classifier", "No classification done, image empty.", LOG_DOMAIN_NONE);
    }
    
    METHOD_EXIT("SVMClassifier::getClassificationResult()");
    return m_pLabelImage;
}


///////////////////////////////////////////////////////////////////////////////
///
/// \brief Saves the label image that resulted from classification
///
/// \param _strPrefix Prefix for filename
///
/// \return Returns if saving was succesfull
///
///////////////////////////////////////////////////////////////////////////////
bool SVMClassifier::saveClassificationResult(const std::string& _strPrefix) const
{
    METHOD_ENTRY("SVMClassifier::saveClassificationResult(const std::string&)");

    if (!m_bGotClassificationResult)
    {
        WARNING_MSG("SVM Classifier", "No classification done, cannot save result.", LOG_DOMAIN_NONE);
        METHOD_EXIT("SVMClassifier::saveClassificationResult(const std::string&)");
        return false;
    }

    INFO_MSG("SVM Classifier", "Saving classification results.", LOG_DOMAIN_NONE);
    
    std::ostringstream oss("");
    oss << _strPrefix << "_labels.tif";
    saveImage(m_pLabelImage, oss.str());
       
    for (int i=0; i<m_unNumberOfClasses; ++i)
    {
        std::ostringstream oss("");
        oss << _strPrefix << "_probability_c" << i+1 << ".tif";
        saveImage(m_Probabilities[i], oss.str());
    }

    METHOD_EXIT("SVMClassifier::saveClassificationResult(const std::string&)");
    return true;
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Saves the scaling values from training
///
/// \param _strFilename Filename
///
/// \return Returns if saving was succesful
///
///////////////////////////////////////////////////////////////////////////////
bool SVMClassifier::saveScaling(const std::string& _strFilename) const
{
    METHOD_ENTRY("SVMClassifier::saveScaling(const std::string&)");

    if (!m_bGotScaling)
    {
        WARNING_MSG("SVM Classifier", "Feature scale has not been loaded or calculated, "
                    "you are about to save default values. ", LOG_DOMAIN_NONE);
    }

    ofstream outfile;
    outfile.open(_strFilename.c_str());
    if (outfile.fail())
    {
        ERROR_MSG("SVM Classifier", "Can't create " << _strFilename, LOG_DOMAIN_FILEIO);
        METHOD_EXIT("SVMClassifier::saveScaling(const std::string&)");
        return false;
    }
    outfile << "ValueMin: " << m_fMin << "\n";
    outfile << "ValueMax: " << m_fMax << "\n";
    outfile.close();
    INFO_MSG("SVM Classifier", "SVM scaling stored in " << _strFilename, LOG_DOMAIN_NONE);

    METHOD_EXIT("SVMClassifier::saveScaling(const std::string&)");
    return true;
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Exports the (training) data to given filename
///
/// \param _strFilename Filename
///
/// \return Returns if exporting was succesful
///
///////////////////////////////////////////////////////////////////////////////
bool SVMClassifier::saveData(const std::string& _strFilename) const
{
    METHOD_ENTRY("SVMClassifier::saveData(const std::string&)");

// 	std::ofstream of(_strFilename.c_str());
// 
// 	int i=0; int j=0;
// 	LabelListType::const_iterator cl=m_pFeatures->getLabelList().begin();
// 	for (FeatureListType::const_iterator ci=m_pFeatures->get().begin(); ci!=m_pFeatures->get().end(); ++ci)
// 	{
// 		int k=0;
// 		of << (*cl) << " ";
// 		for (std::vector<double>::const_iterator cj=(*ci).begin(); cj!=(*ci).end(); ++cj)
// 		{
// 			of << k++ << ":" << (*cj) << " ";
// 		}
// 		of << std::endl;
// 		++i; ++j; ++cl;
// 	}
// 
// 	INFO_MSG("SVM Classifier", "Data exported to " << _strFilename, LOG_DOMAIN_NONE);

    METHOD_EXIT("SVMClassifier::saveData(const std::string&)");
    return true;
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Save SVM model
///
/// \param _strFilename Filename of model to be saved
///
/// \return Returns if saving was succesfull
///
///////////////////////////////////////////////////////////////////////////////
bool SVMClassifier::saveModel(const std::string& _strFilename) const
{
    METHOD_ENTRY("SVMClassifier::saveModel(const std::string&)");

    if (!m_bGotModel)
    {
        WARNING_MSG("SVM Classifier", "No SVM model, nothing is saved. Train or import model first.", LOG_DOMAIN_NONE);
        METHOD_EXIT("SVMClassifier::exportModel(const std::string&)");
        return false;
    }
    else
    {
        INFO_MSG("SVM Classifier", "Saving SVM model to " << _strFilename << ".", LOG_DOMAIN_NONE);
    }

    m_pEstimator->SaveModel(_strFilename.c_str());

    METHOD_EXIT("SVMClassifier::saveModel(const std::string&)");
    return true;
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Loads SVM model based on training
///
/// \param _strFilename Filename of model to be loaded
///
/// \return Returns if loading was succesfull
///
///////////////////////////////////////////////////////////////////////////////
bool SVMClassifier::loadModel(const std::string& _strFilename)
{
    METHOD_ENTRY("SVMClassifier::loadModel(const std::string&)");

    INFO_MSG("SVM Classifier", "Loading SVM model " << _strFilename << ".", LOG_DOMAIN_NONE);

    if (m_bGotModel)
    {
        NOTICE_MSG("SVM Classifier", "SVM model already exists. Replacing.", LOG_DOMAIN_NONE);
    }
    m_pModel = ModelType::New();

    m_pModel->LoadModel(_strFilename.c_str());
    m_bGotModel = true;
    
    m_unNumberOfClasses = m_pModel->GetNumberOfClasses();

    METHOD_EXIT("SVMClassifier::loadModel(const std::string&)");
    return true;
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Loads SVM scaling based on training
///
/// \param _strFilename Filename of scaling to be loaded
///
/// \return Returns if loading was succesfull
///
///////////////////////////////////////////////////////////////////////////////
bool SVMClassifier::loadScaling(const std::string& _strFilename)
{
    METHOD_ENTRY("SVMClassifier::loadScaling(const std::string&)");

    INFO_MSG("SVM Classifier", "Loading SVM scaling " << _strFilename << ".", LOG_DOMAIN_NONE);

    if (m_bGotScaling)
    {
        NOTICE_MSG("SVM Classifier", "SVM scaling already exists. Replacing.", LOG_DOMAIN_NONE);
    }
    ifstream infile;
    infile.open(_strFilename.c_str());
    if (infile.fail())
    {
        ERROR_MSG("SVM Classifier", "Can't open " << _strFilename, LOG_DOMAIN_FILEIO);
        METHOD_EXIT("SVMClassifier::loadScaling(const std::string&)");
        return false;
    }
    bool bParamFailed = false;
    string strDecription;
    infile >> strDecription; if (infile.fail()) bParamFailed = true; 
    infile >> m_fMin;        if (infile.fail()) bParamFailed = true; 
    infile >> strDecription; if (infile.fail()) bParamFailed = true; 
    infile >> m_fMax;        if (infile.fail()) bParamFailed = true;
    infile.close();
    
    if (infile.fail())
    {
        ERROR_MSG("SVM Classifier", "Wrong parameter file", LOG_DOMAIN_NONE);
        METHOD_EXIT("SVMClassifier::loadScaling(const std::string&)");
        return false;
    }
    
    m_bGotScaling = true;

    METHOD_EXIT("SVMClassifier::loadScaling(const std::string&)");
    return true;
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Scales features to given interval
///
/// \param _bCalc Calculate the minimum and maximum value
/// \param _fLower Lower interval border
/// \param _fUpper Upper interval border
///
/// \return Returns if scaling was successful
///
///////////////////////////////////////////////////////////////////////////////
bool SVMClassifier::scaleFeatures(const bool& _bCalc,
                                const double& _fLower, const double& _fUpper)
{
    METHOD_ENTRY("SVMClassifier::scaleFeatures(const double&, const double&)");

    //--------------------------------------------------------------------------
    // Check if everthings setup correctly
    //--------------------------------------------------------------------------
    if (!m_bGotFeatures)
    {
        ERROR_MSG("SVM Classifier", "No features, can't scale. Get features from " <<
                    "Image Feature Extractor or load them. ", LOG_DOMAIN_NONE);
        METHOD_EXIT("SVMClassifier::scaleFeatures(const double&, const double&)");
        return false;
    }
    
    if (_bCalc)
    {
        DEBUG_MSG("SVM Classifier", "Determining extrema in feature vector.", LOG_DOMAIN_NONE);
        
        m_fMin = 1e300;
        m_fMax = -1e300;
        
        FeaturePointDataContainer::ConstIterator it = m_pFeatures->GetPointData()->Begin();
        FeatureVectorType::const_iterator ci;
        while (it != m_pFeatures->GetPointData()->End())
        {
            ci = (it.Value()).begin();
            while (ci != (it.Value()).end())
            {
                if ((*ci) < m_fMin) m_fMin = (*ci);
                if ((*ci) > m_fMax) m_fMax = (*ci);
                
                ++ci;
            }
            ++it;
        }
        m_bGotScaling = true;
    }
    if (!m_bGotScaling)
    {
        WARNING_MSG("SVM Classifier", "Feature scale has not been loaded or calculated, "
                    "using default values. ", LOG_DOMAIN_NONE);
    }
    
    DEBUG_MSG("SVM Classifier", "Min value: " << m_fMin, LOG_DOMAIN_VAR);
    DEBUG_MSG("SVM Classifier", "Max value: " << m_fMax, LOG_DOMAIN_VAR);
    
    INFO_MSG("SVM Classifier", "Scaling features to interval [" << _fLower << "," << _fUpper << "]", LOG_DOMAIN_NONE);
    
    DEBUG(
        #include <fstream>
        #include <sstream>
        std::ostringstream oss("");
        oss << "DEBUG_feature_vectors_" << m_unNoFVFiles << ".txt";
        std::ofstream of(oss.str().c_str());
        ++m_unNoFVFiles;
    );
    FeatureVectorType::iterator jt;
    FeaturePointDataContainer::Iterator it = m_pFeatures->GetPointData()->Begin();
    while (it != m_pFeatures->GetPointData()->End())
    {
        jt = (it.Value()).begin();
        while (jt != (it.Value()).end())
        {
            (*jt) = _fLower + ((*jt) - m_fMin) * (_fUpper - _fLower)/(m_fMax-m_fMin);
            
            DEBUG(of << (*jt) << " ";);
            
            ++jt;
        }
        DEBUG(of << std::endl;);
        ++it;
    }
    DEBUG(of.close(););
    DEBUG_MSG("SVM Classifier", "Feature vectors written to: " << oss.str(), LOG_DOMAIN_NONE);

    METHOD_EXIT("SVMClassifier::scaleFeatures(const double&, const double&)");
    return true;
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Classifies by given features
///
/// \return Returns if classification was succesfull
///
///////////////////////////////////////////////////////////////////////////////
bool SVMClassifier::classify()
{
    METHOD_ENTRY("SVMClassifier::classify()");

    //--------------------------------------------------------------------------
    // Check if everthings setup correctly
    //--------------------------------------------------------------------------
    if (!m_bGotFeatures)
    {
        ERROR_MSG("SVM Classifier", "No features. Get features from " <<
                    "Image Feature Extractor or load them. ", LOG_DOMAIN_NONE);
        METHOD_EXIT("SVMClassifier::classify()");
        return false;
    }
    if (m_pFeatures->GetNumberOfPoints() == 0)
    {
        ERROR_MSG("SVM Classifier", "Not enough feature points (0).", LOG_DOMAIN_NONE);
        METHOD_EXIT("SVMClassifier::classify()");
        return false;
    }
    if (m_pModel->GetNumberOfSupportVectors() == 0)
    {
        ERROR_MSG("SVM Classifier", "Not enough support vectors (0).", LOG_DOMAIN_NONE);
        METHOD_EXIT("SVMClassifier::classify()");
        return false;
    }
    if (!m_bGotSize)
    {
        WARNING_MSG("SVM Classifier", "No label image size specified. Resulting label image will be empty.", LOG_DOMAIN_NONE);
        METHOD_EXIT("SVMClassifier::train()");
        return false;
    }
    if (m_bGotClassificationResult)
    {
        NOTICE_MSG("SVM Classifier", "Already got classification result, overwriting.", LOG_DOMAIN_NONE);
    }

    INFO_MSG("SVM Classifier", "Starting classification.", LOG_DOMAIN_NONE);

    //--------------------------------------------------------------------------
    // Start the classification
    //--------------------------------------------------------------------------
    SampleType::Pointer pSample = SampleType::New();
    pSample->SetPointSet(convertPointSet(m_pFeatures));
    
    DEBUG_MSG("SVM Classifier", "Number of features to be classified: " << pSample->Size(), LOG_DOMAIN_VAR);
    DEBUG_MSG("SVM Classifier", "Number of classes: " << m_unNumberOfClasses, LOG_DOMAIN_VAR);

    m_pClassifier = ClassifierType::New();
    m_pClassifier->SetNumberOfClasses(m_unNumberOfClasses);
    m_pClassifier->SetModel(m_pModel);
    m_pClassifier->SetSample(pSample.GetPointer());

    Log.logSeparator();
    m_pClassifier->Update();
    Log.logSeparator();
    
    ClassifierType::OutputType::Pointer pClassificationResult =	m_pClassifier->GetOutput();
    
    //--------------------------------------------------------------------------
    // Create a new image for the resulting labels
    //--------------------------------------------------------------------------
    m_pLabelImage = LabelImageType::New();
    LabelImageType::IndexType StartIndex;
    LabelImageType::SizeType Size;
    LabelImageType::RegionType Region;
    
    StartIndex[0]=0;
    StartIndex[1]=0;
    Region.SetIndex(StartIndex);
    Region.SetSize(m_LabelImageSize);
    m_pLabelImage->SetRegions(Region);
    m_pLabelImage->Allocate();
    
    ClassifierType::OutputType::ConstIterator ciIter = pClassificationResult->Begin();
    ClassifierType::OutputType::ConstIterator ciLast = pClassificationResult->End();
    typedef itk::ImageRegionIterator<LabelImageType> OutputIteratorType;
    OutputIteratorType itOut(m_pLabelImage, m_pLabelImage->GetLargestPossibleRegion());
    itOut.GoToBegin();
    while ((ciIter != ciLast) && (!itOut.IsAtEnd()))
    {
        itOut.Set(ciIter.GetClassLabel());
        ++ciIter;
        ++itOut;
    }
    
    std::vector<std::vector<double> > Probabilities;
    Probabilities = m_pClassifier->GetProbabilityEstimates();
    
    m_Probabilities.clear();
    
    for (int i=0; i<m_unNumberOfClasses; ++i)
    {
        ImageFloatType::Pointer pProbabilityImage = ImageFloatType::New();
        ImageFloatType::IndexType StartIndex;
        ImageFloatType::SizeType Size;
        ImageFloatType::RegionType Region;
    
        StartIndex[0]=0;
        StartIndex[1]=0;
        Region.SetIndex(StartIndex);
        Region.SetSize(m_LabelImageSize);
        
        pProbabilityImage->SetRegions(Region);
        pProbabilityImage->Allocate();
        itk::ImageRegionIterator<ImageFloatType> itOut(pProbabilityImage, pProbabilityImage->GetLargestPossibleRegion());
        itOut.GoToBegin();
    
        std::vector<std::vector<double> >::const_iterator ciIn = Probabilities.begin();
        while (ciIn != Probabilities.end())
        {
            itOut.Set((*ciIn)[i]);
            ++ciIn;
            ++itOut;
        }
        
        m_Probabilities.push_back(pProbabilityImage);
    }

    INFO_MSG("SVM Classifier", "Classification done.", LOG_DOMAIN_NONE);

    m_bGotClassificationResult = true;
    METHOD_EXIT("SVMClassifier::classify()");
    return true;
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Starts training routines of libSVM, converts features accordingly
///
/// \return Returns if training succeded
///
///////////////////////////////////////////////////////////////////////////////
bool SVMClassifier::train()
{
    METHOD_ENTRY("SVMClassifier::train()");

    //--------------------------------------------------------------------------
    // Check if everthings setup correctly
    //--------------------------------------------------------------------------
    if (!m_bGotFeatures)
    {
        ERROR_MSG("SVM Classifier", "No features. Get features from " <<
                    "Image Feature Extractor or load them. ", LOG_DOMAIN_NONE);
        METHOD_EXIT("SVMClassifier::train()");
        return false;
    }
    if (!m_bGotLabels)
    {
        ERROR_MSG("SVM Classifier", "No labels. Get labels from " <<
                    "Image Feature Extractor or load them. ", LOG_DOMAIN_NONE);
        METHOD_EXIT("SVMClassifier::train()");
        return false;
    }
    
    DEBUG_MSG("SVM Classifier", "Labels  : " << m_pLabels->GetNumberOfPoints(), LOG_DOMAIN_VAR);
    DEBUG_MSG("SVM Classifier", "Features: " << m_pFeatures->GetNumberOfPoints(), LOG_DOMAIN_VAR);
    
    if (m_pFeatures->GetNumberOfPoints() == 0)
    {
        ERROR_MSG("SVM Classifier", "Not enough feature points (0).", LOG_DOMAIN_NONE);
        METHOD_EXIT("SVMClassifier::train()");
        return false;
    }
    // Zero label points will also be catched with the following condition
    if (m_pFeatures->GetNumberOfPoints() != m_pLabels->GetNumberOfPoints())
    {
        ERROR_MSG("SVM Classifier", "Number of labels and feature points differ.", LOG_DOMAIN_NONE);
        METHOD_EXIT("SVMClassifier::train()");
        return false;
    }
    
    //--------------------------------------------------------------------------
    // Start training process
    //--------------------------------------------------------------------------
    m_pEstimator = EstimatorType::New();

    m_pEstimator->SetInputPointSet(m_pFeatures);
    m_pEstimator->SetTrainingPointSet(m_pLabels);
    m_pEstimator->SetNumberOfClasses(m_unNumberOfClasses);
    m_pEstimator->SetSVMType(C_SVC);
    m_pEstimator->SetKernelType(RBF);
    m_pEstimator->SetKernelGamma(0.0);
    m_pEstimator->SetKernelCoef0(0.0);
    m_pEstimator->SetPolynomialKernelDegree(3);
    m_pEstimator->SetNu(0.5);
    m_pEstimator->SetCacheSize(100);
    m_pEstimator->SetEpsilon(1.0e-3);
    m_pEstimator->SetC(1.0e3);
    m_pEstimator->SetP(0.1);
    m_pEstimator->DoShrinking(true);
    m_pEstimator->DoProbabilityEstimates(true);

    Log.logSeparator();
    m_pEstimator->Update();
    Log.logSeparator();

    if (m_bGotModel)
    {
        NOTICE_MSG("SVM Classifier", "SVM model already exists. Replacing.", LOG_DOMAIN_NONE);
    }

    m_pModel = m_pEstimator->GetModel();
    m_bGotModel = true;

    METHOD_EXIT("SVMClassifier::train()");
    return true;
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Converts a point set to make it compatible to classification
///
/// \param _pPS Point set to be converted
///
/// \return Returns converted point set
///
///////////////////////////////////////////////////////////////////////////////
TestFeaturePointSetType::Pointer SVMClassifier::convertPointSet(
                                            const FeaturePointSetType::Pointer _pPS)
{
    METHOD_ENTRY("SVMClassifier::convertPointSet(const FeaturePointSetType::Pointer)");
    
    TestFeaturePointSetType::Pointer	pPS = TestFeaturePointSetType::New();
    TestFeaturePointContainer::Pointer	pFC = TestFeaturePointContainer::New();
    
    TestFeaturePointType FP;
    
    FeaturePointDataContainer::Iterator it = _pPS->GetPointData()->Begin();
    FeatureVectorType::const_iterator ci;
    int i=0;
    while (it != _pPS->GetPointData()->End())
    {
        int j=0;
        ci = (it.Value()).begin();
        while (j<FEATURE_VEC_SIZE_MAX)
        {
            if (ci != (it.Value()).end())
            {
                FP[j] = (*ci);
                ++ci;
            }
            else
            {
                FP[j] = 0;
            }
            ++j;
        }
        pFC->InsertElement(i, FP);
//         pPS->SetPointData(i,(it.Value()));
        ++i;
        ++it;
    }
    DEBUG_MSG("SVM Classifier", "Point set converted to compatible format for classification.", LOG_DOMAIN_NONE);
// 	std::vector<FeatureVectorType>::iterator jt;
// 	int i=0;
// 	for (ci = FeaturePoints.begin(); ci != FeaturePoints.end(); ++ci)
// 	{
// 		
// 		FeaturePointType FP;
// 
// 		FP[0] = -i;
// 
// 		
// 		m_pFeatures->SetPointData(i, (*ci));
// 
// 		++i;
// 	}

    pPS->SetPoints(pFC);
    
    METHOD_EXIT("SVMClassifier::convertPointSet(const FeaturePointSetType::Pointer)");
    return pPS;
}
