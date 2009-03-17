/***************************************************************************
                          geo_image.cpp  -  description
                             -------------------
    begin                : Thu Oct 19 2000
    copyright            : (C) 2000 by Jürgen Bückner
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



#include "geoimage.h"
#include <qcolor.h>
#include <qdir.h>
#include <assert.h>
#include "fatalerror.h"
#include "cleanup.h"

GeoImageCache GeoImage::cache_;

GeoImage::GeoImage()
{
  init();
}

GeoImage::GeoImage(MLParser & parser)
{
  init();
  read(parser);
}

/** constructor read attributes for this GeoImage through dictionary*/
GeoImage::GeoImage(ArgDict & dict)
{
  init();
  configure(dict);
}

/** label-picture constructor */
GeoImage::GeoImage(QString fname, QString key, float west, float north,
                   float east, float south)
{
  init();
  insert("type", new QString("LABEL"));
  insert("key", new QString(key));
  type_ = PFM_UINT16;
  geoNorth_ = north;
  geoSouth_ = south;
  geoWest_ = west;
  geoEast_ = east;
#ifdef WIN32
  insert("geoNorth", new QString(north));
  insert("geoSouth", new QString(south));
  insert("geoWest", new QString(west));
  insert("geoEast", new QString(east));
  insert("file", new QString(fname));
#else
  insert("geoNorth", north);
  insert("geoSouth", south);
  insert("geoWest", west);
  insert("geoEast", east);
  insert("file", fname);
#endif
}

/** label-picture constructor */
GeoImage::GeoImage(QString fname, QString key, int xsize, int ysize,
                   float west, float north, float east, float south)
{
  init();
  cols_ = xsize;
  rows_ = ysize;
  insert("type", new QString("LABEL"));
  insert("key", new QString(key));
  type_ = PFM_SINT;
  geoNorth_ = north;
  geoSouth_ = south;
  geoWest_ = west;
  geoEast_ = east;
#ifdef WIN32
  insert("geoNorth", new QString(north));
  insert("geoSouth", new QString(south));
  insert("geoWest", new QString(west));
  insert("geoEast", new QString(east));
  insert("file", new QString(fname));
#else
  insert("geoNorth", north);
  insert("geoSouth", south);
  insert("geoWest", west);
  insert("geoEast", east);
  insert("file", fname);
#endif
  data_ = new int[xsize * ysize];
#ifdef WIN32
  if (data_ == 0){
    cout << "Out of Memory..5";
    exit(1);
  }
#endif

  for (int i = 0; i < xsize * ysize; i++)
    ((int *) data_)[i] = 0;
}

GeoImage::~GeoImage()
{
  cache_.removeImage(this);
  freeData();
}

void GeoImage::init()
{
  data_ = 0;
  dataSize_=0;
  type_ = UNKNOWN;
  cols_ = rows_ = 0;
  //minval_ = maxval_ = 0.0;
  geoSouth_ = geoNorth_ = geoEast_ = geoWest_ = 0.0;
  resolutionX_ = 0;
  resolutionY_ = 0;
}

/** read attributes for this operator through parser */
void GeoImage::read(MLParser & parser)
{                               //bekommt einen Parser übergeben und liest die Attribute ein
  qDebug("GeoImage::read(MLParser& parser)");
  parser.args(this);
  MLParser::setFloat(geoNorth_, this, "geoNorth");
  MLParser::setFloat(geoSouth_, this, "geoSouth");
  MLParser::setFloat(geoWest_, this, "geoWest");
  MLParser::setFloat(geoEast_, this, "geoEast");
#ifdef WIN32
  if (!find("type"))   insert("type",   new QString("not set"));
  if (!find("file"))	  insert("file",   new QString("not set"));
  if (!find("dir"))    insert("dir",    new QString(""));
  if (!find("res_x"))  insert("res_x",  new QString("not set"));
  if (!find("res_y"))  insert("res_y",  new QString("not set"));
  if (!find("size_x")) insert("size_x", new QString("not set"));
  if (!find("size_y")) insert("size_y", new QString("not set"));
#else
  if (!find("type"))
    insert("type", QString("not set"));
  if (!find("file"))
    insert("file", QString("not set"));
  if (!find("dir"))
    insert("dir", "");
  if (!find("res_x"))
    insert("res_x", QString("not set"));
  if (!find("res_y"))
    insert("res_y", QString("not set"));
  if (!find("size_x"))
    insert("size_x", QString("not set"));
  if (!find("size_y"))
    insert("size_y", QString("not set"));
#endif
}

