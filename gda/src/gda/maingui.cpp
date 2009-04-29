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
 * $Source: /data/cvs/gda/gda/maingui.cpp,v $
 * $Revision: 1.57 $
 * $Date: 2005/10/14 07:38:31 $
 * $Author: mueller $
 * $Locker:  $
 */

#include "maingui.h"
#include <qfiledialog.h>
#include <qtextedit.h>
#include "treeitem.h"
#include "operatorlist.h"
#include "filelist.h"
#include "inode.h"
#include "cleanup.h"
#include "fatalerror.h"
#include <qscrollview.h>
#include <qtimer.h>
#include "task.h"

// #define DEBUGMSG

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

#ifdef QT_NO_DEBUG
void myMessageOutput( QtMsgType type, const char *msg )
{
    switch ( type ) {
    case QtDebugMsg:
      //        fprintf( stderr, "Debug: %s\n", msg );
        break;
    case QtWarningMsg:
        fprintf( stderr, "Warning: %s\n", msg );
        break;
    case QtFatalMsg:
        fprintf( stderr, "Fatal: %s\n", msg );
        abort();                    // deliberately core dump
    }
}
#endif


MainGui::MainGui(int argc, char **argv)
{

  int option_char;
  batchmode_= false;
  no_map_= false;
  QString arg;
  while ((option_char = getopt(argc, argv, "p:bmh?")) != EOF)
    switch (option_char)
    {
      case 'b': batchmode_ = true; break;
      case 'm': no_map_ = true; break;      
      case 'p': arg = optarg; break;
      case 'h': usage(); break;
      case '?': usage(); break;
    }

#ifdef QT_NO_DEBUG
  qInstallMsgHandler( myMessageOutput );
#endif
  QApplication app(argc, argv);
  QtGuiApplication qtapp("share/data/application/gda.app",PRGDIR);
  cleanUp_.prefix(qtapp.prefix());
  operatorList_.readDir(qtapp.prefix()+"/share/data/operators");	
  fileSemanticNet_="default.net";
  qtapp.insertClass("qapp",&app);

  qtapp.insertClass("main",this);

  analysis_=0;
  iNode_=0;
  map_=0;
//  geoImageList_=0;
  mode_ = 0;
  wait_=TRUE;

  treeEditor=new TreeEditor();
  qtapp.insertClass("netEditor",treeEditor);

  propertyEditor=new PropertyEditor();
  qtapp.insertClass("propertyEditor",propertyEditor);

  sceneViewer=new TreeEditor();
  qtapp.insertClass("sceneViewer",sceneViewer);

  taskViewer=new TaskViewer();
  qtapp.insertClass("taskViewer",taskViewer);

  sceneAttributes=new PropertyEditor();
  qtapp.insertClass("sceneAttributes",sceneAttributes);

  trashViewer=new TreeEditor();
  qtapp.insertClass("trashViewer",trashViewer);

  trashAttributes=new PropertyEditor();
  qtapp.insertClass("trashAttributes",trashAttributes);

  mapViewer=new MapViewer();
  qtapp.insertClass("mapViewer",mapViewer);

  debugViewer=new MapViewer();
  qtapp.insertClass("debugViewer",debugViewer);

  formulaEditor=new QTextEdit(0,"formulaEditor");
  qtapp.insertClass("formulaEditor",formulaEditor);

#if 0
  qtapp.insertVarString("projectname",&projectName_,1024);
#endif
//	QListViewItem
  mainwidget=(QMainWindow*)qtapp.create();
	pixmap_generalize=new QPixmap(qtapp.prefix()+"/share/data/application/generalization.xpm");
	if (pixmap_generalize->isNull()) {
		qDebug("pixmap generalize %s not found",
		       (const char*)(qtapp.prefix()+"/share/data/application/generalization.xpm"));
		exit(0);
	}
	pixmap_compound=new QPixmap(qtapp.prefix()+"/share/data/application/compound.xpm");
	if (pixmap_compound->isNull()) {
 		qDebug("pixmap compound %s not found",
 		       (const char*)(qtapp.prefix()+"/share/data/application/compound.xpm"));
		exit(0);
	}

  readyToAna(FALSE);  //disable the analysis buttons
  interactive(FALSE); //disable the analysis buttons

  if (argc >= 2) {
    QString suffix = arg.right( 3 );
    if (suffix == "net") {
      fileSemanticNet_=arg;
      //readyToAna(TRUE);  //enable the analysis buttons XXX only for testing
      semNet.read(fileSemanticNet_);
    } else if (suffix == "gap")
      loadPrjFile(arg);
  }
  treeEditor->rootNode(semNet.rootNode());
  mainwidget->show();
  app.setMainWidget(mainwidget);
  receiveMessage("Ready");	
  qDebug("*** dump object tree ***");
  qtapp.dumpObjectTree();
  mainwidget->dumpObjectTree();
#if 0
  dumpObjectTreeInfo(mainwidget);
#endif
  if (batchmode_)
		QTimer::singleShot(3000, this, SLOT(analyze()));
  app.exec();
  //semNet.write("./output.net");
}

