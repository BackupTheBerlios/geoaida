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
/// \file       feature_extractor.cpp
/// \brief      Implementation of class "FeatureExtractor"
///
/// \date       2009-07-13
/// \author     Torsten Bueschenfeld (bfeld@tnt.uni-hannover.de)
///
////////////////////////////////////////////////////////////////////////////////

#include "feature_extractor.h"

#include <sstream>

DEBUG(
    uint FeatureExtractor::m_unNoImageFiles = 0;
)

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Constructor
///
///////////////////////////////////////////////////////////////////////////////
FeatureExtractor::FeatureExtractor():   m_pInputChannelList(ImageListType::New()),
                                        m_bChannelLoaded(false),
                                        m_bFeaturesExtracted(false),
                                        m_bLabelsExtracted(false),
                                        m_bLabelImageLoaded(false),
                                        m_bLabelSpacingSet(false),
                                        m_fPyramidDecimationRate(FEATURE_EXTRACTOR_DEFAULT_PYRAMID_DECIMATION_RATE),
                                        m_nFilterRadius(FEATURE_EXTRACTOR_DEFAULT_FILTER_RADIUS),
                                        m_nNumberOfPyramidLevels(FEATURE_EXTRACTOR_DEFAULT_NUMBER_OF_PYRAMID_LEVELS),
                                        m_nLabelSpacingX(FEATURE_EXTRACTOR_DEFAULT_SPACING),
                                        m_nLabelSpacingY(FEATURE_EXTRACTOR_DEFAULT_SPACING),
                                        m_nLabelBorderX(FEATURE_EXTRACTOR_DEFAULT_FILTER_RADIUS),
                                        m_nLabelBorderY(FEATURE_EXTRACTOR_DEFAULT_FILTER_RADIUS),
                                        m_pConfiguration(0)
{
    METHOD_ENTRY("FeatureExtractor::FeatureExtractor()");

    METHOD_EXIT("FeatureExtractor::FeatureExtractor()");
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Returns the features
///
/// \return Features
///
///////////////////////////////////////////////////////////////////////////////
FeaturesType& FeatureExtractor::getFeatures()
{
    METHOD_ENTRY("FeatureExtractor::getFeatures()");

    if (!m_bFeaturesExtracted)
    {
        WARNING_MSG("Image Feature Extractor", "No features list was loaded or extracted.")
    }
    METHOD_EXIT("FeatureExtractor::getFeatures()");
    return m_Features;
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Returns the labels
///
/// \return Labels
///
///////////////////////////////////////////////////////////////////////////////
const LabelsType& FeatureExtractor::getLabels() const
{
    METHOD_ENTRY("FeatureExtractor::getLabels()");

    if (!m_bLabelsExtracted)
    {
        WARNING_MSG("Image Feature Extractor", "No label list was loaded or extracted.")
    }

    METHOD_EXIT("FeatureExtractor::getLabels()");
    return m_Labels;
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Returns the size of the label image
///
/// \return Returns label image size
///
///////////////////////////////////////////////////////////////////////////////
const LabelImageType::SizeType FeatureExtractor::getImageSize() const
{
    METHOD_ENTRY("FeatureExtractor::getImageSize()");

    if (!m_bChannelLoaded)
    {
        WARNING_MSG("Image Feature Extractor", "No images were loaded. Can't determine size.")
    }

    METHOD_EXIT("FeatureExtractor::getImageSize()");
    return m_ImageSize;
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Method to add another input channel
///
/// \param ImageName Filename of the input image
///
///////////////////////////////////////////////////////////////////////////////
void FeatureExtractor::addInputChannel(const std::string& ImageName)
{
    METHOD_ENTRY("FeatureExtractor::addInputChannel(const std::string&)");

    INFO_MSG("Image Feature Extractor", "Reading input channel " << ImageName)
    
    ReaderType::Pointer pReader = ReaderType::New();

    pReader->SetFileName(ImageName);
        
    // Scale all images to 16 bit co-domain since we don't know the actual depth
//     typedef itk::RescaleIntensityImageFilter<InputImageType,InputImageType> FilterType;
//     
//     FilterType::Pointer pFilter = FilterType::New();
//     pFilter->SetOutputMinimum(0);
//     pFilter->SetOutputMaximum(65535);
//     pFilter->SetInput(pReader->GetOutput());
//     pFilter->Update();
    pReader->Update();
    m_pInputChannelList->PushBack(pReader->GetOutput());
    
//     DEBUG(
//     {
//         std::ostringstream oss("");
//         oss << "DEBUG_input_channel_" << m_unNoImageFiles << ".tif";
//         saveImage(pReader->GetOutput(), oss.str());
//         ++m_unNoImageFiles;
//     }
//     );
    
//  m_pInputChannelList->Update();  
    
    m_ImageSize = m_pInputChannelList->Front()->GetLargestPossibleRegion().GetSize();
    DOM_VAR(DEBUG_MSG("Image Feature Extractor", "Size: " << m_ImageSize))

    DOM_VAR(INFO_MSG("Image Feature Extractor", "Number of channels: " << m_pInputChannelList->Size()))
    m_bChannelLoaded = true;

    METHOD_EXIT("FeatureExtractor::addInputChannel(const std::string&)");
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Method to clear all channels from list
///
///////////////////////////////////////////////////////////////////////////////
void FeatureExtractor::clearChannels()
{
    METHOD_ENTRY("FeatureExtractor::clearChannels()");

    DEBUG_MSG("Image Feature Extractor", "Removing input channels from memory.")
    
    m_pInputChannelList->Clear();
    m_bChannelLoaded = false;

    METHOD_EXIT("FeatureExtractor::clearChannels()");
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Method to clear all features from point set
///
///////////////////////////////////////////////////////////////////////////////
void FeatureExtractor::clearFeatures()
{
    METHOD_ENTRY("FeatureExtractor::clearFeatures")

    DEBUG_MSG("Image Feature Extractor", "Removing features from memory.")
    
    m_Features.clear();
    m_bFeaturesExtracted = false;

    METHOD_EXIT("FeatureExtractor::clearFeatures")
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Method to clear all labels from point set
///
///////////////////////////////////////////////////////////////////////////////
void FeatureExtractor::clearLabels()
{
    METHOD_ENTRY("FeatureExtractor::clearLabels")

    DEBUG_MSG("Image Feature Extractor", "Removing labels from memory.")
    
    m_Labels.clear();
    m_bLabelsExtracted = false;

    METHOD_EXIT("FeatureExtractor::clearLabels")
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Method to extract features from image
///
/// In this method, features are extracted from the image. All previously
/// added channels will be used to build a vector image. Filters and feature
/// extractors are applied on this vector image.
///
/// \param _nMode Mode of operation. 
///
/// \return Returns if extraction was successful
///
///////////////////////////////////////////////////////////////////////////////
bool FeatureExtractor::extract(const unsigned char& _nMode)
{
    METHOD_ENTRY("FeatureExtractor::extract()");

    if (m_pInputChannelList->Size() * 2 * (m_nNumberOfPyramidLevels+1) > FEATURE_VEC_SIZE_MAX)
    {
        DOM_VAR(INFO_MSG ("Image Feature Extractor", "Maximum feature vector size: " <<
                            FEATURE_VEC_SIZE_MAX))
        ERROR_MSG("Image Feature Extractor", "Maximum feature vector size exceeded. "
                  "Reduce pyramid level, number of input channels or number of "
                  "features.")
        METHOD_EXIT("FeatureExtractor::extract()");
        return false;
    }
    
    m_nLabelBorderX = static_cast<int>(
                        static_cast<double>(m_nFilterRadius) * 
                        pow(m_fPyramidDecimationRate, m_nNumberOfPyramidLevels)
                        + 1);
    m_nLabelBorderY = m_nLabelBorderX;

    DOM_VAR(DEBUG_MSG("Image Feature Extractor", "Label border calculated: " <<
                m_nLabelBorderX))
    
    switch (_nMode)
    {
        //--------------------------------------------------------------------------
        // Extraction with labels
        //--------------------------------------------------------------------------
        case FEATURE_EXTRACTOR_USE_LABELS:
        {
            if (!m_bLabelImageLoaded)
            {
                ERROR_MSG("Image Feature Extractor", "No label image loaded.")
                METHOD_EXIT("FeatureExtractor::extract()");
                return false;
            }
            if (!m_bLabelSpacingSet)
            {
                NOTICE_MSG("Image Feature Extractor", "Label spacing not set. Using default value " <<
                            FEATURE_EXTRACTOR_DEFAULT_SPACING << " for all.")
            }
            
            this->extractWithLabels();
            m_bLabelsExtracted = true;
            break;
        }
        //--------------------------------------------------------------------------
        // Extraction without labels
        //--------------------------------------------------------------------------
        case FEATURE_EXTRACTOR_DEFAULT:
        {
            this->extractWithoutLabels();
            break;
        }
    }

    m_bFeaturesExtracted = true;
    METHOD_EXIT("FeatureExtractor::extract()");
    return true;
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Loads a label image.
///
/// \param _ImageName Filename of the input image
///
///////////////////////////////////////////////////////////////////////////////
void FeatureExtractor::loadLabelImage(const std::string& _ImageName)
{
    METHOD_ENTRY("FeatureExtractor::loadLabelImage(const std::string&)");

    if (m_bLabelImageLoaded == true)
    {
        NOTICE_MSG("Image Feature Extractor", "Labelimage already loaded. Reloading.")
    }

    LabelReaderType::Pointer pLabelReader = LabelReaderType::New();

    pLabelReader->SetFileName(_ImageName);
    pLabelReader->Update();

    INFO_MSG("Image Feature Extractor", "Label image loaded.")

    m_pLabelImage = pLabelReader->GetOutput();
    m_bLabelImageLoaded = true;
    
    METHOD_EXIT("FeatureExtractor::loadLabelImage(const std::string&)");
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Sets a label image.
///
/// \param _pImage Image
///
///////////////////////////////////////////////////////////////////////////////
void FeatureExtractor::setLabelImage(LabelImageType::Pointer _pImage)
{
    METHOD_ENTRY("FeatureExtractor::setLabelImage")

    if (m_bLabelImageLoaded == true)
    {
        NOTICE_MSG("Image Feature Extractor", "Labelimage already set. Resetting.")
    }

    INFO_MSG("Image Feature Extractor", "Label image set.")

    m_pLabelImage = _pImage;
    m_bLabelImageLoaded = true;
    
    METHOD_EXIT("FeatureExtractor::setLabelImage")
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Configures the size of filters
///
/// \param _nR Size of filters (radius)
///
///////////////////////////////////////////////////////////////////////////////
void FeatureExtractor::setFilterRadius(const int& _nR)
{
    METHOD_ENTRY("FeatureExtractor::setFilterRadius(const int&)");

    m_nFilterRadius = _nR;

    METHOD_EXIT("FeatureExtractor::setFilterRadius(const int&)");
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Configures label image for training
///
/// \param _nSX Spacing of samples in label image, x direction
/// \param _nSX Spacing of samples in label image, y direction
///
///////////////////////////////////////////////////////////////////////////////
void FeatureExtractor::setLabelSpacing(const int& _nSX, const int& _nSY)
{
    METHOD_ENTRY("FeatureExtractor::setLabelSpacing(const int&, const int&)");

    if (m_bLabelSpacingSet == true)
    {
        NOTICE_MSG("Image Feature Extractor", "Already set label image spacing. Overwriting.")
    }

    m_nLabelSpacingX = _nSX;
    m_nLabelSpacingY = _nSY;

    m_bLabelSpacingSet = true;

    METHOD_EXIT("FeatureExtractor::setLabelSpacing(const int&, const int&)");
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Configures image pyramid
///
/// \param _nNoPL Number of levels for image pyramid
///
///////////////////////////////////////////////////////////////////////////////
void FeatureExtractor::setNumberOfPyramidLevels(const int& _nNoPL)
{
    METHOD_ENTRY("FeatureExtractor::setNumberOfPyramidLevels(const int&)");

    m_nNumberOfPyramidLevels = _nNoPL-1; // -1 additional levels
    DOM_VAR(INFO_MSG("Image Feature Extractor", "Number of levels for image "
                     "pyramid set to " << _nNoPL))
            
    METHOD_EXIT("FeatureExtractor::setNumberOfPyramidLevels(const int&)");
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Configures image pyramid
///
/// \param _fPDR Rate for subsampling of images within pyramid
///
///////////////////////////////////////////////////////////////////////////////
void FeatureExtractor::setPyramidDecimationRate(const double& _fPDR)
{
    METHOD_ENTRY("FeatureExtractor::setPyramidDecimationRate(const double&)");

    m_fPyramidDecimationRate = _fPDR;
    DOM_VAR(INFO_MSG("Image Feature Extractor", "Decimation Rate for image pyramid "
                     "set to " << _fPDR))

    METHOD_EXIT("FeatureExtractor::setPyramidDecimationRate(const double&)");
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Method to extract features from image incorporating label image.
///
/// In this method the label image is taken into consideration. Since it is
/// often not intended to extract features of every pixel, but from certain
/// pixel of a grid with border, other ORFEO methods for efficient extraction
/// are used.
///
/// \return Returns if extraction was successful
///
///////////////////////////////////////////////////////////////////////////////
bool FeatureExtractor::extractWithLabels()
{
    METHOD_ENTRY("FeatureExtractor::extractWithLabels()");
    
    ////////////////////////////////////////////////////////////////////////////
    // Setup common stuff
    ////////////////////////////////////////////////////////////////////////////

    //--- Define image filters ---//
    typedef LabelImageType::RegionType ValidateRegionType;
    typedef itk::ImageRegionConstIterator<LabelImageType> ConstLabelImageIteratorType;
    
    typedef itk::MeanImageFilter<InputImageType,InputImageType> MeanFilterType;
    typedef itk::GradientMagnitudeImageFilter<InputImageType,InputImageType> GradientFilterType;
    typedef otb::Functor::VarianceTextureFunctor<InputPixelType,InputPixelType> FunctorType;
    typedef otb::TextureImageFunction<InputImageType,FunctorType> VarianceFilterType;
    
    MeanFilterType::Pointer         pMeanFilter = MeanFilterType::New();
    GradientFilterType::Pointer     pGradientFilter = GradientFilterType::New();
    VarianceFilterType::Pointer     TextureFilter = VarianceFilterType::New();
        
    InputImageType::SizeType        nRadius;
    InputImageType::IndexType       nFeatureIndex;
    InputImageType::OffsetType      nOffset;
    InputImageType::SizeType        nSize;
    LabelImageType::IndexType       nLabelIndex;

    nRadius[0] = m_nFilterRadius;
    nRadius[1] = m_nFilterRadius;
    nOffset[0] = 5;
    nOffset[1] = 0;
    TextureFilter->SetRadius(nRadius);
    TextureFilter->SetOffset(nOffset);
    pMeanFilter->SetRadius(nRadius);
    
    m_pInputChannelList->Update();
    ImageListType::ConstIterator it = m_pInputChannelList->Begin();
    nSize = m_ImageSize;
    uint nChannel = 0u;
    uint nX;
    uint nY;
    double fPyramidLevelFactor;
    std::vector<FeatureVectorType> FeaturePoints;
    std::vector<LabelPixelType> LabelPoints;

    INFO_MSG("Image Feature Extractor", "Reading labels")
                    
    INFO_MSG("Image Feature Extractor", "Calculating features of channel 0")

    ////////////////////////////////////////////////////////////////////////////
    // First run to fill LabelPoints vector and first dimension and therfore
    // size of FeaturePoints vector
    ////////////////////////////////////////////////////////////////////////////
    
    //--------------------------
    // Initialise image pyramid
    //--------------------------
    PyramidFilterType::Pointer pPyramid = PyramidFilterType::New();
    pPyramid->SetNumberOfLevels(m_nNumberOfPyramidLevels);
    pPyramid->SetDecimationRatio(m_fPyramidDecimationRate);
    pPyramid->SetInput(it.Get());
    pPyramid->Update();
    fPyramidLevelFactor = 1.0;
    ImageListIterator itP = pPyramid->GetOutput()->Begin();
    
    pGradientFilter->SetInput(it.Get());
    pGradientFilter->Update();
    TextureFilter->SetInputImage(pGradientFilter->GetOutput());
    pMeanFilter->SetInput(it.Get());
    pMeanFilter->Update();
    
    DOM_VAR(INFO_MSG("Image Feature Extractor", "New pyramid level, size: " <<
                     pMeanFilter->GetOutput()->GetLargestPossibleRegion().GetSize()))

    // For debugging draw an image of feature extraction points
    DEBUG(
        Image8BitType::Pointer pValidationMap = Image8BitType::New();
        Image8BitType::IndexType StartIndex;
        Image8BitType::RegionType Region;

        StartIndex[0]=0;
        StartIndex[1]=0;
        Region.SetIndex(StartIndex);
        Region.SetSize(m_ImageSize);
        pValidationMap->SetRegions(Region);
        pValidationMap->Allocate();
        pValidationMap->FillBuffer(0);
    );
    DEBUG(uint nPyramidLevel = 0u;)
    DEBUG(
    {
        std::ostringstream oss("");
        oss << "DEBUG_mean_image_ch1" << "_l" << nPyramidLevel << ".tif";
        saveImage(pMeanFilter->GetOutput(), oss.str());
    }
    );
    DEBUG(
    {
        std::ostringstream oss("");
        oss << "DEBUG_gradient_image_ch1" << "_l" << nPyramidLevel << ".tif";
        saveImage(pGradientFilter->GetOutput(), oss.str());
    }
    );
//     DEBUG(
//     {
//         std::ostringstream oss("");
//         oss << "DEBUG_varianceofgradient_image_ch1"<< "_l" << nPyramidLevel << ".tif";
//         saveImage(TextureFilter->GetOutput(), oss.str());
//     }
//     );


    //--------------------------------------------------------------------------
    // Store first feature vector component, build up list for further channels
    //--------------------------------------------------------------------------
    std::list<InputImageType::IndexType> InputIndexList;
    nY = m_nLabelBorderY;
    while (nY < nSize[1]-m_nLabelBorderY)
    {
        nX = m_nLabelBorderX;
        nFeatureIndex[1] = nY;
        nLabelIndex[1] = nY;
        while (nX < nSize[0]-m_nLabelBorderX)
        {
            nFeatureIndex[0] = nX;
            nLabelIndex[0] = nX;

            ////////////////////////////////////////////////////////////////////
            // Validate label index inside of image region. This is done to
            // ensure only training of features that are calculated on a single
            // label, outside of borders. Iteration through image region should
            // be faster than GetPixel methods.
            ////////////////////////////////////////////////////////////////////
            ValidateRegionType Region;
            ValidateRegionType::IndexType RgStart;
            ValidateRegionType::SizeType RgSize;
            
            RgStart[0] = nX-int(m_nLabelBorderX);
            RgStart[1] = nY-int(m_nLabelBorderY);
            RgSize[0] = int(m_nLabelBorderX)*2+1;
            RgSize[1] = int(m_nLabelBorderY)*2+1;
            Region.SetIndex(RgStart);
            Region.SetSize(RgSize);
            ConstLabelImageIteratorType ciR(m_pLabelImage, Region);
            
            ciR.GoToBegin();
            LabelPixelType nLabel = ciR.Value(); ++ciR;
            bool bValid = true;
            while (!ciR.IsAtEnd())
            {
                if ((ciR.Value() != nLabel) || (ciR.Value() == 0))
                {
                    bValid = false;
                    ciR.GoToEnd();
                }
                else
                {
                    ++ciR;
                }
            }
            ////////////////////////////////////////////////////////////////////
            // If image region is valid, create training data and save the index
            // for further channels.
            ////////////////////////////////////////////////////////////////////
            if (bValid)
            {
                FeatureVectorType vecFeature;
                vecFeature.push_back(TextureFilter->EvaluateAtIndex(nFeatureIndex));
                vecFeature.push_back(pMeanFilter->GetOutput()->GetPixel(nFeatureIndex));
                m_Features.push_back(vecFeature);
            
                m_Labels.push_back(m_pLabelImage->GetPixel(nLabelIndex));

                // Fill validation map for debugging purposes
                DEBUG(
                    pValidationMap->SetPixel(nLabelIndex,255);
                );

                InputIndexList.push_back(nFeatureIndex);
            }
            
            nX += m_nLabelSpacingX;
        }
        nY += m_nLabelSpacingY;
    }
    ++it;
    ++nChannel;
    
    // Save the validation map
    DEBUG(
        saveImage(pValidationMap, "DEBUG_validation_map.tif");
    );

    ////////////////////////////////////////////////////////////////////////////
    // Now do the same for all other channels.
    ////////////////////////////////////////////////////////////////////////////
    while (!((it == m_pInputChannelList->End()) && (itP == pPyramid->GetOutput()->End())))
    {
        int i=0;
    
        //-----------------------------------------------
        // Create new filters due to changing image size
        //-----------------------------------------------
        TextureFilter = VarianceFilterType::New();
        TextureFilter->SetRadius(nRadius);
        TextureFilter->SetOffset(nOffset);
        
        pMeanFilter = MeanFilterType::New();
        pMeanFilter->SetRadius(nRadius);
        
        pGradientFilter = GradientFilterType::New();
        
        //---------------------------------------------------
        // Calculate on sublevels or start new pyramid level
        //---------------------------------------------------
        if (itP != pPyramid->GetOutput()->End())
        {           
            pGradientFilter->SetInput(itP.Get());
            pGradientFilter->Update();
            TextureFilter->SetInputImage(pGradientFilter->GetOutput());
            pMeanFilter->SetInput(itP.Get());
            pMeanFilter->Update();
            fPyramidLevelFactor *= 1/m_fPyramidDecimationRate;
            pPyramid->GetOutput()->Erase(itP); // Free some memory, implies ++itP
            DEBUG(++nPyramidLevel;);
            
            DOM_VAR(INFO_MSG("Image Feature Extractor", "New pyramid level, size: " <<
                             pMeanFilter->GetOutput()->GetLargestPossibleRegion().GetSize()))
            DOM_VAR(DEBUG_MSG("Image Feature Extractor", "Pyramid level factor: " <<
                              fPyramidLevelFactor))
        }
        else
        {
            INFO_MSG("Image Feature Extractor", "Calculating features of channel " <<
                    nChannel)
            pGradientFilter->SetInput(it.Get());
            pGradientFilter->Update();
            TextureFilter->SetInputImage(pGradientFilter->GetOutput());
            pMeanFilter->SetInput(it.Get());
            pMeanFilter->Update();
            
            pPyramid = PyramidFilterType::New();
            pPyramid->SetNumberOfLevels(m_nNumberOfPyramidLevels);
            pPyramid->SetDecimationRatio(m_fPyramidDecimationRate);
            pPyramid->SetInput(it.Get());
            pPyramid->Update();
            fPyramidLevelFactor = 1.0;
            
            itP = pPyramid->GetOutput()->Begin();
            DEBUG(nPyramidLevel = 0u;);
            
            ++it;
            ++nChannel;
            
            DOM_VAR(INFO_MSG("Image Feature Extractor", "New pyramid level, size: " <<
                             pMeanFilter->GetOutput()->GetLargestPossibleRegion().GetSize()))
        }
        DEBUG(
        {
            std::ostringstream oss("");
            oss << "DEBUG_mean_image_ch" << nChannel << "_l" << nPyramidLevel << ".tif";
            saveImage(pMeanFilter->GetOutput(), oss.str());
        }
        );
        DEBUG(
        {
            std::ostringstream oss("");
            oss << "DEBUG_gradient_image_ch" << nChannel << "_l" << nPyramidLevel << ".tif";
            saveImage(pGradientFilter->GetOutput(), oss.str());
        }
        );
//         DEBUG(
//         {
//             std::ostringstream oss("");
//             oss << "DEBUG_varianceofgradient_image_" << nChannel << "_l" << nPyramidLevel << ".tif";
//             saveImage(TextureFilter->GetOutput(), oss.str());
//         }
//         );
        //--------------------------------
        // Store feature vector component
        //--------------------------------
        std::list<InputImageType::IndexType>::const_iterator ciIndex = InputIndexList.begin();
        while (ciIndex != InputIndexList.end())
        {
            InputImageType::IndexType Index;
            Index[0] = (*ciIndex)[0]*fPyramidLevelFactor;
            Index[1] = (*ciIndex)[1]*fPyramidLevelFactor;
            m_Features[i].push_back(TextureFilter->EvaluateAtIndex(Index));
            m_Features[i].push_back(pMeanFilter->GetOutput()->GetPixel(Index));
            ++i;
            ++ciIndex;
        }
    }

    METHOD_EXIT("FeatureExtractor::extractWithLabels()");
    return true;
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Method to extract features from image.
///
/// Since it is common for to extract features for every pixel when in
/// classification, ORFEO methods to filter the whole image are used.
///
/// \return Returns if extraction was successful
///
////////////////////////////////////////////////////////////////////////////////
bool FeatureExtractor::extractWithoutLabels()
{
    METHOD_ENTRY("FeatureExtractor::extractWithoutLabels()");
    
    ////////////////////////////////////////////////////////////////////////////
    // Setup common stuff
    ////////////////////////////////////////////////////////////////////////////

    //--- Define image filters ---//
    typedef itk::ImageRegionConstIterator<InputImageType> ConstInputImageIteratorType;
    
    typedef itk::MeanImageFilter<InputImageType,InputImageType> MeanFilterType;
    typedef itk::GradientMagnitudeImageFilter<InputImageType,InputImageType> GradientFilterType;
    typedef otb::Functor::VarianceTextureFunctor<InputPixelType,InputPixelType> FunctorType;
    typedef otb::UnaryFunctorNeighborhoodWithOffsetImageFilter< InputImageType,
                                                            InputImageType,
                                                            FunctorType>
    VarianceFilterType;
    
    GradientFilterType::Pointer pGradientFilter = GradientFilterType::New();
    MeanFilterType::Pointer     pMeanFilter = MeanFilterType::New();
    VarianceFilterType::Pointer TextureFilter = VarianceFilterType::New();
    
    InputImageType::SizeType        nRadius;
    InputImageType::IndexType       nFeatureIndex;
    InputImageType::OffsetType      nOffset;
    InputImageType::SizeType        nSize;

    nRadius[0] = m_nFilterRadius;
    nRadius[1] = m_nFilterRadius;
    nOffset[0] = 5;
    nOffset[1] = 0;
    TextureFilter->SetRadius(nRadius);
    TextureFilter->SetOffset(nOffset);

    pMeanFilter->SetRadius(nRadius);
    
//     m_pInputChannelList->Update();
    ImageListType::Iterator it = m_pInputChannelList->Begin();
//     nSize = it.Get()->GetLargestPossibleRegion().GetSize();
    nSize = m_ImageSize;
    uint nChannel = 0u;
    uint nX;
    uint nY;
    double fPyramidLevelFactor;
    
    INFO_MSG("Image Feature Extractor", "Calculating features of channel 0")

    ////////////////////////////////////////////////////////////////////////////
    // First run to fill LabelPoints vector and first dimension and therfore
    // size of FeaturePoints vector
    ////////////////////////////////////////////////////////////////////////////
    
    //--------------------------
    // Initialise image pyramid
    //--------------------------
    PyramidFilterType::Pointer pPyramid = PyramidFilterType::New();
    pPyramid->SetNumberOfLevels(m_nNumberOfPyramidLevels);
    pPyramid->SetDecimationRatio(m_fPyramidDecimationRate);
    pPyramid->SetInput(it.Get());
    pPyramid->Update();
    fPyramidLevelFactor = 1.0;
    ImageListIterator itP = pPyramid->GetOutput()->Begin();

    pGradientFilter->SetInput(it.Get());
    TextureFilter->SetInput(pGradientFilter->GetOutput());
    TextureFilter->Update();
    pMeanFilter->SetInput(it.Get());
    pMeanFilter->Update();
    
    DOM_VAR(INFO_MSG("Image Feature Extractor", "New pyramid level, size: " <<
                      pMeanFilter->GetOutput()->GetLargestPossibleRegion().GetSize()))
    
    DEBUG(uint nPyramidLevel = 0u;);
    DEBUG(
    {
        std::ostringstream oss("");
        oss << "DEBUG_mean_image_ch1" << "_l" << nPyramidLevel << ".tif";
        saveImage(pMeanFilter->GetOutput(), oss.str());
    }
    );
    DEBUG(
    {
        std::ostringstream oss("");
        oss << "DEBUG_gradient_image_ch1" << "_l" << nPyramidLevel << ".tif";
        saveImage(pGradientFilter->GetOutput(), oss.str());
    }
    );
    DEBUG(
    {
        std::ostringstream oss("");
        oss << "DEBUG_varianceofgradient_image_ch1" << "_l" << nPyramidLevel << ".tif";
        saveImage(TextureFilter->GetOutput(), oss.str());
    }
    );

    //--------------------------------------
    // Store first feature vector component
    //--------------------------------------
    nY = 0;
    while (nY < nSize[1])
    {
        nX = 0;
        nFeatureIndex[1] = nY;
        while (nX < nSize[0])
        {
            nFeatureIndex[0] = nX;
            
            FeatureVectorType vecFeature;
            vecFeature.push_back(TextureFilter->GetOutput()->GetPixel(nFeatureIndex));
            vecFeature.push_back(pMeanFilter->GetOutput()->GetPixel(nFeatureIndex));
            m_Features.push_back(vecFeature);
            
            ++nX;
        }
        ++nY;
    }
    ++it;
    ++nChannel;
    
    ////////////////////////////////////////////////////////////////////////////
    // Now do the same for all other channels.
    ////////////////////////////////////////////////////////////////////////////
    while (!((it == m_pInputChannelList->End()) && (itP == pPyramid->GetOutput()->End())))
    {
        int i=0;
        
        //-----------------------------------------------
        // Create new filters due to changing image size
        //-----------------------------------------------
        TextureFilter = VarianceFilterType::New();
        TextureFilter->SetRadius(nRadius);
        TextureFilter->SetOffset(nOffset);
        
        pMeanFilter = MeanFilterType::New();
        pMeanFilter->SetRadius(nRadius);
        
        pGradientFilter = GradientFilterType::New();
        
        //---------------------------------------------------
        // Calculate on sublevels or start new pyramid level
        //---------------------------------------------------
        if (itP != pPyramid->GetOutput()->End())
        {
            pGradientFilter->SetInput(itP.Get());
            TextureFilter->SetInput(pGradientFilter->GetOutput());
            TextureFilter->Update();
            pMeanFilter->SetInput(itP.Get());
            pMeanFilter->Update();
            fPyramidLevelFactor *= 1/m_fPyramidDecimationRate;
            pPyramid->GetOutput()->Erase(itP); // Free some memory, implies ++itP
            DEBUG(++nPyramidLevel;);
            
            DOM_VAR(INFO_MSG("Image Feature Extractor", "New pyramid level, size: " <<
                             pMeanFilter->GetOutput()->GetLargestPossibleRegion().GetSize()))
            DOM_VAR(DEBUG_MSG("Image Feature Extractor", "Pyramid level factor: " <<
                              fPyramidLevelFactor))
        }
        else
        {
            INFO_MSG("Image Feature Extractor", "Calculating features of channel " <<
                    nChannel)
            pGradientFilter->SetInput(it.Get());
            TextureFilter->SetInput(pGradientFilter->GetOutput());
            TextureFilter->Update();
            pMeanFilter->SetInput(it.Get());
            pMeanFilter->Update();
            
            pPyramid = PyramidFilterType::New();
            pPyramid->SetNumberOfLevels(m_nNumberOfPyramidLevels);
            pPyramid->SetDecimationRatio(m_fPyramidDecimationRate);
            pPyramid->SetInput(it.Get());
            pPyramid->Update();
            fPyramidLevelFactor = 1.0;
            
            itP = pPyramid->GetOutput()->Begin();
            DEBUG(nPyramidLevel = 0u;);
            
            ++it;
            ++nChannel;
            
            DOM_VAR(INFO_MSG("Image Feature Extractor", "New pyramid level, size: " <<
                             pMeanFilter->GetOutput()->GetLargestPossibleRegion().GetSize()))
        }
        DEBUG(
        {
            std::ostringstream oss("");
            oss << "DEBUG_mean_image_ch" << nChannel << "_l" << nPyramidLevel << ".tif";
            saveImage(pMeanFilter->GetOutput(), oss.str());
        }
        );
        DEBUG(
        {
            std::ostringstream oss("");
            oss << "DEBUG_gradient_image_ch" << nChannel << "_l" << nPyramidLevel << ".tif";
            saveImage(pGradientFilter->GetOutput(), oss.str());
        }
        );
        DEBUG(
        {
            std::ostringstream oss("");
            oss << "DEBUG_varianceofgradient_image_ch" << nChannel << "_l" << nPyramidLevel << ".tif";
            saveImage(TextureFilter->GetOutput(), oss.str());
        }
        );

        //--------------------------------
        // Store feature vector component
        //--------------------------------
        nY = 0;
        while (nY < nSize[1])
        {
            nX = 0;
            nFeatureIndex[1] = double(nY)*fPyramidLevelFactor;
            while (nX < nSize[0])
            {
                nFeatureIndex[0] = double(nX)*fPyramidLevelFactor;
                
                m_Features[i].push_back(TextureFilter->GetOutput()->GetPixel(nFeatureIndex));
                m_Features[i].push_back(pMeanFilter->GetOutput()->GetPixel(nFeatureIndex));
                ++i;
                
                ++nX;
            }
            ++nY;
        }
    }
    
    METHOD_EXIT("FeatureExtractor::extractWithoutLabels()");
    return true;
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Gets parameters for feature extraction from Configurator
///
/// \return If configurator has not been defined, false will be returned
///
///////////////////////////////////////////////////////////////////////////////
bool FeatureExtractor::setParam()
{
    METHOD_ENTRY("FeatureExtractor::setParam()");
    
    if (m_pConfiguration == 0)
    {
        ERROR_MSG("Image Feature Extractor", "No parameters loaded.")
        METHOD_EXIT("FeatureExtractor::setParam()");
        return false;
    }

    m_nNumberOfPyramidLevels = m_pConfiguration->getInt("NumberOfPyramidLevels");
    m_fPyramidDecimationRate = m_pConfiguration->getFlt("PyramidDecimationRate");
    m_nFilterRadius = m_pConfiguration->getInt("FilterRadius");
    m_nLabelSpacingX = m_pConfiguration->getInt("LabelSpacingX");
    m_nLabelSpacingY = m_pConfiguration->getInt("LabelSpacingY");
   
    Log.logSeparator(LOG_LEVEL_INFO);
    INFO(
        std::cout << "Parameters: " << std::endl;
        std::cout << "  Number of pyramid levels: " << m_nNumberOfPyramidLevels << std::endl;
        std::cout << "  Pyramid decimation rate:  " << m_fPyramidDecimationRate << std::endl;
        std::cout << "  Filter radius:            " << m_nFilterRadius << std::endl;
        std::cout << "  Label Spacing:            " << m_nLabelSpacingX << "," << m_nLabelSpacingY << std::endl;
    )
    Log.logSeparator(LOG_LEVEL_INFO);
    
    --m_nNumberOfPyramidLevels; // = Given number - Original level = Additional levels
    m_bLabelSpacingSet = true;
    
    METHOD_EXIT("FeatureExtractor::setParam()");
    return true;
}
