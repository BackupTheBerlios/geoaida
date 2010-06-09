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
/// \file       feature_extractor.h
/// \brief      Prototype of class "FeatureExtractor"
///
/// \date       2009-07-13
/// \author     Torsten Bueschenfeld (bfeld@tnt.uni-hannover.de)
///
////////////////////////////////////////////////////////////////////////////////

#ifndef FEATURE_EXTRACTOR_H
#define FEATURE_EXTRACTOR_H

//--- Standard header --------------------------------------------------------//
#include <list>
#include <vector>
#include <cmath>

//--- Program header ---------------------------------------------------------//
#include "configurator.h"
#include "otb_svmop_common.h"

//--- Misc header ------------------------------------------------------------//
#include "itkImageRegionConstIterator.h"

const unsigned char FEATURE_EXTRACTOR_DEFAULT = 0;
const unsigned char FEATURE_EXTRACTOR_USE_LABELS = 10;
const unsigned char FEATURE_EXTRACTOR_REFINE = 20;

const int       FEATURE_EXTRACTOR_DEFAULT_FILTER_RADIUS = 5;        ///< Default value for filters
// The default number of pyramid levels should stay 0, because
// the default border will be set to default filter radius which
// is only valid for 0 pyramid levels
const int       FEATURE_EXTRACTOR_DEFAULT_NUMBER_OF_PYRAMID_LEVELS = 0; ///< Default value for image pyramid
const double    FEATURE_EXTRACTOR_DEFAULT_PYRAMID_DECIMATION_RATE = 1.5; ///< Default value for image pyramid
const int       FEATURE_EXTRACTOR_DEFAULT_SPACING = 5; ///< Default value that fits filter radius

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Class for extraction of features from images
/// 
////////////////////////////////////////////////////////////////////////////////
class FeatureExtractor
{

    public:

        //--- Constructor ----------------------------------------------------//
        FeatureExtractor();

        //--- Const methods --------------------------------------------------//
        FeaturesType&                   getFeatures();
        const LabelsType&               getLabels() const;
        const InputImageType::SizeType  getImageSize() const;
        
        //--- Methods --------------------------------------------------------//
        void addInputChannel(const std::string&);
        void clearChannels();
        void clearFeatures();
        void clearLabels();
        
        bool extract(const unsigned char& = FEATURE_EXTRACTOR_DEFAULT);
        
        void loadLabelImage(const std::string&);
        void setLabelImage(LabelImageType::Pointer);
        
        void setConfigurator(Configurator* const);
        void setFilterRadius(const int&);
        void setLabelSpacing(const int&, const int&);
        void setNumberOfPyramidLevels(const int&);
        void setPyramidDecimationRate(const double&);

    private:
        
        bool                            extractWithLabels();
        bool                            extractWithoutLabels();
        bool                            setParam();

        ImageListType::Pointer          m_pInputChannelList;            ///< Contains a list of input channel images
        LabelImageType::Pointer         m_pLabelImage;                  ///< Contains the label image

        FeaturesType                    m_Features;                     ///< Contains the extracted features
        LabelsType                      m_Labels;                       ///< Contains the extracted features
        
        InputImageType::SizeType        m_ImageSize;                    ///< Contains size of label image
        bool                            m_bFeaturesExtracted;           ///< Flags if features were extracted
        bool                            m_bChannelLoaded;               ///< Flags if at least one channel was loaded
        bool                            m_bLabelsExtracted;             ///< Flags if labels were extracted
        bool                            m_bLabelImageLoaded;            ///< Flags if label image was loaded
        bool                            m_bLabelSpacingSet;             ///< Flags if label image settings are configured

        double                          m_fPyramidDecimationRate;       ///< Factor for subsamling of images
        int                             m_nFilterRadius;                ///< Radius of feature filters
        int                             m_nLabelSpacingX;               ///< Distance between samples for training
        int                             m_nLabelSpacingY;               ///< Distance between samples for training
        int                             m_nLabelBorderX;                ///< Distance of border when training
        int                             m_nLabelBorderY;                ///< Distance of border when training
        int                             m_nNumberOfPyramidLevels;       ///< Number of subsampled images in image pyramid
        
        Configurator*                   m_pConfiguration;               ///< Configuration of parameters
        
        DEBUG(
            static uint m_unNoImageFiles;                               ///< Indicates the number of written image files
        );

};

//--- Implementation is done here for inlining -------------------------------//

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Get the parameter configuration
///
/// \param _pFeatures PointSet of features
///
///////////////////////////////////////////////////////////////////////////////
inline void FeatureExtractor::setConfigurator(Configurator* const _pConfigurator)
{
    METHOD_ENTRY("FeatureExtractor::setConfigurator");

    m_pConfiguration = _pConfigurator;
    this->setParam();

    METHOD_EXIT("FeatureExtractor::setConfigurator");
}

#endif
