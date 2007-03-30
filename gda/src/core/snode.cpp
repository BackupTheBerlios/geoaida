/***************************************************************************
                          snode.cpp  -  description
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

#include "snode.h"
#include "task.h"
#include "operatorlist.h"
#include "qcolor.h"
#include <qdir.h>
#include <qregexp.h>
#include "cleanup.h"
#include <assert.h>
#ifdef WIN32
#include <stdlib.h> // für exit
#include <qmessagebox.h>
#endif
#define DEBUG_MSG

static QString GetTmpFilename(QString prefix = "", QString path = "")
{
  static int no = 0;
  static QString dir = CleanUp::getTmpDirPID();
#ifdef WIN32
  if (!path.isEmpty())
    return dir + "\\" + path + QString().sprintf("%04d-%s", no++,
                                                prefix.latin1());
  return dir + QString().sprintf("\\%04d-%s", no++, prefix.latin1());
#else
  if (!path.isEmpty())
    return dir + "/" + path + QString().sprintf("%04d-%s", no++,
                                                prefix.latin1());
  return dir + QString().sprintf("/%04d-%s", no++, prefix.latin1());
#endif
}

SNode::SNode()
{
  init();
}

SNode::~SNode()
{
}

/** Copy constructor */
SNode::SNode(SNode & node):TreeNode < SNode, GNode > (node)
{
//  min_ = node.min_;
//  max_ = node.max_;
  tdCounter_ = 0;
  buCounter_ = 0;
  genericAttributes_ = node.genericAttributes_;
  topDown_ = node.topDown_;
  bottomUp_ = node.bottomUp_;
  evaluation_ = node.evaluation_;
  color_ = node.color_;
}

/** Construct a SNode by parsing a file */
SNode::SNode(MLParser & parser)
{
  init();
  read(parser);
}

/** Init member variables */
void SNode::init()
{
//      GNode::init();
//  order_=0;
  refCounter_=0;
  topDown_ = 0;
  bottomUp_ = 0;
  evaluation_ = 0;
//  min_ = 1;
//  max_ = 1;
  sections_ += "bottomUp";
  sections_ += "topDown";
  sections_+="evaluation";
  genericAttributes_.insert("resultNode",
                            new Attribute("resultNode", "FALSE", Attribute::BOOL,
                                          "ResultNode",
                                          "If this attribute is activated this node will be handled as a result/leaf node"));
  genericAttributes_.insert("name",
                            new Attribute("name", "name_", Attribute::STRING,
                                          "Name", "Name of Node"));
  genericAttributes_.insert("class",
                            new Attribute("class", "", Attribute::STRING,
                                          "Class",
                                          "Class of Node<br>This is used for selecting nodes resulting from the topdown operator call"));
  genericAttributes_.insert("order",
                            new Attribute("order", "0", 0, 99999, "order",
                                          "node priority, lowest entry works first (using min and max entry)"));
  genericAttributes_.insert("min",
                            new Attribute("min", "0", 0, 99999, "min",
                                          "minimal number of needed INodes"));
  genericAttributes_.insert("max",
                            new Attribute("max", "0", 0, 99999, "max",
                                          "maximal number of needed INodes"));
  genericAttributes_.insert("topDown",
                            new Attribute("topDown", "dummyTopDown",
                                          "topdown", "TopDownOperator"));
  genericAttributes_.insert("bottomUp",
                            new Attribute("bottomUp", "dummyBottomUp",
                                          "bottomup", "BottomUpOperator"));
  genericAttributes_.insert("evaluation",
                            new Attribute("giscomp", "***No Operator***",
                                          "giscomp", "EvaluationOperator"));
  genericAttributes_.insert("color",
                            new Attribute("color", "255:255:255",
                                          Attribute::COLOR, "Color",
                                          "color to show this kind of node in the result image"));
  attribute("color", "255:255:255");
  genericAttributes_.insert("td_multiclass",
                            new Attribute("td_multiclass", "",
                                          Attribute::BOOL,
                                          "TopDown Multi Class",
                                          "<ul>"
                                          "<li>Multi Class Off- Operator only detects one class</li>"
                                          "<li>Multi Class On - Operator detects multiple classes</li>"
                                          "</ul>"));
  attribute("td_multiclass", "0");
  {
    QStringList *o = new QStringList();
    (*o) += "TopDown";
    (*o) += "BottomUp";
    (*o) += "DebugInfo";
    (*o) += "NoDebugInfo";
    genericAttributes_.insert("debug",
                              new Attribute("debug", "DebugInfo", o, "Debug",
                                            "<ul>"
                                            "<li>TopDown - Start single step when topdown is executed</li>"
                                            "<li>BottomUp - Start single step when bottomup is executed</li>"
                                            "<li>DebugInfo - Keep temporary files</li>"
                                            "<li>NoDebugInfo - Clean up all temporary files</li>"
                                            "</ul>"));
  }
  attribute("debug", "DebugInfo");
}

