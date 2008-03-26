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

//#define DEBUG_PRG

#include "gaimage.h"
#include "gaimaget.h"
#include "pfm.h"
#include "MLParser.h"
#include <qfile.h>
#include <qlist.h>
#include <getopt.h>

using namespace Ga;

typedef Image GaMaskImage;

struct LabelImage
{
  ~LabelImage() {
    fprintf(stderr,"LabelImage::~LabelImage\n");
    image.~Image();
  }
  Image image;
  int offset_x;
  int offset_y;
  int orig_size_x;
  int orig_size_y;
  float scale_x;
  float scale_y;
};

QDict <Image> imageDict;
QDict <LabelImage> labelImageDict;
QDict <QList < ArgDict > > regionFileDict;

void Usage(const char *prg)
{
  fprintf(stderr, "Usage:\n"
          "  %s [-o <outfile>] [-r <result-labelfile>] <region-file> <mask-file> [<mask_x> <mask_y> <mask_size_x> <mask_size_y>]\n"
          "  %s -p <pipename> -l <lockname> \n"
          "Clips the region-file to mask\n"
          "if outfile is given, a new region description is written to outfile\n"
          "else the region-file will be overwritten\n\n"
          "Command in pipemode\n"
          "run <outfile> <region-file> <mask-file> [<mask_x> <mask_y> <mask_size_x> <mask_size_y>]\n"
          "end\n"  , prg,prg);
  exit(1);

}

void maskLabelImage(LabelImage & im, GaMaskImage mask, int mask_x, int mask_y,
                    int mask_size_x, int mask_size_y)
{
  im.orig_size_x = im.image.sizeX();
  im.orig_size_y = im.image.sizeY();
  im.offset_x = 0;
  im.offset_y = 0;
  printf("mask_size %d %d\n",mask_size_x, mask_size_y);
  if (mask_size_x != 0) {
    im.scale_x=float (im.image.sizeX()) / mask_size_x;
    im.scale_y=float (im.image.sizeY()) / mask_size_y;
    printf("scale_x %f scale_y %f\n", im.scale_x, im.scale_y);    
    im.offset_x = int (mask_x * im.scale_x);
    im.offset_y = int (mask_y * im.scale_y);
    int new_size_x =int (mask.sizeX() * im.scale_x);
    int new_size_y =int (mask.sizeY() * im.scale_y);
    printf("new_size %d %d\n",new_size_x, new_size_y);
    if (new_size_x==0 || new_size_y==0) {
      Image img(im.image.typeId(),new_size_x, new_size_y, im.image.noChannels());
      im.image = img;
    }
    else {
      Image img(im.offset_x, im.offset_y, new_size_x, new_size_y, im.image);
      im.image = img;
    }
printf("image_size %d %d\n",im.image.sizeX(),im.image.sizeY());
  }
  if (im.image.sizeX() != mask.sizeX() || im.image.sizeY() != mask.sizeY()) {
    mask = mask.resampleNN(im.image.sizeX(), im.image.sizeY());
  }
  for (int c = 0; c < im.image.noChannels(); c++) {
    GaMaskImage::ConstIterator mit = mask.constBegin();
    GaMaskImage::ConstIterator mitEnd = mask.end();
    void *it = im.image.begin();
    for (; mit != mitEnd; mask.nextCol(mit), im.image.nextCol(it)) {
      if (!mask.getInt(mit)) {
        im.image.set(it, 0.0);
      }
    }
  }
}

LabelImage *readLabelFile(QString filename, GaMaskImage & mask, int &mask_x,
                          int &mask_y, int &mask_size_x, int &mask_size_y)
{
  LabelImage *im = labelImageDict[filename];
  if (im)
    return im;                  // image found in dictionary
  Image *image=imageDict[filename];
  im = new LabelImage();
  if (image)
    im->image=*image;
  else {
    printf("readLabelFile %s\n",filename.latin1());
    image=new Image();
    image->read(filename.latin1());
    imageDict.insert(filename,image);
    im->image=*image;
  }
  labelImageDict.insert(filename, im);
  maskLabelImage(*im, mask, mask_x, mask_y, mask_size_x, mask_size_y);
  return im;
}