/** configure attributes for this GeoImage through dictionary */
void GeoImage::configure(ArgDict & dict)
{
#define copyarg(name) if (dict[name]) insert(name,new QString(*dict[name]));
  copyarg("geoNorth");
  copyarg("geoSouth");
  copyarg("geoWest");
  copyarg("geoEast");
  copyarg("type");
  copyarg("fle");
  copyarg("x_res");
  copyarg("y_res");
  copyarg("size_x");
  copyarg("size_y");
  MLParser::setFloat(geoNorth_, this, "geoNorth");
  MLParser::setFloat(geoSouth_, this, "geoSouth");
  MLParser::setFloat(geoWest_, this, "geoWest");
  MLParser::setFloat(geoEast_, this, "geoEast");
  if (!find("type"))
    insert("type", new QString("not set"));
  if (!find("file"))
    insert("file", new QString("not set"));
  if (!find("dir"))
    insert("dir", new QString(""));
  if (!find("x_res"))
    insert("x_res", new QString("not set"));
  if (!find("y_res"))
    insert("y_res", new QString("not set"));
  if (!find("size_x"))
    insert("size_x", new QString("not set"));
  if (!find("size_y"))
    insert("size_y", new QString("not set"));
#undef copyarg
}

/** load image info - not the data */
void GeoImage::load()
{
  QString fname = filename();
  qDebug("GeoImage::load(%s)", (const char *) fname);
  FILE *fp;
  fp = fopen(fname, "r");
  if (!fp) {
    qDebug("#  (ERROR)GeoImage::load(%s) Can't open file for reading!",
           (const char *) fname);

    throw FatalError(QString().
                     sprintf
                     ("GeoImage::load(%s) Can't open file for reading!",
                      (const char *) fname));
  }
  int pxmtype;
  xelval max_x;
  freeData();
  float minval_, maxval_;

  if (pfm_readpfm_header(fp, &cols_, &rows_, &minval_, &maxval_, &pxmtype))
    type_ = (IMGTYPE) pxmtype;  //0 = FALSE, 1 = TRUE
  else {                        /* now check for all the other fucking p?m-formats */
    pnm_readpnminit(fp, &cols_, &rows_, &max_x, &pxmtype);
    rewind(fp);
    minval_ = 0.0;
    maxval_ = (float) max_x;
    if (pxmtype == PBM_FORMAT || pxmtype == RPBM_FORMAT)
      type_ = PBM;
    if (pxmtype == PGM_FORMAT || pxmtype == RPGM_FORMAT)
      type_ = PGM;
    if (pxmtype == PPM_FORMAT || pxmtype == RPPM_FORMAT)
      type_ = PPM;
  }
  fclose(fp);
  qDebug("##  Image: (%d, %d) %f - %f, type: %d",
         cols_, rows_, minval_, maxval_, type_);
  if (type_ == UNKNOWN)
    qDebug("##  (ERROR) unknown image type!");
#ifdef WIN32
  replace("size_x", &QString::number(cols_));
  replace("size_y", &QString::number(rows_));
  resolutionX_ = (geoEast_ - geoWest_) / cols_;
  replace("res_x", &QString::number(resolutionX_));
  resolutionY_ = (geoNorth_ - geoSouth_) / rows_;
  replace("res_y", &QString::number(resolutionY_));
#else
  replace("size_x", cols_);
  replace("size_y", rows_);
  resolutionX_ = (geoEast_ - geoWest_) / cols_;
  replace("res_x", resolutionX_);
  resolutionY_ = (geoNorth_ - geoSouth_) / rows_;
  replace("res_y", resolutionY_);
#endif
}

