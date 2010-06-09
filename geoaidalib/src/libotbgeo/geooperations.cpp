#include "geooperations.h"
#include <ostream>
#include <otbRemoteSensingRegion.h>
 


namespace otbgeo {

  GeoRegion cropGeoRegion(const GeoRegion& region1, const GeoRegion& region2){
    GeoRegion region;
    GeoRegion::PointType regionIndex;
    GeoRegion::SizeType regionSize;
    
    GeoRegion::PointType region1Index = region1.GetOrigin();
    GeoRegion::PointType region2Index = region2.GetOrigin();
    GeoRegion::SizeType region1Size = region1.GetSize();
    GeoRegion::SizeType region2Size = region2.GetSize();

    const int Dimension = 2;
    for (int i = 0; i < Dimension; i++){
      regionIndex[i] = ( (region1Index[i] > region2Index[i]) ? region1Index[i]: region2Index[i]);
      double index_r = ( (region1Index[i]+region1Size[i] < region2Index[i]+region2Size[i]) ? region1Index[i]+region1Size[i]: region2Index[i]+region2Size[i]);
      regionSize[i] =index_r - regionIndex[i];
    }
    region.SetOrigin(regionIndex);
    region.SetSize(regionSize);
    return region;
  };

  GeoRegion joinGeoRegion(const GeoRegion& region1, const GeoRegion& region2){
    GeoRegion region;
    GeoRegion::PointType regionIndex;
    GeoRegion::SizeType regionSize;
    
    GeoRegion::PointType region1Index = region1.GetOrigin();
    GeoRegion::PointType region2Index = region2.GetOrigin();
    GeoRegion::SizeType region1Size = region1.GetSize();
    GeoRegion::SizeType region2Size = region2.GetSize();

    const int Dimension = 2;
    for (int i = 0; i < Dimension; i++){
      regionIndex[i] = ( (region1Index[i] < region2Index[i]) ? region1Index[i]: region2Index[i]);
      double index_r = ( (region1Index[i]+region1Size[i] > region2Index[i]+region2Size[i]) ? region1Index[i]+region1Size[i]: region2Index[i]+region2Size[i]);
      regionSize[i] =index_r - regionIndex[i];
    }
    region.SetOrigin(regionIndex);
    region.SetSize(regionSize);
    return region;
  };

  GeoRegion createGeoRegion(const ImageBase2DType * const image){

    typedef ImageBase2DType ImageType;
    const  ImageType::PointType& origin = image->GetOrigin();
    const  ImageType::SizeType& size = image->GetLargestPossibleRegion().GetSize();
    const  ImageType::SpacingType& spacing = image->GetSpacing();
    
    
    GeoRegion region;
    GeoRegion::SizeType region_size;
    GeoRegion::IndexType region_index;
    
#ifdef DEBUG
    {
      const typename ImageType::IndexType index = image->GetLargestPossibleRegion().GetIndex();
      std::clog << "###################" << std::endl;
      std::clog << "CreateGeoRegion" << std::endl;
      std::clog << "Image Origin: " << origin[0] << ", " << origin[1] << std::endl;
      std::clog << "Image Region Origin: " << index[0] << ", " << index[1] << std::endl;
      std::clog << "Image Region Size: " << size[0] << ", " << size[1] << std::endl;
      std::clog << "Image Region Spacing: " << spacing[0] << ", " << spacing[1] << std::endl;
      std::clog << "###################" << std::endl;
    }
#endif
    
    region_size[0]  = size[0] * abs(spacing[0]);
    region_size[1]  = size[1] * abs(spacing[1]);
    region_index[0] = origin[0] - (spacing[0]<0 ? region_size[0] : 0);
    region_index[1] = origin[1] - (spacing[1]<0 ? region_size[1] : 0);;
    region.SetSize(region_size);
    region.SetOrigin(region_index);
    
    return region;
  };

