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
/// \brief Copy constructor
///
/// Constructor is private. Copy Constructor needed to ensure use of only one
/// feature list.
///
///////////////////////////////////////////////////////////////////////////////
SVMClassifier::SVMClassifier(FeaturesType& _Features) :
                                    m_Features(_Features),
                                    m_unNumberOfClasses(SVM_CLASSIFIER_DEFAULT_NUMBER_OF_CLASSES),
                                    m_bGotClassificationResult(false),
                                    m_bGotDistanceMaps(false),
                                    m_bGotFeatures(true),
                                    m_bGotLabels(false),
                                    m_bGotModel(false),
                                    m_bGotScaling(false),
                                    m_bGotSize(false),
                                    m_pConfiguration(0)
{
    METHOD_ENTRY("SVMClassifier::SVMClassifier");
    CTOR_CALL("SVMClassifier::SVMClassifier");

    METHOD_EXIT("SVMClassifier::SVMClassifier");
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Destructor
///
///////////////////////////////////////////////////////////////////////////////
SVMClassifier::~SVMClassifier()
{
    METHOD_ENTRY("SVMClassifier::~SVMClassifier");
    DTOR_CALL("SVMClassifier::~SVMClassifier");

    METHOD_EXIT("SVMClassifier::~SVMClassifier");
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Returns the label image
///
/// \return Pointer to label image
///
///////////////////////////////////////////////////////////////////////////////
LabelImageType::Pointer SVMClassifier::getLabelImage() const
{
    METHOD_ENTRY("SVMClassifier::getLabelImage");
    
    if (!m_bGotClassificationResult)
    {
        WARNING_MSG("SVM Classifier", "No classification done, image empty.")
    }
    
    METHOD_EXIT("SVMClassifier::getLabelImage");
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
    METHOD_ENTRY("SVMClassifier::saveClassificationResult");

    if (!m_bGotClassificationResult)
    {
        WARNING_MSG("SVM Classifier", "No classification done, cannot save result.")
        METHOD_EXIT("SVMClassifier::saveClassificationResult");
        return false;
    }

    INFO_MSG("SVM Classifier", "Saving classification results.")
    
    {
    std::ostringstream oss("");
    oss << _strPrefix << "_labels.tif";
    saveImage(m_pLabelImage, oss.str());
    }
       
//     for (int i=0; i<m_unNumberOfClasses; ++i)
//     {
//         std::ostringstream oss("");
//         oss << _strPrefix << "_probability_c" << m_LabelMap[i] << ".tif";
//         saveImage(m_Probabilities[i], oss.str());
//     }
    
//     if (m_pConfiguration->getStr("CalculateUncertainty") == "true")
//     {
//         std::ostringstream oss("");
//         oss << _strPrefix << "_uncertainty.tif";
//         saveImage(m_pUncertainty, oss.str());
//     }
    
    METHOD_EXIT("SVMClassifier::saveClassificationResult");
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
    METHOD_ENTRY("SVMClassifier::saveScaling");

    if (!m_bGotScaling)
    {
        WARNING_MSG("SVM Classifier", "Feature scale has not been loaded or calculated, "
                    "you are about to save default values. ")
    }

    ofstream outfile;
    outfile.open(_strFilename.c_str());
    if (outfile.fail())
    {
        ERROR_MSG("SVM Classifier", "Can't create " << _strFilename)
        METHOD_EXIT("SVMClassifier::saveScaling");
        return false;
    }
    for (int i=0; i<m_Min.size(); ++i)
    {
        outfile << "ValueMin_" << i << ": " << m_Min[i] << "\n";
        outfile << "ValueMax_" << i << ": " << m_Max[i] << "\n";
    }
    outfile.close();
    INFO_MSG("SVM Classifier", "SVM scaling stored in " << _strFilename)

    METHOD_EXIT("SVMClassifier::saveScaling");
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
    METHOD_ENTRY("SVMClassifier::saveData");

// 	std::ofstream of(_strFilename.c_str());
// 
// 	int i=0; int j=0;
// 	LabelListType::const_iterator cl=m_Features->getLabelList().begin();
// 	for (FeatureListType::const_iterator ci=m_Features->get().begin(); ci!=m_Features->get().end(); ++ci)
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
// 	INFO_MSG("SVM Classifier", "Data exported to " << _strFilename)

    METHOD_EXIT("SVMClassifier::saveData");
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
    METHOD_ENTRY("SVMClassifier::saveModel");

    if (!m_bGotModel)
    {
        WARNING_MSG("SVM Classifier", "No SVM model, nothing is saved. Train or import model first.")
        METHOD_EXIT("SVMClassifier::saveModel");
        return false;
    }
    else
    {
        INFO_MSG("SVM Classifier", "Saving SVM model to " << _strFilename << ".")
    }

    m_pModel->SaveModel(_strFilename.c_str());

    METHOD_EXIT("SVMClassifier::saveModel");
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
    METHOD_ENTRY("SVMClassifier::loadModel");

    INFO_MSG("SVM Classifier", "Loading SVM model " << _strFilename << ".")

    if (m_bGotModel)
    {
        NOTICE_MSG("SVM Classifier", "SVM model already exists. Replacing.")
    }
    m_pModel = ModelType::New();

    m_pModel->LoadModel(_strFilename.c_str());
    m_bGotModel = true;
    
    m_unNumberOfClasses = m_pModel->GetNumberOfClasses();

    METHOD_EXIT("SVMClassifier::loadModel");
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
    METHOD_ENTRY("SVMClassifier::loadScaling");

    INFO_MSG("SVM Classifier", "Loading SVM scaling " << _strFilename << ".")

    if (m_bGotScaling)
    {
        NOTICE_MSG("SVM Classifier", "SVM scaling already exists. Replacing.")
    }
    ifstream infile;
    infile.open(_strFilename.c_str());
    if (infile.fail())
    {
        ERROR_MSG("SVM Classifier", "Can't open " << _strFilename)
        METHOD_EXIT("SVMClassifier::loadScaling");
        return false;
    }
    bool bParamFailed = false;
    string strDecription;
    float fTmp;
    m_Min.clear();
    m_Max.clear();
    
    while (!infile.eof())
    {
        infile >> strDecription; 
        infile >> fTmp;
        if (!infile.fail())
        {
            m_Min.push_back(fTmp);
            infile >> strDecription; 
            infile >> fTmp;
            if (!infile.fail())
                m_Max.push_back(fTmp);
        }
    }
    infile.close();
    
//     if (infile.fail())
//     {
//         ERROR_MSG("SVM Classifier", "Wrong parameter file")
//         METHOD_EXIT("SVMClassifier::loadScaling");
//         return false;
//     }
    
    m_bGotScaling = true;

    METHOD_EXIT("SVMClassifier::loadScaling");
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
    METHOD_ENTRY("SVMClassifier::scaleFeatures");

    //--------------------------------------------------------------------------
    // Check if everthings setup correctly
    //--------------------------------------------------------------------------
    if (!m_bGotFeatures)
    {
        ERROR_MSG("SVM Classifier", "No features, can't scale. Get features from " <<
                    "Image Feature Extractor or load them. ")
        METHOD_EXIT("SVMClassifier::scaleFeatures");
        return false;
    }
    
    if (_bCalc)
    {
        DEBUG_MSG("SVM Classifier", "Determining extrema in feature vector.")
        
        int i;
        for (i=0; i<m_Features.begin()->size(); ++i)
        {
            m_Min.push_back(1e300);
            m_Max.push_back(-1e300);
        }
        
        FeaturesType::const_iterator ci = m_Features.begin();
        FeatureVectorType::const_iterator cj;
        
        while (ci != m_Features.end())
        {
            cj = ci->begin();
            i  = 0;
            while (cj != ci->end())
            {
                if ((*cj) < m_Min[i]) m_Min[i] = (*cj);
                if ((*cj) > m_Max[i]) m_Max[i] = (*cj);
                
                ++cj;
                ++i;
            }
            ++ci;
        }
        m_bGotScaling = true;
    }
    if (!m_bGotScaling)
    {
        WARNING_MSG("SVM Classifier", "Feature scale has not been loaded or calculated, "
                    "using default values. ")
    }
    
    DOM_VAR(
        DEBUG(  for (int i=0; i<m_Min.size(); ++i)
                {
                    DEBUG_MSG("SVM Classifier", "Min value dimension " << i << ": " << m_Min[i])
                }
             )
        )
    DOM_VAR(
        DEBUG(  for (int i=0; i<m_Max.size(); ++i)
                {
                    DEBUG_MSG("SVM Classifier", "Max value dimension " << i << ": " << m_Max[i])
                }
             )
        )
    
    INFO_MSG("SVM Classifier", "Scaling features to interval [" << _fLower << "," << _fUpper << "]")
    
    DEBUG(
        #include <fstream>
        #include <sstream>
        std::ostringstream oss("");
        oss << "DEBUG_feature_vectors_" << m_unNoFVFiles << ".txt";
        std::ofstream of(oss.str().c_str());
        ++m_unNoFVFiles;
    );
    FeatureVectorType::iterator jt;
    FeaturesType::iterator it = m_Features.begin();
    while (it != m_Features.end())
    {
        jt = (*it).begin();
        int i = 0;
        while (jt != (*it).end())
        {
            (*jt) = _fLower + ((*jt) - m_Min[i]) * (_fUpper - _fLower)/(m_Max[i]-m_Min[i]);
            
            DEBUG(of << (*jt) << " ";);
            
            ++jt;
            ++i;
        }
        DEBUG(of << std::endl;);
        ++it;
    }
    DEBUG(of.close(););
    DEBUG_MSG("SVM Classifier", "Feature vectors written to: " << oss.str())

    METHOD_EXIT("SVMClassifier::scaleFeatures");
    return true;
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Adds features to the current set of features
///
/// \param _Features Set of features
///
///////////////////////////////////////////////////////////////////////////////
void SVMClassifier::addFeatures(const FeaturesType& _Features)
{
    METHOD_ENTRY("SVMClassifier::addFeatures")

    INFO_MSG("SVM Classifier", "Adding features.")
    DOM_VAR(DEBUG_MSG("SVM Classifier", "  Current: " << m_Features.size()))
    DOM_VAR(DEBUG_MSG("SVM Classifier", "  New:     " <<  _Features.size()))
    
    for (int i=0; i<_Features.size(); ++i)
    {
        m_Features.push_back(_Features[i]);
    }
    m_bGotFeatures = true;

    METHOD_EXIT("SVMClassifier::addFeatures")
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Adds labels to the current set of labels
///
/// \param _Labels Set of labels
///
///////////////////////////////////////////////////////////////////////////////
void SVMClassifier::addLabels(const LabelsType& _Labels)
{
    METHOD_ENTRY("SVMClassifier::addLabels")

    INFO_MSG("SVM Classifier", "Adding labels to point set.")
    DOM_VAR(DEBUG_MSG("SVM Classifier", "  Current: " << m_Labels.size()))
    DOM_VAR(DEBUG_MSG("SVM Classifier", "  New:     " <<  _Labels.size()))
    
    for (int i=0; i<_Labels.size(); ++i)
    {
        m_Labels.push_back(_Labels[i]);
    }
    m_bGotLabels = true;

    METHOD_EXIT("SVMClassifier::addLabels")
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Calculate uncertainty using the probabilities of classification
///        result.
///
/// \return Returns if operation was successful
///
///////////////////////////////////////////////////////////////////////////////
bool SVMClassifier::applyUncertaintyOnLabelImage()
{
    METHOD_ENTRY("SVMClassifier::applyUncertaintyOnLabelImage");
    
    if (!m_bGotClassificationResult)
    {
        ERROR_MSG("SVM Classifier", "No classification result. Can't calculate "
                                    "uncertainty. ")
        METHOD_EXIT("SVMClassifier::applyUncertaintyOnLabelImage")
        return false;
    }
    
    INFO_MSG("SVM Classifier", "Creating new label image by removing uncertain regions.")
    
    typedef itk::BinaryThresholdImageFilter<ImageFloatType,Image8BitType> ThresholdFilterType;
    ThresholdFilterType::Pointer pThresholdFilter = ThresholdFilterType::New();

    pThresholdFilter->SetInsideValue(0);
    pThresholdFilter->SetOutsideValue(1);
    pThresholdFilter->SetLowerThreshold(m_pConfiguration->getFlt("UncertaintyThreshold"));
    pThresholdFilter->SetUpperThreshold(1.0);
    pThresholdFilter->SetInput(m_pUncertainty);
    pThresholdFilter->Update();
    
    typedef itk::ImageRegionIterator<LabelImageType> LabelImageIteratorType;
    typedef itk::ImageRegionConstIterator<Image8BitType> ConstImage8BitIteratorType;
    
    Image8BitType::Pointer pThreshold = pThresholdFilter->GetOutput();
    
    LabelImageIteratorType it(m_pLabelImage, m_pLabelImage->GetLargestPossibleRegion());
    ConstImage8BitIteratorType ci(pThreshold, pThreshold->GetLargestPossibleRegion());
        
    it.GoToBegin();
    ci.GoToBegin();
    
    while (!ci.IsAtEnd())
    {
        it.Value() *= ci.Value();
        ++it;
        ++ci;
    }
    
    METHOD_EXIT("SVMClassifier::applyUncertaintyOnLabelImage")
    return true;
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Calculate the distance maps to reliable results/regions.
///
/// \return Returns if operation was successful
///
///////////////////////////////////////////////////////////////////////////////
bool SVMClassifier::calculateDistanceMaps()
{
    METHOD_ENTRY("SVMClassifier::calculateDistanceMaps");
    
    if (!m_bGotClassificationResult)
    {
        ERROR_MSG("SVM Classifier", "No classification result. Can't calculate "
                                    "distance maps. ")
        METHOD_EXIT("SVMClassifier::calculateDistanceMaps");
        return false;
    }
    typedef itk::BinaryBallStructuringElement<Pixel8BitType,Dimension> StructuringType;
    typedef itk::BinaryThresholdImageFilter<ImageFloatType,Image8BitType> ThresholdFilterType;
    typedef itk::BinaryErodeImageFilter<Image8BitType,Image8BitType,StructuringType> ErodeFilterType;
    typedef itk::DanielssonDistanceMapImageFilter<Image8BitType, DistanceMapImageType > DistanceMapFilterType;
    
    std::vector<ImageFloatType::Pointer>::const_iterator ci = m_Probabilities.begin();
    
    ThresholdFilterType::Pointer pThresholdFilter = ThresholdFilterType::New();
    ErodeFilterType::Pointer pErodeFilter = ErodeFilterType::New();
    DistanceMapFilterType::Pointer pDistFilter;
    
    StructuringType StructuringElement;
    StructuringElement.SetRadius(m_pConfiguration->getFlt("DistanceMapErosionRadius"));
    StructuringElement.CreateStructuringElement();
    pErodeFilter->SetKernel(StructuringElement);
    
    // Clear content, in case this method is called more than once
    INFO_MSG("SVM Classifier", "Calculating distance maps.")    
    m_DistanceMaps.clear();
    
    int i=0;
    while (ci != m_Probabilities.end())
    {
        pThresholdFilter->SetInsideValue(255);
        pThresholdFilter->SetOutsideValue(0);
        pThresholdFilter->SetLowerThreshold(m_pConfiguration->getFlt("DistanceMapThreshold"));
        pThresholdFilter->SetUpperThreshold(1.0);
        pThresholdFilter->SetInput((*ci));
        pErodeFilter->SetInput(pThresholdFilter->GetOutput());
        pDistFilter = DistanceMapFilterType::New();
        pDistFilter->SetInput(pErodeFilter->GetOutput());
        pDistFilter->InputIsBinaryOn();
        pDistFilter->Update();
 
        DEBUG(
        {
            std::ostringstream oss("");
            oss << "DEBUG_Threshold_c" << i+1 << ".tif";
            saveImage(pErodeFilter->GetOutput(), oss.str());
        }
        {
            std::ostringstream oss("");
            oss << "DEBUG_DistanceMap_c" << i+1 << ".tif";
            saveImage(pDistFilter->GetDistanceMap(), oss.str());
        }
        );
        
        m_DistanceMaps.push_back(pDistFilter->GetDistanceMap());
        
        ++i;
        ++ci;
    }
    
    m_bGotDistanceMaps = true;
    
    ////////////////////////////////////////////////////////////////////////////
    // Refine the probabilities. Should become a seperate function
    ////////////////////////////////////////////////////////////////////////////
    {
        #include <cmath>
    
        // Define whole image as region, it's the same for all images
        ImageFloatType::RegionType Region1;
        DistanceMapImageType::RegionType Region2;
        Region1 = m_Probabilities[0]->GetLargestPossibleRegion();
        Region2 = m_Probabilities[0]->GetLargestPossibleRegion();
        
        typedef itk::ImageRegionIterator<ImageFloatType> FloatImageIteratorType;
        typedef itk::ImageRegionConstIterator<DistanceMapImageType> ConstDistanceMapImageIteratorType;
        
        std::vector<ImageFloatType::Pointer>::const_iterator ci = m_Probabilities.begin();
        std::vector<DistanceMapImageType::Pointer>::const_iterator cj = m_DistanceMaps.begin();
        
        float fDistanceMapImpact = m_pConfiguration->getFlt("DistanceMapImpact");
        
        while (ci != m_Probabilities.end())
        {
            FloatImageIteratorType it((*ci), Region1);
            ConstDistanceMapImageIteratorType ck((*cj), Region2);
            
            it.GoToBegin();
            ck.GoToBegin();
            while(!it.IsAtEnd())
            {
                it.Value() *= exp(-fDistanceMapImpact*ck.Value());
                ++ck;
                ++it;
            }
            ++ci;
            ++cj;
        }
        
        {
        // Normalise
        //-----------------------------------------------------------
        // Define whole image as region, it's the same for all images
        ImageFloatType::RegionType Region;
        Region = m_Probabilities[0]->GetLargestPossibleRegion();
        
        // Create new temporary image
        ImageFloatType::Pointer pTmp = ImageFloatType::New();
        pTmp->SetRegions(Region);
        pTmp->Allocate();
        pTmp->FillBuffer(0.0);
        
        typedef itk::ImageRegionIterator<ImageFloatType> FloatImageIteratorType;
        typedef itk::ImageRegionConstIterator<ImageFloatType> FloatImageConstIteratorType;
        
        std::vector<ImageFloatType::Pointer>::const_iterator ci = m_Probabilities.begin();
        
        m_pLabelImage->FillBuffer(0);
        
        INFO_MSG("SVM Classifier", "Normalising probabilities.")
        
        while (ci != m_Probabilities.end())
        {
            FloatImageConstIteratorType cj((*ci), Region);
            FloatImageIteratorType it(pTmp, Region);
                    
            it.GoToBegin();
            cj.GoToBegin();
            while(!cj.IsAtEnd())
            {
                it.Value() += cj.Value();
                ++it;
                ++cj;
            }
            ++ci;
        }
        
         while (ci != m_Probabilities.end())
        {
            FloatImageConstIteratorType cj(pTmp, Region);
            FloatImageIteratorType it((*ci), Region);
                    
            it.GoToBegin();
            cj.GoToBegin();
            while(!cj.IsAtEnd())
            {
                it.Value() /= cj.Value();
                ++it;
                ++cj;
            }
            ++ci;
        }
                
    }
    }
    
    METHOD_EXIT("SVMClassifier::calculateDistanceMaps");
    return true;
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Calculate uncertainty using the probabilities of classification
///        result.
///
/// \return Returns if operation was successful
///
///////////////////////////////////////////////////////////////////////////////
bool SVMClassifier::calculateUncertainty()
{
    METHOD_ENTRY("SVMClassifier::calculateUncertainty");
    
    if (!m_bGotClassificationResult)
    {
        ERROR_MSG("SVM Classifier", "No classification result. Can't calculate "
                                    "uncertainty. ")
        METHOD_EXIT("SVMClassifier::calculateUncertainty");
        return false;
    }
    
    typedef itk::ImageRegionConstIterator<ImageFloatType> ConstFloatImageIteratorType;
    typedef itk::ImageRegionIterator<ImageFloatType>      FloatImageIteratorType;
    
    // Define whole image as region, it's the same for all images
    ImageFloatType::RegionType Region;
    Region = m_Probabilities[0]->GetLargestPossibleRegion();
    
    // Create new image
    m_pUncertainty = ImageFloatType::New();
    m_pUncertainty->SetRegions(Region);
    m_pUncertainty->Allocate();
    m_pUncertainty->FillBuffer(0.0);
    
    std::vector<ImageFloatType::Pointer>::const_iterator ci = m_Probabilities.begin();
    std::vector<ImageFloatType::Pointer>::const_iterator cj = m_Probabilities.begin();
    
    FloatImageIteratorType it(m_pUncertainty, Region);
    it.GoToBegin();
    
    INFO_MSG("SVM Classifier","Calculating uncertainty of classification result.")
    
    // Calculate entropy
    #include <cmath>
    while (ci != m_Probabilities.end())
    {
        ConstFloatImageIteratorType ck((*ci), Region);
        it.GoToBegin();
        while(!it.IsAtEnd())
        {
            it.Value() -= ck.Value() * log2(ck.Value());
            ++ck;
            ++it;
        }
        ++ci;
    }
        
    METHOD_EXIT("SVMClassifier::calculateUncertainty");
    return true;
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Join probabilities from classification and reclassification
///
/// \param _strP1 Prefix for first set of probabilities
/// \param _strM1 First svm model
///
/// \return Returns if operation was successful
///
///////////////////////////////////////////////////////////////////////////////
bool SVMClassifier::joinProbabilities(const std::string& _strP1, const std::string& _strM1,
                                      const std::string& _strP2, const std::string& _strM2)
{
    METHOD_ENTRY("SVMClassifier::joinProbabilities");
    
//     typedef itk::ImageRegionConstIterator<ImageFloatType> ConstFloatImageIteratorType;
//     typedef itk::ImageRegionIterator<ImageFloatType>      FloatImageIteratorType;
    
    // Define whole image as region, it's the same for all images
//     ImageFloatType::RegionType Region;
//     Region = m_Probabilities[0]->GetLargestPossibleRegion();
    
    std::vector<ImageFloatType::Pointer> Probabilities1;
    std::vector<ImageFloatType::Pointer> Probabilities2;
    
    ModelType::Pointer pModel1 = ModelType::New();
    ModelType::Pointer pModel2 = ModelType::New();
    pModel1->LoadModel(_strM1.c_str());
    pModel2->LoadModel(_strM2.c_str());
    
    int* pLabelMap1 = pModel1->GetLabels();
    int* pLabelMap2 = pModel2->GetLabels();
    
    for (int i=0; i<m_unNumberOfClasses; ++i)
    {
        ReaderType::Pointer pReader = ReaderType::New();
        std::ostringstream oss("");
        oss << _strP1 << "_probability_c" << pLabelMap1[i] << ".tif";
        pReader->SetFileName(oss.str());
        pReader->Update();
        Probabilities1.push_back(pReader->GetOutput());
    }
    for (int i=0; i<m_unNumberOfClasses; ++i)
    {
        ReaderType::Pointer pReader = ReaderType::New();
        std::ostringstream oss("");
        oss << _strP2 << "_probability_c" << pLabelMap2[i] << ".tif";
        pReader->SetFileName(oss.str());
        pReader->Update();
        Probabilities2.push_back(pReader->GetOutput());
    }
        
    INFO_MSG("SVM Classifier","Joining probabilities.")
    
    ////////////////////////////////////////////////////////////////////////////
    // Create label image. Should become a seperate function
    ////////////////////////////////////////////////////////////////////////////

    // Define whole image as region, it's the same for all images
    ImageFloatType::RegionType Region;
    Region = Probabilities1[0]->GetLargestPossibleRegion();
    
    // Create new temporary image
    ImageFloatType::Pointer pTmp = ImageFloatType::New();
    pTmp->SetRegions(Region);
    pTmp->Allocate();
    pTmp->FillBuffer(0.0);
    
    typedef itk::ImageRegionIterator<ImageFloatType> FloatImageIteratorType;
    typedef itk::ImageRegionConstIterator<ImageFloatType> FloatImageConstIteratorType;
    typedef itk::ImageRegionIterator<LabelImageType> LabelImageIteratorType;
    
    std::vector<ImageFloatType::Pointer>::const_iterator ci = Probabilities1.begin();
    std::vector<ImageFloatType::Pointer>::const_iterator cj = Probabilities2.begin();
    
    m_pLabelImage->FillBuffer(0);
    
    INFO_MSG("SVM Classifier", "Creating label image from probabilities.")
    
    int i=0;
    while (ci != Probabilities1.end())
    {
        FloatImageConstIteratorType ck((*ci), Region);
        FloatImageConstIteratorType cl((*cj), Region);
        FloatImageIteratorType jt(pTmp, Region);
        LabelImageIteratorType it(m_pLabelImage, Region);
                
        it.GoToBegin();
        jt.GoToBegin();
        ck.GoToBegin();
        cl.GoToBegin();
        while(!it.IsAtEnd())
        {
            if (ck.Value() > jt.Value())
            {
                jt.Value() = ck.Value();
                it.Value() = pLabelMap1[i];
            }
            if (cj != Probabilities2.end())
            {
                if (cl.Value() > jt.Value())
                {
                    jt.Value() = cl.Value();
                    it.Value() = pLabelMap2[i];
                }
            }

            ++it;
            ++jt;
            ++ck;
            ++cl;
        }
        
        ++ci;
        if (cj != Probabilities2.end()) ++cj;
        ++i;
    }
    
//     for (int i=0; i<m_unNumberOfClasses; ++i)
//     {
//         std::ostringstream oss("");
//         oss << _strPrefix << "_probability_c" << m_LabelMap[i] << ".tif";
//         saveImage(m_Probabilities[i], oss.str());
//     }
    
//     // Calculate entropy
//     #include <cmath>
//     while (ci != m_Probabilities.end())
//     {
//         ConstFloatImageIteratorType ck((*ci), Region);
//         it.GoToBegin();
//         while(!it.IsAtEnd())
//         {
//             it.Value() -= ck.Value() * log2(ck.Value());
//             ++ck;
//             ++it;
//         }
//         ++ci;
//     }
        
    METHOD_EXIT("SVMClassifier::joinProbabilities")
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
    // Check if everthing's setup correctly
    //--------------------------------------------------------------------------
    if (!m_bGotFeatures)
    {
        ERROR_MSG("SVM Classifier", "No features. Get features from " <<
                    "Image Feature Extractor or load them. ")
        METHOD_EXIT("SVMClassifier::classify");
        return false;
    }
    if (m_Features.size() == 0)
    {
        ERROR_MSG("SVM Classifier", "Not enough feature points (0).")
        METHOD_EXIT("SVMClassifier::classify");
        return false;
    }
    if (m_pModel->GetNumberOfSupportVectors() == 0)
    {
        ERROR_MSG("SVM Classifier", "Not enough support vectors (0).")
        METHOD_EXIT("SVMClassifier::classify");
        return false;
    }
    if (!m_bGotSize)
    {
        WARNING_MSG("SVM Classifier", "No label image size specified. Resulting label image will be empty.")
        METHOD_EXIT("SVMClassifier::classify");
        return false;
    }
    if (m_bGotClassificationResult)
    {
        NOTICE_MSG("SVM Classifier", "Already got classification result, overwriting.")
    }

    INFO_MSG("SVM Classifier", "Starting classification.")

    //--------------------------------------------------------------------------
    // Start the classification
    //--------------------------------------------------------------------------
    DOM_VAR(DEBUG_MSG("SVM Classifier", "Number of features to be classified: " << m_Features.size()))
    DOM_VAR(DEBUG_MSG("SVM Classifier", "Number of classes: " << m_unNumberOfClasses))

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
    
    typedef itk::ImageRegionIterator<LabelImageType> OutputIteratorType;
    OutputIteratorType itOut(m_pLabelImage, m_pLabelImage->GetLargestPossibleRegion());
    itOut.GoToBegin();
    
    FeaturesType::const_iterator ci = m_Features.begin();
    while ((ci != m_Features.end()) && (!itOut.IsAtEnd()))
    {
        itOut.Set(m_pModel->EvaluateLabel((*ci)));
        ++ci;
        ++itOut;
    }
    
//     std::vector<std::vector<double> > Probabilities;
    int* pLabelMap;
//     
//     Probabilities = m_pClassifier->GetProbabilityEstimates();
    pLabelMap = m_pModel->GetLabels();
//     m_Probabilities.clear();
    m_LabelMap.clear();
    
    for (int i=0; i<m_unNumberOfClasses; ++i)
    {
        m_LabelMap.push_back(pLabelMap[i]);
    }
    
//     for (int i=0; i<m_unNumberOfClasses; ++i)
//     {
//         ImageFloatType::Pointer pProbabilityImage = ImageFloatType::New();
//         ImageFloatType::IndexType StartIndex;
//         ImageFloatType::SizeType Size;
//         ImageFloatType::RegionType Region;
//     
//         StartIndex[0]=0;
//         StartIndex[1]=0;
//         Region.SetIndex(StartIndex);
//         Region.SetSize(m_LabelImageSize);
//         
//         pProbabilityImage->SetRegions(Region);
//         pProbabilityImage->Allocate();
//         itk::ImageRegionIterator<ImageFloatType> itOut(pProbabilityImage, pProbabilityImage->GetLargestPossibleRegion());
//         itOut.GoToBegin();
//     
//         std::vector<std::vector<double> >::const_iterator ciIn = Probabilities.begin();
//         while (ciIn != Probabilities.end())
//         {
//             itOut.Set((*ciIn)[i]);
//             ++ciIn;
//             ++itOut;
//         }
//         
//         m_Probabilities.push_back(pProbabilityImage);
//     }

    INFO_MSG("SVM Classifier", "Classification done.")

    m_bGotClassificationResult = true;
    METHOD_EXIT("SVMClassifier::classify");
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
    METHOD_ENTRY("SVMClassifier::train");

    //--------------------------------------------------------------------------
    // Check if everthings setup correctly
    //--------------------------------------------------------------------------
    if (!m_bGotFeatures)
    {
        ERROR_MSG("SVM Classifier", "No features. Get features from " <<
                    "Image Feature Extractor or load them. ")
        METHOD_EXIT("SVMClassifier::train");
        return false;
    }
    if (!m_bGotLabels)
    {
        ERROR_MSG("SVM Classifier", "No labels. Get labels from " <<
                    "Image Feature Extractor or load them. ")
        METHOD_EXIT("SVMClassifier::train");
        return false;
    }
    
    DOM_VAR(DEBUG_MSG("SVM Classifier", "Labels  : " << m_Labels.size()))
    DOM_VAR(DEBUG_MSG("SVM Classifier", "Features: " << m_Features.size()))
    
    if (m_Features.size() == 0)
    {
        ERROR_MSG("SVM Classifier", "Not enough feature points (0).")
        METHOD_EXIT("SVMClassifier::train");
        return false;
    }
    // Zero label points will also be catched with the following condition
    if (m_Features.size() != m_Labels.size())
    {
        ERROR_MSG("SVM Classifier", "Number of labels and feature points differ.")
        METHOD_EXIT("SVMClassifier::train");
        return false;
    }
    
    if (m_bGotModel)
    {
        NOTICE_MSG("SVM Classifier", "SVM model already exists. Replacing.")
    }
    m_pModel = ModelType::New();
    m_bGotModel = true;
    
    //--------------------------------------------------------------------------
    // Start training process
    //--------------------------------------------------------------------------
    m_pModel->SetSVMType(C_SVC);
    m_pModel->SetKernelType(RBF);
    m_pModel->SetKernelGamma(0.0);
    m_pModel->SetKernelCoef0(0.0);
    m_pModel->SetPolynomialKernelDegree(3);
    m_pModel->SetNu(0.5);
    m_pModel->SetCacheSize(100);
    m_pModel->SetEpsilon(1.0e-3);
    m_pModel->SetC(1.0e3);
    m_pModel->SetP(0.1);
    m_pModel->DoShrinking(true);
    m_pModel->DoProbabilityEstimates(true);

    FeaturesType::iterator ci = m_Features.begin();
    LabelsType::const_iterator cj = m_Labels.begin();
    while (ci != m_Features.end())
    {
        FeatureVectorType vecF = (*ci);
        LabelPixelType L = (*cj);
        m_pModel->AddSample(vecF, L);
        ++ci;
        ++cj;
    }
    
    Log.logSeparator();
    m_pModel->Train();
    Log.logSeparator();
    
    m_unNumberOfClasses = m_pModel->GetNumberOfClasses();
    m_LabelMap.clear();
    for (int i=0; i<m_unNumberOfClasses; ++i)
    {
        m_LabelMap.push_back(m_pModel->GetLabels()[i]);
    }

    METHOD_EXIT("SVMClassifier::train");
    return true;
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Creates a label image by using the heighest probability
///
///////////////////////////////////////////////////////////////////////////////
void SVMClassifier::createLabelImageFromProbabilities()
{
    METHOD_ENTRY("SVMClassifier::createLabelImageFromProbabilities");
    
    // Define whole image as region, it's the same for all images
    ImageFloatType::RegionType Region;
    Region = m_Probabilities[0]->GetLargestPossibleRegion();
    
    // Create new temporary image
    ImageFloatType::Pointer pTmp = ImageFloatType::New();
    pTmp->SetRegions(Region);
    pTmp->Allocate();
    pTmp->FillBuffer(0.0);
    
    typedef itk::ImageRegionIterator<ImageFloatType> FloatImageIteratorType;
    typedef itk::ImageRegionConstIterator<ImageFloatType> FloatImageConstIteratorType;
    typedef itk::ImageRegionIterator<LabelImageType> LabelImageIteratorType;
    
    std::vector<ImageFloatType::Pointer>::const_iterator ci = m_Probabilities.begin();
    
    m_pLabelImage->FillBuffer(0);
    
    INFO_MSG("SVM Classifier", "Creating label image from probabilities.")
    
    int i=0;
    while (ci != m_Probabilities.end())
    {
        FloatImageConstIteratorType cj((*ci), Region);
        FloatImageIteratorType jt(pTmp, Region);
        LabelImageIteratorType it(m_pLabelImage, Region);
                
        it.GoToBegin();
        jt.GoToBegin();
        cj.GoToBegin();
        while(!it.IsAtEnd())
        {
            if (cj.Value() > jt.Value())
            {
                jt.Value() = cj.Value();
                it.Value() = m_LabelMap[i];
            }
            ++it;
            ++jt;
            ++cj;
        }
        ++ci;
        ++i;
    }
    
    METHOD_EXIT("SVMClassifier::createLabelImageFromProbabilities");
}
