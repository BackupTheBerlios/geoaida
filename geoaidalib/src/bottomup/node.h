/***************************************************************************
                          node.h  -  description
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


#ifndef NODE_H
#define NODE_H

#include "gaimage.h"
#include <qstring.h>
#include <qpoint.h>
#include <qfile.h>
#include "MLParser.h"
#if 0
extern "C" {
#include <pbm.h>
#include <pgm.h>
#include <ppm.h>
#include <pnm.h>
#include "pfm.h"
}
#endif

#include <qdatastream.h>
#ifdef WIN32
#include "stack.h"
#else
#include <stack.h>
#endif

/**class to handel the infos for one image
  *@author Jürgen Bückner
  */
class NodeList;
class Ga::Image;

class Node : public ArgDict {
public: 
  /** default constructor */
	Node();

  /** constructor read attributes for this Node through parser*/
	Node(MLParser& parser);

  /** denstructor */
	~Node();

protected: // Protected attributes
	enum IMGTYPE {
    PFM_FLOAT=0,
    PFM_SINT,
    PFM_UINT,
    PFM_SINT16,
    PFM_UINT16,
		PBM,
		PGM,
		PPM,	
		UNKNOWN
	};
	
public:
	/** init routine */
	void init();

  /** read attributes for this Node through parser */
  void read(MLParser& parser);

  /** load image info - not the data */
	void load(NodeList& nodeList);

  /** write data to file */
	void write(QTextStream& fp, QString keyword="node");

  /** return the image filename */
  QString filename();
  /** set the image filename */
  void filename(QString);

  /** return data */
	Ga::Image* data();
#if 0	
  /** write a scrap of the data.
  * return the filename.
  * if the file exist do nothing .
  * argument fname is optional .
  * the coordinates of the image part are geodata e.g. Gauss Krueger */
	QString part(float north, float south, float west, float east, QString fname = "");
#endif	
  //** set value in node */
  void setValue(QString key, QString val);
  //** get value from node */
  QString* getValue(const QString key);
  /** return data label id */
	int id() {return id_;}
  /** set label id */
	void id(int l);
  /** set label weighing */
	void p(float f);
  /** get label weighing */
	float p(void) {return valuation_;}
#if 0	
  /** return data typ */
	int dataTyp() {return type_;}	
  /** set data typ */
	void dataTyp(int t);
#endif	
  /** return cols */
	int sizeX() {return cols_;}
  /** return rows */
	int sizeY() {return rows_;}
  /** return cols */
	int cols() {return cols_;}
  /** return rows */
	int rows() {return rows_;}
  /** return  geoNorth*/
	float geoNorth() {return geoNorth_;}
  /** return  geoSouth*/
	float geoSouth() {return geoSouth_;}
  /** return  geoEast*/
	float geoEast() {return geoEast_;}
  /** return  geoWest*/
	float geoWest() {return geoWest_;}
  /** return lower left x-value*/
	int llx(int v=-1) {if (v==-1) return llx_; else llx_=v; return v;}
  /** return lower left y-value*/
	int lly(int v=-1) {if (v==-1) return lly_; else lly_=v; return v;}
  /** return upper right x-value*/
	int urx(int v=-1) {if (v==-1) return urx_; else urx_=v; return v;}
  /** return upper right y-value*/
	int ury(int v=-1) {if (v==-1) return ury_; else ury_=v; return v;}
  /** return x-resolution of the image*/
	float xResolution();
  /** return y-resolution of the image*/
	float yResolution();
  /** set class name*/
	void classname(QString str);
  /** return class name*/
	QString classname() {return class_;}
  /** return identifier name*/
	QString name() {return name_;}
  /** return node address*/
	QString addr() {return addr_;}
  /** return node address*/
	QString key() {return key_;}
  /** return info about this node*/
	void info() {
 		QDictIterator<QString> it(*this);
		for (;it.current(); ++it)
		  if(it.currentKey())
		  	printf("%s=\"%s\" ",it.currentKey().latin1(),(it.current())->latin1() );
		  else qDebug("(WARNING) Node::info - currentKey() is unreadably");
	};
  /** return stack - for bottom-up */
  Stack& stack(void);
  /** Removes the top item from the local stack and returns it. */
  StackElem* stackPop();
  /** Adds an element d to the top of the local stack. */
  void stackPush(const StackElem* d);
  /** Returns the number of items in the local stack.  */
  uint stackCount(void);
  /** Returns TRUE is the local stack contains no elements to be popped; otherwise returns FALSE.  */
  bool stackIsEmpty ();
  /** Removes the top item from the local stack and deletes it if autoDelete() is TRUE. Returns TRUE if there was an item to pop;
otherwise returns FALSE.  */
  bool stackRemove (void) ;
  /** Removes all items from the local stack, deleting them if autoDelete() is TRUE.  */
  void stackClear(void);
  /** update the internal variables using the dictionary */
  void update();

	//Operatoren:
  /** return class name*/
	//friend ostream& operator<< (ostream& os, Node& node);

protected: // Protected attributes
	QString filename_, class_, name_, addr_, key_;
	Ga::Image* data_;
	int cols_, rows_, id_, llx_, lly_, urx_, ury_;
	float geoNorth_, geoSouth_, geoEast_, geoWest_, valuation_;
	float minval_, maxval_;
 	
private:	 // Private attributes
  /** test the consistent of the image date */
  bool testSize(int cols, int rows, IMGTYPE type);
  /** geo position to image position */
  int geo2pic(float x, float y, int *rx, int *ry);
  /** stack for bottom-up operator */
  Stack stack_;

};

#endif