int MainGui::error() {
  if (analysis_) return (analysis_->error() ? 1 : 0);
  return 0;
}

void MainGui::fileLoad() {
}

void MainGui::usage()
{
  printf ("\"gda\" graphic data analyser\n");
  printf ("usage: gda -p project-file.gap <-b batch-mode> <-m no-map> \n");
  exit(-1);
}

void MainGui::netLoad() {
#ifdef DEBUGMSG
  qDebug("Sem. Net File Load");
#endif
  propertyEditor->itemSelected(0);
  QString fname=QFileDialog::getOpenFileName("","*.net",mainwidget,"Load Net");
  if (fname.isEmpty()) return;
  fileSemanticNet_=fname;
  readyToAna(TRUE);  //enable the analysis buttons XXX only for testing
  treeEditor->rootNode(0);
  semNet.read(fileSemanticNet_);
  treeEditor->rootNode(semNet.rootNode());
}

void MainGui::resultSave() {
  if (!analysis_) {
    receiveMessage("Error: no instance net defined");
    return;
  }
#ifdef DEBUGMSG
  qDebug("MainGui::resultSave");
#endif
  propertyEditor->itemSelected(0);
  QString fname=QFileDialog::getSaveFileName(fileInstanceNet_,"*.inet",mainwidget,"Save Result");
  if (fname.isEmpty()) return;
  fileInstanceNet_=fname;
  QFileInfo finfo(fname);
  fileMap_=finfo.dirPath()+"/"+finfo.baseName(true)+".map";
  analysis_->writeInstanceNet(fileInstanceNet_);
  qDebug("MainGui::resultNetSave: mapname=%s",fileMap_.latin1());
  map_->replace("dir","");
  map_->replace("file",fileMap_);
  map_->write();
  treeEditor->rootNode(semNet.rootNode());
}

