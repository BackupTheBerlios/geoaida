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
/// \file		otb_svmop_common.h
/// \brief		Commonly used libraries and type definitions
///
/// \date		2009-07-13
/// \author		Torsten Bueschenfeld (bfeld@tnt.uni-hannover.de)
///
////////////////////////////////////////////////////////////////////////////////

#ifndef OTB_SVMOP_COMMON_H
#define OTB_SVMOP_COMMON_H

#include "log.h"

#include "itkBinaryBallStructuringElement.h"
#include "itkGradientMagnitudeImageFilter.h"
#include "itkListSample.h"
#include "itkMeanImageFilter.h"
#include "itkPointSet.h"
#include "itkPointSetToListAdaptor.h"
#include "itkRescaleIntensityImageFilter.h"

#include "otbImage.h"
#include "otbImageList.h"
#include "otbImageFileReader.h"
#include "otbMorphologicalPyramidAnalysisFilter.h"
#include "otbOpeningClosingMorphologicalFilter.h"
#include "otbStreamingImageFileWriter.h"
#include "otbSVMClassifier.h"
#include "otbSVMPointSetModelEstimator.h"
#include "otbTextureFunctors.h"
#include "otbUnaryFunctorNeighborhoodWithOffsetImageFilter.h"
#include "otbTextureImageFunction.h"
#include "otbVectorImage.h"

//--- Define input and output images ---//
const unsigned int Dimension = 2;
typedef unsigned char	Pixel8BitType;
typedef unsigned short	Pixel16BitType;
typedef float			PixelFloatType;
typedef float			FeaturePixelType;
typedef PixelFloatType	InputPixelType;
typedef unsigned char	LabelPixelType;
typedef otb::Image<Pixel8BitType, Dimension> Image8BitType;
typedef otb::Image<Pixel16BitType, Dimension> Image16BitType;
typedef otb::Image<PixelFloatType, Dimension> ImageFloatType;
typedef otb::Image<InputPixelType, Dimension> InputImageType;
typedef otb::Image<LabelPixelType, Dimension> LabelImageType;

typedef otb::ImageList<InputImageType> ImageListType;

// Define pyramid filtering types
typedef itk::BinaryBallStructuringElement<InputPixelType,Dimension> StructuringElementType;
typedef otb::OpeningClosingMorphologicalFilter<	InputImageType,
												InputImageType,
												StructuringElementType>
		OpeningClosingFilterType;
typedef otb::MorphologicalPyramidAnalysisFilter<InputImageType,
												InputImageType,
												OpeningClosingFilterType>
		PyramidFilterType;
typedef PyramidFilterType::OutputImageListType::Iterator ImageListIterator;

//--- Define Pipes ---//
typedef otb::ImageFileReader<InputImageType> ReaderType;
typedef otb::ImageFileReader<LabelImageType> LabelReaderType;
typedef otb::StreamingImageFileWriter<Image8BitType> Writer8BitType;
typedef otb::StreamingImageFileWriter<Image16BitType> Writer16BitType;
typedef otb::StreamingImageFileWriter<LabelImageType> LabelWriterType;


//--- Define types for svm learning and classification ---//
const unsigned int FeatureSpaceDimension = 1;
typedef itk::PointSet<LabelPixelType, 1>						LabelPointSetType;
typedef LabelPointSetType::PointType							LabelPointType;
typedef LabelPointSetType::PointsContainer						LabelPointContainer;

typedef std::vector<FeaturePixelType>							FeatureVectorType;
typedef itk::PointSet<FeatureVectorType,FeatureSpaceDimension>	FeaturePointSetType;
typedef FeaturePointSetType::PointType							FeaturePointType;
typedef FeaturePointSetType::PointsContainer					FeaturePointContainer;
typedef FeaturePointSetType::PointDataContainer					FeaturePointDataContainer;

typedef itk::PointSet<FeatureVectorType,32>						TestFeaturePointSetType;
typedef TestFeaturePointSetType::PointType						TestFeaturePointType;
typedef TestFeaturePointSetType::PointsContainer				TestFeaturePointContainer;
// typedef FeaturePointSetType::PointDataContainer					TestFeaturePointDataContainer;

typedef itk::Statistics::PointSetToListAdaptor<TestFeaturePointSetType>				SampleType;
typedef otb::SVMModel<SampleType::MeasurementVectorType::ValueType,LabelPixelType>	ModelType;

typedef otb::SVMPointSetModelEstimator<FeaturePointSetType,LabelPointSetType>		EstimatorType;
typedef otb::SVMClassifier<SampleType,LabelPixelType>								ClassifierType;

//--- Functions --------------------------------------------------------------//

void saveImage(const Image8BitType::Pointer, const std::string&);
void saveImage(const Image16BitType::Pointer, const std::string&);
void saveImage(const ImageFloatType::Pointer, const std::string&);

#endif