int sizeOfData(int type)
{
  switch (type) {
  case GeoImage::PFM_FLOAT:
    return sizeof(float);
  case GeoImage::PFM_UINT:
    return sizeof(unsigned int);
  case GeoImage::PFM_SINT:
    return sizeof(signed int);
  case GeoImage::PFM_SINT16:
    return sizeof(signed short int);
  case GeoImage::PFM_UINT16:
    return sizeof(unsigned short int);
  case GeoImage::PFM_BYTE:
    return sizeof(char);
  case GeoImage::PFM_3BYTE:
    return 3*sizeof(char);
  case GeoImage::PBM:
    return sizeof(char);
  case GeoImage::PGM:
    return sizeof(char);
  case GeoImage::PPM:
    return 3*sizeof(char);
  default:
    return sizeof(int);
  }
}

/** return data */
const void *GeoImage::data()
{
  if (data_)
    return (void *) data_;
  qDebug("GeoImage::data() - load data");
  QString fname = filename();
  qDebug("GeoImage::data() %s", (const char *) fname);

  FILE *fp;
  fp = fopen(fname, "r");
  if (!fp) {
    qDebug("#  (ERROR)GeoImage::load(%s) Can't open file for reading!",
           (const char *) fname);
    return 0;
  }
  int cols, rows;
  //float minval, maxval;
  switch (type_) {
  case PFM_FLOAT:
  case PFM_UINT:
  case PFM_SINT:
  case PFM_SINT16:
  case PFM_UINT16:
  case PFM_BYTE:
  case PFM_3BYTE:
    data_ = pfm_readpfm_type(fp, &cols, &rows, &minval_, &maxval_, type_, 0);
    testSize(cols, rows, type_);
    break;
  case PBM:
    data_ = (void *) pbm_readpbm(fp, &cols, &rows);
    minval_ = 0.0, maxval_ = 1.0;
    testSize(cols, rows, type_);
    break;
  case PGM:{
      gray maxval, **data_g;
      data_g = pgm_readpgm(fp, &cols, &rows, &maxval);
      data_ = (void *) data_g;
      minval_ = 0.0, maxval_ = (float) maxval;
      testSize(cols, rows, type_);
    }
    break;
  case PPM:
    pixval maxval;
    pixel **dat_p;
    dat_p = ppm_readppm(fp, &cols, &rows, &maxval);
    data_ = (void *) dat_p;
    minval_ = 0.0, maxval_ = (float) maxval;
    testSize(cols, rows, type_);
    break;
  default:
    qDebug("Unknown type %d",type_);
    fclose(fp);
    return 0;
  }
  fclose(fp);
  if (data_) dataSize_=rows_*cols_*sizeOfData(type_);
  cache_.useImage(this);
  return data_;
}

/** size of data */
int GeoImage::dataSize()
{
  return dataSize_;
}

/** frees the data */
void GeoImage::freeData()
{
  if (data_) {
    switch (type_) {
    case PBM:
      pbm_freearray(data_, rows_);
      break;
    case PGM:
      pgm_freearray(data_, rows_);
      break;
    case PPM:
      ppm_freearray(data_, rows_);
      break;
    case PFM_3BYTE:{
        PFM3Byte *ppmbuf = (PFM3Byte *) data_;
        free(ppmbuf->r);
        free(ppmbuf->g);
        free(ppmbuf->b);
        free(data_);
      }
      break;
    default:
      free(data_);
      break;
    }
    data_ = 0;
    dataSize_=0;
  }
}

