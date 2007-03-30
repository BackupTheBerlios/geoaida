/***************************************************************************
                          inode.cpp  -  description
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
 * $Source: /data/cvs/gda/gda/core/inode.cpp,v $
 * $Revision: 1.62 $
 * $Date: 2002/08/19 13:02:19 $
 * $Author: pahl $
 * $Locker:  $
 */

#include <assert.h>
#include "inode.h"
#include "fatalerror.h"
#ifdef WIN32
#include <qmessagebox.h>
#endif


const char *const INode::iTypeName[] = {
  "TRASH",
  "MI",                  //Missing
  "IH",                  //Hypothesis
  "CI",                  //Complete Instance
  "PI",                  //Partial Instance
  "HI",                  //Complete Holistic Instance
  "TD_ERROR",
  "BU_ERROR",
  "TD_ABORTED",
  "BU_ABORTED"
};

/* Mögliche Übergänge
  IH -> PI -> CI 
  IH -> IM
  PI -> IM
  IH -> HI
*/
#define DEBUGMSG
INode::INode(SNode * node)
{
  init();
  sNode(node);
  if (sNode_) {
    attribute("name", node->GNode::name());
    attribute("class", node->GNode::name());
  }
}

/** Copy constructor */
INode::INode(INode & node):TreeNode < INode, GNode > (node)
{
  sNode_ = 0;
  sNode(node.sNode());
  execState(node.execState());
  status(node.status());
  childList_ = node.childList_;
  childcount_ = node.childcount_;
  aktivcount_ = node.aktivcount_;
  ordercount_ = node.ordercount_;
  aktivorder_ = node.aktivorder_;
  truncation_ = node.truncation_;
  analysis_ = node.analysis_;
  output_ = node.output_;
  if (node.labelImage_)
    labelImage_ = node.labelImage_->shallowCopy();
  else
    labelImage_ = 0;
}

/** Construct an iNode from reading a file with given parser. */
INode::INode(MLParser & parser)
{
  init();
  read(parser);
}

INode::~INode()
{
  if (sNode_)
    sNode_->unlink();
}

/** initialize inode */
void INode::init()
{
  labelImage_ = 0;
  sNode_ = 0;
  status(HI);                   //default;
  execState(TD);                //default;
  aktivcount_ = childcount_ = ordercount_ = aktivorder_ = 0;
  truncation_ = FALSE;
  childList_ = NULL;
}

/** Make a copy of this INode */
GNode *INode::copy()
{
  GNode *gn = new INode(*this);
#ifdef WIN32
  if (gn == 0){
    cout << "Out of Memory..9";
    exit(1);
  }
#endif
  return gn;
}

/** Returns a new INode */
GNode *INode::newNode()
{
  GNode *gn = new INode(0);
#ifdef WIN32
  if (gn == 0){
    cout << "Out of Memory..10";
    exit(1);
  }
#endif
  return gn;
}

/** Returns true if  this class is a cname */
bool INode::isA(QString cname)
{
  return (cname == "INode");
}

/** Link a sNode to this iNode. */
bool INode::linkSNode(SNode * parent)
{
  bool status = true;
  if (!parent_) {
    sNode(parent);
    QListIterator < INode > it(children());
    for (; it.current(); ++it) {
      INode *node = it.current();
      status = status & node->linkSNode(parent);
    }
  }
  else {
    sNode(parent->findClass(attribute("class")));
    if (!sNode_)
      return false;
    QListIterator < INode > it(children());
    for (; it.current(); ++it) {
      INode *node = it.current();
      status = status & node->linkSNode(sNode_);
    }
  }
  return status;
}

/** This method is called, when a task is finished for this inode
  * pid ==  0 -> no operator or node have no holistic operator
  * pid == -1 -> can't start process
  */