/** Get min amount of SNodes */
int SNode::minNumNode() {
  return attributeInt("min");
}

/** Get max amount of SNode */
int SNode::maxNumNode() {
  return attributeInt("max");
}

/** Get order of SNode */
int SNode::order() {
  return attributeInt ("order");
}

/** Set the attribute key to val */
void SNode::attributeSet(QString key, QString val)
{
#ifdef DEBUG_MSG
  qDebug("SNode::attributeSet %s=%s\n", (const char *) key,
         (const char *) val);
#endif
  GNode::attributeSet(key, val);
  if ((key == "name") && (!attribList_["class"]))
    attributeSet("class", val);
  if (key == "isCompound")
    isCompound_ = (val.toInt() != 0);
  if (key == "topDown")
    changeOperator(topDown_, val);
  if (key == "bottomUp")
    changeOperator(bottomUp_, val);
  if (key == "evaluation")
    changeOperator(evaluation_, val);
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

/** This node is a compound of its subnodes. */
bool SNode::isCompound()
{
  return isCompound_;
}

/** Set this node to be a compound or a generalization node. */
void SNode::isCompound(bool val)
{
  if (val)
    attribute("isCompound", 1);
  else
    attribute("isCompound", 0);
}

/** Makes a copy of this node */
GNode *SNode::copy()
{
  GNode *gn = new SNode(*this);
#ifdef WIN32
  if (gn == 0){
    cout << "Out of Memory..15";
    exit(1);
  }
#endif
  return gn;
}

/** returns a new node of type SNode */
GNode *SNode::newNode()
{
  GNode *gn = new SNode();
#ifdef WIN32
  if (gn == 0){
    cout << "Out of Memory..16";
    exit(1);
  }
#endif
  return gn;
}

/** set node variables using the attribute list */
void SNode::setVars()
{
  GNode::setVars();
//  MLParser::setInt(min_, &attribList_, "min");
//  MLParser::setInt(max_, &attribList_, "max");
  QString val;
  val = "";
  MLParser::setString(val, &attribList_, "topDown");
  if (!val.isEmpty())
    topDown_ = operatorList_[val];
#define OLD_NET_COMPATIBILITY
#ifdef OLD_NET_COMPATIBILITY
  if (topDown_) {
    QDict < Attribute > *attribs = topDown_->attributeDesc();
    if (attribs) {
      QDictIterator < Attribute > it(*attribs);
      for (; it.current(); ++it) {
#ifdef WIN32
        QString name = (*it.current()).name();
#else
        QString name = (**it).name();
#endif
        QString *val=attribList().find(name);
        if  (val) {
#ifdef WIN32
          qDebug("SNode::setVars: substituting %s with %s",name.latin1(),(*it.current()).fullname().latin1());
          attribList().replace((*it.current()).fullname(),val);
#else
          qDebug("SNode::setVars: substituting %s with %s",name.latin1(),(**it).fullname().latin1());
          attribList().replace((**it).fullname(),val);
#endif
          attribList().remove(name);
        }
      }
    }
  }
#endif
  val = "";
  MLParser::setString(val, &attribList_, "bottomUp");
  bottomUp_ = operatorList_[val];

#ifdef OLD_NET_COMPATIBILITY
  if (bottomUp_) {
    QDict < Attribute > *attribs = bottomUp_->attributeDesc();
    if (attribs) {
      QDictIterator < Attribute > it(*attribs);
      for (; it.current(); ++it) {
#ifdef WIN32
        QString name = (*it.current()).name();
#else
        QString name = (**it).name();
#endif  
        QString *val=attribList().find(name);
        if  (val) {
#ifdef WIN32
          qDebug("SNode::setVars: substituting %s with %s",name.latin1(),(*it.current()).fullname().latin1());
          attribList().replace((*it.current()).fullname(),val);
#else
          qDebug("SNode::setVars: substituting %s with %s",name.latin1(),(**it).fullname().latin1());
          attribList().replace((**it).fullname(),val);
#endif
          attribList().remove(name);
        }
      }
    }
  }
#endif
  val = "";
  MLParser::setString(val, &attribList_, "evaluation");
  evaluation_ = operatorList_[val];
  MLParser::setString(val, &attribList_, "color");
  attributeSet("color", val);
}

/** Execute the given operator. This function spawns a new process
  * and returns. When the task is finish iNode->taskFinished() is called. */
void SNode::execOperator(Operator * op, INode * iNode, AttribList & attribs,
                         QString path, int &counter)
{
  if (!op) {
    iNode->taskFinished(0,0);
    return;
  }
  CleanUp::mkdir(CleanUp::getTmpDirPID(), path);
  iNode->output(GetTmpFilename("output", path));
  attribs.replace("output", &(iNode->output()));
#ifdef WIN32
  attribs.replace("tmpdir", &CleanUp::getTmpDirPID());
#else
  attribs.replace("tmpdir", CleanUp::getTmpDirPID());
#endif
  QString cmd=op->command(attribs);
  QString stdout_file=iNode->output()+".stdout";
  QString stderr_file=iNode->output()+".stderr";
  QFile fp(iNode->output()+".cmd");
  if (fp.open(IO_WriteOnly)) {
    fp.writeBlock(cmd,cmd.length());
    fp.close();
  }
//  cmd+=" >"+stdout_file+" 2>"+stderr_file;
  cmd+=" 2>"+stderr_file;
  taskTable.queue(cmd, iNode);
  counter++;
  stateChanged(guiPtr_);
}

static void putAttribs(SNode & snode, AttribList & attribs, QString section)
{
  QDict < Attribute > *attribDict = snode.attributeDesc(section);
  AttribList & attribList = snode.attribList();
  if (attribDict) {
    QDictIterator < Attribute > it(*attribDict);
    for (; it.current(); ++it) {
      QString name = (**it).name();
      QString *val = attribList[(**it).fullname()];
      if (val)
        attribs.replace(name, val);
    }
  }
}

/** Execute the topDown operator. This function spawns a new process
  * and returns. When the topDown task is finish iNode->taskFinished() is called. */
void SNode::execTopDownOp(INode * iNode)
{
#ifdef DEBUG_MSG
  qDebug("#* SNode::execTopDown(%s) (%p)", name_.latin1(), this);
#ifdef WIN32
  QMessageBox::information(0,"SNode","execTopDown",QMessageBox::Default);
#endif
#endif
  ASSERT(iNode->parent());
//((*(TreeGNode*)(&(*(TreeNode<GNode,TreeGNode>*)(&(*(GNode*)(&(*(TreeNode<INode,GNode>*)(&*iNode))))))))).parent_
//Hier Absturz weil parent = 0
//Darf nur aus Analyze::start() gestartet werden, nicht aus MainGui::Start()!
#ifdef WIN32
  INode *parent = 0;
  if (iNode)
    if (iNode->parent())
      parent = iNode->parent();
    else
      parent = iNode;
  else return;
  if (parent->isA("SNode")) //rootNode
    QMessageBox::information(0,"SNode","Problem",QMessageBox::Default);
#else
  INode *parent = iNode->parent();
#endif

  AttribList attribs(parent->attribList());
  QDict < Attribute > *topDownAttrib = attributeDesc("topDown");
  // now generate image specific attributes
  if (topDownAttrib) {
    QDictIterator < Attribute > it(*topDownAttrib);
    for (; it.current(); ++it) {
      switch ((**it).type()) {
      case Attribute::IMAGE:{
          QString imageName = attribute((**it).fullname());
          QString keyName = (**it).name();
          GeoImage *image = geoImageList_.geoImage(imageName);
          if (image) {
            QDictIterator < QString > it(*image);
            for (; it.current(); ++it) {
              attribs.replace(keyName + "_" + it.currentKey(),
                              (it.current()));
            }
            QString partName;
            if (topDown_ && topDown_->runGlobal())
              partName = image->part(geoImageList_.geoWest(),
                                     geoImageList_.geoNorth(),
                                     geoImageList_.geoEast(),
                                     geoImageList_.geoSouth());
            else
              partName = image->part(parent->attributeFloat("geoWest"),
                                     parent->attributeFloat("geoNorth"),
                                     parent->attributeFloat("geoEast"),
                                     parent->attributeFloat("geoSouth"));
            cleanUp_.append(partName);
            attribs.replace(keyName + "_file", partName);
          }
        }
        break;
      default:
        break;
      }
    }
  }
  // create a mask image and store mask attributes
  GeoImage *labelImage = parent->labelImage();
  if (labelImage) {
    QDictIterator < QString > it(*labelImage);
    for (; it.current(); ++it) {
      attribs.replace("mask_" + it.currentKey(), it.current());
    }
    QString maskfile;
    if (topDown_ && topDown_->runGlobal()) {
      int global_mask_x, global_mask_y, x2, y2, local_mask_x, local_mask_y;
      labelImage->picBBox(geoImageList_.geoWest(),geoImageList_.geoNorth(),
              geoImageList_.geoEast(),geoImageList_.geoSouth(),
              global_mask_x, y2, x2, global_mask_y);
      int global_mask_size_x=x2-global_mask_x+1;
      int global_mask_size_y=y2-global_mask_y+1;
      labelImage->picBBox(parent->attributeFloat("geoWest"),
                          parent->attributeFloat("geoNorth"),
                          parent->attributeFloat("geoEast"),
                          parent->attributeFloat("geoSouth"),
                          local_mask_x, y2, x2, local_mask_y);

      attribs.replace("mask_x",local_mask_x-global_mask_x);
      attribs.replace("mask_y",local_mask_y-global_mask_y);
      attribs.replace("mask_size_x",global_mask_size_x);
      attribs.replace("mask_size_y",global_mask_size_y);
#ifdef DEBUG_MSG
      qDebug("SNode::execTopDown: mask: w=%f, n=%f, e=%f, s=%f\n"
             "                          x=%d, y=%d, size_x=%d, size_y=%d\n"
             "                          mw=%f, mn=%f, me=%f, ms=%f\n"
             "                          msize_x=%d, msize_y=%d\n",
              geoImageList_.geoWest(),geoImageList_.geoNorth(),
              geoImageList_.geoEast(),geoImageList_.geoSouth(),
              local_mask_x-global_mask_x, local_mask_y-global_mask_y,global_mask_size_x, global_mask_size_y,
              labelImage->geoWest(),labelImage->geoNorth(),labelImage->geoEast(),labelImage->geoSouth(),
              labelImage->cols(),labelImage->rows());
#endif
    }
    maskfile = labelImage->mask(parent->attributeFloat("geoWest"),
                                parent->attributeFloat("geoNorth"),
                                parent->attributeFloat("geoEast"),
                                parent->attributeFloat("geoSouth"),
                                parent->attributeInt("id"), parent->path());
    attribs.replace("mask_file", maskfile);
    cleanUp_.append(maskfile);
  }
  else attribs.replace("mask_file", "\"\"");

  attribs.replace("minRes", geoImageList_.minResolution());
  attribs.replace("maxRes", geoImageList_.maxResolution());
  attribs.replace("numRegions",SNode::parent()->refCounter_);
#if 1
  putAttribs(*this, attribs, "generic");
  putAttribs(*this, attribs, "topDown");
#else
  {
    QDictIterator < QString > it(attribList());
    for (; it.current(); ++it) {
      attribs.replace(it.currentKey(), it.current());
    }
  }
#endif
#ifdef DEBUG_MSG
  {
    qDebug("SNode::execTopDown: attributes");
    QTextOStream ts(stderr);
    ts << attribs;
  }
#endif
  execOperator(topDown_, iNode, attribs, parent->path(), tdCounter_);
}

/** Execute the bottomUp operator. This function spawns a new process
  * and returns. When the bottomUp task is finish iNode->taskFinished() is called. */
void SNode::execBottomUpOp(INode * iNode)
{
#ifdef DEBUG_MSG
  qDebug("#* SNode::execBottomUp(%s)", name_.latin1());
#endif
  CleanUp::mkdir(CleanUp::getTmpDirPID(), iNode->path());
  QString iname = GetTmpFilename("input", iNode->path());
  QFile ifp(iname);
  if (!ifp.open(IO_WriteOnly)) {
    qDebug("Could not open tempfile %s\n", iname.latin1());
    iNode->taskFinished(0,0);
    return;
  }

  {
    if (!iNode->parent()) {     //! Debug
      qDebug("SNode::execBottomUp: root node evaluated");
    }
#if 0                           // This code deletes the trashnodes
    {                           // Delete trash nodes
      QListIterator < INode > it =
        QListIterator < INode > (iNode->children());
      while (it.current()) {
        INode *node = it.current();
        if (node->status() == TRASH) {
          iNode->childUnlink(node);
          delete node;
        }
        else
          ++it;
      }
    }
#endif
    QTextStream str(&ifp);
    QListIterator < INode > it = QListIterator < INode > (iNode->children());
    for (; it.current(); ++it) {
      INode *node = it.current();
#if 1                           // If trashnode is not deleted (s.a.) trashnodes must be skipped here
      if (node->status() == TRASH)
        continue;
#endif
      GeoImage *img = node->labelImage();
      if (img) {
        node->attribute("file_geoWest", img->geoWest());
        node->attribute("file_geoSouth", img->geoSouth());
        node->attribute("file_geoEast", img->geoEast());
        node->attribute("file_geoNorth", img->geoNorth());
      }
      node->attribute("addr", QString().sprintf("%p", node));
      node->write(str, "", false);
      node->attributeRemove("addr");
    }
  }
  ifp.close();

#if 1
  AttribList attribs;
  putAttribs(*this, attribs, "generic");
  putAttribs(*this, attribs, "bottomUp");
#else
  AttribList attribs(attribList());
#endif
  iNode->attribute("input", iname);
  {
    QDictIterator < QString > it(iNode->attribList());
    for (; it.current(); ++it) {
#ifdef WIN32
      QMessageBox::information(0,"SNode",it.currentKey(),1);
#endif
      attribs.replace(it.currentKey(), it.current());
    }
  }
  execOperator(bottomUp_, iNode, attribs, iNode->path(), buCounter_);
}

#if 0
QStringList & SNode::predefinedAttribs()
{
  return predefinedAttribs_;
}
#endif

/** Returns true if  this class is a cname */
bool SNode::isA(QString cname)
{
  return (cname == "SNode");
}

/** Get the attribute description for the specified section */
QDict < Attribute > *SNode::attributeDesc(QString section)
{
#ifdef DEBUG_MSG
  qDebug("SNode::attributeDesc(%s)\n", section.latin1());
#endif
  if (section == "generic")
    return &genericAttributes_;
  if ((section == "topDown") && (topDown_))
    return topDown_->attributeDesc();
  if ((section == "bottomUp") && (bottomUp_))
    return bottomUp_->attributeDesc();
  if ((section == "evaluation") && (evaluation_))
    return evaluation_->attributeDesc();
#ifdef DEBUG_MSG
  qDebug("SNode::attributeDesc(%s) not found\n", section.latin1());
#endif
  return 0;
}

/** Evaluates the result of TopDownOperator    */
QList < INode > SNode::evalTopDown(INode * inode)
{
#ifdef DEBUG_MSG
  qDebug("#* SNode::evalTopDown(%s)", name_.latin1());
#endif
  tdCounter_--;
  stateChanged(guiPtr_);
  assert(inode);
  QList < INode > list;
  cleanUp_.append(inode->output());
  QFile fp(inode->output());
  if (!fp.open(IO_ReadOnly)) {
    qDebug("SNode::evalTopDown(%s): file not founed\n",
           (const char *) (inode->output()));
    return list;
  }
  MLParser parser(&fp);
  QString classname;
  MLParser::setString(classname, &(attribList()), "class");
#ifdef DEBUG_MSG
  qDebug("SNode::evalTopDown: Searching for %s\n", classname.latin1());
#endif
  int tag;
  do {
    tag = parser.tag(nodeTagTable);
#ifdef DEBUG_MSG
    qDebug("tag=%d\n", tag);
#endif
    AttribList *attribList = parser.args();
    switch (tag) {
    case TOK_REGION: {
        SNode *sNode;
        INode *node;
        QString *classNameRegion = (*attribList)["class"];
#ifdef DEBUG_MSG
        qDebug("class is %s\n", (const char *) *classNameRegion);
#endif
        if (classNameRegion) {
          if (qstricmp(classname, *classNameRegion) == 0) {
            sNode = this;
          }
          else if (parent() && attributeBool("td_multiclass")) {
            sNode = parent()->findClass(*classNameRegion);
          }
          else
            sNode = 0;
        }
        else
          sNode = 0;

        if (sNode) {
#ifdef DEBUG_MSG
          qDebug("SNode::evalTopDown(): found %s\n",
                 classNameRegion->latin1());
#endif
          node = new INode(sNode);
          node->configure(*attribList);
          if (node->attribute("name").isEmpty() && (*attribList)["id"])
            node->attribute("name", sNode->name() + QString().sprintf("_%03d",
                                                                      ((*attribList)["id"])->toInt()));
          assert(inode->parent());
          double gN = inode->parent()->attributeFloat("geoNorth");
          double gS = inode->parent()->attributeFloat("geoSouth");
          double gW = inode->parent()->attributeFloat("geoWest");
          double gE = inode->parent()->attributeFloat("geoEast");
          node->setGeoRegion(gW, gN, gE, gS);
          cleanUp_.append(node->attribute("file"));
          list.append(node);
        }
        break;
      }
    }
    delete attribList;
  } while (tag != MLParser::END_OF_FILE);
//!     fp.remove();

  return list;
}

/** Evaluates the result of the bottom-up-operator   */
QList < INode > &SNode::evalBottomUp(INode * iNode)
{
#ifdef DEBUG_MSG
  qDebug("#* SNode::evalBottomUp(%s)", name_.latin1());
#endif
  buCounter_--;
  stateChanged(guiPtr_);
  cleanUp_.append(iNode->attribute("input"));
  iNode->attributeRemove("input");

  QList < INode > *groupList = new QList < INode >;
  groupList->setAutoDelete(false);

  INode *list = 0;

  cleanUp_.append(iNode->output());
  QFile fp(iNode->output());
  if (!fp.open(IO_ReadOnly)) {
    qDebug("SNode::evalBottomUp(%s): file not found\n",
           (const char *) (iNode->output()));
    return *groupList;
  }
  MLParser parser(&fp);
#ifdef DEBUG_MSG
  qDebug("SNode::evalBottomUp: Parsing %s\n", (const char *) iNode->output());
#endif
  int tag;
  do {
    tag = parser.tag(nodeTagTable);
#ifdef DEBUG_MSG
    qDebug("tag=%d\n", tag);
#endif
    AttribList *attribList = parser.args();
    INode *node;
    switch (tag) {
    case TOK_GROUP:
      list = new INode(this);
#ifdef WIN32
      if (list == 0){
        cout << "Out of Memory..16";
        exit(1);
      }
#endif
//                      list->setAutoDelete(false);
      list->update(iNode->attribList());
      list->attributeRemove("llx");
      list->attributeRemove("urx");
      list->attributeRemove("lly");
      list->attributeRemove("ury");
      list->attributeRemove("file_geoWest");
      list->attributeRemove("file_geoNorth");
      list->attributeRemove("file_geoEast");
      list->attributeRemove("file_geoSouth");
      list->update(*attribList);
      if (!(*attribList)["name"] && (*attribList)["id"])
        list->attribute("name", iNode->name() + QString().sprintf("_%03d",((*attribList)["id"])->toInt()));
      break;
    case -TOK_GROUP:
      if (list->children().count() != 0) {
        list->setGeoRegion(list->attributeFloat("file_geoWest"),
                           list->attributeFloat("file_geoNorth"),
                           list->attributeFloat("file_geoEast"),
                           list->attributeFloat("file_geoSouth"));
        cleanUp_.append(list->attribute("file"));
        groupList->append(list);
      }
      else
        delete list;
#ifdef DEBUG_MSG
      qDebug("End of group\n");
#endif
      list = 0;
      break;
    case TOK_NODE:
      {
        QString *s = (*attribList)["addr"];
        if (!s || s->isEmpty())
          break;
        node = 0;
        if (sscanf((const char *) *s, "%p", &node) != 1)
          break;
#ifdef DEBUG_MSG
        qDebug("Found %s\n", (const char *) (node->name()));
#endif
        node->update(*attribList);
        node->attributeRemove("addr");
        node->setGeoRegion(node->attributeFloat("file_geoWest"),
                           node->attributeFloat("file_geoNorth"),
                           node->attributeFloat("file_geoEast"),
                           node->attributeFloat("file_geoSouth"));
        cleanUp_.append(node->attribute("file"));
        list->childLink(node);
        //! Set attributes added by BottomUp-Operator
      }
      break;
    case -TOK_NODE:
      break;
    }
    delete attribList;
  } while (tag != MLParser::END_OF_FILE);
//!     fp.remove();

  if (list)
    delete list;
  if (!iNode->parent()) {
#ifdef DEBUG_MSG
    qDebug
      ("#  (Warning) %s(%p) has no parent -> write result './new_result1.net'\n",
       (const char *) (iNode->attribute("name")), iNode);
#endif
    QFile fp("new_result1.net");
    fp.open(IO_WriteOnly);
    QTextStream str(&fp);
    iNode->write(str);
    fp.close();
  }

  {                             // Debug
    QListIterator < INode > it(*groupList);
    for (; it.current(); ++it) {
#ifdef DEBUG_MSG
      qDebug("count=%d\n", it.current()->children().count());
#endif
    }
  }
  return *groupList;
}

/** says whether the node is hollistisch */
bool SNode::holistic(void)
{
  if (topDown_)
    return TRUE;
  else
    return FALSE;
}

/** No descriptions */
uint SNode::color()
{
  return color_;
}

bool SNode::synchron_ = FALSE;

/** find a snode with the given classname. Only direct children of this snode are checked. */
SNode *SNode::findClass(QString classname)
{
  QListIterator < SNode > it(this->children());
  for (; it.current(); ++it) {
    SNode *node = it.current();
    if (qstricmp(classname, node->attribute("class")) == 0)
      return node;
  }
  return 0;
}

void SNode::setGuiPtr(TreeItem * ptr)
{
  GNode::setGuiPtr(ptr);
  tdCounter_ = 0;
  buCounter_ = 0;
}

/** change an operator and change the corresponding attributes */
void SNode::changeOperator(Operator * &op, QString opName)
{
  Operator *newOp = operatorList_[opName];
  if (op == newOp)
    return;
  if (op) {
    QDictIterator < QString > it(attribList());
    while (it.current()) {
#ifdef WIN32
      if (it.currentKey().contains(op->type().lower() + "\\")) {
#else
      if (it.currentKey().contains(op->type().lower() + "/")) {
#endif
        attribList().remove(it.currentKey());
      }
      else
        ++it;

    }
  }
  op = newOp;
  if (!op)
    return;
  QDict < Attribute > *attribs = op->attributeDesc();
  if (attribs) {
    QDictIterator < Attribute > it(*attribs);
    for (; it.current(); ++it) {
#ifdef WIN32
      QString name = (*it.current()).fullname();
      QString val = (*it.current()).value();
#else
      QString name = (**it).fullname();
      QString val = (**it).value();
#endif
      if (!val.isEmpty())
        attribList().replace(name, new QString(val));
    }
  }

}
/** increase the reference counter */
void SNode::link()
{
  refCounter_++;
}

/** decrease the reference counter */
void SNode::unlink()
{
  refCounter_--;
  assert(refCounter_>=0);
}
