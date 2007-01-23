
#ifndef PFM_H
#define PFM_H

/**
  *@author Martin Pahl
  */

/*
 * <type>
 * # Comment (optional)
 * <byteorder> - (bin-Format only (F4-F6))
 * # Comment (optional)
 * <cols> <rows>
 * # Comment (optional)
 * <minval> <maxval> <factor> <offset> - factor and offset are optional
 * <data>
 *
 * type
 * ----
 * F0 - float ASCII
 * F1 - int ASCII
 * F2 - int16 ASCII
 * F3 - double ASCII
 * F4 - float binary
 * F5 - int binary
 * F6 - int16 binary
 * F7 - double binary
 * P1 - PBM ASCII
 * P2 - PGM ASCII
 * P3 - PPM ASCII
 * P4 - PBM binary
 * P5 - PGM binary
 * P6 - PPM binary
 *
 * byteorder
 * ---------
 * L - Little endian
 * B - Big endian
 *
 * if factor and offset are given, any data is multiplied by factor and
 * then offset is added.
 */


#include <stdio.h>
#include <math.h>

enum PFMType
{
  PFM_FLOAT_ASCII,
  PFM_INT_ASCII,
  PFM_INT16_ASCII,
  PFM_SINT16_ASCII,
  PFM_DOUBLE_ASCII,
  PFM_FLOAT_BIN,
  PFM_INT_BIN,
  PFM_INT16_BIN,
  PFM_SINT16_BIN,
  PFM_DOUBLE_BIN,
  PFM_PGM_ASCII,
  PFM_PBM_ASCII,
  PFM_PPM_ASCII,
  PFM_PGM_BIN,
  PFM_PBM_BIN,
  PFM_PPM_BIN
};

enum PFMEndian
{
  PFM_LittleEndian=0,
  PFM_BigEndian=1
};

#define PFMStorageTypeDefine \
  PFM_FLOAT=0,\
  PFM_SINT,\
  PFM_UINT,\
  PFM_SINT16,\
  PFM_UINT16,\
  PFM_BYTE,\
  PFM_BIT,\
  PFM_3BYTE,\
  PFM_DOUBLE,\
  PFM_LAST

enum PFMStorageType
{
  PFMStorageTypeDefine
};

typedef struct {
 unsigned char* r;
 unsigned char* g;
 unsigned char* b;
} PFM3Byte;

typedef float PFMConvert(float v, float min_val, float max_val);

#ifdef __cplusplus
extern "C" {
#endif

/** pfm_readpfm_type:
    Read an entire floatmap file into memory, returning the
    allocated array as pointer of type storageType (see above).
    Furthermore the function filling in the rows, cols, minval
    and maxval variables. The optional function convFunc convert
    the type of the input data to storageType */
void *pfm_readpfm_type(FILE *fp,
           int *cols,
           int *rows,
           float *minval,
           float *maxval,
           int storageType,
           PFMConvert *convFunc);
/** Read an entire floatmap file into memory, returning the
    allocated array as pointer to float and filling in the
    rows, cols, minval and maxval variables.
    */
float *pfm_readpfm(FILE *fp,
       int *cols, int *rows,
       float *minval, float *maxval);
/** Write the header (rows, cols, minval and maxval) and the specified
    part (x1, y1, x2, y2) of the data to the giver filepointer.
    The storageType must name the type of the data. Specified part can
    be outside the date. This data are filled up with minval.
*/
int pfm_writepfm_region_type(FILE *fp, const void *data,
           int cols, int rows,
           float minval, float maxval,
           int x1, int y1, int x2, int y2,
           int storageType);
/** Write the header (rows, cols, minval and maxval) and the specified
    part (x1, y1, x2, y2) of the floatmap to the giver filepointer.
    The storageType and data type must be float. Specified part can
    be outside the date. This data are filled up with minval.
*/
int pfm_writepfm_region(FILE *fp, const float *data,
      int cols, int rows,
      float minval, float maxval,
      int x1, int y1, int x2, int y2);
/** Write the header (rows, cols, minval and maxval) and all
    data. The storageType must name the type of the data.
*/
int pfm_writepfm_type(FILE *fp, const void *data,
          int cols, int rows,
          float minval, float maxval,
          int storageType);
/** Write the header (rows, cols, minval and maxval) and all
    data of the floatmap. The storageType and data type are float.
*/
int pfm_writepfm(FILE *fp, const float *data,
     int cols, int rows,
     float minval, float maxval);
/** This function filling in the rows and cols variables.
*/
int pfm_readpfm_size(FILE *fp,
         int *cols, int *rows);
/** This function read the header and filling in the rows, cols, minval
    and maxval variables.
*/
int pfm_readpfm_header(FILE *fp,
           int *cols, int *rows,
           float *minval, float *maxval,
           int *storageType);

void pfm_geo_set(double geoWest, double geoNorth, double geoEast, double geoSouth);
int pfm_geo_get(double* geoWest, double* geoNorth, double* geoEast, double* geoSouth);
void pfm_comment_set(const char* comment);
const char* pfm_comment_get();
#ifdef __cplusplus
     }
#endif
#endif
