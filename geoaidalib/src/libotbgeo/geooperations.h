#ifndef GEOOPERATIONS_INCLUDE
#define GEOOPERATIONS_INCLUDE

#include <inttypes.h>
#include <itkImageBase.h>  
#include <otbImageList.h>
#include <otbRemoteSensingRegion.h>

#include <otbImage.h>
#include <otbVectorImage.h>

#include <otbStreamingResampleImageFilter.h>
#include <otbPerBandVectorImageFilter.h>
#include <itkTranslationTransform.h>
#include <itkNearestNeighborInterpolateImageFunction.h>
#include <itkWindowedSincInterpolateImageFunction.h>
#include <otbBSplineInterpolateImageFunction.h>

namespace otbgeo {
      

  typedef itk::ImageBase<2> ImageBase2DType;
  typedef otb::ImageList<ImageBase2DType> ImageBaseListType;
  /**
   * Describes size and location of a geographic area.
   */
  typedef otb::RemoteSensingRegion<double> GeoRegion;
  
  /**
   * Determine common GeoRegion
   */
  GeoRegion cropGeoRegion(const GeoRegion& region1, const GeoRegion&  region2);

  /** 
   * Determine enclosing GeoRegion
   */
  GeoRegion joinGeoRegion(const GeoRegion& region1, const GeoRegion&  region2);

  /**
   *  Reads geo-coordinates from image
   *
   * @param image Image to read geo-coordinates from
   * @return GeoRegion containing geo-coordinates from image
   */
  GeoRegion createGeoRegion(const ImageBase2DType* const image);
  
  /**
   * Determines Image Region that lies within the given geoRegion.  
   * 
   * @param geoRegion GeoRegion
   * @param image Image
   * @return Image Region that lies within geoRegion.
   */
  ImageBase2DType::RegionType geocutImage(const GeoRegion& geoRegion, const ImageBase2DType::Pointer image);

  /**
   * Determines GeoRegion that is common for all given images.
   * @param images List of images to use.
   * @return GeoRegion that is common for all images in images.
   */
  GeoRegion findCommonRegion(ImageBaseListType::Pointer images);

  /**
   * Determines minimum GeoRegion that contains all given images.
   * @param images List of images to use.
   * @return Output region
   */
  GeoRegion findEnclosingRegion(ImageBaseListType::Pointer images);

   /**
   * Extract image data that lies within the given geoRegion using resampling.
   *
   * @param geoRegion GeoRegion
   * @param image Image
   * @param spacing Spacing of the output image
   * @return Image that contains the image data.
   */
  template <class TLabel> typename otb::Image<TLabel, 2>::Pointer resampleImage(const GeoRegion& geoRegion, const typename otb::Image<TLabel, 2>::Pointer image, double spacing, bool smooth=false) {
    typedef otb::Image<TLabel> ImageType;
    
    // Calculate output spacing and origin and update the georegion
    typename ImageType::SpacingType outputImageSpacing;
    outputImageSpacing[0] = spacing;
    outputImageSpacing[1] = spacing;

    typename ImageType::SizeType outputImageSize;
    outputImageSize[0] = static_cast<long>(geoRegion.GetSize()[0] / spacing);
    outputImageSize[1] = static_cast<long>(geoRegion.GetSize()[1] / spacing);
    
    typename ImageType::PointType outputImageOrigin;
    outputImageOrigin[0] = geoRegion.GetOrigin()[0];
    outputImageOrigin[1] = geoRegion.GetOrigin()[1];
    
    if (image->GetSpacing()[0] < 0)
    {
      outputImageOrigin[0] += outputImageSpacing[0] * outputImageSize[0];
      outputImageSpacing[0] = -outputImageSpacing[0];
    }

    if (image->GetSpacing()[1] < 0)
    {
      outputImageOrigin[1] += outputImageSpacing[1] * outputImageSize[1];
      outputImageSpacing[1] = -outputImageSpacing[1];
    }

    // Create and execute resample filter
    typedef otb::StreamingResampleImageFilter<ImageType, ImageType> ResamplerType;
    typedef itk::TranslationTransform<double, 2> TransformType;
    typedef itk::NearestNeighborInterpolateImageFunction<ImageType> NearestNeighbourInterpolatorType;
    typedef itk::WindowedSincInterpolateImageFunction<ImageType, 3> LanczosInterpolatorType;
    typedef otb::BSplineInterpolateImageFunction<ImageType> BSplineInterpolatorType;
    
    typename TransformType::OutputVectorType offset;
    offset[0] = (std::abs(image->GetSpacing()[0]) - spacing) * (image->GetSpacing()[0] < 0 ? 0.5 : -0.5);
    offset[1] = (std::abs(image->GetSpacing()[1]) - spacing) * (image->GetSpacing()[1] < 0 ? 0.5 : -0.5);
    
    typename TransformType::Pointer transform = TransformType::New();
    transform->SetOffset(offset);
    
    typename NearestNeighbourInterpolatorType::Pointer nn_interpolator = NearestNeighbourInterpolatorType::New();    
    typename LanczosInterpolatorType::Pointer l_interpolator = LanczosInterpolatorType::New();
    typename BSplineInterpolatorType::Pointer bs_interpolator = BSplineInterpolatorType::New();
    bs_interpolator->SetSplineOrder(3);    

    typename ResamplerType::Pointer resampler = ResamplerType::New();
    resampler->SetInput(image);
    resampler->SetOutputOrigin(outputImageOrigin);
    resampler->SetOutputSpacing(outputImageSpacing);
    resampler->SetSize(outputImageSize);
    resampler->SetTransform(transform);
    if (smooth)
      resampler->SetInterpolator(bs_interpolator);
    else
      resampler->SetInterpolator(nn_interpolator);
    resampler->Update();

    return resampler->GetOutput();
  }

