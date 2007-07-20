/***************************************************************************
                          gnode.cpp  -  description
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

#include "gnode.h"
#ifdef WITH_GUI_SUPPORT 
#include "../semnetgui/treeitem.h"
#endif


// #define DEBUGMSG

const static QString nullString("");

static QString keywords[] = { "NODE", "REGION", "GROUP", "" };
const MLTagTable nodeTagTable(keywords);

GNode::GNode()
{
  init();
#ifdef WIN32
  qDebug ("GNode.parent_ = %x", parent_);
//  parent_ = &node;
#endif

}

GNode::~GNode()
{
  QListIterator < GNode > it = QListIterator < GNode > (children());
  for (; it.current(); ++it) {
    delete(it.current());
  }
#if defined WITH_GUI_SUPPORT || WIN32
  if (guiPtr_) delete guiPtr_;
#endif
}

/** Copy constructor */
GNode::GNode(GNode & node)
{
  init();
#ifdef WIN32
  qDebug ("GNode.parent_ = %x", &node);
  parent_ = &node;
#endif
  sections_ = node.sections_;
  {
    QDictIterator < QString > it(node.attribList_);
    while (it.current()) {
      attribList_.replace(it.currentKey(), new QString(*it.current()));
      ++it;
    }
  }
  name_ = node.name_;
  {
    QListIterator < GNode > it = QListIterator < GNode > (node.children());
    for (; it.current(); ++it) {
      childLink((it.current())->copy());
    }
  }
}

/** Construct a GNode by parsing a file */
GNode::GNode(MLParser & parser)
{
  init();
  read(parser);
}

/** Init member variables */
void GNode::init()
{
#ifdef WIN32
  name_ = QString("");    
  guiPtr_ = NULL;
#else
  name_ = ""; 
  guiPtr_ = 0;
#endif
  parent_ = 0;
//  qDebug ("GNode generiert");
  sections_ += "generic";
  showDetails_ = true;
}

/** Read GNode by parsing a file */
void GNode::read(MLParser & parser)
{
//Gerhards
	//error C2660: 'args' : Funktion akzeptiert keine 2 Parameter
//! MP In der neuesten Version akzeptiert die Funktion sehr wohl 2 Parameter.
//  Bitte qtguiapplication neu auschecken!!!
//Wo finde ich die???
#ifdef WIN32
  parser.args(&attribList_);
#else
  parser.args(&attribList_,false);
#endif
  setVars();
#ifdef DEBUGMSG
//  qDebug("Node: name=%s\n", (const char *) name_);
#endif
  int tag = parser.tag(nodeTagTable);
  while (tag == TOK_NODE) {
    GNode *node = this->newNode();
    node->read(parser);
    childLink(node);
    tag = parser.tag(nodeTagTable);
  }
  if (tag != -TOK_NODE) {
    qDebug("Warning: /node missing\n");
  }
}

/** Write the node-Date to a file */
void GNode::write(QTextStream & fp, QString indent, bool recursive)
{
  fp << indent << "<node ";
#ifdef WIN32
	QDictIterator<QString> it=QDictIterator<QString>(attribList_);
  	for (;it.current(); ++it)
   {	 fp << it.currentKey() << "=\"" << *(it.current()) << "\" ";
  	}
#else
  fp << attribList_;
#endif
  if (children().count() == 0 || !recursive) {
    fp << "/>" << endl;
  }
  else {
    fp << ">" << endl;
    QListIterator < GNode > it = QListIterator < GNode > (children());
    for (; it.current(); ++it) {
      it.current()->write(fp, indent + "  ");
    }
    fp << indent << "</node>" << endl;
  }
}

/** Set the name_ */
void GNode::name(QString name)
{
  attributeRemove("name");
  attribute("name", name);
}

/** Set the attribute key to val */
void GNode::attribute(QString key, QString val)
{
  attributeSet(key, val);
}

/** returns the name of this node */
const QString & GNode::name() const
{
  return name_;
}

/** Set the attribute key to val */
void GNode::attribute(QString key, int val)
{
  attributeSet(key, QString().setNum(val));
}

/** Set the attribute key to val */
void GNode::attribute(QString key, double val)
{
  attributeSet(key, QString().sprintf("%f", val));
}

/** Set the attribute key to val */
void GNode::attribute(QString key, const char* val)
{
  attributeSet(key, QString(val));
}

/** Get the value of attribute key */
const QString & GNode::attribute(QString key)
{
  QString *s = attribList_[key];
  if (s)
    return *s;
  else
    return nullString;
}

/** Get the value of attribute key as float*/
const float GNode::attributeFloat(QString key)
{
#ifdef DEBUGMSG
  qDebug("GNode::attributeFloat(%s)=%f (%s)",
         key.latin1(), attribute(key).toFloat(), attribute(key).latin1());
#endif
  return attribute(key).toFloat();
}

/** Get the value of attribute key as int*/
const int GNode::attributeInt(QString key)
{
  return attribute(key).toInt();
}

/** Get the value of attribute key as bool */
const bool GNode::attributeBool(QString key)
{
  bool val = false;
  MLParser::setBool(val, &attribList_, key);
  return val;
}

/** Remove the attribute key */
bool GNode::attributeRemove(QString key)
{
  return attribList_.remove(key);
}

/** Get the attribute list */
AttribList & GNode::attribList()
{
  return attribList_;
}

/**  Makes a copy of this node */
GNode *GNode::copy()
{
  GNode *gn = new GNode(*this);
#ifdef WIN32
  if (gn == 0){
    cout << "Out of Memory..7";
    exit(1);
  }
#endif
  return gn;
}

/** return a new Node */
GNode *GNode::newNode()
{
  GNode *gn = new GNode();
#ifdef WIN32
  if (gn == 0){
    cout << "Out of Memory..8";
    exit(1);
  }
#endif
  return gn;
}

/** Set the attribute key to val */
void GNode::attributeSet(QString key, QString val)
{
  attribList_.replace(key, new QString(val));
  if (key == "name")
    name_ = val;
}

/** Set variables of this node using the attributes */
void GNode::setVars()
{
  MLParser::setString(name_, &attribList_, "name");
}

/** returns true if this class is a cname */
bool GNode::isA(QString cname)
{
  return (cname == "GNode");
}

/** Returns the attribute sections of this node */
const QStringList & GNode::attributeSections()
{
  return sections_;
}

/** Get the attribute description for the specified section */
QDict < Attribute > *GNode::attributeDesc(QString section)
{
  return 0;
}

/** Configure this node using attribList    */
void GNode::configure(AttribList & attribList)
{
  attribList_.clear();
  {
    QDictIterator < QString > it = QDictIterator < QString > (attribList);
    for (; it.current(); ++it) {
      attributeSet(it.currentKey(), *(it.current()));
    }
  }


}

/** Update attributes of this node with the given attribute list  */
void GNode::update(AttribList & attribList)
{
  QDictIterator < QString > it = QDictIterator < QString > (attribList);
  for (; it.current(); ++it) {
    attributeSet(it.currentKey(), *(it.current()));
  }
}

/** return true if detail should be shown */
bool GNode::showDetails()
{
  return showDetails_;
}

/** Set the ShowDetail state */
void GNode::setShowDetails(bool state)
{
  showDetails_ = state;
}

void GNode::setGuiPtr(TreeItem * ptr)
{
  guiPtr_ = ptr;
}