bool GeoImage::testSize(int cols, int rows, IMGTYPE)
{
  if (cols != cols_ || rows != rows_) {
    qDebug("##  (ERROR) inconsistent image data!");
    // delete *data_;  // XXXX Fallunterscheidung dann löschen
    freeData();
    type_ = UNKNOWN;
    return FALSE;
  }
  return TRUE;
}

/** get the filename */
QString GeoImage::filename()
{
  if (find("dir") && !find("dir")->isEmpty())
#ifdef WIN32
    return (*(find("dir"))) + "\\" + (*(find("file")));
#else
    return (*(find("dir"))) + "/" + (*(find("file")));
#endif
  else
    return (*(find("file")));
}

/** write a scrap of the data,
  * return the filename,
  * if the file exist do nothing
  * argument fname is optional
  * the coordinates of the image part are geodata e.g. Gauss Krueger **/
QString
  GeoImage::part(float west, float north, float east, float south,
                 QString fname)
{
  if (fname.isEmpty()) {        //create output filname
    ASSERT(find("key"));
    QString dir = CleanUp::getTmpDir();
    fname.sprintf("%s/%s_%f_%f_%f_%f", dir.latin1(), find("key")->latin1(),
                  west, north, east, south);
  }
  qDebug("#  GeoImage::part %s (%f, %f, %f, %f)", (const char *) fname, west,
         north, east, south);
  QFile f(fname);
  if (f.exists())
    return fname;

  const void *data_p = data();        //get pointer to data
  ASSERT(data_p);
  if (type_ == UNKNOWN)
    return 0;
  int dx, dy, i, j, rx1, ry1, rx2, ry2;
  picBBox(west, north, east, south, rx1, ry2, rx2, ry1);
  dx = rx2 - rx1 + 1; if (dx>cols_) dx=cols_;
  dy = ry2 - ry1 + 1; if (dy>rows_) dy=rows_;
  if (dx < 0 || dy < 0) {
    qDebug("#  (ERROR) GeoImage::part: (dx=%d=%d-%d || dy=%d=%d-%d)", dx, rx2,
           rx1, dy, ry2, ry1);
    throw FatalError(QString().
                     sprintf("GeoImage::part: (dx=%d=%d-%d || dy=%d=%d-%d)",
                             dx, rx2, rx1, dy, ry2, ry1));
  }

  FILE *of = fopen(fname, "w");
  if (!of) {
    throw FatalError(QString().
                     sprintf
                     ("GeoImage::part Can't open file %s for writing!\n",
                      (const char *) fname));
  }

  switch (type_) {
  case PBM:{
      bit *bpi, *bpo, **dat_b_out = pbm_allocarray(dx, dy);
      bit **dat_b = (bit **) data_p;
      for (i = 0; i < dy; i++) {
        bpi = dat_b[i + ry1] + rx1;
        bpo = dat_b_out[i];
        for (j = 0; j < dx; j++) {
          *bpo = *bpi;
          bpo++;
          bpi++;
        }
      }
      pbm_writepbm(of, (bit **) dat_b_out, dx, dy, 0);
      pbm_freearray(dat_b_out, dy);
    }
    break;
  case PGM:{
      gray *gpi, *gpo, **dat_g_out = pgm_allocarray(dx, dy);
      gray **dat_g = (gray **) data_p;
      gray maxval = 0;
      for (i = 0; i < dy; i++) {
        gpi = dat_g[i + ry1] + rx1;
        gpo = dat_g_out[i];
        for (j = 0; j < dx; j++) {
          *gpo = *gpi;
          if (*gpi > maxval)
            maxval = *gpi;
          gpo++;
          gpi++;
        }
      }
      pgm_writepgm(of, (gray **) dat_g_out, dx, dy, maxval, 0);
      pgm_freearray(dat_g_out, dy);
    }
    break;
  case PPM:{
      pixel *ppi, *ppo, **dat_p_out = ppm_allocarray(dx, dy);
      pixel **dat_p = (pixel **) data_p;
      pixval maxval = 255;      //! should be calculated
      for (i = 0; i < dy; i++) {
        ppi = dat_p[i + ry1] + rx1;
        ppo = dat_p_out[i];
        for (j = 0; j < dx; j++) {
          *ppo = *ppi;
          ppo++;
          ppi++;
        }
      }
      ppm_writeppm(of, (pixel **) dat_p_out, dx, dy, maxval, 0);
      ppm_freearray(dat_p_out, dy);
    }
    break;
  default:
    pfm_geo_set(geoWest(),geoNorth(),geoEast(),geoSouth());
    pfm_writepfm_region_type(of, data_, cols_, rows_, minval_, maxval_,
                             rx1, ry1, rx2, ry2, type_);
    break;
  }
  fclose(of);
  return fname;
}

