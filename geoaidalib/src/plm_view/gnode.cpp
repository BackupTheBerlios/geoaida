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

#include <qcolor.h>
#include <qregexp.h>
#include "gnode.h"
#ifdef WITH_GUI_SUPPORT
#include "treeitem.h"
#endif

#ifdef DEBUG
 #define DEBUG_MSG
#endif

const static QString nullString("");

static QString keywords[] = { "NODE", "REGION", "GROUP", "" };
const MLTagTable nodeTagTable(keywords);

GNode::GNode()
{
  init();
}

GNode::~GNode()
{
  QListIterator < GNode > it = QListIterator < GNode > (children());
  for (; it.current(); ++it) {
    delete(it.current());
  }
#ifdef WITH_GUI_SUPPORT
  if (guiPtr_) delete guiPtr_;
#endif
}

/** Copy constructor */
GNode::GNode(GNode & node)
{
  init();
  {
    QDictIterator < QString > it(node.attribList_);
    while (it.current()) {
      attribList_.replace(it.currentKey(), new QString(*it.current()));
      ++it;
    }
  }
  name_ = node.name_;
  id_=node.id_;
  idStart_=node.idStart_;
  idEnd_=node.idEnd_;
  color_=node.color_;
  depth_=node.depth_;
  {
    QListIterator < GNode > it = QListIterator < GNode > (node.children());
    for (; it.current(); ++it) {
      childLink((it.current())->copy());
    }
  }
}

/** Construct a GNode by parsing a file */
GNode::GNode(MLParser & parser, GNode* classNode)
{
  init();
  classNode_=classNode;
  read(parser,0,classNode==0);
}

/** Init member variables */
void GNode::init()
{
  classDict_.setAutoDelete(false);
  guiPtr_ = 0;
  name_ = "";
  parent_ = 0;
  showDetails_ = true;
  idStart_=0;
  idEnd_=0;
  id_=0;
  color_=0;
  depth_=0;
  classNode_=0;
}

/** Read GNode by parsing a file */
void GNode::read(MLParser & parser,int depth, bool isClassTree)
{
  depth_=depth;
  parser.args(&attribList_,false);
  setVars();
  if (!isClassTree && parent()) {
    QString className=attribute("class");
    if (className.isEmpty()) {
      className="not set";
    }
    GNode* cNode=parent()->classNode()->findClass(className);
    if (!cNode) {
      cNode=new GNode();
      cNode->name(className);
      cNode->attributeSet("class",className);
      GNode* cNodeTemplate=classDict_[className];
      if (cNodeTemplate) {
        cNode->attribute("color",cNodeTemplate->attribute("color"));
      }
      else {
        cNode->attribute("color",
                         QString().sprintf("%d:%d:%d", rand()%256, rand()%256, rand()%256));
        classDict_.insert(className,cNode);

      }
      parent()->classNode()->childLink(cNode);
    }
    classNode(cNode);
  }
#ifdef DEBUG_MSG
  qDebug("Node: name=%s\n", (const char *) name_);
#endif
  int tag = parser.tag(nodeTagTable);
  while (tag>0 && tag !=MLParser::END_OF_FILE) {

    GNode *node = this->newNode();
    childLink(node);
    node->read(parser,depth_+1,isClassTree);
    tag = parser.tag(nodeTagTable);
  }
  if (tag == MLParser::END_OF_FILE) {
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

/** returns the class name of this node */
const QString & GNode::className() const
{
  return className_;
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
#ifdef DEBUG_MSG
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
  return new GNode(*this);
}

/** return a new Node */
GNode *GNode::newNode()
{
  return new GNode();
}

/** Set the attribute key to val */
void GNode::attributeSet(QString key, QString val)
{
  attribList_.replace(key, new QString(val));
  if (key == "name")
    name_ = val;
  if (key == "class")
    className_ = val;
  if (key == "color") {
    int r = 0;
    int g = 0;
    int b = 0;
    QString attrib = val;
    attrib.replace(QRegExp("[,;:]"), " ");
    QTextIStream istr(&attrib);
    if (!istr.atEnd())
      istr >> r;
    if (!istr.atEnd())
      istr >> g;
    if (!istr.atEnd())
      istr >> b;
    color_ = qRgb(r, g, b);
  }

}

/** Set variables of this node using the attributes */
void GNode::setVars()
{
  MLParser::setString(name_, &attribList_, "name");
  MLParser::setString(className_, &attribList_, "class");
  MLParser::setInt(id_,&attribList_,"id");
  idStart_=id_;
  idEnd_=id_;
  MLParser::setInt(idStart_,&attribList_,"idStart");
  MLParser::setInt(idEnd_,&attribList_,"idEnd");
  QString val="";
  MLParser::setString(val, &attribList_, "color");
  if (val=="") color_=qRgb(depth_%256,depth_%256,depth_%256);
  else attributeSet("color", val);
}

/** returns true if this class is a cname */
bool GNode::isA(QString cname)
{
  return (cname == "GNode");
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

/** find a gnode with the given classname. Only direct children of this gnode are checked. */
GNode *GNode::findClass(QString classname)
{
  QListIterator<GNode> it(this->children());
  for (; it.current(); ++it) {
    GNode *node = it.current();
    if (qstricmp(classname, node->className()) == 0)
      return node;
  }
  return 0;
}

QDict<GNode> GNode::classDict_;
