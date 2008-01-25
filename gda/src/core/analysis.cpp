/***************************************************************************
                          analysis.cpp  -  description
                             -------------------
    begin                : Mon Oct 16 2000
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

/*
 * $Source: /data/cvs/gda/gda/core/analysis.cpp,v $
 * $Revision: 1.25 $
 * $Date: 2005/10/14 07:38:31 $
 * $Author: mueller $
 * $Locker:  $
 */

#ifdef DEBUG
  #define DEBUGMSG
#endif

#include "analysis.h"
#include "task.h"
#include "cleanup.h"
#include "MLParser.h"

Analysis::Analysis():iNodeRoot_(0), semNet_(0)
{                               //default constructor
  init();
}

Analysis::Analysis(const QString & prjfile):iNodeRoot_(0), semNet_(0)
{                               //file constructor
  init();
  readProject(prjfile);
}

Analysis::Analysis(SemNet * sn):iNodeRoot_(0), semNet_(0)
{
  init();
  semNet_ = sn;
}

Analysis::Analysis(SemNet * sn, GeoImageList * gil, GeoImageList * lil):iNodeRoot_(0),
semNet_
(0)
{
  init();
  semNet_ = sn;
  geoImageList_ = gil;
  labelImageList_ = lil;
}

Analysis::~Analysis()
{                               //destructor
  if (iNodeRoot_)
    delete iNodeRoot_;
  if (map_)
    delete map_;
}

void Analysis::init(void)
{                               //initialize - default
  synchron_ = FALSE;
  stepwise_ = FALSE;
  no_map_ = FALSE;
  iNodeRoot_ = 0;
  trashNodeRoot_ = 0;
  geoImageList_ = 0;
  labelImageList_ = 0;
  semNet_ = 0;
  map_ = 0;
  error_ = false;
}

void Analysis::setError(bool state)
{
  error_ = state;
}

/** read the project file. The semantic net and the image description*/
void Analysis::readProject(QString fname)
{
#if 0
  qDebug("Analysis::readProject: file: %s", (const char *) fname);
  QFile fp(fname);
  if (!fp.open(IO_ReadOnly)) {
    emit message(QString().
                 sprintf("Project %s not found", (const char *) fname));
    qDebug("GeoImageList::read(%s): file not founed", (const char *) fname);
    return;
  }
  //  if (geoImageList_) delete geoImageList_;
  //  geoImageList_ = new GeoImageList;
  MLParser parser(&fp);
  QString keywords[] = { "geoproject",
    "geosemnet",
    "geoimagelist",
    "geoinstancenet", ""
  };
  enum
  {
    TOK_GEOPROJECT = 1,
    TOK_GEOSEMNET,
    TOK_GEOIMAGELIST,
    TOK_GEOINSTANCENET
  };
  const MLTagTable nodeTagTable(keywords);
  int tag, flag = 0;
  do {
    tag = parser.tag(nodeTagTable);
    switch (tag) {
    case TOK_GEOPROJECT:{
        flag = 1;
        break;
      }
    case TOK_GEOSEMNET:{
        if (flag == 1) {
          ArgDict *args = parser.args();
          QString dir = "", netfile = "";
          MLParser::setString(netfile, args, "file");   //read tempor.
          MLParser::setString(dir, args, "dir");        //read tempor.
          dir += "/" + netfile;
          readSemNet(netfile);
          delete args;
        }
        break;
      }
    case TOK_GEOIMAGELIST:{
        if (flag == 1) {
          geoImageList_.read(parser);
        }
        break;
      }
    case TOK_GEOINSTANCENET:{
        if (flag == 1) {
          ArgDict *args = parser.args();
          QString dir = "", netfile = "";
          QString map = "";
          MLParser::setString(netfile, args, "file");   //read tempor.
          MLParser::setString(dir, args, "dir");        //read tempor.
          MLParser::setString(map, args, "dir");        //read tempor.
          dir += "/" + netfile;
          readInstanceNet(netfile);
          qDebug("Analysis::readProject: map=%s", map.latin1());
          delete args;
        }
        break;
      }
    default:{
        ArgDict *args = parser.args();
        delete args;
        break;
      }
    }
  } while ((tag != MLParser::END_OF_FILE) && (tag != -TOK_GEOPROJECT));

  fp.close();
#endif
}

/** read the semantic net */
void Analysis::readSemNet(QString fname)
{
  if (semNet_)
    delete semNet_;
  semNet_ = new SemNet;
  semNet_->read(fname);
}

/** write the semantic net to the given file */
void Analysis::writeSemNet(QString fname)
{
  semNet_->write(fname);
}

/** write the instance net to the given file */
void Analysis::writeInstanceNet(QString fname)
{
  if (!iNodeRoot_)
    return;
  QFile fp(fname);
  fp.open(IO_WriteOnly);
  QTextStream str(&fp);
  iNodeRoot_->write(str);

}

/** calculate the judgement for this search tree node */
void Analysis::calcJudge(void)
{
}

