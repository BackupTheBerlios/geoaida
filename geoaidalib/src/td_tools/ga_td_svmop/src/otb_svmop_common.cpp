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
/// \file       otb_svmop_common.cpp
/// \brief      Implementation of common functions
///
/// \date       2010-01-27
/// \author     bfeld
///
////////////////////////////////////////////////////////////////////////////////

#include "otb_svmop_common.h"

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Stores a given 8 bit image to disk
///
/// \param _pImg Pointer to image
/// \param _Filename File to write to
///
////////////////////////////////////////////////////////////////////////////////
void saveImage(const Image8BitType::Pointer _pImg,
               const std::string& _Filename)
{
    METHOD_ENTRY("FeatureExtractor::saveImage");
    
    DOM_FIO(INFO_MSG("Image Feature Extractor", "Writing 8-bit image " << _Filename))
    Writer8BitType::Pointer pWriter = Writer8BitType::New();
    pWriter->SetFileName(_Filename);
    pWriter->SetInput(_pImg);
    pWriter->Update();
    
    METHOD_EXIT("FeatureExtractor::saveImage");
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Stores a given 16 bit image to disk
///
/// \param _pImg Pointer to image
/// \param _Filename File to write to
///
////////////////////////////////////////////////////////////////////////////////
void saveImage(const Image16BitType::Pointer _pImg,
               const std::string& _Filename)
{
    METHOD_ENTRY("FeatureExtractor::saveImage");
    
    DOM_FIO(INFO_MSG("Image Feature Extractor", "Writing 16-bit image " << _Filename))
    Writer16BitType::Pointer pWriter = Writer16BitType::New();
    pWriter->SetFileName(_Filename);
    pWriter->SetInput(_pImg);
    pWriter->Update();
    
    METHOD_EXIT("FeatureExtractor::saveImage");
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Stores a given 32 bit image as 16 bit image to disk
///
/// \param _pImg Pointer to image
/// \param _Filename File to write to
///
////////////////////////////////////////////////////////////////////////////////
void saveImage(const Image32BitType::Pointer _pImg,
               const std::string& _Filename)
{
    METHOD_ENTRY("FeatureExtractor::saveImage");
    
    typedef itk::RescaleIntensityImageFilter<Image32BitType,Image16BitType> FilterType;
    
    FilterType::Pointer pFilter = FilterType::New();
    pFilter->SetOutputMinimum(0);
    pFilter->SetOutputMaximum(65535);
    pFilter->SetInput(_pImg);
    DOM_FIO(INFO_MSG("Image Feature Extractor", "Writing 16-bit image " << _Filename))
    Writer16BitType::Pointer pWriter = Writer16BitType::New();
    pWriter->SetFileName(_Filename);
    pWriter->SetInput(pFilter->GetOutput());
    pWriter->Update();
    
    METHOD_EXIT("FeatureExtractor::saveImage");
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Stores a given float image to disk
///
/// \param _pImg Pointer to image
/// \param _Filename File to write to
///
////////////////////////////////////////////////////////////////////////////////
void saveImage(const ImageFloatType::Pointer _pImg,
               const std::string& _Filename)
{
    METHOD_ENTRY("FeatureExtractor::saveImage");
    
//     typedef itk::RescaleIntensityImageFilter<ImageFloatType,Image16BitType> FilterType;
//     typedef itk::MinimumMaximumImageFilter<ImageFloatType> MaxFilterType;
//     
//     FilterType::Pointer pFilter = FilterType::New();
//     MaxFilterType::Pointer pMaxFilter = MaxFilterType::New();
//     pMaxFilter->SetInput(_pImg);
//     pMaxFilter->Update();
//     pFilter->SetOutputMinimum(0);
//     pFilter->SetOutputMaximum(pMaxFilter->GetMaximum());
//     pFilter->SetInput(_pImg);
    DOM_FIO(INFO_MSG("Image Feature Extractor", "Writing float image " << _Filename))
    WriterFloatType::Pointer pWriter = WriterFloatType::New();
    pWriter->SetFileName(_Filename);
    pWriter->SetInput(_pImg);
    pWriter->Update();
    
    METHOD_EXIT("FeatureExtractor::saveImage");
}
