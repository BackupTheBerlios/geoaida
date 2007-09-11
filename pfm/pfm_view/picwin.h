/***************************************************************************
                          pic_win.h  -  description
                             -------------------
    begin                : Fri Sep 1 2000
    copyright            : (C) 2000 by jürgen bückner
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
/* -*- mode:c++ -*- */


#ifndef _pic_win_H
#define _pic_win_H

//------------------------------------
// #####       RCS - Info:       #####
//------------------------------------

//static char rcsid[] = 
//  "$$";

//------------------------------------
// #####        #includes:       #####
//------------------------------------
// QT include ......
#include <qmainwindow.h>
#include <qscrollview.h>
#include <qimage.h>
#include <qpixmap.h>
#include <qpicture.h>
#include <qmenubar.h>
#include <qfiledialog.h>
#include <qmessagebox.h>
#include <qpopupmenu.h>
#include <qlabel.h>
#include <qpainter.h>
#include <qkeycode.h>
#include <qapplication.h>
#include <qlayout.h>
#include <qpushbutton.h>
// PFM include ......
#include "pfm.h"
// other include ......
//#include <ostream.h>
#include <assert.h>
// local include ......
#include "combislider.h"

//------------------------------------
// #####        #defines:        #####
//------------------------------------

//------------------------------------
// #####        #typedefs:       #####
//------------------------------------
class QLabel;
class QMenuBar;
class QPopupMenu;
class Plot2D;

//------------------------------------
// #####    #global variable:    #####
//------------------------------------

//------------------------------------
// #####        #macros:         #####
//------------------------------------

//------------------------------------
// #####  #routine definitions:  #####
//------------------------------------

//------------------------------------
// #####   #class definitions:   #####
//------------------------------------

class PicWin : public QMainWindow
{
    Q_OBJECT
public:
    PicWin( QWidget *parent=0, const char *name=0, int wFlags=0 );
    ~PicWin();
    bool  loadImage( const char *fileName );

public slots:
  void  scaleImgSize(float, float);
  /** set the contrast by evaluating the min and max values from the selected region */
  void autoContrast();
  /** No descriptions */
  void histogram();
  /** No descriptions */
  void selectAll();
  /** No descriptions */
  void shape();
  /** shift the image by one column (cylindrical) */
  void shiftImage();
  /** histogram for each sun dir */
  void sundir();

protected slots:
    void  myPaintEvent( QPaintEvent * );
    void  myResizeEvent( QResizeEvent * );
    void  myMousePressEvent( QMouseEvent * );
    void  myMouseReleaseEvent( QMouseEvent * );
    void  myMouseMoveEvent( QMouseEvent * );

private:
    void     scale();
    void     decompress(char* filename, int& compressed);
    int      conversion_flags;
    char    *filename;
    bool     smooth() const;
    bool     useColorContext() const;
    int      alloc_context;
    bool     convertEvent( QMouseEvent* e, int& x, int& y );
    QImage   image;       // the loaded image
    QPixmap  pm;          // the converted pixmap
    QPixmap  pmScaled;    // the scaled pixmap
    void     loadpfm(char*);
    float   *pfm_data; //PFM data
    float    pfm_min_val; //PFM min value
    float    pfm_max_val; //PFM max value
    
    QMenuBar    *menubar;
    QPopupMenu  *file;
    QPopupMenu  *options;
    QPopupMenu  *editimg, *editsize;
    QPopupMenu  *saveimage;
    QPopupMenu  *savepixmap;
    QWidget     *helpmsg;
    QLabel      *status;
    QScrollView *scrollView;
    QLabel      *paintWidget;
    int         si, sp, ac, co, mo, fd, bd, // Menu item ids
    td, ta, ba, fa, au, ad, dd,
    ss, cc;
    void  updateStatus();
    void  setMenuItemFlags();
    bool   reconvertImage();
    int    pickx, picky;
    int    clickx, clicky;
    int llx,lly,urx,ury;
    bool  may_be_other;
    static PicWin* other;
    ButtonState button;
    /** histogramm of image area */
    QPicture histogramm_;
    Plot2D* histDialog_;
    QPicture shape_;
    Plot2D* shapeDialog_;
  
private slots:
  void  newWindow();
  void  openFile();
  void  reload();
  void  saveImage(int);
  void  savePixmap(int);
  void  editImg();
  void  absImg();
  void  ScaleSizeImg() {scaleImgSize(1.0,1.0);}
  void  ScaleSizeImg016() {scaleImgSize(0.0625,0.0625);}
  void  ScaleSizeImg08() {scaleImgSize(0.125,0.125);}
  void  ScaleSizeImg04() {scaleImgSize(0.25,0.25);}
  void  ScaleSizeImg02() {scaleImgSize(0.5,0.5);}
  void  ScaleSizeImg2() {scaleImgSize(2.0,2.0);}
  void  ScaleSizeImg4() {scaleImgSize(4.0,4.0);}
  void  ScaleSizeImg8() {scaleImgSize(8.0,8.0);}
  void  ScaleSizeImg16() {scaleImgSize(16.0,16.0);}
  void  rndColImg();
  void  circularColorImg();
  void  origColImg();
  void  slotScaleImg(float, float);
  void  giveHelp();
  void  doOption(int);
  void  close();
  void  quit();

  signals:
  void  closePic(PicWin*);

};

// -----------------------------------------------------------------
class EditImg : public QDialog
{
  Q_OBJECT
public:
  EditImg( QWidget *parent=0, const char *name=0, float min_ = 0.0, float max_ = 256.0);

private:
  QBoxLayout *layout;
  QVBoxLayout *mainList;
  combiSlider *slider;
  QHBoxLayout *footer;
  QPushButton* cancel;
  QPushButton* apply;

private slots:
  void  slotGetVal();

signals:
  void  sigVal(float, float);

};

#endif 
