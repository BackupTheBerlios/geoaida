/***************************************************************************
                          nodelist.h  -  description
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


#ifndef NODE_LIST_H
#define NODE_LIST_H

#include <qstring.h>
#include <qstringlist.h>
#include <qpoint.h>
#include <qdict.h>
#include "gasensor.h"
#include "garelation.h"
#include "qfile.h"
#include "node.h"
#include "gaimage.h"
#include "stack.h"

#ifdef WIN32
#include <qlist.h> 
#define  QPtrList	QList
#define  QPtrListIterator QListIterator
#endif




/**class to handel a list of image informations
  *@author Jürgen Bückner
  */

class NodeList  : public QDict<Node> {
public: 
  /** default constructor */
	NodeList();

  /** Copy constructor */
	//  NodeList(const NodeList& nl);
  NodeList(const NodeList& nl, bool deep=true);
  void copy(const NodeList& nl, bool deep);
  NodeList& operator=(const NodeList& nl);

  /** constructor resd from the provided filename*/
	NodeList(QString filename);

  /** denstructor */
	~NodeList();

  /** read a list of Node and the attributes through parser */
  void read(MLParser& parser);

  /** read a list of Node and the attributes from the provided filename */
  void read(QString filename);

  /** read a list of Node and the attributes from the provided filepointer */
  void read(QIODevice& fp);

  /** print info  */
  void info(void);

  /** return a list of included images*/
  QStringList& list(void) {
    return list_;
  };

  /** set nodelist attribute (group attribute) */
  void set(QString name, QString val);
  /** get nodelist attribute (group attribute) */
  QString get(QString name);

	/** operator +=  for NodeList*/
  NodeList &operator += (NodeList& nl);

  /** return void pointer to the Node class */
  Node* node(QString imgKey);
  /** return void pointer to the image data */
  void* data(QString imgKey);
  /** return the cols numbers of the image */
  int cols(QString imgKey);
  /** return the rows numbers of the image */
  int rows(QString imgKey);
  /** return geoNorth value of the image*/
	float geoNorth(QString imgKey);
  /** return geoSouth value of the image*/
	float geoSouth(QString imgKey);
  /** return geoEast value of the image*/
	float geoEast(QString imgKey);
  /** return geoWest  valueof the image*/
	float geoWest(QString imgKey);
  /** return Info the image*/
	void info(QString imgKey);
  /** Returns the number of items in the list.*/
	int size() {return list_.count();};
	/** Overloading insert function*/
	void insert(QString key, Node* node);
	/** calculate new geo coordinates and resolution using all images belong to included nodes*/
	void calcNewGEOValues(float &gn,float &gs,float &gw,float &ge,int &sizeX,int &sizeY,float &xRes,float &yRes);
#if 0	
  /** write a scrap of the data to a file*/
	QString part(QString imgKey, float north, float south, float west, float east, QString fname="");
#endif	
  /** return a list of pointer to the objects of specified class type */
  NodeList* selectClass (QString classname);
  /** calculate for all classes 'classname' the term 'term' with the function 'fkt'*/
  void calcForSelect (QString classname, QString term, int (*fkt)(void));
  /** calculate for all classes 'classname' the term 'term' with the function 'fkt'*/
  void calcForSelect (QString classname, QString term, QString (*fkt)(void));
  /** calculate for all classes 'classname' the term 'term' using the class 'GASensor'*/
	void calcForSelect (QString classname, QString term, GASensor* sensor);
  /** calculate for all classes 'classname' the term 'term' using the class 'GASensor'*/
	void calcForSelect (QString term, GASensor* sensor);
  /** calculate for all classes 'classname' the relation with the function 'fkt'*/
	void calcRelation (Node* rnode, Node* lnode, GARelation* relation);
  /** merge for all images in list (scale to a common resulution)
  * write result to file 'fname'
    flag 'newReg'
      true - generate new nodes when regions are split into subregions
      false - one node can associate to more than on region
  */
  //NodeList* merge (QString fname, bool newReg = 0);
  NodeList* merge (bool newReg = false, QString outImgName="");
  /** write a nodelist to the given textstream */
  void write(QTextStream &fp, QString groupFileName="");
  /** write a nodelist as a regionsfile */
  void writeRegionFile(QTextStream &fp);
  /** write the resulting label image */
  void writeOutImage(QString filename);
  /** write the resulting group label image */
  void writeGroupImage(QString filename);
  /** copy the nodes in this nodelist into the group image */
  void genGroupImage();
  Ga::Image& readLabelFile(QString filename, double gW, double gN, double gE, double gS);

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
  /** Removes the top item from the local stack and deletes it if autoDelete() is TRUE.
      Returns TRUE if there was an item to pop; otherwise returns FALSE.  */
  bool stackRemove (void) ;
  /** Removes all items from the local stack, deleting them if autoDelete() is TRUE.  */
  void stackClear(void);
  /** set the out-image-file-name in all nodes of NODELIST */
  void setImgName(QString);
  /** update the internal variables using the dictionary */


protected: // Protected attributes
  QStringList list_;
  /** dictionary of the labelimages */
  static QDict<Ga::Image> imageDict_;
  float gW_, gN_, gE_, gS_;
  float xRes_, yRes_;
  int sizeX_, sizeY_;
  static Ga::Image outImage_;
  static Ga::Image groupImage_;
  static int maxID_;
  static int groupIdCounter_;
  int groupId_;
  /** stack for generic bottom-up operator */
  Stack stack_;
  ArgDict attribList_;

class SortEle {//help class to sort nortlist; needed by help
  public:
    SortEle(Node* n, float p, QString key = "") {
      node_=n;
			p_ = p;
			key_ = key;
    }
	  bool operator == (SortEle& lval){return (p_==lval.p());}
	  bool operator < (SortEle& lval){return (p_<lval.p());}
    float p(){return p_;}
    void p(float p){p_ = p;}
    void setGeoPos(int lx, int ly, int ux, int uy) {
      llx=lx; lly=ly; urx=ux; ury=uy;}
    QString key(){return key_;}
    void key(QString k){key_ = k;}
    Node* node(){return node_;}
    QString key_;
    float p_;
    Node* node_;
    int llx, lly, urx, ury;
  };

class SortPtrList : public QPtrList<SortEle> { //sort list of 'SortEle' objects
  public:
	SortPtrList() : QPtrList<SortEle> () {};
	~SortPtrList() {};
	
	virtual int compareItems(Item il, Item ir) {
		SortEle *lval = (SortEle*)il;
		SortEle *rval = (SortEle*)ir;
		if (lval->p() == rval->p()) return 0;
		if (lval->p()<rval->p()) return -1;
		return 1;
	}
};



};

#endif