void MainGui::resultLoad() {
#ifdef DEBUGMSG
  qDebug("MainGui::resultLoad");
#endif
  propertyEditor->itemSelected(0);
  QString fname=QFileDialog::getOpenFileName("","*.inet",mainwidget,"Load Result");
  if (fname.isEmpty()) return;
  
  QFile fp(fname);
  if (!fp.open(IO_ReadOnly)) {
    qDebug("MainGui::resultLoad(%s): file not found",(const char*)fname);
    return;
  }
  // Clean up
  treeEditor->disconnectSlots();
  sceneViewer->disconnectSlots();
  sceneViewer->rootNode(0);
  trashViewer->disconnectSlots();
  trashViewer->rootNode(0);
  mapViewer->set(0,0);
  iNode_=0;
  map_=0;
  analysis_=0;
  readyToAna(FALSE);  //disable the analysis buttons
  interactive(FALSE); //disable the analysis buttons

  MLParser parser(&fp);
  ArgDict* args=parser.args();
  readInstanceNet(fname);
  treeEditor->connectSlots();
  sceneViewer->rootNode(iNode_);
  sceneViewer->connectSlots();
  if (iNode_) {
    float gN=iNode_->attributeFloat("geoNorth");
    float gE=iNode_->attributeFloat("geoEast");
    float gS=iNode_->attributeFloat("geoSouth");
    float gW=iNode_->attributeFloat("geoWest");
    QFileInfo finfo(fname);
    QString map=finfo.dirPath()+"/"+finfo.baseName(true)+".map";
    GeoImage* mapImage=new GeoImage(map,"Map",gW,gN,gE,gS);
    if (mapImage) {
      try {
        mapImage->load();
        mapViewer->set(iNode_,mapImage);
	if (geoImageList_.count())
	  mapViewer->setDataImage(geoImageList_[geoImageList_.list()[0]]);
      }
      catch (FatalError err) {
        delete mapImage;
        mapImage=0;
      }
    }
    fileInstanceNet_=fname;
    fileMap_=map;
  }
}

void MainGui::netSave() {
#ifdef DEBUGMSG
  qDebug("Sem. Net File Save");
#endif
  propertyEditor->itemSelected(0);
  QString fname=QFileDialog::getSaveFileName(fileSemanticNet_,"*.net",mainwidget,"Save Net");
  if (fname.isEmpty()) return;
  fileSemanticNet_=fname;
  semNet.write(fileSemanticNet_);
  //?	treeEditor->rootNode(semNet.rootNode());
}

void MainGui::prjLoad() {
#ifdef DEBUGMSG
  qDebug("Project File Load");
#endif
  propertyEditor->itemSelected(0);
  QString fname=QFileDialog::getOpenFileName("","*.gap",mainwidget,"Load Project");
  if (fname.isEmpty()) return;
  loadPrjFile(fname);
  
}
	
/** read an instance net from the given file */
void MainGui::readInstanceNet(QString fname)
{
  qDebug("Analysis::readInstancNet: file: %s",(const char*)fname);
  if (!semNet.rootNode()) {
    receiveMessage("Please load a project first");
    return;
  }
  QFile fp(fname);
  if (!fp.open(IO_ReadOnly)) {
    receiveMessage(QString().sprintf("Project %s not found",(const char*)fname));
    qDebug("GeoImageList::read(%s): file not found",(const char*)fname);
    return;
  }
	MLParser parser(&fp);
	int tag=parser.tag(nodeTagTable);
	if (tag==GNode::TOK_NODE) {
   	INode* rootNode=new INode(parser);
   if (!rootNode->linkSNode(semNet.rootNode())) {
      receiveMessage("Instance net does not fit in semantic net");
      delete rootNode;
      return;
    }
    iNode_=rootNode;
  }	
}