void INode::taskFinished(int pid, int exit_state)
{
  try {
    int flag = -1;
#ifdef DEBUGMSG
    qDebug("#  INode::taskFinished (%s)(%p) pid=%d ES=%d",
           (const char *) name(), this, pid, execState());
#endif
    switch (execState()) {
    case TD:
      if (exit_state!=0) {
        analysis()->setError(true);
        status(TD_ERROR);
        analysis()->ready();
      qDebug("INode::184:clean up necessary?");
        return;
      }
      flag = evalTopDown(pid);
      break;
    case BU:
      if (exit_state!=0) {
        analysis()->setError(true);
        status(BU_ERROR);
        analysis()->ready();
     qDebug ( "INode::194:clean up necessary?");
        return;
      }
      flag = evalBottomUp(pid);
      break;
    }
#ifdef DEBUGMSG
    qDebug("#  INode::taskFinished: finish ... (%s) pid=%d %p flag=%d\n",
           (const char *) name(), pid, this, flag);
#endif
    switch (flag) {
    case 1:
      INode::analysis()->ready();
      break;
    case 0:
#ifdef WIN32
        analysis()->setError(true);
        status(BU_ERROR);
        analysis()->ready();
#else
      delete this;
#endif
      break;
#ifdef WIN32
    case -1:
        analysis()->setError(true);
        status(BU_ERROR);
        analysis()->ready();
#endif
    }
  }
  catch(FatalError err) {
    qDebug("INode::taskFinished(): Exception(%s) %s", name().latin1(),
           err.message().latin1());
//    emit analysis_->message(err.message());
  }
}

/** handle the results of the BottomUp operator
  * pid ==  0 -> no operator or node have no holistic operator
  * pid == -1 -> can't start process
  * return -1 - error
  * return  0 - delete 'this'
  * return  1 - analyze is ready
  * return  2 - processing
  * remember: a node without SNode is a "Trash-Node"
  * this node must have children
  */
int INode::evalBottomUp(int pid)
{
#ifdef DEBUGMSG
  qDebug("#*  INode::evalBottomUp(%s): pid=%d (%p)\n", (const char *) name(),
         pid, this);
#endif
  if (pid <= 0) {
#ifdef DEBUGMSG
    qDebug("#  (ERROR) evalBottomUp(%p): BottomUp operator have no result \n",
           this);
#endif
    //! hier noch das Icon im Browser auf rot setzen (Problem)
    status(TRASH);
    if (parent())
      parent()->decrementCount();
    return -1;
  }

  execState(BU);
  if (isRoot()) {               //! Debug
    qDebug("INode::evalBottomUp: is Root");
#ifdef WIN32
  QMessageBox::information(0,"INode","evalBottomUp: is Root",QMessageBox::Cancel);
#endif

  }
  analysis_->nodeChange(this);
  QList < INode > &nodeList = sNode_->evalBottomUp(this);       //new composition
  analysis_->nodeChange(this);

#ifdef DEBUGMSG
  qDebug("#  (INFO)  nodeList count: %d\n", nodeList.count());
#endif
  if (isRoot()) {
    switch (nodeList.count()) {
    case 0:
#ifdef DEBUGMSG
      qDebug("#  (ERROR) No szene found(%p)\n", this);
#endif
#ifdef WIN32
  QMessageBox::warning(0,"INode","evalBottomUp: No szene found");
#endif
      return 1;
    case 1:{
#ifdef DEBUGMSG
        qDebug("#  Root node is unique (%p)\n", this);
#endif
        //alle bis auf die zurückgegebenen aus this entfernen
        {                       //! eigentlich können die children direkt gelöscht werden!
          INode *trashNode = new INode(*this);
#ifdef WIN32
          if (trashNode == 0){
            cout << "Out of Memory..11";
            exit(1);
          }
#endif
          trashNode->status(TRASH);
          {                     // normally this code should do nothing because the children are already moved
            INode *el;
            while (children().count()) {
              el = (children()).first();
              trashNode->childLink(el);
              analysis_->nodeChange(el);
            }
          }
#if 0
          delete trashNode;
#else
          analysis_->setTrashRoot(trashNode);
#endif
        }
        {
          INode *el;
          INode *iNode = nodeList.first();
          update(iNode->attribList());
          labelImage(iNode->labelImage());
          el = (iNode->children()).first();
          while (el) {
            childLink(el);
            analysis_->nodeChange(el);
            el = (iNode->children()).first();
          }
          delete iNode;
        }
        delete & nodeList;
        return 1;               //=> analyze is ready
      }
    default:
#ifdef DEBUGMSG
      qDebug("#  (ERROR) Root node must be unique (%p)\n", this);
#endif
#ifdef WIN32
  QMessageBox::warning(0,"INode","evalBottomUp: Root node must be unique");
#endif
      delete & nodeList;
      return -1;
    }
  }

  if (nodeList.isEmpty()) {
#ifdef DEBUGMSG
    qDebug
      ("#  (ERROR) evalBottomUp: QList<<INode> nodeList is empty - all trash\n");
#endif
    status(TRASH);
    parent()->decrementCount();
    delete & nodeList;
    //return 0; // XXX TEST - fuer die Ergebnisdarstellung
    return 2;                   //=>
  }

  QListIterator < INode > it(nodeList);
  for (; it.current(); ++it) {
    INode *iNode = it.current();
    analysis_->nodeChange(iNode);
    parent()->childLink(iNode);
    iNode->status(CI);
    INode *el;
    for (el = iNode->children().first(); el != 0;
         el = iNode->children().next()) {
      if (el->status() <= MI)
        iNode->status(MI);
      analysis_->nodeChange(el);        //info to the rest of the world
    }
  }

  INode *p = parent();          //for later access
  if ((this->children()).isEmpty()) {
#ifdef DEBUGMSG
    qDebug("#  (INFO) evalBottomUp: empty list - no trash\n");  //
#endif
    parent()->childUnlink(this);
    p->decrementCount();
    delete & nodeList;
    return 0;                   // delete this;
  }
  else {
    //this->sNode(0); //make a TRASH Node; -> has no SNode
    this->status(TRASH);
    p->decrementCount();
    delete & nodeList;
    //return 0; // XXX TEST - fuer die Ergebnisdarstellung
    return 2;
  }
}