  ImageBase2DType::RegionType geocutImage(const GeoRegion& geoRegion, const ImageBase2DType::Pointer image) {
    typedef ImageBase2DType ImageType;
    
    GeoRegion::PointType geoOrigin = geoRegion.GetOrigin();
    ImageType::PointType imageOrigin = image->GetOrigin();
    GeoRegion::SizeType geoSize = geoRegion.GetSize();
    ImageType::SizeType imageSize = image->GetLargestPossibleRegion().GetSize();
    ImageType::SpacingType imageSpacing = image->GetSpacing();
    
    assert((imageSpacing[0] != 0)&&(imageSpacing[1] != 0));
    
    bool equal = ((geoOrigin[0] == imageOrigin[0]) && (geoOrigin[1] == imageOrigin[1])
		  &&
		  (geoSize[0] == imageSize[0]) && (geoSize[1] == imageSize[1]));
    
#ifdef DEBUG
    {
      std::clog << "###################" << std::endl;
      std::clog << "GeocutImage" << std::endl;
      std::clog << "Image Origin" <<  ": " << imageOrigin[0] << ", " << imageOrigin[1] << std::endl;
      std::clog << "Image Size" << ": " << imageSize[0] << ", " << imageSize[1] << std::endl;
      std::clog << "TargetRegion Origin" <<  ": " << geoOrigin[0] << ", " << geoOrigin[1] << std::endl;
      std::clog << "TargetRegion Size" << ": " << geoSize[0] << ", " << geoSize[1] << std::endl;
      if (equal)
	std::clog << "Result: Everything is fine" << std::endl;
      else
	std::clog << "Result: Cut Region" << std::endl;
      std::clog << "--" << std::endl;
    }
#endif
    
    
    ImageType::RegionType imageRegion;
    ImageType::RegionType::IndexType imageRegionIndex;  
    ImageType::RegionType::SizeType imageRegionSize;
    {
      
      double s0 = abs(geoSize[0]/imageSpacing[0]);
      double s1 = abs(geoSize[1]/imageSpacing[1]);
      imageRegionSize[0]=s0;
      imageRegionSize[1]=s1;
      
      double i0  = abs((geoOrigin[0] - imageOrigin[0])/imageSpacing[0]);
      double i1 = abs((geoOrigin[1] - imageOrigin[1])/imageSpacing[1]) - s1 ;
      imageRegionIndex[0] = i0;
      imageRegionIndex[1] = i1;
      
      
#ifdef DEBUG
      {
	ImageType::IndexType mrIndex = image->GetLargestPossibleRegion().GetIndex();
	ImageType::SizeType mrSize = image->GetLargestPossibleRegion().GetSize();
	std::clog << "Image Maximum Region Index" << ": " << mrIndex[0] << ", " << mrIndex[1] << std::endl;
	std::clog << "Image Maximum Region Size" << ": " << mrSize[0] << ", " << mrSize[1] << std::endl;
	
	std::clog << "Calculated Index" <<  ": " << i0 << ", " << i1 << std::endl;
	std::clog << "Calculated Size" <<  ": " << s0 << ", " << s1 << std::endl;
	
	std::clog << "RequestedRegion Index" <<  ": " << imageRegionIndex[0] << ", " << imageRegionIndex[1] << std::endl;
	std::clog << "RequestedRegion Size" << ": " << imageRegionSize[0] << ", " << imageRegionSize[1] << std::endl;
	std::clog << "Image Spacing" << ": " << imageSpacing[0] << ", " << imageSpacing[1] << std::endl;
	std::clog << "###################" << std::endl;
      }
#endif
    }
    imageRegion.SetIndex(imageRegionIndex);
    imageRegion.SetSize(imageRegionSize);
    return imageRegion;
  };