void MainGui::loadPrjFile(QString fname)
{
  fileProject_=fname;

  QFile fp(fname);
  if (!fp.open(IO_ReadOnly)) {
    qDebug("GeoImageList::read(%s): file not found",(const char*)fname);
    return;
  }
	_qtguiappl->widget()->setCaption("GeoAIDA ("+QFileInfo(fname).baseName()+")");
  // Clean up
  geoImageList_.clear();
  labelImageList_.clear();
  treeEditor->disconnectSlots();
  sceneViewer->disconnectSlots();
  sceneViewer->rootNode(0);
  trashViewer->disconnectSlots();
  trashViewer->rootNode(0);
  mapViewer->set(0,0);
  /*
 	if (analysis_) delete analysis_;
 	else {
  	if (iNode_) delete iNode_;
  	if (map_) delete map_;
  }
  */
  iNode_=0;
  map_=0;
  analysis_=0;
  readyToAna(FALSE);  //disable the analysis buttons
  interactive(FALSE); //disable the analysis buttons

  MLParser parser(&fp);
  QString keywords[]={"geoproject",
                      "geosemnet",
                      "geoimagelist",
                      "geoinstancenet",
		      "geoinstnethist",
		      "geoinstnetfut",
		      "geofilelist",
		      ""};
  enum {
    TOK_GEOPROJECT=1,
    TOK_GEOSEMNET,
    TOK_GEOIMAGELIST,
    TOK_GEOINSTANCENET,
    TOK_GEOINSTNETHIST,
    TOK_GEOINSTNETFUT,
    TOK_GEOFILELIST
  };
  const MLTagTable nodeTagTable(keywords);
  int tag, flag = 0;
  do {
    tag=parser.tag(nodeTagTable);
    switch (tag) {
    case TOK_GEOPROJECT: {
      flag = 1;
      break;
    } 
    case TOK_GEOSEMNET: {
      if (flag == 1) {
        ArgDict* args=parser.args();
        QString dir="", netfile="";
        MLParser::setString(netfile,args,"file");  //read tempor.
        MLParser::setString(dir,args,"dir");  //read tempor.
        fileSemanticNet_= dir + "/" + netfile;
        semNet.read(fileSemanticNet_);
        treeEditor->rootNode(semNet.rootNode());
        delete args;      
        }
      break;
    } 
    case TOK_GEOIMAGELIST: {
      if (flag == 1) { //da 'analysis' je Durchlauf neu erzeugt wird
        geoImageList_.read(parser);  //lesen wir das mal hier ein
      }
      break;
    } 
    case TOK_GEOFILELIST: {
      if (flag == 1) { 
        fileList_.read(parser);  
      }
      break;
    } 
    case TOK_GEOINSTANCENET: {
      if (flag == 1) {
        ArgDict* args=parser.args();
        QString dir="", netfile="";
        QString map="";
        MLParser::setString(netfile,args,"file");  //read tempor.
        MLParser::setString(dir,args,"dir");  //read tempor.
        MLParser::setString(map,args,"map");  //read tempor.
        dir += "/" + netfile;
        readInstanceNet(netfile);
        treeEditor->connectSlots();
        sceneViewer->rootNode(iNode_);
        sceneViewer->connectSlots();
        if (iNode_) {
          float gN=iNode_->attributeFloat("geoNorth");
          float gE=iNode_->attributeFloat("geoEast");
          float gS=iNode_->attributeFloat("geoSouth");
          float gW=iNode_->attributeFloat("geoWest");
          GeoImage* mapImage=new GeoImage(map,"Map",gW,gN,gE,gS);
          if (mapImage) {
            try {
              mapImage->load();
              mapViewer->set(iNode_,mapImage);
	      if (geoImageList_.count())
		mapViewer->setDataImage(geoImageList_[geoImageList_.list()[0]]);
            }
            catch (FatalError err) {
              delete mapImage;
              mapImage=0;
            }
          }
        }
        fileInstanceNet_=netfile;
        fileMap_=map;
        delete args;
      }
      break;
    }
	case TOK_GEOINSTNETHIST: {
      if (flag == 1) {
        ArgDict* args=parser.args();
        QString dir="", netfile="";
        QString map="";
        MLParser::setString(netfile,args,"file");  //read tempor.
        MLParser::setString(dir,args,"dir");  //read tempor.
        MLParser::setString(map,args,"map");  //read tempor.
        dir += "/" + netfile;
        readInstanceNet(netfile);
        treeEditor->connectSlots();
        sceneViewer->rootNode(iNode_);
        sceneViewer->connectSlots();
        if (iNode_) {
          float gN=iNode_->attributeFloat("geoNorth");
          float gE=iNode_->attributeFloat("geoEast");
          float gS=iNode_->attributeFloat("geoSouth");
          float gW=iNode_->attributeFloat("geoWest");
          GeoImage* mapImage=new GeoImage(map,"Map",gW,gN,gE,gS);
          if (mapImage) {
            try {
              mapImage->load();
              mapViewer->set(iNode_,mapImage);
	      if (geoImageList_.count())
		mapViewer->setDataImage(geoImageList_[geoImageList_.list()[0]]);
            }
            catch (FatalError err) {
              delete mapImage;
              mapImage=0;
            }
          }
        }
        fileInstanceNet_=netfile;
        fileMap_=map;
        delete args;
      }
      break;
    }
    default: {
      ArgDict* args=parser.args();
      delete args;
      break;
    }
    }
  } while ((tag!=MLParser::END_OF_FILE ) && (tag != -TOK_GEOPROJECT));
  // TEST
  // geoImageList_->part("VIS_1", 5580570.4, 5578235.5, 3402580.5, 3404340.6);
  // geoImageList_->part("VIS_2", 5580570.4, 5578235.5, 3402580.5, 3404340.6, "./TMPTMP.pfm");
  // geoImageList_->part("VIS_77", 5580570.4, 5578235.5, 3402580.5, 3404340.6);
  // TEST
  fp.close();        
  readyToAna(TRUE);  //enable the analysis buttons
}