/** handle the results of the TopDown operator
  * pid ==  0 -> no operator or node have no holistic operator
  * pid == -1 -> can't start process
  * never call for "root" node (this is not root node)
  * this can be "last" node
  */
int INode::evalTopDown(int pid)
{
#ifdef DEBUGMSG
  qDebug("#*  INode::evalTopDown(%s)(%p): pid=%d Start\n",
         (const char *) name(), this, pid);
#endif
  CHECK_PTR(sNode_);
//Hier Absturz weil parent = 0
  CHECK_PTR(parent());
  INode* parent=INode::parent();
  if (pid <= 0) {
#ifdef DEBUGMSG
    qDebug
      ("#  (ERROR -1/0) evalTopDown: can't start process \n  try it structural\n");
#endif
#warning Without Dummy TopDown nothing works
    if (!isLast() && !truncation())
      childTopDown();           //do it structural
    else if (isLast()) {                      //is last node
      status(MI);
      execState(BU);
      parent->childUnlink(this);      //!remove this temporary INode
      parent->decrementCount();
      return 0;
    } else { //is trash
      status(TRASH);
      execState(BU);
      parent->childUnlink(this);      //!remove this temporary INode
      parent->decrementCount();
      return 0;
    }
  }

  if (sNode_->holistic()) {     //pid > 0
    QList < INode > iNodeList = sNode_->evalTopDown(this);
#ifdef DEBUGMSG
qDebug("##*** evalTopDown: %d(min) < %d(ist) < %d(max)",sNode()->minNumNode(),iNodeList.count(), sNode()->maxNumNode());
#endif
    if (!(sNode()->minNumNode() == 0 && sNode()->maxNumNode() == 0 )) //beide eintraege null -> alles erlaubt
      if (iNodeList.count() < sNode()->minNumNode() || iNodeList.count() > sNode()->maxNumNode() ) {
#ifdef DEBUGMSG
qDebug("*****  iNodeList.count %d, sNode->minNumNode %d, sNode->maxNumNode %d \n",iNodeList.count(),sNode()->minNumNode(),sNode()->maxNumNode());
#endif
        status(MI);
        execState(BU);
        truncation(TRUE);
        //! unlink here
        parent->status(TRASH);
        parent->truncation(TRUE);
        parent->childUnlink(this);      //!remove this temporary INode
        parent->decrementCount();
        return 0;
      }
#ifdef DEBUGMSG
    qDebug("#  (INFO) sNode is holistic; count new nodes: %d\n", iNodeList.count());
#endif

    if (iNodeList.isEmpty()) {
#ifdef DEBUGMSG
      qDebug("#  (warning) operator return no result - missing Instance");
#endif
      status(MI);
      execState(BU);
      //! unlink here
      parent->childUnlink(this);      //!remove this temporary INode
      parent->decrementCount();
      return 0;
    }

    parent->incrementCount(iNodeList.count() - 1);
    INode *el;
    parent->childUnlink(this);        //!remove this temporary INode
    for (el = iNodeList.first(); el != 0; el = iNodeList.next()) {
#ifdef DEBUGMSG
      qDebug("# einhaengen: (%p) %s in %s", this,(const char *)el->name(), (const char *)parent->name());
#endif
      if (el->isLast())
        el->status(CI);  //el->execState(BU); ???????????
      else
        el->status(PI);
      el->execState(TD); //}?????????????
      parent->childLink(el);         //insert the new INodes
      analysis_->nodeChange(el);
      if (el->isLast())
        parent->decrementCount();
      else
        el->childTopDown();     //last node have no children
    }
    //delete this; //XXXX
#ifdef DEBUGMSG
      qDebug("# einhaengen: (%p) fertig in %s",this,(const char *)parent->name());
#endif
    return 0;
  }
  else {
    qDebug
      ("#  (ERROR 1) taskFinished no operator found \n  try it structural\n");
    if (isLast())
      qDebug("#  (ERROR 1) last node must be holistic!\n");
    parent->childUnlink(this);        //!remove this temporary INode
    return 0;
  }
}