/** Set the operator call for the analysis */
void Analysis::synchron(bool b)
{
  synchron_ = b;
  if (iNodeRoot_)
    (iNodeRoot_->sNode())->synchron(b);
  if (b)
    taskTable.setMaxJobs(1);
  else
    taskTable.setMaxJobs(4);
}

/** generate the first instance, the goal instance.
    It is the root node of the "Semantic Net" */
int Analysis::genGoal(INode* iNode)
{
  if (!semNet_ || iNodeRoot_) { //WRONG: semNet must exist but no iNodeRoot_
    return 1;                   // WRONG: semNet must exist
    qDebug("  (warning) Analysis::genGoal(): I'm at a loss what to do! \n");
  }

  if (!iNode)
    iNodeRoot_ = new INode(semNet_->rootNode());  //generate root INote
  else
    iNodeRoot_=iNode;
  CHECK_PTR(iNodeRoot_);
  if (geoImageList_) {
    iNodeRoot_->attribute("geoNorth", geoImageList_->geoNorth());
    iNodeRoot_->attribute("geoSouth", geoImageList_->geoSouth());
    iNodeRoot_->attribute("geoEast", geoImageList_->geoEast());
    iNodeRoot_->attribute("geoWest", geoImageList_->geoWest());
  }
  if (!iNode) {
    INode::analysis(this);
    //iNodeRoot_->analysis(this);
    iNodeRoot_->status(HI);
    iNodeRoot_->execState(TD);
  }
  return 0;                     // OK
}

/** slot to told the analyze from the root node that all was finished */
void slotready()
{
}

/** start the analysis, the semantic net must be loaded */
void Analysis::slotstart()
{
  start();
}

/** function to start the analysis */
void Analysis::start()
{
  if (genGoal() == 0) {
    emit message("Starting analysis");
    (iNodeRoot_->sNode())->synchron(synchron());
    iNodeRoot_->startAnalyze();
  }
}

/** called when the analyze is ready  */
void Analysis::ready()
{
  if (error()) {
    emit message("Analysis aborted");
  }
  else {
    if (!no_map_)
      prepareResultImage();
    iNodeRoot_->status(CI);
    emit message("Analysis finished");

  }
  qDebug("######## READY ########\n");
//!  cleanUp_.deleteFiles();
  emit sigReady(iNodeRoot_, map_);
}

/** is called when a INode was changed or created */
void Analysis::nodeChange(INode * iNode)
{
#ifdef DEBUG_MSG
  if (iNode)
    qDebug("#  Node change %s (%p)\n", (const char *) (iNode->name()), this);
#endif
  if (iNode)
    if (iNode->sNode()) {
      if (iNode->execState() == TD
          && ((iNode->sNode())->attribute("debug")).compare("TopDown") == 0) {
        stepwise_ = true;
        emit sigInteractive(TRUE);
      }
      if (iNode->execState() == BU
          && ((iNode->sNode())->attribute("debug")).compare("BottomUp") ==
          0) {
        stepwise_ = true;
        emit sigInteractive(TRUE);
      }
    }
  if (stepwise_) {
    emit sigInteractive(TRUE);
    if (iNode) {                //debug info in GUI statusbar
      QString name = "change: ";
      name += iNode->attribute("name");
      emit message(name);
    }
    emit sigNodeChange(iNodeRoot_);     //QQQqqq
  }
}

/** return the geo image list - a QStringList with the keys of the images */
QStringList Analysis::imagelist()
{
  return geoImageList_->list();
}

/** Prepare the result image with the given instantiated net and the corresponding label image tiles */
void Analysis::prepareResultImage()
{
  iNodeRoot_->setNewID(1);
  emit message("Preparing result map");
  if (!iNodeRoot_->labelImage()) {
    qDebug("Analysis::prepareResultImage: no labelimage");
    return;
  }
  int size_x =
    int ((geoImageList_->geoEast() -
          geoImageList_->geoWest()) / labelImageList_->maxResolution());
  int size_y =
    int ((geoImageList_->geoNorth() -
          geoImageList_->geoSouth()) / labelImageList_->maxResolution());
  GeoImage *img = new GeoImage("result.plm", "result", size_x, size_y,
                               geoImageList_->geoWest(),
                               geoImageList_->geoNorth(),
                               geoImageList_->geoEast(),
                               geoImageList_->geoSouth());
  if (!img->mergeInto(*(iNodeRoot_->labelImage()), 0,
                      iNodeRoot_->attributeInt("id"),
                      iNodeRoot_->attributeInt("IDStart"))) {
    iNodeRoot_->attribute("status", "deleted");
  }
  iNodeRoot_->mergeResultImage(*img);
  img->write();
  map_ = img;
  emit sigMapView(iNodeRoot_, map_);
#ifdef DEBUGMSG
  qDebug("Analysis::prepareResultImage: image=(%dx%d)", size_x, size_y);
#endif
}

/** Sets the trash node for the scene viewer */
void Analysis::setTrashRoot(INode * trashNode)
{
  trashNodeRoot_ = trashNode;
}

/** returns the trash root node */
INode *Analysis::trashNode()
{
  return trashNodeRoot_;
}