/** start the analyze with the loaded sem. net */
void MainGui::analyze() {
  try {
    if (!semNet.rootNode() || (semNet.rootNode())->children().isEmpty()) {
      receiveMessage("No semantic net defined");
      qDebug("Start analyze - no semantic net, nothing to do");
      return;
    }
#ifdef DEBUGMSG
  	qDebug("Start analyze (mode: %d)", mode_);
#endif
    treeEditor->disconnectSlots();
    sceneViewer->disconnectSlots();
    sceneViewer->rootNode(0);
    trashViewer->disconnectSlots();
    trashViewer->rootNode(0);
    propertyEditor->setNode(0,0);
    sceneAttributes->setNode(0,0);
    trashAttributes->setNode(0,0);

    mapViewer->set(0,0);
   	if (analysis_) delete analysis_;
   	else {
    	if (iNode_) delete iNode_;
    	if (map_) delete map_;
    }
    analysis_=0;
    iNode_=0;
    map_=0;
  	interactive(TRUE);//enable some analysis buttons
  	readyToAna(FALSE);  //disable some analysis buttons
  	analysis_ = new Analysis(&semNet, &geoImageList_,&labelImageList_); //set the semantic net GeoImageList
  	connect(analysis_,SIGNAL(message(QString)),this,SLOT(receiveMessage(QString)));
  	connect(analysis_,SIGNAL(sigMapView(INode*,GeoImage*)),mapViewer,SLOT(set(INode*,GeoImage*)));
  	connect(analysis_,SIGNAL(sigInteractive(bool)),this,SLOT(slotInteractive(bool)));
  	opMode(mode_);
    if (no_map_)
      analysis_->map_mode(TRUE);

  	connect(analysis_, SIGNAL(sigReady(INode*,GeoImage*)), this, SLOT(slotAnalysisReady(INode*,GeoImage*)) );
    connect(analysis_, SIGNAL(sigNodeChange(INode*)), this, SLOT(slotNodeChange(INode*)) );

    if (batchmode_)
  	  connect(analysis_, SIGNAL(sigReady(INode*,GeoImage*)), qApp , SLOT (quit()) );

	//	start analysis
  	analysis_->start();	
  	emit interactive(FALSE);//disable some analysis buttons
  	emit readyToAna(TRUE);  //enable some analysis buttons
  }
  catch (FatalError err) {
  	emit interactive(FALSE);//disable some analysis buttons
  	emit readyToAna(TRUE);  //enable some analysis buttons
  	receiveMessage(err.message());
  }
}

