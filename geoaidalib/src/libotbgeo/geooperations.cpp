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

}