/** call the TopDown operator for all childs (of this)
  * "this" is not a last node */
/** return
  0 - all sub nodes are handled or node is trash
  1 - some more sub node must be calculate
*/
/** first
  0 - FALSE - call to handel nodes with next priority / order
  1 - TRUE (default) - first call for this object
*/
bool INode::childTopDown(bool first)
{
#ifdef DEBUGMSG
  qDebug("#  INode::childTopDown (%s) cont_td (this %p) first: %d", (const char *) name(), this, first);
    qDebug("##(xxx) INode::childTopDown (%s) (this %p) #childcount %d, ordercount %d, aktivorder %d, aktivcount %d, truncation %d",
      (const char *) name(), this, childcount_,ordercount_,aktivorder_,aktivcount_,truncation());
#endif
  SNode *el; // hilfspointer
  if (first) {// first call of 'childTopDown' for this object
    childList_ = &(sNode_->children()); // liste der subknoten erzeugen
    childcount_ = childList_->count(); // anzahl aller (noch) zu bearbeitenden knoten
    aktivorder_=-1; // prioritaet der zu bearbeitenden nodes
    if (childcount_ == 0) { //no sub nodes
      aktivcount_ = 0;
      execBottomUp();             //execState(BU);// BottomUp
      return 0;
    }
  } else { // recursiv call of 'childTopDown' for this object
    if (aktivcount_ > 0 || ordercount_ > 0 ) return 1;
    if (childcount_ <= 0) return 0;//all sub nodes are handled
    if (truncation()) return 0; //all sub nodes are handelt or node is trash
  }

  //Praemisse (ordercount_ == 0) && (childcount_ > 0)
  while (ordercount_ == 0) {
    aktivorder_ ++;
    for (el = childList_->first(); el != 0; el = childList_->next())
      if(el->order() == aktivorder_) ordercount_++; // anzahl der nodes der aktuellen prioritaet
  }
#ifdef DEBUGMSG
    qDebug("## INode::childTopDown (%s) (this %p) #childs %d, ordercount %d, aktivorder %d, aktivcount_ %d",
      (const char *) name(), this, childcount_, ordercount_, aktivorder_,aktivcount_);
#endif

    INode *inode;
    for (el = childList_->first(); el != 0; el = childList_->next()) {
      if (truncation()) { //bedingungen nicht mehr erfüllt
        aktivcount_ = 0;
        execBottomUp();             //execState(BU);// BottomUp
        return 0;
      }
      if (el->order() == aktivorder_) {
        incrementCount(); //einer mehr in der queue -> siehe decrementCount()
        childcount_--; ordercount_--; //aktuelle wird gleich bearbeitet
        inode = new INode(el);    //new INode
#ifdef WIN32
         if (inode == 0){
          cout << "Out of Memory..12";
          exit(1);
          }
#endif
        CHECK_PTR(inode);
        inode->status(HI);
        inode->execState(TD);
        childLink(inode);         // remount node in the tree
        inode->execTopDown();     //start topdown operator
        //childcount_--; ordercount_--; //aktuelle wird gleich bearbeitet
//!MP25.07.2001 inode might be deleted
//!      analysis_->nodeChange(inode);  //info to the rest of the world
        analysis_->nodeChange(0);
      }
    }
  return 1;
}

