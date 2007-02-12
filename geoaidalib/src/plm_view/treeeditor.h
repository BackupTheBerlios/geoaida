/***************************************************************************
                          treeedit.h  -  description
                             -------------------
    begin                : Wed Sep 6 2000
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
 * $Source: /data/cvs/geoaidalib/geoaida/plm_view/treeeditor.h,v $
 * $Revision: 1.2 $
 * $Date: 2002/06/10 05:49:28 $
 * $Author: pahl $
 * $Locker:  $
 */

#ifndef TREEEDITOR_H
#define TREEEDITOR_H

#include <qlistview.h>
#include "gnode.h"

/**Editor for Semantic net (tree)
  *@author Martin Pahl
  */

class TreeItem;

class TreeEditor:public QListView
{
Q_OBJECT
public:
  TreeEditor(QWidget * parent = 0, const char *name = 0);
   ~TreeEditor();
  /** Set the rootNode_ of the semantic net */
  void rootNode(GNode * node);
  void connectSlots();
  void disconnectSlots();
protected:                     // Protected attributes
  /** Root Node of the semantic net */
  /** Clipboard */
  GNode * clipboardNode_;
  GNode *rootNode_;
  bool itemFound_;
  /** if true do no view updates */
  bool noUpdate_;
  /** maximum depth to show */
  int maxDepth_;
public slots:                 // Public slots
  /** slot to popup a menu for the selected item */
  void popupMenu(QListViewItem * item, const QPoint & point, int column);
  /** Slot should be called, whenever the selecte item changes */
  void itemSelected(QListViewItem * item);
  /** No descriptions */
  void setCurrentNode(GNode * node = 0);
signals:                    // Signals
  /** Signal is emitted, whenever the selected item changes */
  void itemChanged(TreeItem * listViewItem, GNode * node);
  /** No descriptions */
  void treeChanged();
protected slots:              // Protected slots
  /** No descriptions */
  void showDetails(QListViewItem *);
  /** No descriptions */
  void hideDetails(QListViewItem *);
  /** No descriptions */
protected:                     // Protected methods
  void searchNode(TreeItem * startItem, GNode * searchNode);
  /** Recursivly set the maximum depth to show */
  void setMaxDepth(TreeItem* root, int depth, bool set);
};

#endif
