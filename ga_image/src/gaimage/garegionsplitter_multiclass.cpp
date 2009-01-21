#include "garegionsplitter_multiclass.h"


using namespace std;
using namespace Ga;




int relabelOutput(const Image& imageToRelabel,
		  ClassnameMapper classnamemapper,
		  const string& labelimagefilename, 
		  const string& regiondescriptionfilename, int minsize, int maxsize, int greyValueDifference){
  int exit_value=EXIT_SUCCESS;
  vector<RegDesc> regList;  
  Image maskimg = Image(typeid(int), imageToRelabel.sizeX(), imageToRelabel.sizeY(), 0);
  maskimg=1;

  MulticlassRegionFinder rf(maskimg, imageToRelabel, greyValueDifference);
  rf.setClassnameMapper(classnamemapper);

  Image tmpImage(typeid(float), imageToRelabel.sizeX(), imageToRelabel.sizeY(), 1);
  
  RegionSplitterT<RegDesc, MulticlassRegionFinder> rSplitter(regList, tmpImage,rf, minsize, maxsize);
  rSplitter.setLabelFile(labelimagefilename);
  rSplitter.split();  

  tmpImage.write(labelimagefilename.c_str());
  clog << "Finished writing labelimage to filename " << labelimagefilename << endl;
  int t=regionsToFile(regiondescriptionfilename, regList);

  if (t != EXIT_SUCCESS){
    cerr << "Can't open regionfile " << regiondescriptionfilename << endl;
    exit (EXIT_FAILURE);
  }
  return exit_value;
}

int simpleOutput(const Image& imageToRelabel,
		       ClassnameMapper classnamemapper,
		       const std::string& labelimagefilename, 
		       const std::string& regiondescriptionfilename)
{
   
   
   // Determine Bounding Boxes for each region
   map<int, BoundingBox> bBoxes;
   for (int x=0; x < imageToRelabel.sizeX(); x++)
     for (int y=0; y < imageToRelabel.sizeY(); y++)
	 {
	    float currentLabel = imageToRelabel.getFloat(x, y);
	    if (currentLabel)
	      {		 
		 int label = currentLabel;
		 
		 BoundingBox* current_BBox = &bBoxes[label];
		 if (current_BBox->llx > x)
		   current_BBox->llx = x;
		 if (current_BBox->lly < y)
		   current_BBox->lly = y;
		 if (current_BBox->urx < x)
		   current_BBox->urx = x;
		 if (current_BBox->ury > y)
		   current_BBox->ury = y;
		 
	      }	    
	 }
   
   vector<RegDesc> regList;
   
   ClassnameMapper::iterator pos;
   for (pos=classnamemapper.begin(); pos != classnamemapper.end(); ++pos)
     {

	if (bBoxes.count(pos->first))
	  {	     

	     BoundingBox cBBox = bBoxes[pos->first];
	     RegDesc reg;
	     reg.file_=labelimagefilename;
	     reg.class_=pos->second;
	     reg.name_ = "Resultat " +  pos->second;
	     reg.id_=pos->first;
	     reg.llx_=cBBox.llx;
	     reg.lly_=cBBox.lly;
	     reg.urx_=cBBox.urx;
	     reg.ury_=cBBox.ury;
	     regList.push_back(reg);
#ifdef DEBUG_MSG
	     clog << reg.toString() << endl;
#endif	    
	  }
	else{

	}
	
     }
   // Write Label Image
     {	
	Image outImage = imageToRelabel;
	outImage.write(labelimagefilename.c_str());
     }
   // Write Label Descriptions
   int t=regionsToFile(regiondescriptionfilename, regList);
   return t;
   
}


