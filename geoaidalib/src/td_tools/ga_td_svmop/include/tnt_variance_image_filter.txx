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
/// \file       tnt_variance_image_filter.txx
/// \brief      Template implementation of class "tntVarianceImageFilter"
///
/// \date       2010-09-29
/// \author     Torsten Bueschenfeld (bfeld@tnt.uni-hannover.de)
///
////////////////////////////////////////////////////////////////////////////////
#ifndef __TNT_VARIANCE_IMAGE_FILTER_TXX
#define __TNT_VARIANCE_IMAGE_FILTER_TXX


// First make sure that the configuration is available.
// This line can be removed once the optimized versions
// gets integrated into the main directories.
#include "itkConfigure.h"

#include "tnt_variance_image_filter.h"

#include "itkConstNeighborhoodIterator.h"
#include "itkNeighborhoodInnerProduct.h"
#include "itkImageRegionIterator.h"
#include "itkNeighborhoodAlgorithm.h"
#include "itkZeroFluxNeumannBoundaryCondition.h"
#include "itkOffset.h"
#include "itkProgressReporter.h"

template <class TInputImage, class TOutputImage>
VarianceImageFilter<TInputImage, TOutputImage>
::VarianceImageFilter()
{
    m_Radius.Fill(1);
}

template <class TInputImage, class TOutputImage>
void 
VarianceImageFilter<TInputImage, TOutputImage>
::GenerateInputRequestedRegion() throw (InvalidRequestedRegionError)
{
    // call the superclass' implementation of this method
    Superclass::GenerateInputRequestedRegion();
    
    // get pointers to the input and output
    typename Superclass::InputImagePointer inputPtr = 
        const_cast< TInputImage * >( this->GetInput() );
    typename Superclass::OutputImagePointer outputPtr = this->GetOutput();
    
    if ( !inputPtr || !outputPtr )
    {
        return;
    }

    // get a copy of the input requested region (should equal the output
    // requested region)
    typename TInputImage::RegionType inputRequestedRegion;
    inputRequestedRegion = inputPtr->GetRequestedRegion();

    // pad the input requested region by the operator radius
    inputRequestedRegion.PadByRadius( m_Radius );

    // crop the input requested region at the input's largest possible region
    if ( inputRequestedRegion.Crop(inputPtr->GetLargestPossibleRegion()) )
    {
        inputPtr->SetRequestedRegion( inputRequestedRegion );
        return;
    }
    else
    {
        // Couldn't crop the region (requested region is outside the largest
        // possible region).  Throw an exception.

        // store what we tried to request (prior to trying to crop)
        inputPtr->SetRequestedRegion( inputRequestedRegion );
        
        // build an exception
        InvalidRequestedRegionError e(__FILE__, __LINE__);
        e.SetLocation(ITK_LOCATION);
        e.SetDescription("Requested region is (at least partially) outside the largest possible region.");
        e.SetDataObject(inputPtr);
        throw e;
    }
}


template< class TInputImage, class TOutputImage>
void
VarianceImageFilter< TInputImage, TOutputImage>
::ThreadedGenerateData(const OutputImageRegionType& outputRegionForThread,
                       int threadId)
{
    unsigned int i;
    ZeroFluxNeumannBoundaryCondition<InputImageType> nbc;

    ConstNeighborhoodIterator<InputImageType> bit;
    ImageRegionIterator<OutputImageType> it;
    
    // Allocate output
    typename OutputImageType::Pointer output = this->GetOutput();
    typename  InputImageType::ConstPointer input  = this->GetInput();
    
    // Find the data-set boundary "faces"
    typename NeighborhoodAlgorithm::ImageBoundaryFacesCalculator<InputImageType>::FaceListType faceList;
    NeighborhoodAlgorithm::ImageBoundaryFacesCalculator<InputImageType> bC;
    faceList = bC(input, outputRegionForThread, m_Radius);

    typename NeighborhoodAlgorithm::ImageBoundaryFacesCalculator<InputImageType>::FaceListType::iterator fit;

    // support progress methods/callbacks
    ProgressReporter progress(this, threadId, outputRegionForThread.GetNumberOfPixels());
    
    InputRealType mean;
    InputRealType var;

    // Process each of the boundary faces.  These are N-d regions which border
    // the edge of the buffer.
    for (fit=faceList.begin(); fit != faceList.end(); ++fit)
    { 
        bit = ConstNeighborhoodIterator<InputImageType>(m_Radius,
                                                        input, *fit);
        unsigned int neighborhoodSize = bit.Size();
        it = ImageRegionIterator<OutputImageType>(output, *fit);
        bit.OverrideBoundaryCondition(&nbc);
        bit.GoToBegin();

        while ( ! bit.IsAtEnd() )
        {
            mean = NumericTraits<InputRealType>::Zero;
            var = NumericTraits<InputRealType>::Zero;
            for (i = 0; i < neighborhoodSize; ++i)
            {
                mean += static_cast<InputRealType>( bit.GetPixel(i) );
            }
            
            // get the mean value
            mean /= double(neighborhoodSize);
            
            for (i = 0; i < neighborhoodSize; ++i)
            {
                var += (static_cast<InputRealType>(bit.GetPixel(i))-mean) * 
                       (static_cast<InputRealType>(bit.GetPixel(i))-mean);
            }
        
            // Store variance
            it.Set( static_cast<OutputPixelType>(var / double(neighborhoodSize)) );
            
            ++bit;
            ++it;
            progress.CompletedPixel();
        }
    }
}

/**
 * Standard "PrintSelf" method
 */
template <class TInputImage, class TOutput>
void
VarianceImageFilter<TInputImage, TOutput>
::PrintSelf(
  std::ostream& os, 
  Indent indent) const
{
    Superclass::PrintSelf( os, indent );
    os << indent << "Radius: " << m_Radius << std::endl;

}

#endif