/** write the image to disk */
void GeoImage::write()
{
  QString *fname = find("file");
  ASSERT(fname);
  switch (type_) {
  case PBM:
  case PGM:
  case PPM:
    qDebug("GeoImage::write: Format %d not supported", type_);
    break;
  default:{
      FILE *of = fopen(*fname, "w");
      if (!of) {
        fprintf(stderr,
                "GeoImage::write: (ERROR) Can't open file %s for writing!\n",
                (const char *) fname);
        return;
      }
      pfm_geo_set(geoWest(),geoNorth(),geoEast(),geoSouth());
      pfm_writepfm_type(of, data_, cols_, rows_, 1, -1, type_);
      fclose(of);
      break;
    }
  }
}


float GeoImage::geo2picX(float x)
{
  return (x - geoWest_) / (geoEast_ - geoWest_) * cols_;
}

float GeoImage::geo2picY(float y)
{
  return rows_ / (geoSouth_ - geoNorth_) * (y - geoNorth_);
}

/** Convert picture coordinates to geo coordinates */
float GeoImage::pic2geoX(float x)
{
  return (geoEast_ - geoWest_) / cols_ * (x + 0.5) + geoWest_;
}

/** Convert picture coordinates to geo coordinates */
float GeoImage::pic2geoY(float y)
{
  return (y + 0.5) * (geoSouth_ - geoNorth_) / rows_ + geoNorth_;
}

float GeoImage::pixelSizeX()
{
  return (geoEast_ - geoWest_) / cols_;
}

float GeoImage::pixelSizeY()
{
  return (geoNorth_ - geoSouth_) / rows_;
}

/** Calculate the bounding box in pixeln from given bounding box in geocoordinates */
void
  GeoImage::picBBox(float gW, float gN, float gE, float gS, int &llx,
                    int &lly, int &urx, int &ury)
{
  llx = int (floor(geo2picX(gW) + 0.5));
  urx = int (floor(geo2picX(gE) - 0.5));
  lly = int (floor(geo2picY(gS) - 0.5));
  ury = int (floor(geo2picY(gN) + 0.5));
}

/** Calculate the bounding box in geocoordinates from given bounding box in pixeln */
void
  GeoImage::geoBBox(int llx, int lly, int urx, int ury, float &gW, float &gN,
                    float &gE, float &gS)
{
  gW = pic2geoX(llx - 0.5);
  gE = pic2geoX(urx + 0.5);
  gS = pic2geoY(lly + 0.5);
  gN = pic2geoY(ury - 0.5);
}