void MainGui::opMode(int m){
#ifdef DEBUGMSG
	qDebug("###  MainGui::opMode (%d)",m);
#endif
	mode_ = m;
	if (!analysis_) return;
	if (m == 0) {
	  analysis_->stepwise(FALSE);
	  analysis_->synchron(FALSE);
	} else if (m == 1) {
	  analysis_->stepwise(TRUE);
	  analysis_->synchron(FALSE);	
	}else if (m == 2) {
	  analysis_->stepwise(FALSE);
	  analysis_->synchron(TRUE);	
	} else if (m == 3) {
	  analysis_->stepwise(TRUE);
	  analysis_->synchron(TRUE);	
	}
}

int MainGui::step(){ // Step button pressed
#ifdef DEBUGMSG
	qDebug("###  MainGui::step");
#endif
	wait_ = FALSE;
	return 1;
}

int MainGui::stop(){ // Stop button pressed
#ifdef DEBUGMSG
	qDebug("###  MainGui::stop");
#endif
	wait_ = TRUE;
	if (analysis_) analysis_->stepwise(TRUE);
  return 1;
}

void MainGui::proceed(){ // Continue button pressed
#ifdef DEBUGMSG
	qDebug("###  MainGui::continue");
#endif
	wait_ = FALSE;
	if (analysis_) analysis_->stepwise(FALSE);
}

/** slot for the ready signal
  * write the result in "result.net" */
void MainGui::slotAnalysisReady(INode* iNode, GeoImage* map) {
  iNode_ = iNode;
  map_=map;
  if (!fileInstanceNet_.isEmpty() && iNode_) {
	  QFile fp(fileInstanceNet_);
    fp.open(IO_WriteOnly);
	  QTextStream str(&fp);
	  iNode_->write(str);
	  fp.close();
  }
  if (!fileMap_.isEmpty() && map_) {
    map_->replace("dir","");
    map_->replace("file",fileMap_);
    map_->write();
  }
  treeEditor->connectSlots();
  sceneViewer->rootNode(iNode_);
  sceneViewer->connectSlots();
  trashViewer->rootNode(analysis_->trashNode());
  trashViewer->connectSlots();
  mapViewer->set(iNode_,map_);
  if (geoImageList_.count())
    mapViewer->setDataImage(geoImageList_[geoImageList_.list()[0]]);
  QFile fp("result.net");
  fp.open(IO_WriteOnly);
	QTextStream str(&fp);
	iNode_->write(str);
	fp.close();
	
}

/** slot become a signal if an INode in the instace net is change */
void MainGui::slotNodeChange(INode* iNode){ //iNode = root node
#ifdef DEBUGMSG
  qDebug("##  MainGui::slotNodeChange step: %d, syn: %d\n",analysis_->stepwise(),analysis_->synchron());
#endif
  iNode_ = iNode;
  sceneViewer->rootNode(iNode_);
  //qDebug("QQQQQ %s %s",((iNode_->sNode())->attribute("debug")).latin1(),((iNode_->sNode())->attribute("name")).latin1());
  if (analysis_->stepwise())
    while (wait_)
      qApp->processOneEvent();
//!      qApp->processEvents(200);
  wait_ = true;
}

void MainGui::showResult() {	
  sceneViewer->rootNode(iNode_);
  sceneViewer->rootNode(analysis_->trashNode());
  QFile fp("result.net");
  fp.open(IO_WriteOnly);
	QTextStream str(&fp);
	iNode_->write(str);
	fp.close();
}

