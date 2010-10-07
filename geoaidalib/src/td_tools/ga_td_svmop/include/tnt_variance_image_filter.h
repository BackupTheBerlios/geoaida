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
/// \file       tnt_variance_image_filter.h
/// \brief      Prototype of class "tntVarianceImageFilter"
///
/// \date       2010-09-29
/// \author     Torsten Bueschenfeld (bfeld@tnt.uni-hannover.de)
///
////////////////////////////////////////////////////////////////////////////////
#ifndef __TNT_VARIANCE_IMAGE_FILTER_H
#define __TNT_VARIANCE_IMAGE_FILTER_H


// First make sure that the configuration is available.
// This line can be removed once the optimized versions
// gets integrated into the main directories.
#include "itkConfigure.h"

#include "itkImageToImageFilter.h"
#include "itkImage.h"
#include "itkNumericTraits.h"

using namespace itk;

////////////////////////////////////////////////////////////////////////////////
///
/// \class VarianceImageFilter
///
/// \brief Prototype of class "tntVarianceImageFilter"
///
/// Computes an image where a given pixel is the variance value of the
/// the pixels in a neighborhood about the corresponding input pixel.
///
////////////////////////////////////////////////////////////////////////////////
template <class TInputImage, class TOutputImage>
class VarianceImageFilter : public ImageToImageFilter< TInputImage, TOutputImage >
{
    public:
        
        /** Extract dimension from input and output image. */
        itkStaticConstMacro(InputImageDimension, unsigned int,
                            TInputImage::ImageDimension);
        itkStaticConstMacro(OutputImageDimension, unsigned int,
                            TOutputImage::ImageDimension);

        /** Convenient typedefs for simplifying declarations. */
        typedef TInputImage  InputImageType;
        typedef TOutputImage OutputImageType;

        /** Standard class typedefs. */
        typedef VarianceImageFilter                                  Self;
        typedef ImageToImageFilter< InputImageType, OutputImageType> Superclass;
        typedef SmartPointer<Self>                                   Pointer;
        typedef SmartPointer<const Self>                             ConstPointer;

        /** Method for creation through the object factory. */
        itkNewMacro(Self);

        /** Run-time type information (and related methods). */
        itkTypeMacro(VarianceImageFilter, ImageToImageFilter);
        
        /** Image typedef support. */
        typedef typename InputImageType::PixelType               InputPixelType;
        typedef typename OutputImageType::PixelType              OutputPixelType;
        typedef typename NumericTraits<InputPixelType>::RealType InputRealType;
        
        typedef typename InputImageType::RegionType  InputImageRegionType;
        typedef typename OutputImageType::RegionType OutputImageRegionType;
        typedef typename InputImageType::SizeType    InputSizeType;

        /** Set the radius of the neighborhood used to compute the mean. */
        itkSetMacro(Radius, InputSizeType);

        /** Get the radius of the neighborhood used to compute the mean */
        itkGetConstReferenceMacro(Radius, InputSizeType);
        
        /** VarianceImageFilter needs a larger input requested region than
        * the output requested region.  As such, VarianceImageFilter needs
        * to provide an implementation for GenerateInputRequestedRegion()
        * in order to inform the pipeline execution model.*/
        virtual void GenerateInputRequestedRegion() throw(InvalidRequestedRegionError);

        #ifdef ITK_USE_CONCEPT_CHECKING
        /** Begin concept checking */
        itkConceptMacro(InputHasNumericTraitsCheck,
                        (Concept::HasNumericTraits<InputPixelType>));
        /** End concept checking */
        #endif

    protected:
        
        VarianceImageFilter();
        virtual ~VarianceImageFilter() {}
        void PrintSelf(std::ostream& os, Indent indent) const;

        /** VarianceImageFilter can be implemented as a multithreaded filter.
        * Therefore, this implementation provides a ThreadedGenerateData()
        * routine which is called for each processing thread. The output
        * image data is allocated automatically by the superclass prior to
        * calling ThreadedGenerateData().  ThreadedGenerateData can only
        * write to the portion of the output image specified by the
        * parameter "outputRegionForThread"*/
        void ThreadedGenerateData(const OutputImageRegionType& outputRegionForThread,
                                    int threadId );

    private:
        
        VarianceImageFilter(const Self&); //purposely not implemented
        void operator=(const Self&); //purposely not implemented

        InputSizeType m_Radius;
};
  
#include "tnt_variance_image_filter.txx"

#endif
