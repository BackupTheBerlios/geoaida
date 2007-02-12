/***************************************************************************
                          regionmask.cpp  -  description
                             -------------------
    begin                : Wed Mar 21 2001
    copyright            : (C) 2001 by jrgen bckner
    email                : bueckner@tnt.uni-hannover.de
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "gaimage.h"
#include "gaimaget.h"
#include "pfm.h"
#include "MLParser.h"
#include <qfile.h>
#include <qlist.h>
using namespace Ga;

typedef Image GaMaskImage;

QDict<Image> labelImageDict;

void Usage(const char* prg) {
  fprintf(stderr,"Usage:\n"
	 "  %s <region-file> <mask-file>\n"
	 "Clips the region-file to mask",prg);
  exit(1);

}

void maskLabelImage(Image& im, GaMaskImage mask) {
	if (im.sizeX()!=mask.sizeX() || im.sizeY()!=mask.sizeY()) {
		mask=mask.resampleNN(im.sizeX(),im.sizeY());
	}
	for (int c=0; c<im.noChannels(); c++) {
		GaMaskImage::ConstIterator mit=mask.constBegin();
		GaMaskImage::ConstIterator mitEnd=mask.end();
		void* it=im.begin();
		for (; mit!=mitEnd; mask.nextCol(mit), im.nextCol(it)) {
			if (!mask.getInt(mit)) {
				im.set(it,0.0);
			}
		}
	}
}

Image* readLabelFile(QString filename,GaMaskImage& mask) {
	Image* im=labelImageDict[filename];
	if (im) return im; // image found in dictionary
	im=new Image();
	im->read(filename.latin1());
	labelImageDict.insert(filename,im);
	maskLabelImage(*im,mask);
	return im;
}

bool processRegion(ArgDict& args, GaMaskImage& mask) {
#define DefInt(name) int name; MLParser::setInt(name,&args,#name);
#define DefFloat(name) float name; MLParser::setFloat(name,&args,#name);
	QString labelFileName;
	MLParser::setString(labelFileName,&args,"file");
	if (labelFileName.isEmpty()) return true; // no label file given
	Image* labelImage=readLabelFile(labelFileName,mask);
	DefInt(llx);
	DefInt(lly);
	DefInt(urx);
	DefInt(ury);
	if (mask.getFloat(llx,lly,0,false)
		&& mask.getFloat(llx,ury,0,false)
		&& mask.getFloat(urx,ury,0,false)
		&& mask.getFloat(urx,lly,0,false))
		return true;; // Boundingbox is within mask

	// adjust boundingbox to mask
	DefFloat(geoWest);
	DefFloat(geoNorth);
	DefFloat(geoEast);
	DefFloat(geoSouth);
	DefInt(id);
	int nllx=urx;
	int nlly=ury;
	int nurx=llx;
	int nury=lly;
	bool found=false;					
	for (int y=ury; y<=lly; y++) {
		for (int x=llx; x<=urx; x++) {
			if (labelImage->getFloat(x,y,0,0.0)==id) {
				found=true;
				if (x<nllx) nllx=x;
				if (x>nurx) nurx=x;
				if (y<nlly) nlly=y;
				if (y>nury) nury=y;
			}
		}
	}
	if (!found) return found;
	
	args.replace("llx",nllx);
	args.replace("lly",nlly);
	args.replace("urx",nurx);
	args.replace("ury",nury);
#if 0	
	if (args["geoWest"]) {	
		float nGeoWest=(geoEast-geoWest)/(urx-llx)*(nllx-llx)+geoWest;
		float nGeoNorth=(geoNorth-geoSouth)/(ury-lly)*(nury-lly)+geoSouth;
		float nGeoEast=(geoEast-geoWest)/(urx-llx)*(nurx-llx)+geoWest;
		float nGeoSouth=(geoNorth-geoSouth)/(ury-lly)*(nlly-lly)+geoSouth;
		args.replace("geoWest",nGeoWest);
		args.replace("geoNorth",nGeoNorth);
		args.replace("geoEast",nGeoEast);
		args.replace("geoSouth",nGeoSouth);
	}
#endif	
	return found;
		
#undef DefInt
#undef DefFloat	
}

int main(int argc, char *argv[])
{	
	if (argc!=3) Usage(argv[0]);
	
	// Read maskfile
	FILE *mfp=fopen(argv[2],"r");
//	FILE *mfp=fopen("/project/geoaida/tmp/mask.pbm","r");
	
	if (!mfp) {
		fprintf(stderr,"mask file %s not found\n",argv[2]);
		return 1;
	}
	GaMaskImage mask;
	mask.read(mfp);
	fclose(mfp);
	
	// read regionfile
//  QFile rfp("/project/geoaida/tmp/reglist.dest");
  QFile rfp(argv[1]);
  if (!rfp.open(IO_ReadOnly)) {
    fprintf(stderr,"regionfile %s not founed\n",argv[1]);
    return 1;
  }
  // Read and process regions
  QList<ArgDict> regionList;
  MLParser parser(&rfp);
	QString keywords[]={"region",""};
	const MLTagTable nodeTagTable(keywords);
	const int TOK_REGION=1;
	int tag;
	do {
		tag=parser.tag(nodeTagTable);
		ArgDict* args;
		switch (tag) {
			case TOK_REGION: {
					args=parser.args();
					if (processRegion(*args,mask))
						regionList.append(args);
					else
						delete args;
					break;
				}
			case -TOK_REGION:
				break;
			default: {
					args=parser.args();
					delete args;
					qDebug("Unknown keyword");
					break;
				}
		}
	} while (tag!=MLParser::END_OF_FILE );
	rfp.close();
	// Write labels
	QDictIterator<Image> git(labelImageDict);
	for (;git.current();++git) {
		Image *im=git.current();
		qDebug("Writing %s",git.currentKey().latin1());
		im->write(git.currentKey().latin1());
	}
	// Write regions
  if (!rfp.open(IO_WriteOnly)) {
    fprintf(stderr,"cannot open regionfile %s for writing\n",argv[1]);
    return 1;
  }
	QListIterator<ArgDict> it(regionList);
	QTextStream ts(&rfp);
	for (;it.current();++it) {
		ArgDict* argDict=it.current();
		assert(argDict);
		ts << "<region ";
		ts << (*argDict);
		ts << " />" << endl;
	}
	rfp.close();
}
	
