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
/// \file		feature_extractor.h
/// \brief		Prototype of class "FeatureExtractor"
///
/// \date		2009-07-13
/// \author		Torsten Bueschenfeld (bfeld@tnt.uni-hannover.de)
///
////////////////////////////////////////////////////////////////////////////////

#ifndef FEATURE_EXTRACTOR_H
#define FEATURE_EXTRACTOR_H

//--- Standard header --------------------------------------------------------//
#include <list>
#include <string>
#include <vector>
#include <cmath>

//--- Program header ---------------------------------------------------------//
#include "log.h"
#include "otb_svmop_common.h"

//--- Misc header ------------------------------------------------------------//
#include "itkImageRegionConstIterator.h"


const unsigned char FEATURE_EXTRACTOR_DEFAULT = 0;
const unsigned char FEATURE_EXTRACTOR_USE_LABELS = 10;

const int		FEATURE_EXTRACTOR_DEFAULT_FILTER_RADIUS = 5;		///< Default value for filters
// The default number of pyramid levels should stay 0, because
// the default border will be set to default filter radius which
// is only valid for 0 pyramid levels
const int		FEATURE_EXTRACTOR_DEFAULT_NUMBER_OF_PYRAMID_LEVELS = 0; ///< Default value for image pyramid
const double	FEATURE_EXTRACTOR_DEFAULT_PYRAMID_DECIMATION_RATE = 1.5; ///< Default value for image pyramid
const int		FEATURE_EXTRACTOR_DEFAULT_SPACING = 5; ///< Default value that fits filter radius

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
		const FeaturePointSetType::Pointer	getFeatures() const;
		const LabelPointSetType::Pointer	getLabels() const;
		const InputImageType::SizeType		getImageSize() const;

		//--- Methods --------------------------------------------------------//
		void addInputChannel(const std::string&);
		void clearChannels();
		bool extract(const unsigned char& = FEATURE_EXTRACTOR_DEFAULT);
        
		void loadLabelImage(const std::string&);
        bool loadParam(const std::string&);
        
		void setFilterRadius(const int&);
		void setLabelSpacing(const int&, const int&);
		void setNumberOfPyramidLevels(const int&);
		void setPyramidDecimationRate(const double&);

	private:
		
		bool							extractWithLabels();
		bool							extractWithoutLabels();

		ImageListType::Pointer			m_pInputChannelList;			///< Contains a list of input channel images
		LabelImageType::Pointer			m_pLabelImage;					///< Contains the label image

		FeaturePointSetType::Pointer	m_pFeatures;					///< Contains the extracted features
		LabelPointSetType::Pointer		m_pLabels;						///< Contains the extracted features
		
		InputImageType::SizeType		m_ImageSize;					///< Contains size of label image
		bool							m_bFeaturesExtracted;			///< Flags if features were extracted
		bool							m_bChannelLoaded;				///< Flags if at least one channel was loaded
		bool							m_bLabelsExtracted;				///< Flags if labels were extracted
		bool							m_bLabelImageLoaded;			///< Flags if label image was loaded
		bool							m_bLabelSpacingSet;				///< Flags if label image settings are configured

		double							m_fPyramidDecimationRate;		///< Factor for subsamling of images
		int								m_nFilterRadius;				///< Radius of feature filters
		int								m_nLabelSpacingX;				///< Distance between samples for training
		int								m_nLabelSpacingY;				///< Distance between samples for training
		int								m_nLabelBorderX;				///< Distance of border when training
		int								m_nLabelBorderY;				///< Distance of border when training
		int								m_nNumberOfPyramidLevels;		///< Number of subsampled images in image pyramid
		
		DEBUG(
			static uint m_unNoImageFiles;								///< Indicates the number of written image files
		);
		
		//--- Constant methods [private] -------------------------------------//
// 		void	saveImage(const Image8BitType::Pointer,
// 						  const std::string&) const;
// 		void	saveImage(const Image16BitType::Pointer,
// 						  const std::string&) const;
//     public:	void	saveImage(const ImageFloatType::Pointer,
// 						  const std::string&) const;

};

#endif
