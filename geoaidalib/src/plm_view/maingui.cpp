/***************************************************************************
                          maingui.cpp  -  description
                             -------------------
    begin                : Tue Sep 12 2000
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
 * $Source: /data/cvs/geoaidalib/geoaida/plm_view/maingui.cpp,v $
 * $Revision: 1.15 $
 * $Date: 2003/01/09 16:08:21 $
 * $Author: pahl $
 * $Locker:  $
 */

#include "maingui.h"
#include <getopt.h>
#include <qfiledialog.h>
#include <qtextedit.h>
#include "treeitem.h"

//#define DEBUG_PRG
#define DEBUG_MSG

using namespace Ga;

static void Usage(const char *prg)
{
  printf ("Usage:\n"
          "  %s [-l <labelfile>] [-r region-/netfile ] [-d datafile]\n",prg);
  exit(0);
}

#if 0
/* Debug function only */
#include <qobjectlist.h>
void dumpObjectTreeInfo(QObject* obj)
{
  obj->dumpObjectInfo();
  const QObjectList* objList=obj->children();
  if (!objList) return;
  qDebug("<");
  QObjectListIt it(*objList);
  for (; it.current(); ++it) {
    dumpObjectTreeInfo(it.current());
  }
  qDebug(">");
}
/* Debug function only */
#endif

MainGui::MainGui(int argc, char **argv)
{
	QApplication app(argc, argv);
  QtGuiApplication qtapp("share/data/application/plm_view.app",PRGDIR);
  qtapp.insertClass("qapp",&app);
  qtapp.insertClass("main",this);

  rootNode_=0;
  classRootNode_=0;
  classNetLoaded_=false;
  map_=0;
  dataimage_=0;

  treeEditor_=new TreeEditor();
  qtapp.insertClass("treeEditor",treeEditor_);

  classTree_=new TreeEditor();
  qtapp.insertClass("classTree",classTree_);

  propertyEditor_=new PropertyEditor();
  qtapp.insertClass("propertyEditor",propertyEditor_);

  mapViewer_=new MapViewer();
  qtapp.insertClass("mapViewer",mapViewer_);

  mainwidget_=(QMainWindow*)qtapp.create();
  mainwidget_->show();
  app.setMainWidget(mainwidget_);

#ifdef DEBUG_PRG
	netfile_="/home/martin/project/building/building/multilevel/detectbuilding.regs";
	labelfile_="/home/martin/project/building/building/multilevel/detectbuilding.plm";
	readNet(netfile_);
	readMap(labelfile_);
	treeEditor_->rootNode(rootNode_);
	mapViewer_->set(rootNode_,map_);
	treeEditor_->connectSlots();
#else	
  const char* labelFile=0;
  const char* regionFile=0;
  const char* classFile=0;
  const char* dataFile=0;
	{
    int opt;
    char options[] = "r:l:d:c:h";
    while ((opt = getopt (argc, argv, options)) != -1) {
      switch (opt) {
      case 'l':
        labelFile = optarg;
        break;
      case 'r':
        regionFile = optarg;
        break;
      case 'c':
        classFile = optarg;
        break;
      case 'd':
        dataFile = optarg;
        break;
      case 'h':
        Usage(argv[0]);
        break;
      }
    }
  }
  if (classFile) {
    classnetfile_=classFile;
    readClassNet(classnetfile_);
  	classTree_->rootNode(classRootNode_);
  }
  if (regionFile) {
    netfile_=regionFile;
    readNet(netfile_);
  	treeEditor_->rootNode(rootNode_);
  	if (!classFile) classTree_->rootNode(classRootNode_);
  }
  if (labelFile) {
    labelfile_=labelFile;
    readMap(labelfile_);
		mapViewer_->set(rootNode_,map_);
  	treeEditor_->connectSlots();
  }
  if (dataFile) {
    datafile_=dataFile;
    readDataImage(datafile_);
    mapViewer_->setDataImage(dataimage_);
    qDebug("dataimage: %s",datafile_.latin1());

  }
#endif
  app.exec();
  //semNet.write("./output.net");
}

MainGui::~MainGui()
{
}

/** Read a semantic net  */
void MainGui::readNet(QIODevice & fp)
{
  if (rootNode_)
    delete rootNode_;
  if (!classNetLoaded_ && classRootNode_)
    delete classRootNode_;
  rootNode_=0;
  MLParser parser(&fp);
  {
    GNode* cNode=new GNode();
    cNode->name("Root");
    if (classRootNode_) {
      cNode->childLink(classRootNode_);
    }
    classRootNode_=cNode;
  }
  rootNode_=new GNode();
  rootNode_->classNode(classRootNode_);

  int tag = parser.tag(nodeTagTable);
  while (tag !=MLParser::END_OF_FILE) {
    if (tag>0) {
      GNode *node= new GNode();
      rootNode_->childLink(node);
      node->read(parser,1);
    }
    tag=parser.tag(nodeTagTable);
  }
  if (rootNode_->children().count()==1) {
    GNode* node=rootNode_->children().first();
    rootNode_->childUnlink(node);
    delete rootNode_;
    rootNode_=node;
  }
  if (classRootNode_->children().count()==1) {
    GNode* node=classRootNode_->children().first();
    classRootNode_->childUnlink(node);
    delete classRootNode_;
    classRootNode_=node;
  }

  if (rootNode_->children().isEmpty()) {
    delete rootNode_;
    rootNode_=0;
    delete classRootNode_;
    classRootNode_=0;
  }
  if (rootNode_)
    qDebug("MainGui::readNet: Root=%s\n", (const char *) rootNode_->name());
}

