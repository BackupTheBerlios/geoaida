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
    METHOD_ENTRY("FeatureExtractor::saveImage(const Image8BitType::Pointer, const std::string&)");
    
    INFO_MSG("Image Feature Extractor", "Writing image " << _Filename, LOG_DOMAIN_NONE);
    Writer8BitType::Pointer pWriter = Writer8BitType::New();
    pWriter->SetFileName(_Filename);
    pWriter->SetInput(_pImg);
    pWriter->Update();
    
    METHOD_EXIT("FeatureExtractor::saveImage(const Image8BitType::Pointer, const std::string&)");
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
    METHOD_ENTRY("FeatureExtractor::saveImage(const Image16BitType::Pointer, const std::string&)");
    
    INFO_MSG("Image Feature Extractor", "Writing image " << _Filename, LOG_DOMAIN_NONE);
    Writer16BitType::Pointer pWriter = Writer16BitType::New();
    pWriter->SetFileName(_Filename);
    pWriter->SetInput(_pImg);
    pWriter->Update();
    
    METHOD_EXIT("FeatureExtractor::saveImage(const Image16BitType::Pointer, const std::string&)");
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Stores a given float image as 16bit image to disk
///
/// \param _pImg Pointer to image
/// \param _Filename File to write to
///
////////////////////////////////////////////////////////////////////////////////
void saveImage(const ImageFloatType::Pointer _pImg,
               const std::string& _Filename)
{
    METHOD_ENTRY("FeatureExtractor::saveImage(const ImageFloatType::Pointer, const std::string&)");
    
    typedef itk::RescaleIntensityImageFilter<ImageFloatType,Image8BitType> FilterType;
    
    FilterType::Pointer pFilter = FilterType::New();
    pFilter->SetOutputMinimum(0);
    pFilter->SetOutputMaximum(255);
    pFilter->SetInput(_pImg);
    DEBUG_MSG("Image Feature Extractor", "Rescaling image " << _Filename, LOG_DOMAIN_NONE);
    INFO_MSG("Image Feature Extractor", "Writing image " << _Filename, LOG_DOMAIN_NONE);
    Writer8BitType::Pointer pWriter = Writer8BitType::New();
    pWriter->SetFileName(_Filename);
    pWriter->SetInput(pFilter->GetOutput());
    pWriter->Update();
    
    METHOD_EXIT("FeatureExtractor::saveImage(const ImageFloatType::Pointer, const std::string&)");
}