void MainGui::start() {
#ifdef DEBUGMSG
	qDebug("MainGui::Start\n");
#endif
	propertyEditor->itemSelected(0);
	
	INode *iNode=new INode(semNet.rootNode());
	{
		INode *node=new INode(semNet.rootNode());
		node->name("node1");
		iNode->childLink(node);
	}
	{
		INode *node=new INode(semNet.rootNode());
		node->name("node2");
		iNode->childLink(node);
	}
	{
		INode *node=new INode(semNet.rootNode());
		node->name("node3");
		iNode->childLink(node);
	}
	{
		INode *node=new INode(semNet.rootNode());
		node->name("node4");
		iNode->childLink(node);
  }
	semNet.rootNode()->execTopDownOp(iNode);
	taskTable.wait(0);
	semNet.rootNode()->evalTopDown(iNode);
	semNet.rootNode()->execBottomUpOp(iNode);
	taskTable.wait(0);
	semNet.rootNode()->evalBottomUp(iNode);
}

MainGui::~MainGui()
{
}

/** Messages received by this slot are emitted with message() */
void MainGui::receiveMessage(QString msg)
{
  emit message(msg,10000);
}

/** Remove temporary files */
void MainGui::cleanUp()
{
  cleanUp_.deleteFiles();
}

/** enable / disable button (step, continue,stop) */
void MainGui::slotInteractive(bool b){
  emit interactive(b);
}

static void setLabelImage(INode* root, QString indent="")
{
  if (!root) return;
  qDebug("%s%s",indent.latin1(),root->name().latin1());
  root->setGeoRegion(root->attributeFloat("file_geoWest"),
                     root->attributeFloat("file_geoNorth"),
                     root->attributeFloat("file_geoEast"),
                     root->attributeFloat("file_geoSouth"));
  QPtrList<INode>& children=root->children();
  if (children.count()==0) return;
  indent+=" ";
  for (INode *in=children.first(); in!=0; in=children.next())
    setLabelImage(in,indent);
}


/** Testfunktion */
void MainGui::test()
{
  try {
    if (!semNet.rootNode() || (semNet.rootNode())->children().isEmpty()) {
      receiveMessage("No semantic net defined");
      qDebug("Start analyze - no semantic net, nothing to do");
      return;
    }
#ifdef DEBUGMSG
  	qDebug("Start analyze (mode: %d)", mode_);
#endif
#if 0
    treeEditor->disconnectSlots();
    sceneViewer->disconnectSlots();
    sceneViewer->rootNode(0);
    trashViewer->disconnectSlots();
    trashViewer->rootNode(0);
    mapViewer->set(0,0);
#endif
   	if (analysis_) delete analysis_;
    analysis_=0;
  	interactive(TRUE);//enable some analysis buttons
  	readyToAna(FALSE);  //disable some analysis buttons
  	analysis_ = new Analysis(&semNet, &geoImageList_,&labelImageList_); //set the semantic net GeoImageList
  	connect(analysis_,SIGNAL(message(QString)),this,SLOT(receiveMessage(QString)));
  	connect(analysis_,SIGNAL(sigMapView(INode*,GeoImage*)),mapViewer,SLOT(set(INode*,GeoImage*)));
  	connect(analysis_,SIGNAL(sigInteractive(bool)),this,SLOT(slotInteractive(bool)));
  	opMode(mode_);

  	connect(analysis_, SIGNAL(sigReady(INode*,GeoImage*)), this, SLOT(slotAnalysisReady(INode*,GeoImage*)) );
    connect(analysis_, SIGNAL(sigNodeChange(INode*)), this, SLOT(slotNodeChange(INode*)) );
  	analysis_->genGoal(iNode_);
    setLabelImage(iNode_);
    analysis_->prepareResultImage();
  }
  catch (FatalError err) {
  	emit interactive(FALSE);//disable some analysis buttons
  	emit readyToAna(TRUE);  //enable some analysis buttons
  	receiveMessage(err.message());
  }
}

/** Save the display map */
void MainGui::mapSave()
{
#ifdef DEBUGMSG
	qDebug("MapViewer File Save");
#endif
	QString fname=QFileDialog::getSaveFileName("","*.ppm",mainwidget,"Save Map");
	if (fname.isEmpty()) return;
	mapViewer->save(fname,"PPM");
}
