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
 * $Source: /data/cvs/geoaidalib/geoaida/plm_view/maingui.h,v $
 * $Revision: 1.7 $
 * $Date: 2003/01/09 15:37:35 $
 * $Author: pahl $
 * $Locker:  $
 */

#ifndef MAINGUI_H
#define MAINGUI_H

#include <qobject.h>

/**Contains the main class
  *@author Martin Pahl
  */

#include "QtGuiApplication.h"
#include <qmainwindow.h>
#include <qapp.h>
#include <gaimage.h>
#include <qfile.h>
#include "treeeditor.h"
#include "treeitem.h"
#include "propertyeditor.h"
#include "mapviewer.h"

class MainGui : public QObject
{
  Q_OBJECT
public:
	MainGui(int,char**);
	~MainGui();
	/** Read a semantic class net  */
	void readClassNet(QIODevice & fp);
	/** Read a semantic class net by using a filename */
	void readClassNet(const QString & fname);
	/** Read a semantic net  */
	void readNet(QIODevice & fp);
	/** Read a semantic net by using a filename */
	void readNet(const QString & fname);
	/** Read a map by using a filename */
	void readMap(const QString & fname);
	/** Read a data image by using a filename */
	void readDataImage(const QString & fname);
	
protected: // Protected attributes
  TreeEditor* treeEditor_;
  TreeEditor* classTree_;
  PropertyEditor *propertyEditor_;
  MapViewer *mapViewer_;
  QMainWindow* mainwidget_;
  QString labelfile_;
  QString netfile_;
  QString classnetfile_;
  QString datafile_;
  /**  */
  GNode* rootNode_;
  /**  */
  GNode* classRootNode_;
  bool classNetLoaded_;
  /** map image */
  Ga::Image* map_;
  /** data image */
  Ga::Image* dataimage_;
public slots: // Public slots
  /** load a new data image */
  void loadDataImage();
  /** load a new map */
  void loadMap();
  /** load a new net */
  void loadNet();
  /** load a new class net */
  void loadClassNet();
  void clearClassNet();
};

#endif
