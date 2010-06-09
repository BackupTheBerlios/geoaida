#ifndef GEOOPERATIONS_INCLUDE
#define GEOOPERATIONS_INCLUDE

#include <inttypes.h>
#include <itkImageBase.h>  
#include <otbImageList.h>
#include <otbRemoteSensingRegion.h>

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

 
}



#endif