bool processRegion(ArgDict & args, GaMaskImage & mask, int mask_x, int mask_y,
                   int mask_size_x, int mask_size_y)
{
#define DefInt(name) int name; MLParser::setInt(name,&args,#name);
#define DefFloat(name) float name; MLParser::setFloat(name,&args,#name);
  QString labelFileName;
  MLParser::setString(labelFileName, &args, "file");
  if (labelFileName.isEmpty())
    return true;                // no label file given
  LabelImage *labelImage =
    readLabelFile(labelFileName, mask, mask_x, mask_y, mask_size_x,
                  mask_size_y);
  if (mask_size_x!=0 || mask_size_y!=0) {
    args.replace("scale_x", labelImage->scale_x);
    args.replace("scale_y", labelImage->scale_y);
    args.replace("offset_x", labelImage->offset_x);
    args.replace("offset_y", labelImage->offset_y);
  }

  DefInt(llx);
  DefInt(lly);
  DefInt(urx);
  DefInt(ury);
  llx -= labelImage->offset_x;
  lly -= labelImage->offset_y;
  urx -= labelImage->offset_x;
  ury -= labelImage->offset_y;

#if 0 
  // Does not work e.g. if two regions have the same bounding box, but have no overlapping region.
  //! Delete this 
  if (mask.getFloat(llx, lly, 0, false)
      && mask.getFloat(llx, ury, 0, false)
      && mask.getFloat(urx, ury, 0, false)
      && mask.getFloat(urx, lly, 0, false)) { //BBox NEU SETZEN
    args.replace("llx", llx);
    args.replace("lly", lly);
    args.replace("urx", urx);
    args.replace("ury", ury);
    return true;;               // Boundingbox is within mask
  }
#endif

  // adjust boundingbox to mask
  DefFloat(geoWest);
  DefFloat(geoNorth);
  DefFloat(geoEast);
  DefFloat(geoSouth);
  DefInt(id);
  if (!id)
    return false;
//  qDebug("id=%05d: (%03d/%03d) (%03d/%03d)",id,llx,lly,urx,ury);
  int nllx = urx;
  int nlly = ury;
  int nurx = llx;
  int nury = lly;
  bool found = false;
  for (int y = ury; y <= lly; y++) {
    for (int x = llx; x <= urx; x++) {
      if (labelImage->image.getFloat(x, y, 0, 0.0) == id) {
        found = true;
        if (x < nllx)
          nllx = x;
        if (x > nurx)
          nurx = x;
        if (y > nlly)
          nlly = y;
        if (y < nury)
          nury = y;
      }
    }
  }
//  qDebug("id=%05d: (%03d/%03d) (%03d/%03d)",id,nllx,nlly,nurx,nury);
  if (!found)
    return found;
//  qDebug("id=%05d: (%03d/%03d) (%03d/%03d)",id,nllx,nlly,nurx,nury);

  args.replace("llx", nllx);
  args.replace("lly", nlly);
  args.replace("urx", nurx);
  args.replace("ury", nury);
#if 0
  if (args["geoWest"]) {
    float nGeoWest =
      (geoEast - geoWest) / (urx - llx) * (nllx - llx) + geoWest;
    float nGeoNorth =
      (geoNorth - geoSouth) / (ury - lly) * (nury - lly) + geoSouth;
    float nGeoEast =
      (geoEast - geoWest) / (urx - llx) * (nurx - llx) + geoWest;
    float nGeoSouth =
      (geoNorth - geoSouth) / (ury - lly) * (nlly - lly) + geoSouth;
    args.replace("geoWest", nGeoWest);
    args.replace("geoNorth", nGeoNorth);
    args.replace("geoEast", nGeoEast);
    args.replace("geoSouth", nGeoSouth);
  }
#endif
  return found;

#undef DefInt
#undef DefFloat
}

