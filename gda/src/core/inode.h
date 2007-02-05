/***************************************************************************
                          inode.h  -  description
                             -------------------
    begin                : Thu Sep 28 2000
    copyright            : (C) 2000 by Martin Pahl
    email                : pahl@tnt.uni-hannover.de
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

/*
 * $Source: /data/cvs/gda/gda/core/inode.h,v $
 * $Revision: 1.32 $
 * $Date: 2002/05/13 10:17:42 $
 * $Author: gerhards $
 * $Locker:  $
 */

#ifndef INODE_H
#define INODE_H

#include "geoimage.h"
#include "gnode.h"
#include "snode.h"
#include "analysis.h"

enum IType
{ TRASH, MI, IH, CI, PI, HI, TD_ERROR, BU_ERROR, TD_ABORTED, BU_ABORTED };  //analysis status of the instance INode
enum EState
{ TD, BU };                     //exec. status TopDown BottomUp

//class SNode;
class Analysis;

class INode:public TreeNode < INode, GNode >
{
public:
  static const char *const iTypeName[];
  INode(SNode * node);
  /** Copy constructor */
  INode(INode & node);
  /** Construct an iNode from reading a file with given parser. */
  INode(MLParser & parser);
  virtual ~INode();
  /** initialize inode */
  virtual void init();
  /** Returns a new INode */
  virtual GNode *newNode();
  /** Make a copy of this INode */
  virtual GNode *copy();
  /** returns true if this class is a cname */
  virtual bool isA(QString cname);
  /** Link a sNode to this iNode. */
  bool linkSNode(SNode * parent);
  /** This method is called, when a task is finished for this inode */
  void taskFinished(int pid, int status);
  /** Sets the SNode this INode is type of */
  void sNode(SNode * node);
  /** returns the corresponding SNode */
  SNode *sNode();
  /** Sets the filename for operator output    */
  void output(QString val);
  /** Gets the filename for operator output    */
    QString & output();
  /** set the analysis status of the INode {TRASH, MI, IH, CI, PI, HI}*/
  void status(IType st)
  {
    status_ = st;
    attribute("status", iTypeName[st]);
  };
  /** return the analysis status of the INode {TRASH, MI, IH, CI, PI, HI} */
  IType status() const
  {
    return status_;
  };

  /** Sets the pointer to the analyze class */
  static void analysis(Analysis * an)
  {
    analysis_ = an;
  };
  /** Get the pointer to the analyze class */
  static Analysis *analysis(void)
  {
    return analysis_;
  };

  /** start the analyze at the root node*/
  void startAnalyze()
  {
    childTopDown();
  };
  /** set the exec. status of the instace {TopDown BottomUp} */
  void execState(EState es)
  {
    execState_ = es;
  };
  /** return the exec. status of the instace {TopDown BottomUp} */
  EState execState(void)
  {
    return execState_;
  };
  /** call the TopDown Operator of the SNode  */
  void execTopDown();
  /** Execute the bottomUp operator */
  void execBottomUp();
  /** decrement the aktiv counter * aktiv TopDown child operators
    * -1 -> ERROR (aktivcount < 0)
    *  0 -> aktivcount == 0
    *  1 -> aktivcount > 0 */
  int decrementCount();
  /** increment the aktiv counter  */
  void incrementCount(int c = 1);
  /** Sets the label image or returns the label image if image=0 */
  GeoImage *labelImage(GeoImage * image = 0);
  /** Sets up the region description for this inode using the available information e.g. labelimage */
  void setGeoRegion(float geoWest = 0.0, float geoNorth = 0.0, float geoEast =
                    0.0, float geoSouth = 0.0, INode* parent=0);
  /** for preparing the result label image every inode gets a new id */
  int setNewID(int id);
  /** merge inodes labelimage into the result image */
  void mergeResultImage(GeoImage & resultImg);
  /** returns the path to this inode */
  QString path();
  /** set flag to handle min and max restriction of snode definition */
  void truncation(bool);
  /** get flag to handle min and max restriction of snode definition */
  bool truncation(void);
protected:                     // Protected attributes
  /** handle the results of the BottomUp operator
    * pid ==  0 -> no operator or node have no holistic operator
    * pid == -1 -> can't start process
    * return 0 - delete 'this'
    * remember: a node without SNode is a "Trash-Node"
    * this node must have children
    */
  int evalBottomUp(int pid);
        /** handle the results of the TopDown operator
    * pid ==  0 -> no operator or node have no holistic operator
    * pid == -1 -> can't start process
    */
  int evalTopDown(int pid);
  /** call the TopDown operator for all childs (of this)*/
  bool childTopDown(bool first = TRUE);
  /** return TRUE if the Node is the last Node (leaf node) or the last node to analyze otherwise return FALSE  */
  bool isLast();
  /** return TRUE if the Node is the root node of the net otherwise return FALSE  */
  bool isRoot();


  /** pointer to the SNode this INode is type of */
  SNode *sNode_;
  /** Holds the filename for operator output */
  QString output_;
  /**analysis status of the instace {TRASH, MI, IH, CI, PI, HI}*/
  IType status_;
  /**exec. status of the instace {TopDown BottomUp}*/
  EState execState_;
  /** counter for the aktive childs (TopDown)*/
  unsigned int aktivcount_;
  /** label image showing the position of this instance node */
  GeoImage *labelImage_;
  /** pointer to the analyze */
  static Analysis *analysis_;
  /** anzahl der nodes die bei td noch ausgefüht werden müssen. */
  int childcount_;
  /** anzahl der nodes die bei td für die aktuelle priorität noch ausgefüht werden müssen. */
  int ordercount_;
  /** Memo für die Priorität, wird von 0 ab hochgezählt */
  int aktivorder_;
  /** flag to handle min and max restriction of snode definition */
  bool truncation_;
  /** childlist of 'this' used in childTopDown */
  QList < SNode > *childList_;

};

#endif