/** call the TopDown Operator of the SNode  */
void INode::execTopDown()
{
#ifdef DEBUGMSG
  qDebug("#*  INode::execTopDown(%s)(%p): Start\n", (const char *) name(),this);
#ifdef WIN32
  QMessageBox::information(0,"INode","execTopDown",QMessageBox::Default);
#endif
#endif
  if (analysis()->error()) {
    status(TD_ABORTED);
    return;
  }
  execState(TD);
  CHECK_PTR(this->sNode());
  if (sNode_->holistic())
    sNode_->execTopDownOp(this);        //start TD
  else if (!sNode_->attributeBool("td_multiclass"))
    taskFinished(0,0);
  else {
    parent()->decrementCount();
//      status(TRASH);
    parent()->childUnlink(this);        //!remove this temporary INode
    delete this;
  }

}

/** returns the corresponding SNode */
SNode *INode::sNode()
{
  return sNode_;
}

/** Sets the SNode this INode ist type of */
void INode::sNode(SNode * node)
{
  if (sNode_)
    sNode_->unlink();
  sNode_ = node;
  if (node)
    node->link();
}

/** Sets the filename for operator output    */
void INode::output(QString val)
{
  output_ = val;
}

/** Gets the filename for operator output    */
QString & INode::output()
{
  return output_;
}

/** decrement the aktiv counter * aktiv TopDown child operators
 ++ RETURN:
  * -1 -> ERROR (aktivcount < 0)
  *  0 -> aktivcount == 0
  *  1 -> aktivcount > 0
  *  "this" in not a last node*/
int INode::decrementCount()
{
#ifdef DEBUGMSG
  qDebug("#  decrementCount(%s)(%p): Start, aktivcount = %d\n",
         (const char *) name(), this, aktivcount_);
#endif
  analysis_->nodeChange(this);
  if (aktivcount_ == 0) {
#ifdef DEBUGMSG
    qDebug("#  (ERROR) INode::decrementCount < 0!!\n");
#endif
    return -1;
  }

  aktivcount_--;
  if (!childTopDown(FALSE)) {//return FALSE when all sub nodes are handeled or node is trash
    if (truncation() && !isRoot()) {
      parent()->decrementCount();
    } else {
      execBottomUp();// jetzt geht es wieder nach oben
    }
    return 0;
  }
  return 1;
}


/** increment the aktiv counter  */
void INode::incrementCount(int c)
{
#ifdef DEBUGMSG
  qDebug("#  incrementCount(%s)(%p): Start, aktivcount = %d + %d\n",(const char *) name(), this, aktivcount_, c);
#endif
  if (c > 0)
    aktivcount_ += c;
}

/** Execute the bottomUp operator */
void INode::execBottomUp()
{
#ifdef DEBUGMSG
  qDebug("#*  INode::execBottomUp(%s)(%p): Start\n", (const char *) name(), this);
#endif
  if (analysis()->error()) {
    status(BU_ABORTED);
    return;
  }

  execState(BU);
  CHECK_PTR(this->sNode());
  if (sNode_)
    sNode_->execBottomUpOp(this);       //start BU
}

/** return TRUE if the Node is the last Node (leaf node) or the last node to analyze otherwise return FALSE  */
bool INode::isLast()
{
#ifdef DEBUGMSG
  qDebug("#isLast# INode::isLast(%s)(%p): %d\n", (const char *) name(), this, sNode_->attributeBool("resultNode"));
#endif
  if (sNode_->attributeBool("resultNode")) return TRUE;
  return ((sNode_->children()).isEmpty());
}

/** return TRUE if the Node is the root node of the net otherwise return FALSE  */
bool INode::isRoot()
{
  return (!parent());
}

