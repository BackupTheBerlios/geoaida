/***************************************************************************
                          snode.h  -  description
                             -------------------
    begin                : Wed Jul 12 2000
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

#ifndef SNODE_H
#define SNODE_H

#ifdef WIN32
//#define	_isnan(v)	isnan(v)
#include <stdio.h>
#endif

#include "MLParser.h"
#include "treenode.h"
#include "gnode.h"
#include <qstringlist.h>
#include <qlist.h>
#include <qobject.h>

/**Base Node
  *@author Martin Pahl
  */

//extern const MLTagTable nodeTagTable;
const int TOK_NODE = 1;

typedef ArgDict AttribList;

class Operator;
class INode;
/** Signal is emitted whenever tdCount_ or buCount_ changes */
void stateChanged(TreeItem *);

class SNode:public TreeNode < SNode, GNode >
{
public:                        // Public attributes
  SNode();
  /** Copy constructor */
  SNode(SNode & node);
  /** Construct a SNode by parsing a file */
  SNode(MLParser & parser);
  virtual ~ SNode();
  /** Get max amount of SNode */
  int maxNumNode(void);
  /** Get min amount of SNodes */
  int minNumNode(void);
  /** Get order of SNodes */
  int order(void);
  /** Set minimum amount of SNodes */
  //void min(int val);
  /** Set order of SNodes */
  //void order(int val);
  /** Set minimum amount of SNodes */
  //void min(int val);
  /** Set this node to be a compound or a generalization node. */
  void isCompound(bool val);
  /** This node is a compound of its subnodes. */
  bool isCompound();
  /** returns a new node of type SNode */
  virtual GNode *newNode();
  /** Makes a copy of this node */
  virtual GNode *copy();
  /** Sets the operator call for the analysis */
  static void synchron(bool b)
  {
    synchron_ = b;
  };
  /** Get the operator call for the analysis */
  static bool synchron(void)
  {
    return synchron_;
  };
  /** set node variables using the attribute list */
  virtual void setVars();
  /** Execute the given operator. This function spawns a new process
    and returns. When the task is finish iNode->taskFinished() is called.
  */
  void execOperator(Operator * op, INode * iNode, AttribList & attribs,
                    QString path, int &counter);
  /** Execute the topDown operator. This function spawns a new process
      and returns. When the topDown task is finish iNode->taskFinished() is called.
    */
  void execTopDownOp(INode * iNode);
  /** Execute the bottomUp operator. This function spawns a new process
      and returns. When the bottomUp task is finish iNode->taskFinished() is called.
    */
  void execBottomUpOp(INode * iNode);
#if 0
  virtual QStringList & predefinedAttribs();
#endif
  /** Returns true if  this class is a cname */
  virtual bool isA(QString cname);
  /** Get the attribute description for the specified section */
  virtual QDict < Attribute > *attributeDesc(QString section);
  /** Evaluates the result of TopDownOperator    */
  QList<INode> evalTopDown(INode * inode);
  /** Evaluate the result of the bottom-up-operator   */
  QList<INode> &evalBottomUp(INode * iNode);
  /** says whether the node is hollistisch */
  bool holistic(void);
  /** No descriptions */
  uint color();
  /** find a snode with the given classname. Only direct children of this snode are checked. */
  SNode *findClass(QString classname);
  /** No descriptions */
  int tdCounter()
  {
    return tdCounter_;
  }
  int buCounter()
  {
    return buCounter_;
  }
  /** change an operator and change the corresponding attributes */
  void changeOperator(Operator* &op, QString opName);
  /** increase the reference counter */
  void link();
  /** decrease the reference counter */
  void unlink();
  virtual void setGuiPtr(TreeItem * ptr);
protected:                     // Protected methods
  /** Init member variables */
  virtual void init();
  /** Set the attribute key to val */
  virtual void attributeSet(QString key, QString val);
protected:                     // Protected attributes
  Operator * topDown_;
  Operator *bottomUp_;
  Operator *evaluation_;
  /** maximal number of objects */
  //int max_;
  /** minimal number of objects */
  //int min_;
  /** Node is a compound of  its subnodes. 
      If isCompound is false, this node is generalization
      of its subnodes.
  */
  bool isCompound_;
  /** Dictionary of the general attribute of this node. These are the attributes not
			provided by the operators, e.g. name, class of this node.
	*/
  QDict < Attribute > genericAttributes_;
  /** set TRUE for synchron analyze working otherwise FALSE (default: FALSE) */
  static bool synchron_;
  /** counter of running td operators */
  int tdCounter_;
  /** counter of running bu operators */
  int buCounter_;
  uint color_;
  /** Counter to count the inodes linked to this snode */
  int refCounter_;
  /** regelt die reihenfolge des td aufrufes, prioritätenregelung des aufrufes, beginnend mit 0, 1 ,.... */
  //int order_;
#if 0
public:                        // Public attributes
  /** Predefined Attributes */
  static QStringList predefinedAttribs_;
#endif
};


#endif
