/***************************************************************************
                          gnode.h  -  description
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

#ifndef GNODE_H
#define GNODE_H

#include "MLParser.h"
#include "treegnode.h"
#include "treenode.h"
#include <qstringlist.h>
#include <qlist.h>
#include <qdict.h>
#include <stdlib.h>

/**Base Node
  *@author Martin Pahl
  */

extern const MLTagTable nodeTagTable;

typedef ArgDict AttribList;

class OperatorObject;
class TreeItem;

class GNode:public TreeNode < GNode, TreeGNode >
{
public:                        // Public attributes
  GNode();
  /** Copy constructor */
  GNode(GNode & node);
  /** Construct a GNode by parsing a file */
  GNode(MLParser & parser, GNode *classNode);
  virtual ~ GNode();
  /** Read a GNode by parsing a file */
  void read(MLParser & parser,int depth=0, bool isClassTree=false);
  /** Write the node-Date to a file */
  void write(QTextStream & fp, QString indent = "", bool recursive = true);
  /** returns the name of this node */
  virtual const QString & name() const;
  /** returns the name of this node */
  virtual const QString & className() const;
  /** Set the name_ */
  void name(QString name);
  /** Get the value of attribute key */
  const QString & attribute(QString key);
  /** Get the value of attribute key as float*/
  const float attributeFloat(QString key);
  /** Get the value of attribute key as int*/
  const int attributeInt(QString key);
  /** Get the value of attribute key as bool*/
  const bool attributeBool(QString key);
  /** Set the attribute key to val */
  void attribute(QString key, const char* val);
  /** Set the attribute key to val */
  void attribute(QString key, double val);
  /** Set the attribute key to val */
  void attribute(QString key, int val);
  /** Set the attribute key to val */
  void attribute(QString key, QString val);
  /** Remove the attribute key */
  bool attributeRemove(QString key);
  /** Get the attribute list */
  AttribList & attribList();
  /** Set variables of this node using the attributes */
  virtual void GNode::setVars();
  /**  Makes a copy of this node */
  virtual GNode *copy();
  /** return a new Node */
  virtual GNode *newNode();
  /** returns true if this class is a cname */
  virtual bool isA(QString cname);
  /** Set the ShowDetail state */
  void setShowDetails(bool state);
  /** return true if detail should be shown */
  bool showDetails();
  virtual void setGuiPtr(TreeItem * ptr);
  int id();
  int idStart();
  int idEnd();
  uint color();
  /** return the depth of this node */
  int depth();
  /** Find a gnode with the given classname. Only direct children of this gnode are checked. */
  GNode* findClass(QString classname);
  GNode* classNode();
  GNode* classNode(GNode* node);
protected:                     // Protected methods
  /** Init member variables */
  virtual void init();
  /** Set the attribute key to val */
  virtual void attributeSet(QString key, QString val);
protected:                     // Protected attributes
  /** name of the node */
  QString name_;
  /** classname of the node */
  QString className_;
  /** list of attributes */
  AttribList attribList_;
  bool showDetails_;
  /** Pointer to gui */
  TreeItem *guiPtr_;
  int id_;
  int idStart_;
  int idEnd_;
  /**  */
  GNode* classNode_;
  uint color_;
  /** depth of this node in the tree */
  int depth_;;
  static QDict<GNode> classDict_;
public:                        // Public attributes
  enum
  {
    TOK_NODE = 1,
    TOK_REGION,
    TOK_GROUP
  };
};

inline uint GNode::color()
{
  return color_;
}

inline int GNode::idStart()
{
  return idStart_;
}

inline int GNode::idEnd()
{
  return idEnd_;
}

inline int GNode::id()
{
  return id_;
}

/** return the depth of this node */
inline int GNode::depth()
{
  return depth_;
}

inline GNode* GNode::classNode()
{
  return classNode_;
}

inline GNode* GNode::classNode(GNode* node)
{
  classNode_=node;
  return classNode_;
}


#endif