/** Read a semantic net by using a filename */
void MainGui::readNet(const QString & fname)
{
  qDebug("MainGui::readNet: %s\n",fname.latin1());
  QFile fp(fname);
  if (!fp.open(IO_ReadOnly)) {
    qDebug("MainGui::read(%s): file not founed\n", (const char *) fname);
    return;
  }
  readNet(fp);
  fp.close();
}

/** Read a semantic class net  */
void MainGui::readClassNet(QIODevice & fp)
{
  if (classRootNode_)
    delete classRootNode_;
  rootNode_=0;
  MLParser parser(&fp);
  classRootNode_=new GNode(parser,0);

#if 0  
  int tag = parser.tag(nodeTagTable);
  while (tag !=MLParser::END_OF_FILE) {
    if (tag>0) {
      GNode *node= new GNode();
      rootNode_->childLink(node);
      node->read(parser,1,true);
    }
    tag=parser.tag(nodeTagTable);
  }
  if (rootNode_->children().count()==1) {
    GNode* node=rootNode_->children().first();
    rootNode_->childUnlink(node);
    delete rootNode_;
    rootNode_=node;
    node=classRootNode_->children().first();
    classRootNode_->childUnlink(node);
    delete classRootNode_;
    classRootNode_=node;

  }
#endif
  GNode* cNode=classRootNode_->children().first();
  classRootNode_->childUnlink(cNode);
  delete classRootNode_;
  classRootNode_=cNode;

  if (classRootNode_->children().isEmpty()) {
    delete classRootNode_;
    classRootNode_=0;
  }
  if (classRootNode_)
    qDebug("MainGui::readClassNet: Root=%s\n", (const char *) classRootNode_->name());
}

/** Read a semantic class net by using a filename */
void MainGui::readClassNet(const QString & fname)
{
  qDebug("MainGui::readNet: %s\n",fname.latin1());
  QFile fp(fname);
  if (!fp.open(IO_ReadOnly)) {
    qDebug("MainGui::read(%s): file not founed\n", (const char *) fname);
    return;
  }
  readClassNet(fp);
  fp.close();
}

/** Read a map by using a filename */
void MainGui::readMap(const QString & fname)
{
  qDebug("MainGui::readMap: %s\n",fname.latin1());
  if (map_)
    delete map_;
  map_=new Image();
  map_->read(fname);
  if (map_->isEmpty()) {
    delete map_;
    map_=0;
    return;
  }
  if (dataimage_)
    map_->setGeoCoordinates(dataimage_->geoWest(),dataimage_->geoNorth(),
                            dataimage_->geoEast(),dataimage_->geoSouth());

}

/** Read a data image by using a filename */
void MainGui::readDataImage(const QString & fname)
{
  qDebug("MainGui::readDataImage: %s\n",fname.latin1());
  if (dataimage_)
    delete dataimage_;
  dataimage_=new Image();
  dataimage_->read(fname);
  if (dataimage_->isEmpty()) {
    delete dataimage_;
    dataimage_=0;
    return;
  }
  if (map_)
    map_->setGeoCoordinates(dataimage_->geoWest(),dataimage_->geoNorth(),
                            dataimage_->geoEast(),dataimage_->geoSouth());
}



/** load a new data image */
void MainGui::loadDataImage()
{
	QString fname=QFileDialog::getOpenFileName("","*.pfm;*.ppm;*.pnm;*.pgm;*.pbm;*.plm",mainwidget_,"Load Data Image");
	if (fname.isEmpty()) return;
	datafile_=fname;
	readDataImage(datafile_);
	mapViewer_->setDataImage(dataimage_);
}

/** load a new map */
void MainGui::loadMap()
{
	QString fname=QFileDialog::getOpenFileName("","*.plm;*.map",mainwidget_,"Load Map File");
	if (fname.isEmpty()) return;
	labelfile_=fname;
	readMap(labelfile_);
	treeEditor_->rootNode(rootNode_);
	mapViewer_->set(rootNode_,map_);
 	treeEditor_->connectSlots();
}

/** load a new net */
void MainGui::loadNet()
{
	QString fname=QFileDialog::getOpenFileName("","*.regs;*.inet",mainwidget_,"Load Net-/Region-Data");
	if (fname.isEmpty()) return;
	treeEditor_->rootNode(0);
  if (!classNetLoaded_) classTree_->rootNode(0);
	netfile_=fname;
	readNet(netfile_);
	treeEditor_->rootNode(rootNode_);
	classTree_->rootNode(classRootNode_);
	mapViewer_->set(rootNode_,map_);
 	treeEditor_->connectSlots();
}

/** load a new class net */
void MainGui::loadClassNet()
{
	QString fname=QFileDialog::getOpenFileName("","*.net",mainwidget_,"Load Class Net");
	if (fname.isEmpty()) return;
	classTree_->rootNode(0);
  treeEditor_->rootNode(0);
	classnetfile_=fname;
	readClassNet(classnetfile_);
	classTree_->rootNode(classRootNode_);
  classNetLoaded_=classRootNode_!=0;
}

void MainGui::clearClassNet()
{
  classNetLoaded_=false;
}
