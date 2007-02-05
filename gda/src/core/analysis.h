/***************************************************************************
                          analysis.h  -  description
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
 * $Source: /data/cvs/gda/gda/core/analysis.h,v $
 * $Revision: 1.13 $
 * $Date: 2005/10/14 07:38:31 $
 * $Author: mueller $
 * $Locker:  $
 */

#ifndef ANALYSIS_H
#define ANALYSIS_H

#include <qobject.h>
#include "inode.h"
#include "semnet.h"
#include "geoimagelist.h"

/**class to control the analysis and handle the search tree node
  *@author Jürgen Bückner
  */

class Analysis:public QObject
{
Q_OBJECT public:
  /** default constructor */
  Analysis();

        /** read sem net from file */
  Analysis(const QString & fname);

        /** initialization with "Semanic Net"  */
  Analysis(SemNet *sn, GeoImageList *gil, GeoImageList *lil);

        /** initialization with "Semanic Net"  */
  Analysis(SemNet * sn);

  /** destructor */
  ~Analysis();

  /** set default values*/
  void init(void);

  /** return a pointer to the "Semantic Net" */
  SemNet *semNet(void)
  {
    return semNet_;
  };

  /** set the pointer to the "Semantic Net" */
  void semNet(SemNet * sn)
  {
    semNet_ = sn;
  };

  /** calculate the judgement for the whole interpretation */
  void calcJudge(void);

  /** generate the first instance, the goal instance.
      It is the root node of the "Semantic Net"  = RULE_1 */
  int genGoal(INode* iNode=0);
  /** function to start the analysis */
  void start();
  /** called when the analyze is ready  */
  void ready();
  /** is called when a INode was change or created */
  void nodeChange(INode *);
  /** Sets the operator call for the analysis */
  void synchron(bool b);
  /** Get the operator call for the analysis */
  bool synchron(void) const
  {
    return synchron_;
  };
  /** Sets the operator call for the analysis */
  void stepwise(bool b)
  {
    stepwise_ = b;
  };
  /** Sets the operator call for the analysis */
  void map_mode(bool b)
  {
    no_map_ = b;
  };
  
  /** Get the operator call for the analysis */
  bool stepwise(void) const
  {
    return stepwise_;
  };
  /** Sets the GeoImageList for the analysis */
  void geoImageList(GeoImageList * gil)
  {
    geoImageList_ = gil;
  };
  /** Returns the GeoImageList for the analysis */
  GeoImageList *geoImageList(void)
  {
    return geoImageList_;
  };
  /** Sets the GeoImageList for the analysis */
  void labelImageList(GeoImageList * lil)
  {
    labelImageList_ = lil;
  };
  /** Returns the GeoImageList for the analysis */
  GeoImageList *labelImageList(void)
  {
    return labelImageList_;
  };
  /** read the project file. The semantic net and the image description*/
  void readProject(QString fname);
  /** read the semantic net */
  void readSemNet(QString fname);
  /** write the semantic net to the given file */
  void writeSemNet(QString fname);
  /** write the instance net to the given file */
  void writeInstanceNet(QString fname);
  /** return the geo image list - a QStringList with the keys of the images */
  QStringList imagelist();
  /** Prepare the result image with the given instantiated net and the corresponding label image tiles */
  void prepareResultImage();
  /** Sets the trash node for the scene viewer */
  void setTrashRoot(INode * trashNode);
  /** returns the trash root node */
  INode *trashNode();
  /** returns the error state of the analysis */
  bool error();
  /** sets the error state of the analysis */
  void setError(bool state);
protected:                     // Protected members
  /** pointer to the root INode of the INode tree for this search tree node  */
  INode * iNodeRoot_;
  /** pointer to the root INode of the INode tree for this search tree node  */
  INode *trashNodeRoot_;
  /** pointer to the root of the semantic net */
  SemNet *semNet_;
  /** pointer to the result map */
  GeoImage *map_;
  /** pointer to the infos for the used input images */
  GeoImageList *geoImageList_;
  /** pointer to the infos for the generated label images */
  GeoImageList *labelImageList_;
  /** TRUE for synchron analyze working otherwise FALSE (default: FALSE). Set the member of SNode. */
  bool synchron_;
  /** TRUE for stepwise  analyze working otherwise FALSE (default: FALSE)  */
  bool stepwise_;
  /** TRUE if no result map wished (default: FALSE)  */
  bool no_map_;
  /** TRUE if there was an error while executing an operator */
  bool error_;
  public slots:                 // Public slots
  /** start the analysis, the semantic net must be loaded */
  void slotstart();

signals:                       // Signals
  /** emitted signal when the analyze is ready  */
  void sigReady(INode *, GeoImage *);
  /** emitted signal when a node in the instances node was changes */
  void sigNodeChange(INode *);
  void sigMapView(INode *, GeoImage *);
  /** message from the analysis */
  void message(QString);
  /** enable / disable button (step, continue,stop) in GUI  */
  void sigInteractive(bool);
};

inline bool Analysis::error()
{
  return error_;
}

#endif