QList<ArgDict>* readRegionFile(const char* filename)
{
  QList <ArgDict>* region=regionFileDict[filename];
  if (region) 
    return region;

  // read regionfile
//  QFile rfp("/project/geoaida/tmp/reglist.dest");
  QFile rfp(filename);
  if (!rfp.open(IO_ReadOnly)) {
    fprintf(stderr, "regionfile %s not found\n", filename);
    return 0;
  }
  // Read and process regions
  QList < ArgDict > *regionList=new QList < ArgDict >();
  regionList->setAutoDelete(true);
  MLParser parser(&rfp);
  QString keywords[] = { "region", "" };
  const MLTagTable nodeTagTable(keywords);
  const int TOK_REGION = 1;
  int tag;
  do {
    tag = parser.tag(nodeTagTable);
    ArgDict *args;
    switch (tag) {
    case TOK_REGION:{
        args = parser.args();
	regionList->append(args);
        break;
      }
    case -TOK_REGION:
      break;
    case MLParser::END_OF_FILE:
      break;
    default:{
        args = parser.args();
        delete args;
        qDebug("Unknown keyword %s in line %d", parser.lasttagstr().latin1(),
               parser.lineNumber());
        break;
      }
    }
  } while (tag != MLParser::END_OF_FILE);
  rfp.close();
  regionFileDict.insert(filename,regionList);
}

int DoIt(const char* outFile, const char* regFile, const char* maskFile, const char* resultFile,
          int mask_x, int mask_y, int mask_size_x, int mask_size_y)
{
  // Read maskfile
  FILE *mfp = fopen(maskFile, "r");
//      FILE *mfp=fopen("/project/geoaida/tmp/mask.pbm","r");

  if (!mfp) {
    fprintf(stderr, "mask file %s not found\n", maskFile);
    return 1;
  }
  GaMaskImage mask;
  mask.read(mfp);
  fclose(mfp);

  QList<ArgDict> *regionSourceList=readRegionFile(regFile);
  if (!regionSourceList) return 1;
  QList<ArgDict> regionList;

  // Process regions
  for (ArgDict* arg = regionSourceList->first();
       arg;
       arg=regionSourceList->next()) {
    ArgDict* args=new ArgDict(*arg);
    if (processRegion
	(*args, mask, mask_x, mask_y, mask_size_x, mask_size_y))
      regionList.append(args);
    else
      delete args;
  }
  
  // Write labels
  QFile rfp(outFile ? outFile : regFile);
  if (outFile) {
    if (labelImageDict.count() > 1) {
      fprintf(stderr, "regionmask: Cannot generate multiple labelfiles\n");
      return 1;
    }
    else {
      if (regionList.count()>0) {
        ArgDict *dict = regionList.first();
        QString *oldfile = (*dict)["file"];
        QString *labelfile = new QString();
        if (resultFile)
          labelfile->sprintf("%s", resultFile);
        else
          labelfile->sprintf("%s.plm", outFile);
        LabelImage *im = labelImageDict.take(*oldfile);
        labelImageDict.replace(*labelfile, im);
        QListIterator < ArgDict > it(regionList);
        for (; it.current(); ++it) {
          ArgDict *argDict = it.current();
          assert(argDict);
          argDict->replace("file", labelfile);
        }
      }
    }
  }
  else {
    printf("regionmask: overwriting %s\n",regFile);
    outFile = regFile;
  }
  QDictIterator < LabelImage > git(labelImageDict);
  if (regionList.count()>0) {
    for (; git.current(); ++git) {
      LabelImage *im = git.current();
      qDebug("Writing %s", git.currentKey().latin1());
      im->image.write(git.currentKey().latin1());
    }
  }
  // Write regions
  if (!rfp.open(IO_WriteOnly)) {
    fprintf(stderr, "cannot open regionfile %s for writing\n", outFile);
    return 1;
  }
  if (regionList.count()>0) {
    QListIterator < ArgDict > it(regionList);
    QTextStream ts(&rfp);
    for (; it.current(); ++it) {
      ArgDict *argDict = it.current();
      assert(argDict);
      ts << "<region ";
      ts << (*argDict);
      ts << " />" << endl;
    }
  }
  rfp.close();
  labelImageDict.setAutoDelete(true);
  labelImageDict.clear();
  regionList.setAutoDelete(true);
  return 0;
}