/** Sets the label image or returns the label image if image=0 */
GeoImage *INode::labelImage(GeoImage * image)
{
  if (image) {
    if (labelImage_)
      labelImage_->unlink();
    labelImage_ = image->shallowCopy();
  }
  return labelImage_;
}

/** Sets up the region description for this inode using the available information e.g. labelimage */
void INode::setGeoRegion(float gW, float gN, float gE, float gS, INode* parent)
{
#define defFloat(name) float name=0.0; MLParser::setFloat(name,&attribList_,#name);
#define defInt(name) int name=0; MLParser::setInt(name,&attribList_,#name);
#define defString(name) QString name; MLParser::setString(name,&attribList_,#name);
  defFloat(geoNorth);
  defFloat(geoSouth);
  defFloat(geoWest);
  defFloat(geoEast);
  defFloat(scale_x);
  defFloat(scale_y);
  defInt(offset_x);
  defInt(offset_y);
  defInt(urx);
  defInt(ury);
  defInt(llx);
  defInt(lly);
  defString(file);
  defString(name);
  if ((gN == gS) && (gW == gE)) {
    qDebug("INode::setRegion: gN=gS && gW==gE");
    gN = geoNorth;
    gS = geoSouth;
    gW = geoWest;
    gE = geoEast;
  }
  //! Dealing with the case "geocoordinates AND bounding box are given" is missing
  //! Geocoordinates of the labelimage must be calculated in this case.
  labelImage_ =
    labelImageList_.loadLabelImage(file, sNode()->GNode::name(), gW, gN, gE, gS);
#define TRY_TO_CORRECT_SCALING_ERRORS
#ifdef TRY_TO_CORRECT_SCALING_ERRORS
  // Correct errors in geocoordinates cause by scaling
  if (parent && scale_x && scale_y
     && parent->labelImage() && (scale_x!=1.0 || scale_y!=1.0)) {
    qDebug("INode::setRegion: correct geocoordinates:");
    qDebug("  old: gW=%f, gN=%f, gE=%f, gS=%f",gW,gN,gE,gS);
    GeoImage* labelImage=parent->labelImage();
    int pllx=int(offset_x/scale_x);
    int pury=int(offset_y/scale_y);
    int purx=int((offset_x+labelImage_->cols())/scale_x);
    int plly=int((offset_y+labelImage_->rows())/scale_y);
    qDebug(" scale_x=%f, scale_y=%f, offset_x=%d offset_y=%d",
           scale_x, scale_y, offset_x, offset_y);
    qDebug(" pllx=%d, plly=%d, purx=%d, pury=%d",
           pllx, plly, purx, pury);
    labelImage->geoBBox(pllx,plly,purx,pury,gN,gS,gW,gE);
    qDebug("  new: gW=%f, gN=%f, gE=%f, gS=%f",gW,gN,gE,gS);
    labelImage_->geoWest(gW);
    labelImage_->geoNorth(gN);
    labelImage_->geoEast(gE);
    labelImage_->geoSouth(gS);
  }
#endif
  assert(labelImage_);
  labelImage_->load();
  if (!attribList_["geoNorth"] || attribList_["geoNorth"]->isEmpty()) { // geo coordinates are not set
    qDebug("INode::setRegion: geo ausrechnen");
    labelImage_->geoBBox(llx, lly, urx, ury, geoWest, geoNorth, geoEast,
                         geoSouth);
  }
  else if (!attribList_["llx"]) {       // boundingbox not set, but geo coordinates are set
    qDebug("INode::setRegion: bbox ausrechnen");
    // if file geocoordinates describe a smaller area than the inode geocoordinates,
    // adjust the inode geocoordinates
    if (geoWest<gW) geoWest=gW;
    if (geoNorth>gN) geoNorth=gN;
    if (geoEast>gE) geoEast=gE;
    if (geoSouth<gS) geoSouth=gS;

    labelImage_->picBBox(geoWest, geoNorth, geoEast, geoSouth, llx, lly, urx,
                         ury);
#ifdef WIN32
//c:\eigenedateien\cpptest\geoaida\gda\gda\core\inode.cpp(739) : error C2664: 'replace' : Konvertierung des Parameters 2 von 'class QString' in 'const class QString *' nicht moeglich
//        Kein benutzerdefinierter Konvertierungsoperator verfuegbar, der diese Konvertierung durchfuehren kann, oder der Operator kann nicht aufgerufen werden
    attribList_.replace("llx",&QString().sprintf("%d", llx));
    attribList_.replace("lly",&QString().sprintf("%d", lly));
    attribList_.replace("urx",&QString().sprintf("%d", urx));
    attribList_.replace("ury",&QString().sprintf("%d", ury));
#else
    attribList_.replace("llx",QString().sprintf("%d", llx));
    attribList_.replace("lly",QString().sprintf("%d", lly));
    attribList_.replace("urx",QString().sprintf("%d", urx));
    attribList_.replace("ury",QString().sprintf("%d", ury));
#endif
  }
  else {
    //! Todo: calculate the labelimage geo coordinates from the given geo coordinates
    //        and label boundingbox
  }
#ifdef WIN32
  attribList_.replace("geoNorth",&QString().sprintf("%f", geoNorth));
  attribList_.replace("geoSouth",&QString().sprintf("%f", geoSouth));
  attribList_.replace("geoWest", &QString().sprintf("%f", geoWest));
  attribList_.replace("geoEast", &QString().sprintf("%f", geoEast));
#else
  attribList_.replace("geoNorth",QString().sprintf("%f", geoNorth));
  attribList_.replace("geoSouth",QString().sprintf("%f", geoSouth));
  attribList_.replace("geoWest", QString().sprintf("%f", geoWest));
  attribList_.replace("geoEast", QString().sprintf("%f", geoEast));
#endif
  qDebug("INode::setGeoRegion: %s file=%s(%f,%f,%f,%f) bbox=(%5d, %5d, %5d %5d) geo=(%f, %f, %f, %f) res=(%f, %f)",
     name.latin1(), file.latin1(), gW, gN, gE, gS, llx, lly, urx, ury,
     geoWest, geoNorth, geoEast, geoSouth, labelImage_->resolutionX(),
     labelImage_->resolutionY());

#undef defFloat
#undef defInt
}

