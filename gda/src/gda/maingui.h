/***************************************************************************
                          maingui.h  -  description
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
 * $Source: /data/cvs/gda/gda/maingui.h,v $
 * $Revision: 1.28 $
 * $Date: 2005/10/14 07:38:31 $
 * $Author: mueller $
 * $Locker:  $
 */

#ifndef MAINGUI_H
#define MAINGUI_H

#include <qobject.h>
#include <analysis.h>

/**Contains the main class
  *@author Martin Pahl
  */

#include "QtGuiApplication.h"
#include <qmainwindow.h>
#include <qapp.h>
#include <qtextedit.h>
#include "treeeditor.h"
#include "treeitem.h"
#include "propertyeditor.h"
#include "mapviewer.h"
#include "taskviewer.h"
#include "semnet.h"
#include "geoimagelist.h"


class MainGui : public QObject
{
  Q_OBJECT
public:
	MainGui(int,char**);
	~MainGui();
	
public slots:
  void fileLoad();
  void netLoad();
  void netSave();
  void prjLoad();
  void resultSave();
  void resultLoad();
  void usage();
  void start();
  /** anaysis button */
  void analyze();
	void showResult();
  /** slot for the ready signal */
  void slotAnalysisReady(INode*,GeoImage*);
  /** slot become a signal if an INode in the instace net is change */
  void slotNodeChange(INode*);
  /** set operator mode */
  void opMode(int m);
  /** step button */
  int  step();
  /** stop button */
  int  stop();
  /** continuous button */
  void proceed();
  /** Messages received by this slot are emitted with message() */
  void receiveMessage(QString);
  /** Remove temporary files */
  void cleanUp();
  /** enable / disable button (step, continue,stop) */
  void slotInteractive(bool);
  /** Testfunktion */
  void test();
  /** Save the display map */
  void mapSave();

public:
  SemNet semNet;
  TreeEditor* treeEditor;
  PropertyEditor *propertyEditor;
  /** filename for the semantic net */
  QString fileSemanticNet_;
  /** filename for the project description */
  QString fileProject_;
  /** filename for the instance net */
  QString fileInstanceNet_;
  /** filename for the map */
  QString fileMap_;
  QMainWindow *mainwidget;
  TreeEditor *sceneViewer;
  TreeEditor *trashViewer;
  MapViewer *mapViewer;
  MapViewer *debugViewer;
  PropertyEditor *sceneAttributes;
  PropertyEditor *trashAttributes;
  TaskViewer* taskViewer;
  QTextEdit* formulaEditor;
protected: // Protected attributes
  /**  */
  INode* iNode_;
  /** map image */
  GeoImage* map_;
  /** pointer to the analysis class */
  Analysis *analysis_;
//  /** pointer to the infos for the used input images */
//  GeoImageList* geoImageList_; geoImageList_ now is a global pointer
  /** analysis modus: 1) stepwise = FALSE, synchron = FALSE;  2) stepwise = FALSE, synchron = TRUE; 3) stepwise = TRUE, synchron = TRUE; */
  int mode_;
  /** flag for stepwise analysis */
  bool batchmode_;
  /** flag if no result map wished */
  bool no_map_;  
  /** start analysis and quit automatic */
  bool wait_;
  /** read an instance net from the given file */
  void readInstanceNet(QString fname);
  /** load a project file */
  void loadPrjFile(QString);

signals: // Signals
  /** make some buttons disable {Analysis, Strategy-Selector} */
  void readyToAna(bool);
  /** make some buttons disable {Step, Continue, Stop} */
  void interactive(bool);
  /** message from geoaida. Can be use by the GUI */
  void message(QString,int);
};

#endif