int main(int argc, char *argv[])
{
  printf("regionmask: %d\n",getpid());
  for (int i=0; i<argc; i++) printf("%s ",argv[i]);
  printf("\n");
#ifdef DEBUG_PRG
/*
  DoIt("/home/bueckner/.gda/tmp/30350/Szene/Region_013/Wirtschaftsflaeche_013/0008-output.yy",
       "/home/bueckner/.gda/tmp/30350/texseg_Settlement-Agriculture-Greenland-Forest_rmk001_3401660.000000_5580010.000000_3402180.000000_5579540.000000_1.000000.regs",
       "/home/bueckner/.gda/tmp/30350/Szene/Region_013/Wirtschaftsflaeche_013/3401660.000000_5579850.000000_3402099.000000_5579540.000000.pbm",
       0, 160, 520, 470);
  printf("\n FIN ... regionmask\n");
  exit(1);
*/
#endif
  char *outfile = 0;
  const char* pipeName=0;
//  const char* pipeName="/tmp/pipe";
  const char* lockName=0;
  const char* resultImage=0;
  {
    int opt;
    char options[] = "o:p:l:r:";
    while ((opt = getopt(argc, argv, options)) != -1) {
      switch (opt) {
      case 'r':
        resultImage = optarg;
        break;
      case 'o':
        outfile = optarg;
        break;
      case 'p':
        pipeName=optarg;
      case 'l':
        lockName=optarg;
      }
    }
  }
  if (!pipeName && (argc - optind < 2)) {
    Usage(argv[0]);
    return -1;
  }

  if (pipeName) {
    printf("regionmask: pipe mode %s lockfile %s\n",pipeName,lockName);
    QFile pipe(pipeName);
    int numCalls =-1;
    while (numCalls) {
      printf("Waiting for command\n");
      if (!pipe.open(IO_ReadOnly)) {
        fprintf(stderr,"Can't open pipe %s for reading\n",pipeName);
        return 1;
      }
      QString buffer;
      pipe.readLine(buffer,1024);
      pipe.close();
      QTextIStream cmdStr(&buffer);
      QString cmd;
      cmdStr >> cmd;
      printf("Found buffer %s\n",buffer.latin1());
      // numCalls <numCalls>
      if (cmd=="numCalls") {
        cmdStr >> numCalls;
        continue;
      }
      if (cmd=="end") {
        numCalls=0;
        break;
      }
      if (cmd!="run") {
        fprintf(stderr,"Unknown command: %s\n",buffer.latin1());
        continue;
      }
      // run outfile regfile maskfile mask_x mask_y mask_size_x mask_size_y
      int mask_x = 0;
      int mask_y = 0;
      int mask_size_x = 0;
      int mask_size_y = 0;
      QString outFile,regFile,maskFile;
      cmdStr >> outFile >> regFile >> maskFile >> mask_x >> mask_y >> mask_size_x >> mask_size_y;
      printf("Running regionmask %s %s %s %d %d %d %d\n",
             outFile.latin1(),
             regFile.latin1(),
             maskFile.latin1(),
             mask_x,mask_y,mask_size_x,mask_size_y);
      fflush(stdout);
      sleep(0);
      DoIt(outFile,regFile,maskFile,resultImage,mask_x,mask_y,mask_size_x,mask_size_y);
      printf("Done -- Removing lockfile %s\n",lockName);
      if (lockName) {
        QFile::remove(lockName);
      }
      printf("Done -- Removed lockfile\n");
      numCalls--;
		printf ("numCalls: %i\n", numCalls);
    }
      QFile::remove(pipeName);
      if (lockName) {
      QFile::remove(lockName);
	}

  }
  else {
    int mask_x = 0;
    int mask_y = 0;
    int mask_size_x = 0;
    int mask_size_y = 0;
    if (argc - optind > 5) {
      sscanf(argv[optind + 2], "%d", &mask_x);
      sscanf(argv[optind + 3], "%d", &mask_y);
      sscanf(argv[optind + 4], "%d", &mask_size_x);
      sscanf(argv[optind + 5], "%d", &mask_size_y);
    }
    DoIt(outfile,argv[optind],argv[1 + optind],resultImage,mask_x, mask_y, mask_size_x, mask_size_y);
  }
}
