/***************************************************************************
                          ga_label2description.cpp  -  description
                             -------------------
    begin                : Thu Dec  7 10:21:00 MET 2000
    copyright            : (C) 2000 by jürgen bückner
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
// ./ga_label2description out_label2description /project/geoaida/share/data/images/region001.pgm out_label2description.pgm testclass 100 0.4

#include <stdio.h>
#include <stdlib.h>
#include <iostream.h>
#ifdef XXXXXXX
#include "bottomup.h"
//#include "gareadwrite.h"
#include "regionsensort.h"
#include <qlist.h>
#include "nodelist.h"
//#include "garegion.h"
#endif

void usage(char* prg)
{
  cout << "usage: "<< prg << " resultfile inimage outimage classname label [weighting]" << endl
       << "\t resultfile  - 'XML' region descripion" << endl
       << "\t inimage     - input image" << endl
       << "\t outimage    - result image" << endl
       << "\t classname   - classname" << endl
       << "\t label       - label number of the class " << endl
       << "\t weighting    - optional weighting for all classes " << endl
       << "\t   description: " << endl
       << "\t     read 'input image' and label this image for" << endl
       << "\t     the class 'classname' with has the label 'label'." << endl
       << "\t     The unique new label image is written as " << endl
       << "\t     'outimage' and the description in 'resultfile'. " << endl
       << "               " << endl;
  exit(-1);
}

int main(int argc, char *argv[])
{
  if (argc < 6)
    usage(argv[0]);
#ifdef XXXXX
  char* result = argv[1];
  char* inImgName = argv[2];
  char* outImgName = argv[3];
  char* classname = argv[4];
  int classlabel = atoi(argv[5]);
  float p = 0.5;
  if (argc == 7) p = atof(argv[6]);
  /*
  char* result = "OUT_label2description";
  char* inImgName = "/project/geoaida/share/data/images/region001.pgm";
  char* outImgName = "OUT_label2description.pgm";
  char* classname = "NEWCLASS";
  int classlabel = 100; */

  QFile fp(result);
  if (!fp.open(IO_WriteOnly)) qDebug("write: file not accesable to %s\n",argv[2]);
  QTextStream str(&fp);

  RegionSensor *rs = new RegionSensor(inImgName);
  int classCount;
  IMGTYPE type_ = rs->type();
	switch (type_) { //label is need for template function
		case _PFM_SINT: {
		  ImageT<int>* img = (ImageT<int>*)(rs->image());
			labelBin(*img, (int)classlabel);
      classCount = (int) labelImage(*img);
      img.write(outImgName);
      //NSPgaWriteImage(img, outImgName);
	    for (int i = 2; i<classCount; i++) {
      	Node* node = new Node;
      	node->id(i);
      	node->p(p);
      	node->classname(classname);
      	node->filename(outImgName);
      	node->dataTyp(type_);
	      rs->node(node);
        rs->basic((int)i);
        node->write(str);
        //rs->node()->info(); cout <<endl;
    	}
		}
		break;		
		case _PFM_UINT: {
		  ImageT<unsigned int>* img = (ImageT<unsigned int>*)(rs->image());
			labelBin(*img, (unsigned int)classlabel);
      classCount = (int) labelImage(*img);
      img.write(outImgName);
      //NSP gaWriteImage(img, outImgName);
	    for (int i = 2; i<classCount; i++) {
      	Node* node = new Node;
      	node->id(i);
      	node->p(p);
      	node->classname(classname);
      	node->filename(outImgName);
      	node->dataTyp(type_);
	      rs->node(node);
        rs->basic((unsigned int)i);
        node->write(str);
        //rs->node()->info(); cout <<endl;
    	}
		}
		break;
		case _PFM_SINT16: {
		  ImageT<short>* img = (ImageT<short>*)(rs->image());
			labelBin(*img, (short)classlabel);
      classCount = (int) labelImage(*img);
      img.write(outImgName);
      //NSP gaWriteImage(img, outImgName);
	    for (int i = 2; i<classCount; i++) {
      	Node* node = new Node;
      	node->id(i);
      	node->p(p);
      	node->classname(classname);
      	node->filename(outImgName);
      	node->dataTyp(type_);
	      rs->node(node);
        rs->basic((short)i);
        node->write(str);
        //rs->node()->info(); cout <<endl;
    	}
		}
		break;
		case _PFM_UINT16: {
		  ImageT<unsigned short>* img = (ImageT<unsigned short>*)(rs->image());
			labelBin(*img, (unsigned short)classlabel);
      classCount = (int) labelImage(*img);
      img.write(outImgName);
      //NSP gaWriteImage(img, outImgName);
	    for (int i = 2; i<classCount; i++) {
      	Node* node = new Node;
      	node->id(i);
      	node->p(p);
      	node->classname(classname);
      	node->filename(outImgName);
      	node->dataTyp(type_);
	      rs->node(node);
        rs->basic((unsigned short)i);
        node->write(str);
        //rs->node()->info(); cout <<endl;
    	}
		}
		break;
		case _PGM: {
		  ImageT<unsigned char>* img = (ImageT<unsigned char>*)(rs->image());
			labelBin(*img, (unsigned char)classlabel);
      classCount = (int) labelImage(*img);
      //cout  << outImgName<<" .. "<<classname<<"..classCount:" << classCount<< "; x:"<<img->sizeX()<< "; y:"<<img->sizeY() <<endl;
      img.write(outImgName);
      //NSP gaWriteImage(img, outImgName);
	    for (int i = 2; i<classCount; i++) {
      	Node* node = new Node;
      	node->id(i);
      	node->p(p);
      	node->classname(classname);
      	node->filename(outImgName);
      	node->dataTyp(type_);
	      rs->node(node);
        rs->basic((unsigned char)i);
        node->write(str);
        //rs->node()->info(); cout <<endl;
    	}
		}
		break;
		default:		
			qDebug("ERROR: RegionSensor::calc - wrong image type");
		break;
	}	
	
	fp.close();
#endif 0	
	cout << "FIN .. "<<endl;
#ifdef WIN32
  return (0);
#endif
}