   /**
   * Extract image data that lies within the given geoRegion using resampling.
   *
   * @param geoRegion GeoRegion
   * @param image VectorImage
   * @param spacing Spacing of the output image
   * @return Image that contains the image data.
   */
  template <class TLabel> typename otb::VectorImage<TLabel, 2>::Pointer resampleImage(const GeoRegion& geoRegion, const typename otb::VectorImage<TLabel, 2>::Pointer image, double spacing, bool smooth=false) {
    typedef otb::Image<TLabel> BandType;
    typedef otb::VectorImage<TLabel> ImageType;
    
    // Calculate output spacing and origin
    typename ImageType::SpacingType outputImageSpacing;
    outputImageSpacing[0] = spacing;
    outputImageSpacing[1] = spacing;

    typename ImageType::SizeType outputImageSize;
    outputImageSize[0] = static_cast<long>(geoRegion.GetSize()[0] / spacing);
    outputImageSize[1] = static_cast<long>(geoRegion.GetSize()[1] / spacing);
    
    typename ImageType::PointType outputImageOrigin;
    outputImageOrigin[0] = geoRegion.GetOrigin()[0];
    outputImageOrigin[1] = geoRegion.GetOrigin()[1];
    
    if (image->GetSpacing()[0] < 0)
    {
      outputImageOrigin[0] += geoRegion.GetSize()[0];
      outputImageSpacing[0] = -outputImageSpacing[0];
    }

    if (image->GetSpacing()[1] < 0)
    {
      outputImageOrigin[1] += geoRegion.GetSize()[1];
      outputImageSpacing[1] = -outputImageSpacing[1];
    }

    // Create resample filter
    typedef otb::StreamingResampleImageFilter<BandType, BandType> ResamplerType;
    typedef itk::TranslationTransform<double, 2> TransformType;
    typedef itk::NearestNeighborInterpolateImageFunction<BandType> NearestNeighbourInterpolatorType;
    typedef itk::WindowedSincInterpolateImageFunction<BandType, 3> LanczosInterpolatorType;
    typedef otb::BSplineInterpolateImageFunction<BandType> BSplineInterpolatorType;
    
    typename TransformType::OutputVectorType offset;
    offset[0] = (std::abs(image->GetSpacing()[0]) - spacing) * (image->GetSpacing()[0] < 0 ? 0.5 : -0.5);
    offset[1] = (std::abs(image->GetSpacing()[1]) - spacing) * (image->GetSpacing()[1] < 0 ? 0.5 : -0.5);
    
    typename TransformType::Pointer transform = TransformType::New();
    transform->SetOffset(offset);
    
    typename NearestNeighbourInterpolatorType::Pointer nn_interpolator = NearestNeighbourInterpolatorType::New();
    //typename LanczosInterpolatorType::Pointer l_interpolator = LanczosInterpolatorType::New();
    typename BSplineInterpolatorType::Pointer bs_interpolator = BSplineInterpolatorType::New();
    bs_interpolator->SetSplineOrder(3);

    typename ResamplerType::Pointer resampler = ResamplerType::New();
    resampler->SetOutputOrigin(outputImageOrigin);
    resampler->SetOutputSpacing(outputImageSpacing);
    resampler->SetSize(outputImageSize);
    resampler->SetTransform(transform);
    if (smooth)
      resampler->SetInterpolator(bs_interpolator);
    else
      resampler->SetInterpolator(nn_interpolator);

    // Create per-band image filter
    typedef otb::PerBandVectorImageFilter<ImageType, ImageType, ResamplerType> PerBandResamplerType;
    
    typename PerBandResamplerType::Pointer perbandresampler = PerBandResamplerType::New();
    perbandresampler->SetFilter(resampler);
    perbandresampler->SetInput(image);
    perbandresampler->Update();
    
    return perbandresampler->GetOutput();
  }

}



#endif