/** Generate a mask image (pbm)  */
QString GeoImage::mask(float west, float north, float east, float south,
                       int id, QString prefixDir, QString fname)
{

#define GenMaskBlockStorage(T) \
{ \
	T* iptr; \
	bit *optr; \
	int x,y; \
	for (y = 0; y < dy; y++) { \
	  if (y<-ry1 || y>=rows_-ry1) iptr=0; \
	  else iptr = ((T*)data_p)+(y + ry1)*cols_+rx1; \
	 	optr = dat_b_out[y]; \
	  for (x = 0; x < dx; x++) { \
	    if (!iptr || x<-rx1 || x>=cols_-rx1) *optr=0; \
	  	else *optr=(*iptr==(T)id?1:0); \
	   	optr ++; \
	    if (iptr) iptr ++; \
	  } \
	} \
}

#define GenMaskRowStorage(T) \
{ \
	T* iptr; \
	bit *optr; \
	int x,y; \
	for (y = 0; y < dy; y++) { \
	  if (y<-ry1 || y>=rows_-ry1) iptr=0; \
	  else iptr = (((T**)data_p)[y + ry1]) + rx1; \
	 	optr = dat_b_out[y]; \
	  for (x = 0; x < dx; x++) { \
	    if (!iptr || x<-rx1 || x>=cols_-rx1) *optr=0; \
	  	else *optr=(*iptr==(T)id?1:0); \
	   	optr ++; \
	    if (iptr) iptr ++; \
	  } \
	} \
}

  if (fname.isEmpty()) {        //create output filname
    ASSERT(find("key"));
    QString dir = CleanUp::mkdir(CleanUp::getTmpDirPID(), prefixDir);
    fname.sprintf("%s/%f_%f_%f_%f.pbm", dir.latin1(),
                  west, north, east, south);
  }
  qDebug("#  GeoImage::mask %s (%f, %f, %f, %f)", (const char *) fname,
         west, north, east, south);
  QFile f(fname);
  if (f.exists())
    return fname;

  const void *data_p = data();        //get pointer to data
  ASSERT(data_p);
  if (type_ == UNKNOWN)
    return 0;
  int dx, dy, rx1, ry1, rx2, ry2;
  picBBox(west, north, east, south, rx1, ry2, rx2, ry1);
  dx = rx2 - rx1 + 1;
  dy = ry2 - ry1 + 1;
  if (dx <= 0 || dy <= 0)
    qDebug("#  (ERROR) GeoImage::part: (dx=%d=%d-%d || dy=%d=%d-%d)", dx, rx2,
           rx1, dy, ry2, ry1);

  FILE *of = fopen(fname, "w");
  if (!of) {
    fprintf(stderr, "#  (ERROR) Can't open file %s for writing!\n",
            (const char *) fname);
    return "";
  }

  bit **dat_b_out = pbm_allocarray(dx, dy);
  switch (type_) {
  case PFM_FLOAT:
    GenMaskBlockStorage(float);
    break;
  case PFM_UINT:
    GenMaskBlockStorage(unsigned int);
    break;
  case PFM_SINT:
    GenMaskBlockStorage(signed int);
    break;
  case PFM_UINT16:
    GenMaskBlockStorage(unsigned short);
    break;
  case PFM_SINT16:
    GenMaskBlockStorage(signed short);
    break;
  case PBM:{
      bit *iptr;
      bit *optr;
      int x, y;
      for (y = 0; y < dy; y++) {
        iptr = (((bit **) data_p)[y + ry1]) + rx1;
        optr = dat_b_out[y];
        for (x = 0; x < dx; x++) {
          *optr = (*iptr == (bit) id ? 1 : 0);
          optr++;
          iptr++;
        }
      }
    }
//              GenMaskRowStorage(bit);
    break;
  case PFM_BYTE:
    GenMaskBlockStorage(unsigned char);
    break;
  default:
    pbm_freearray(dat_b_out, dy);
    fclose(of);
    return "";
    break;
  }
  pbm_writepbm(of, (bit **) dat_b_out, dx, dy, 0);
  pbm_freearray(dat_b_out, dy);

  fclose(of);
  return fname;
#undef GenMaskBlockStorage
#undef GenMaskRowStorage
}

/** returns a pointer to this image and increments the reference counter */
GeoImage *GeoImage::shallowCopy()
{
  ref();
  return this;
}

/** decrements the reference counter from this image and returns a NULL-pointer. If the 
reference counter is 0, this image is deleted. */
GeoImage *GeoImage::unlink()
{
  if (deref())
    delete this;
  return 0;
}

