////////////////////////////////////////////////////////////////////////////////
///
/// \file		garegionsplitter.cpp
/// \brief		Implementation of methods and functions for region handling
///
/// \date		Aug. 2007
/// \author		Christian Becker (becker@tnt.uni-hannover.de)
///				Torsten Büschenfeld (bfeld@tnt.uni-hannover.de)
///
/// \note		Tabulator size 4 used
///
////////////////////////////////////////////////////////////////////////////////

#include "garegionsplitter.h"

namespace Ga
{

vector<RegDesc> splitIntoRegions(Image &labelpic,  
                                     RegionFinder& regfind,
                                     string regionclass,
                                     string labelfile,
                                     int minsize, int maxsize)
{
	if (maxsize <0 )
		maxsize=INT_MAX;

	vector<RegDesc> rList;
	RegionSplitterT<RegDesc, RegionFinder> rSplitter(rList,labelpic,regfind,minsize,maxsize);
	rSplitter.setRegionClass(regionclass);
	rSplitter.setLabelFile(labelfile);

	rSplitter.split();

	return rList;

}

vector<RegDesc> splitIntoRegions(Image &labelpic,  
                                     RegionFinder& regfind,
                                     int minsize, int maxsize)
{
    return splitIntoRegions(labelpic, regfind, "undefined", "", minsize, maxsize);
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Save region description to file
///
/// \param filename Name of file the description will be saved to
/// \param reglist Structure defining description format
///
////////////////////////////////////////////////////////////////////////////////
int regionsToFile(string filename, vector<RegDesc>& reglist)
{

	ostringstream out;
	// vector<RegDesc>::iterator regIter= reglist.begin();

	for (int i=2; i < reglist.size(); i++) // Skip the first 2 elements because they contain id 0 and 1 (= background regions), 
                                         // I'm not sure why they are included in the list at all...
	{
		RegDesc reg = reglist[i];
    out << reg.toString() << endl;
	}


	ofstream outputFile(filename.c_str(), ios::out);

	if (!outputFile)
	{
		cerr << "can't open region output file \"" << filename << "\"" << endl;
		return EXIT_FAILURE;
	}
	outputFile << out.str();
	return EXIT_SUCCESS;
}
} // namespace Ga