  GeoRegion findCommonRegion(ImageBaseListType::Pointer images){
    typedef ImageBase2DType ImageType;
    typedef ImageBaseListType ImageList;
#ifdef DEBUG
      {
	std::clog << "####################" << std::endl;
	std::clog << "findCommonRegion" << std::endl;
      }
#endif
      int i=0;      
      ImageList::ConstIterator iter = images->Begin();
      ImageType::Pointer image = iter.Get();
      GeoRegion region = createGeoRegion(image);
      GeoRegion::IndexType index = region.GetOrigin();
      for (++iter; iter != images->End();++iter){    
	ImageType::Pointer image = iter.Get();
	GeoRegion currentRegion = createGeoRegion(image);
	
#ifdef DEBUG
	{      
	  GeoRegion::IndexType index = region.GetOrigin();
	  GeoRegion::SizeType size = region.GetSize();
	  GeoRegion::IndexType currentIndex = currentRegion.GetOrigin();
	  GeoRegion::SizeType currentSize = currentRegion.GetSize();
	  
	  std::clog << "Region Check:" << std::endl;
	  std::clog << "Region Index Until Now: " << index[0] << ", " << index[1] << std::endl;
	  std::clog << "Region Size Until Now: " << size[0] << ", " << size[1] << std::endl;
	  std::clog << "Region Index To Check: " << currentIndex[0] << ", " << currentIndex[1] << std::endl;
	  std::clog << "Region Size To Check: " << currentSize[0] << ", " << currentSize[1] << std::endl;
	  
	  std::clog << "--" << std::endl;
	}
#endif
	/*
	  if (!region.Crop(currentRegion)){
	  std::cerr << "Error: Images do not overlap." << std::endl;
	  exit(1);
	  };
	*/
	region = cropGeoRegion(region, currentRegion);
	
#ifdef DEBUG
	{
	  GeoRegion::IndexType index = region.GetOrigin();
	  GeoRegion::SizeType size = region.GetSize();
	  std::clog << "After Crop:" << std::endl;    
	  std::clog << "Common Region Origin:" << index[0] << ", " << index[1] << std::endl;    
	  std::clog << "Common Region Size:" << size[0] << ", " << size[1] << std::endl;    
	  std::clog << "####################" << std::endl;
	}
#endif
	
      }
      
      return region;
  };
    
  GeoRegion findEnclosingRegion(ImageBaseListType::Pointer images){
    typedef ImageBase2DType ImageType;
    typedef otb::ImageList<ImageType> ImageList;
#ifdef DEBUG
      {
	std::clog << "####################" << std::endl;
	std::clog << "findEnclosingRegion" << std::endl;
      }
#endif
      int i=0;     
      ImageList::ConstIterator iter = images->Begin();
      ImageType::Pointer image = iter.Get();
      GeoRegion region = createGeoRegion(image);
      GeoRegion::IndexType index = region.GetOrigin();
      for (++iter; iter != images->End();++iter){    
	ImageType::Pointer image = iter.Get();
	GeoRegion currentRegion = createGeoRegion(image);
	
#ifdef DEBUG
	{      
	  GeoRegion::IndexType index = region.GetOrigin();
	  GeoRegion::SizeType size = region.GetSize();
	  GeoRegion::IndexType currentIndex = currentRegion.GetOrigin();
	  GeoRegion::SizeType currentSize = currentRegion.GetSize();
	  
	  std::clog << "Region Check:" << std::endl;
	  std::clog << "Region Index Until Now: " << index[0] << ", " << index[1] << std::endl;
	  std::clog << "Region Size Until Now: " << size[0] << ", " << size[1] << std::endl;
	  std::clog << "Region Index To Check: " << currentIndex[0] << ", " << currentIndex[1] << std::endl;
	  std::clog << "Region Size To Check: " << currentSize[0] << ", " << currentSize[1] << std::endl;
	  
	  std::clog << "--" << std::endl;
	}
#endif
	/*
	  if (!region.Crop(currentRegion)){
	  std::cerr << "Error: Images do not overlap." << std::endl;
	  exit(1);
	  };
	*/
	region = joinGeoRegion(region, currentRegion);
	
#ifdef DEBUG
	{
	  GeoRegion::IndexType index = region.GetOrigin();
	  GeoRegion::SizeType size = region.GetSize();
	  std::clog << "After Join:" << std::endl;    
	  std::clog << "Common Region Origin:" << index[0] << ", " << index[1] << std::endl;    
	  std::clog << "Common Region Size:" << size[0] << ", " << size[1] << std::endl;    
	  std::clog << "####################" << std::endl;
	}
#endif
	
      }
      
      return region;
  };

}