/** for preparing the result label image every inode gets a new id */
int INode::setNewID(int id)
{
  attribute("IDStart", id);
  QListIterator < INode > it = QListIterator < INode > (children());
  for (; it.current(); ++it) {
    ++id;
    id = it.current()->setNewID(id);
  }
  attribute("IDEnd", id);
  return id;
}

/** merge inodes labelimage into the result image */
void INode::mergeResultImage(GeoImage & resultImg)
{
  qDebug("INode::mergeResultImage: name=%s", name_.latin1());
  QListIterator < INode > it = QListIterator < INode > (children());
  for (; it.current(); ++it) {
    INode *node = it.current();
    if (node->status() == TRASH)
      continue;
    if (node->labelImage() && node->attributeInt("id")) {
      // id==0 is background and should not be merged into the result image
      if (!resultImg.
          mergeInto(*(node->labelImage()), attributeInt("IDStart"),
                    node->attributeInt("id"), node->attributeInt("IDStart")))
        node->attribute("status", "deleted");
    }
    else {
      node->attribute("status", "no labelimage");
      qDebug("INode::mergeResultImage: %s ommitted",
             (const char *) node->name());
    }
  }
  for (it.toFirst(); it.current(); ++it) {
    INode *node = it.current();
    if (node->status() == TRASH)
      continue;
    if (node->attributeInt("id")) {
      // id==0 is background and should not be merged into the result image
      node->mergeResultImage(resultImg);
    }
    else {
      qDebug("INode::mergeResultImage: %s ommitted",
             (const char *) node->name());
    }
  }
}

/** returns the path to this inode */
QString INode::path()
{
#ifdef WIN32
  if (parent())
    return parent()->path() + name() + "\\";
  return name() + "\\";
#else
  if (parent())
    return parent()->path() + name() + "/";
  return name() + "/";
#endif
}

/** set flag to handle min and max restriction of snode definition */
void INode::truncation(bool v){
qDebug("****:truncate %d => %d (%s %p)",truncation_,v,(const char *)name(),this);
  truncation_ = v;
  if (truncation_) status(TRASH);
}
/** get flag to handle min and max restriction of snode definition */
bool INode::truncation(void){
  return truncation_;
}


Analysis *INode::analysis_;