/** Merge the region with the id from labelimage img into this image with the newId, if the previous id in this image is compareId */
bool GeoImage::mergeInto(GeoImage & img, int compareId, int id, int newId)
{
  int llx, lly, urx, ury;
  int x, y;
  bool objectInserted = false;
#ifdef WIN32
#pragma message ("besser picBBox benutzen?")
#else
#warning besser picBBox benutzen?
#endif
  llx = int (geo2picX(img.pic2geoX(0)));
  lly = int (geo2picY(img.pic2geoY(img.rows())));
  urx = int (geo2picX(img.pic2geoX(img.cols())));
  ury = int (geo2picY(img.pic2geoY(0)));
  if (llx < 0)
    llx = 0;
  if (lly >= rows_)
    lly = rows_;
  if (urx >= cols_)
    urx = cols_;
  if (ury < 0)
    ury = 0;

  //! This loop should be optimized
  for (y = ury; y < lly; y++)
    for (x = llx; x < urx; x++) {
#if 0
      assert(x >= 0);
      assert(x < cols());
      assert(y >= 0);
      assert(y < rows());
#endif
      if (getId(x, y) != compareId)
        continue;
      float gx, gy;
      gx = pic2geoX(x);
      gy = pic2geoY(y);
      int nx, ny;
      nx = int (img.geo2picX(gx));
      ny = int (img.geo2picY(gy));
#if 0
      assert(nx >= 0);
      assert(nx < img.cols());
      assert(ny >= 0);
      assert(ny < img.rows());
#endif
      if (img.getId(nx, ny) == id) {
//                              qDebug("GeoImage::mergeInto 1->(%d,%d)",x,y);
        ((int *) data_)[y * cols_ + x] = newId;
        objectInserted = true;
      }
    }
  return objectInserted;
}

/** gets the id at the picture coordinatex x, y */
int GeoImage::getId(int x, int y)
{
  if (x < 0 || x >= cols_ || y < 0 || y >= rows_) {
    qDebug("GeoImage::getId() out of range (%d,%d) image size %dx%d", x, y,
           cols_, rows_);
    return -1;
  }
  assert(x >= 0);
  assert(x < cols_);
  assert(y >= 0);
  assert(y < rows_);
  const void* data_p=data();
  if (!data_p)
    return 0;
  switch (type_) {
  case PFM_SINT:{
      const signed int *p = (const signed int *) data_p;
      return (int) p[y * cols_ + x];
    }
  case PFM_SINT16:{
      const signed short int *p = (const signed short int *) data_p;
      return (int) p[y * cols_ + x];
    }
  case PFM_UINT16:{
      const unsigned short int *p = (const unsigned short int *) data_p;
      return (int) p[y * cols_ + x];
    }
  case PFM_BYTE:{
      const unsigned char *p = (const unsigned char *) data_p;
      return (int) p[y * cols_ + x];
    }
  case PFM_3BYTE:{
      const PFM3Byte *p = (const PFM3Byte *) data_p;
      unsigned char r = (p->r)[y * cols_ + x];
      unsigned char g = (p->g)[y * cols_ + x];
      unsigned char b = (p->b)[y * cols_ + x];
      return qRgb(r, g, b);
    }
  case PBM:{
      const bit *p = (const bit *) data_p;
      return (int) p[y * cols_ + x];
    }
  case PGM:{
      const gray *p = (const gray *) data_p;
      return (int) p[y * cols_ + x];
    }
  case PPM:{
      const pixel **p = (const pixel **) data_p;
      pixel v = p[y][x];
      return qRgb(PPM_GETR(v), PPM_GETG(v), PPM_GETB(v));
    }
  default:
    return 0;
  }
}

/** gets the id at the geo coordinatex gx, gy */
int GeoImage::getId(float gx, float gy)
{
  return getId(geo2picX(gx), geo2picY(gy));
}

