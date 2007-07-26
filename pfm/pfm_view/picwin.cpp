/***************************************************************************
                          pic_win.cpp  -  description
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

//#define DEBUG_PRG

#include "picwin.h"
#include <float.h>
#include <cmath>
#include <cstdlib>
#include <qstatusbar.h>
#include "paintwidget.h"
#include "plot2d.h"
#include <iostream>
#include "icons/mini-junktion.xpm"
#include "icons/tnt.xpm"
using namespace std;

static void convertLabel(QImage& img, float* data, float min=0, float max=255);
static void convertScalar(QImage& img, float* data, float min=0, float max=255);
static void calcMinMax(const QImage &img, float *data, float &min, float &max);

inline static void setPixel(QImage& image, int x, int y, int v)
{
    if (v<0)
      image.setPixel(x,y,0);
    else if (v>255)
      image.setPixel(x,y,255);
    else
      image.setPixel(x,y,v);
}

PicWin::PicWin( QWidget *parent, const char *name, int wFlags )
    : QMainWindow( parent, name, wFlags ),
      conversion_flags( PreferDither ),
      filename( 0 ),
      helpmsg( 0 ),
      image((const char**)tnt_xpm),
      pm((const char**)tnt_xpm),
      pmScaled((const char**)tnt_xpm),
      llx(0),lly(0),urx(0),ury(0)
{
    pickx = -1;
    clickx = -1;
    alloc_context = 0;

    pfm_data = (float*)NULL;
    pfm_min_val= 0.0;
    pfm_max_val= 1.0;

    filename = new char[300];

    setIcon(QPixmap((const char**)sicon));
    setIconText("Picture Viewer");
    setName("Picture Viewer");
    setCaption("Picture Viewer");

    menubar = new QMenuBar(this);
    menubar->setSeparator( QMenuBar::InWindowsStyle );

    histDialog_=0;
    shapeDialog_=0;

    QStrList fmt = QImage::outputFormats();
    saveimage = new QPopupMenu();
    savepixmap = new QPopupMenu();
    for (const char* f = fmt.first(); f; f = fmt.next()) {
        saveimage->insertItem( f );
        savepixmap->insertItem( f );
    }
    connect( saveimage, SIGNAL(activated(int)), this, SLOT(saveImage(int)) );
    connect( savepixmap, SIGNAL(activated(int)), this, SLOT(savePixmap(int)) );
 
    file = new QPopupMenu();
    menubar->insertItem( "&File", file );
    file->insertItem( "New window", this,  SLOT(newWindow()), CTRL+Key_N );
    file->insertItem( "Open...", this,  SLOT(openFile()), CTRL+Key_O );
    file->insertItem( "Reload", this,  SLOT(reload()), Key_Return );
     si = file->insertItem( "Save orig image", saveimage );
    sp = file->insertItem( "Save modified image", savepixmap );
    file->insertSeparator();
    file->insertItem( "Close (this)", this,  SLOT(close()), CTRL+Key_C );
    file->insertItem( "Quit (all)", this,  SLOT(quit()), CTRL+Key_Q);

    options =  new QPopupMenu();
    menubar->insertItem( "&Options", options );
    ac = options->insertItem( "AutoColor" );
    co = options->insertItem( "ColorOnly" );
    mo = options->insertItem( "MonoOnly" );
    options->insertSeparator();
    fd = options->insertItem( "DiffuseDither" );
    bd = options->insertItem( "OrderedDither" );
    td = options->insertItem( "ThresholdDither" );
    options->insertSeparator();
    ta = options->insertItem( "ThresholdAlphaDither" );
    ba = options->insertItem( "OrderedAlphaDither" );
    fa = options->insertItem( "DiffuseAlphaDither" );
    options->insertSeparator();
    ad = options->insertItem( "PreferDither" );
    dd = options->insertItem( "AvoidDither" );
    options->insertSeparator();
    ss = options->insertItem( "Smooth scaling" );
    cc = options->insertItem( "Use colour context" );
    options->insertSeparator();
    options->insertItem( "SelectAll", this, SLOT(selectAll()), CTRL+Key_S);
    if ( QApplication::colorSpec() == QApplication::ManyColor )
    options->setItemEnabled( cc, FALSE );
    options->setCheckable( TRUE );
    setMenuItemFlags();

    editimg =  new QPopupMenu();
    menubar->insertItem( "&Color", editimg );
    editimg->insertItem( "Orig. Color" , this,  SLOT(origColImg()), CTRL+Key_N);
    editimg->insertItem( "Change contrast" , this,  SLOT(editImg()), CTRL+Key_E);
    editimg->insertItem( "Auto Contrast", this, SLOT(autoContrast()), Key_A);
    editimg->insertItem( "Use absolute value" , this,  SLOT(absImg()), CTRL+Key_A);
    editimg->insertItem( "Random Color" , this,  SLOT(rndColImg()), Key_R);
    editimg->insertItem( "Circular Colormap" , this,  SLOT(circularColorImg()), Key_C);
    editimg->insertItem( "Histogram", this, SLOT(histogram()), Key_H);
    editimg->insertItem( "Shape", this, SLOT(shape()), Key_S);
    editimg->setEnabled(FALSE);
    
    editsize =  new QPopupMenu();
    menubar->insertItem( "&Scale size", editsize );
    editsize->insertItem( "Orig /  16", this,  SLOT(ScaleSizeImg016()));
    editsize->insertItem( "Orig /  8" , this,  SLOT(ScaleSizeImg08()), CTRL+Key_8);
    editsize->insertItem( "Orig /  4" , this,  SLOT(ScaleSizeImg04()), CTRL+Key_4 );
    editsize->insertItem( "Orig /  2" , this,  SLOT(ScaleSizeImg02()), CTRL+Key_2);
    editsize->insertItem( "Orig     " , this,  SLOT(ScaleSizeImg()), Key_1);
    editsize->insertItem( "Orig *  2" , this,  SLOT(ScaleSizeImg2()), Key_2);
    editsize->insertItem( "Orig *  4" , this,  SLOT(ScaleSizeImg4()), Key_4);
    editsize->insertItem( "Orig *  8" , this,  SLOT(ScaleSizeImg8()), Key_8);
    editsize->insertItem( "Orig * 16" , this,  SLOT(ScaleSizeImg16()));
    
   editsize =  new QPopupMenu();
    menubar->insertItem( "Special", editsize );
    editsize->insertItem( "Shift Image" , this,  SLOT(shiftImage()), Key_Space);
    editsize->insertItem( "Sun Direction" , this,  SLOT(sundir()), Key_D);
    editimg->setEnabled(FALSE);

    menubar->insertSeparator();

    QPopupMenu* help = new QPopupMenu();
    menubar->insertItem( "&Help", help );
    help->insertItem( "Help!", this, SLOT(giveHelp()), CTRL+Key_H );

    connect( options, SIGNAL(activated(int)), this, SLOT(doOption(int)) );

    QStatusBar* sb=new QStatusBar(this);
    status = new QLabel(sb);
    sb->addWidget(status,true);
    status->setFrameStyle( QFrame::WinPanel | QFrame::Sunken );
    status->setFixedHeight( fontMetrics().height() + 4 );

    scrollView=new QScrollView(this);
    paintWidget=new PaintWidget();
    scrollView->addChild(paintWidget);
    paintWidget->setMouseTracking( TRUE );
    scrollView->setMouseTracking(TRUE);
    connect(paintWidget,SIGNAL(mouseMoved(QMouseEvent*)),this,SLOT(myMouseMoveEvent(QMouseEvent*)));
    connect(paintWidget,SIGNAL(mousePressed(QMouseEvent*)),this,SLOT(myMousePressEvent(QMouseEvent*)));
    connect(paintWidget,SIGNAL(mouseReleased(QMouseEvent*)),this,SLOT(myMouseReleaseEvent(QMouseEvent*)));
    connect(paintWidget,SIGNAL(widgetResized(QResizeEvent*)),this,SLOT(myResizeEvent(QResizeEvent*)));
    connect(paintWidget,SIGNAL(sigPaint(QPaintEvent*)),this,SLOT(myPaintEvent(QPaintEvent*)));
    scrollView->show();
    setCentralWidget(scrollView);
    //pm.loadFromData((const char**)tnt_xpm);

}

PicWin::~PicWin()
{
    if ( alloc_context )
  QColor::destroyAllocContext( alloc_context );
    if ( other == this )
  other = 0;
}

/*
  This function modifies the conversion_flags when an options menu item
  is selected, then ensures all menu items are up to date, and reconverts
  the image if possibly necessary.
*/
void PicWin::doOption(int item)
{
    if ( item == ss || item == cc ) {
  // Toggle
  bool newbool = !options->isItemChecked(item);
  options->setItemChecked(item, newbool);
  // And reconvert...
  reconvertImage();
  repaint(image.hasAlphaBuffer());  // show image in widget
  return;
    }

    if ( options->isItemChecked( item ) ) return; // They are all radio buttons

    int ocf = conversion_flags;

    if ( item == ac ) {
  conversion_flags = conversion_flags & ~ColorMode_Mask | AutoColor;
    } else if ( item == co ) {
  conversion_flags = conversion_flags & ~ColorMode_Mask | ColorOnly;
    } else if ( item == mo ) {
  conversion_flags = conversion_flags & ~ColorMode_Mask | MonoOnly;
    } else if ( item == fd ) {
  conversion_flags = conversion_flags & ~Dither_Mask | DiffuseDither;
    } else if ( item == bd ) {
  conversion_flags = conversion_flags & ~Dither_Mask | OrderedDither;
    } else if ( item == td ) {
  conversion_flags = conversion_flags & ~Dither_Mask | ThresholdDither;
    } else if ( item == ta ) {
  conversion_flags = conversion_flags & ~AlphaDither_Mask | ThresholdAlphaDither;
    } else if ( item == fa ) {
  conversion_flags = conversion_flags & ~AlphaDither_Mask | DiffuseAlphaDither;
    } else if ( item == ba ) {
  conversion_flags = conversion_flags & ~AlphaDither_Mask | OrderedAlphaDither;
    } else if ( item == ad ) {
  conversion_flags = conversion_flags & ~DitherMode_Mask | PreferDither;
    } else if ( item == dd ) {
  conversion_flags = conversion_flags & ~DitherMode_Mask | AvoidDither;
    }

    if ( ocf != conversion_flags ) {
  setMenuItemFlags();
  // And reconvert...
  reconvertImage();
  repaint(image.hasAlphaBuffer());  // show image in widget
    }
}

/*
  Set the options menu to reflect the conversion_flags value.
*/
void PicWin::setMenuItemFlags()
{
    // File
    bool valid_image = pm.size() != QSize( 0, 0 );
    file->setItemEnabled( si, valid_image );
    file->setItemEnabled( sp, valid_image );

    // Options
    bool may_need_color_dithering =
        !valid_image || image.depth() == 32 && QPixmap::defaultDepth() <= 8;
    bool may_need_dithering = may_need_color_dithering
     || image.depth() > 1 && options->isItemChecked(mo)
     || image.depth() > 1 && QPixmap::defaultDepth() == 1;
    bool has_alpha_mask = !valid_image || image.hasAlphaBuffer();

    options->setItemEnabled( fd, may_need_dithering );
    options->setItemEnabled( bd, may_need_dithering );
    options->setItemEnabled( td, may_need_dithering );

    options->setItemEnabled( ta, has_alpha_mask );
    options->setItemEnabled( fa, has_alpha_mask );
    options->setItemEnabled( ba, has_alpha_mask );

    options->setItemEnabled( ad, may_need_color_dithering );
    options->setItemEnabled( dd, may_need_color_dithering );

    options->setItemChecked( ac, (conversion_flags & ColorMode_Mask) == AutoColor );
    options->setItemChecked( co, (conversion_flags & ColorMode_Mask) == ColorOnly );
    options->setItemChecked( mo, (conversion_flags & ColorMode_Mask) == MonoOnly );
    options->setItemChecked( fd, (conversion_flags & Dither_Mask) == DiffuseDither );
    options->setItemChecked( bd, (conversion_flags & Dither_Mask) == OrderedDither );
    options->setItemChecked( td, (conversion_flags & Dither_Mask) == ThresholdDither );
    options->setItemChecked( ta, (conversion_flags & AlphaDither_Mask) == ThresholdAlphaDither );
    options->setItemChecked( fa, (conversion_flags & AlphaDither_Mask) == DiffuseAlphaDither );
    options->setItemChecked( ba, (conversion_flags & AlphaDither_Mask) == OrderedAlphaDither );
    options->setItemChecked( ad, (conversion_flags & DitherMode_Mask) == PreferDither );
    options->setItemChecked( dd, (conversion_flags & DitherMode_Mask) == AvoidDither );
}

void PicWin::updateStatus()
{
  if ( pm.size() == QSize( 0, 0 ) ) {
  if ( filename )
      status->setText("Could not load image");
  else
      status->setText("No image - select Open from File menu.");
    } else {
  QString message, moremsg;
  message.sprintf("%dx%d", image.width(), image.height());
  if ( pm.size() != pmScaled.size() ) {
      moremsg.sprintf(" [%dx%d]", pmScaled.width(),
    pmScaled.height());
      message += moremsg;
  }
  moremsg.sprintf(", %d bits ", image.depth());
  message += moremsg;
  if (image.valid(pickx,picky)) {
      moremsg.sprintf("(%d,%d)=#%0*x ",
        pickx, picky,
        image.hasAlphaBuffer() ? 8 : 6,
        image.pixel(pickx,picky));
      message += moremsg;
      if(pfm_data) {

#ifdef linux
int __isnanf(float);
#ifndef NAN
#define NAN sqrt(-1)
#endif
#endif
        float f = pfm_data[picky*image.width()+pickx];
        if (isnan(f)) moremsg.sprintf(", NaN");
        else moremsg.sprintf(", %f ", f);
        message += moremsg;
      }
  }
  if ( image.numColors() > 0 ) {
      if (image.valid(pickx,picky)) {
    moremsg.sprintf(", %d/%d colors", image.pixelIndex(pickx,picky),
        image.numColors());
      } else {
    moremsg.sprintf(", %d colors", image.numColors());
      }
      message += moremsg;
  }
  if ( image.hasAlphaBuffer() ) {
      if ( image.depth() == 8 ) {
    int i;
    bool alpha[256];
    int nalpha=0;

    for (i=0; i<256; i++)
        alpha[i] = FALSE;

    for (i=0; i<image.numColors(); i++) {
        int alevel = image.color(i) >> 24;
        if (!alpha[alevel]) {
      alpha[alevel] = TRUE;
      nalpha++;
        }
    }
    moremsg.sprintf(", %d alpha levels", nalpha);
      } else {
    // Too many pixels to bother counting.
    moremsg = ", 8-bit alpha channel";
      }
      message += moremsg;
  }
  status->setText(message);
    }
}

/*
  This function saves the image.
*/
void PicWin::saveImage( int item )
{
  const char* fmt = saveimage->text(item);
  QString savefilename = QFileDialog::getSaveFileName(0, 0, 0, filename);
  if ( !savefilename.isEmpty() )
  if ( !image.save( savefilename, fmt ) )
      QMessageBox::warning( this, "Save failed", "Error saving file" );
}

/*
  This function saves the converted image.
*/
void PicWin::savePixmap( int item )
{
  const char* fmt = savepixmap->text(item);
  QString savefilename = QFileDialog::getSaveFileName(0, 0, 0, filename);
  if ( !savefilename.isEmpty() )
  if ( !pmScaled.save( savefilename, fmt ) )
      QMessageBox::warning( this, "Save failed", "Error saving file" );
}


void PicWin::newWindow() {
    PicWin* that = new PicWin(0, "new window", WDestructiveClose);
    that->show();
}


void PicWin::loadpfm(char* filename) {
  FILE *infile;
  infile = fopen(filename,"r");
  if (!infile) {
       fprintf(stderr,"Can't open file %s for reading!\n", filename);
       return;
  }

 int cols, rows;
 pfm_data=pfm_readpfm(infile,&cols,&rows,&pfm_min_val,&pfm_max_val);
 qDebug("PicWin::loadpfm8 min=%f max=%f",pfm_min_val,pfm_max_val);
 fclose(infile);
 if ((image.width() != cols) || (image.height() != rows)) {
   fprintf(stderr,"ERROR: reading %s - wrong size!\n", filename);
   return;
 }
}

void PicWin::editImg() {
  if(!pfm_data) return;
  EditImg* edit_img = new EditImg(this, "TEST",pfm_min_val,pfm_max_val);
	edit_img->show();
}

void PicWin::absImg() {
 if(!pfm_data) return;
 float v, max = pfm_max_val, min = 0;
 float factor;
 if (pfm_min_val > 0) {factor = pfm_max_val-pfm_min_val; min = pfm_min_val;}//min>0 max>0
 else if (pfm_max_val > 0)
   if(-pfm_min_val > pfm_max_val) factor = -pfm_min_val;//min<0 max>0 min>max
   else factor = pfm_max_val; //min<0 max>0 min<max
 else  {factor = pfm_min_val - pfm_max_val; min = pfm_max_val; max = -pfm_max_val;}//min<0 max<0
 int x, y, cols = image.width();
 for (y=0; y<image.height(); y++)
   for (x=0; x<cols; x++) {
    v = pfm_data[y*cols+x];
    if (v<0) v=-v;
    //printf("(%d,%d), f: %f, v: %f %f, min: %f, max: %f, fmin: %f, fmax: %f\n",
    //x,y,factor,v,(v-min)/factor*254+1,min,max,pfm_min_val,pfm_max_val);
    setPixel(image,x,y,int((v-min)/factor*254+1));
   }
 // And reconvert...
 reconvertImage();
 repaint(image.hasAlphaBuffer());	// show image in widget
}

void PicWin::rndColImg() {
 for (int i=0; i<256; i++) image.setColor( i, qRgb(random()%256,random()%256,random()%256) );
 // And reconvert.
 convertLabel(image,pfm_data);
 reconvertImage();
 repaint(image.hasAlphaBuffer());	// show image in widget
}

void PicWin::circularColorImg() {
 for (int i=0; i<256; i++) {
   QColor c;
   c.setHsv(int(i/256.0*360),255,128);
   image.setColor( i, c.rgb());
 }
 // And reconvert...
 reconvertImage();
 repaint(image.hasAlphaBuffer());	// show image in widget
}

void PicWin::origColImg() {
 if(!pfm_data)return;

 for (int i=0; i<256; i++) image.setColor( i, qRgb(i,i,i) );
 if(!pfm_data)return;

 calcMinMax(image,pfm_data,pfm_min_val,pfm_max_val);
 convertScalar(image,pfm_data,pfm_min_val,pfm_max_val);
 // And reconvert...
 reconvertImage();
 repaint(image.hasAlphaBuffer());	// show image in widget
}

void PicWin::slotScaleImg(float min, float max) {
 convertScalar(image,pfm_data,min,max);
 // And reconvert...
 reconvertImage();
 repaint(image.hasAlphaBuffer());	// show image in widget
}

/** set the contrast by evaluating the min and max values from the selected region */
void PicWin::autoContrast()
{
  if (!pfm_data) return;
  double f=double(pm.width())/pmScaled.width();
  int tllx=int(llx*f);
  int tlly=int(lly*f);
  int turx=int(urx*f);
  int tury=int(ury*f);
  if (tllx>turx) qSwap(tllx,turx);
  if (tury>tlly) qSwap(tlly,tury);

  if (turx>=image.width())
      turx=image.width()-1;
  if (tlly>=image.height())
      tlly=image.height()-1;
  if (tllx < 0)
      tllx=0;
  if (tury < 0)
      tury=0;

  assert(tllx>=0);
  assert(tlly>=0);
  assert(turx>=0);
  assert(tury>=0);
  assert(turx<image.width());
  assert(tllx<image.width());
  assert(tury<image.height());
  assert(tlly<image.height());

  float min=FLT_MAX;
  float max=FLT_MIN;
  int cols=image.width();
  for (int y=tury; y<=tlly; y++) {
    for (int x=tllx; x<=turx; x++) {
      float v=pfm_data[y*cols+x];
      if (v<min) min=v;
      if (v>max) max=v;
    }
  }
  slotScaleImg(min,max);
}

/** No descriptions */
void PicWin::histogram()
{
  if (!pfm_data) return;
  double f=double(pm.width())/pmScaled.width();
  int tllx=int(llx*f);
  int tlly=int(lly*f);
  int turx=int(urx*f);
  int tury=int(ury*f);
  if (tllx>turx) qSwap(tllx,turx);
  if (tury>tlly) qSwap(tlly,tury);
  if (turx>=image.width() || tlly>=image.height()) return;
  float min=FLT_MAX;
  float max=FLT_MIN;
	float mean = 0.0, meanx = 0.0;
  int cols=image.width();
  for (int y=tury; y<=tlly; y++) {
		meanx = 0.0;
    for (int x=tllx; x<=turx; x++) {
      float v=pfm_data[y*cols+x];
      if (v<min) min=v;
      if (v>max) max=v;
			meanx+=v;
    }
		mean += meanx / (turx-tllx+1);
  }
	mean/=(tlly-tury+1);

  if (max<256 && min>=0 && max>128) {min=0; max=256;}
  float range=max-min;
  int maxcount=1;
  int maxcl=0;
  int value[256];
  for (int i=0; i<256; i++) value[i]=0;
  for (int y=tury; y<=tlly; y++) {
    for (int x=tllx; x<=turx; x++) {
      float v=pfm_data[y*cols+x];
      int cl=int((v-min)/range*256);
      if (cl==256) cl=255;
      value[cl]++;
      if (value[cl]>maxcount) {
        maxcount=value[cl];
        maxcl=cl;
      }
    }
  }
  const int win_height=512;
  const int win_width=256;
  QPainter p;
  p.begin(&histogramm_);
  p.setPen(red);
  QString s;
  s.sprintf("max=%f",max);
  p.drawText(5,15,s);
  s.sprintf("min=%f",min);
  p.drawText(5,35,s);
  s.sprintf("mean=%f",mean);
  p.drawText(5,55,s);
  s.sprintf("maxcount=%d(class=%d(%f))",maxcount,maxcl,maxcl*range/256+min);
  qDebug("picwin::histogram %d",maxcount);
  p.drawText(5,75,s);
//  p.clear();
  p.setWindow(0,-100,win_width,win_height+101);
//  p.scale(1,-1);
//  p.translate(0,win_height+1);
  p.setPen(blue);
  p.drawLine(0,-win_height,win_width,-win_height);
  p.drawLine(0,0,win_width,0);
  p.setPen(blue);
  p.drawLine(0,-win_height,win_width,-win_height);
  p.drawLine(0,0,win_width,0);
  if (range!=0.0) {// Draw COS
    float step=pow(10,floor(log10(maxcount)));
    p.setPen(red);
    for (float base=step; base<maxcount; base+=step) {
      p.drawLine(0,int(win_height-base/maxcount*win_height),
                 win_width,int(win_height-base/maxcount*win_height));
    }
  }
  p.setPen(black);
//  histogramm_.fill();
//  p.clear();
  bool flag=true;
  QBrush brush(SolidPattern);
  for (int cl=0; cl<256; cl++) {
    if (flag) brush.setColor(blue);
    else brush.setColor(black);
    flag=!flag;
    int h=int(double(value[cl])/maxcount*win_height);
    if (!h && value[cl]) {
      h=5;
      brush.setColor(red);
    }
    p.fillRect(cl,win_height,2,-h,brush);
  }
  p.end();
  if (histDialog_) {
    delete histDialog_;
    histDialog_=0;
  }
  if (!histDialog_) histDialog_=new Plot2D(0,"Histogram");
  histDialog_->set(histogramm_);
  histDialog_->resize(win_width+1,win_height);
  histDialog_->show();
  histDialog_->raise();
}

int follow_ray (int start_x, int start_y, float *pfm_data, int alpha, int cols,
							int max_x, int min_x, int max_y, float shadecol, int &points){

int sum=0, x=start_x, y=start_y, deltax, deltay;
float a;

a= (float(alpha)*M_PI) /180.0;
	#ifdef DEBUG_PRG
//	cout<<"Startpunkt (="<<x<<","<<y<<")"<<endl;
	#endif							
if (a<= M_PI/4){
	do{	
		#ifdef DEBUG_PRG
//		cout<<"Teste Punkt (="<<x<<","<<y<<")"<<endl;
		#endif							
		points++;
		if (pfm_data[y*cols+x]== shadecol)
			sum++;		
		x++;
		deltax= x-start_x;
		y= start_y+int(tan(a)*float(deltax));
   	}
	while ((x<=max_x) && (y<=max_y));
	}
else if ((a>M_PI/4) && (a<M_PI/2)){
	do{	
		#ifdef DEBUG_PRG
//		cout<<"Teste Punkt (="<<x<<","<<y<<")"<<endl;
		#endif		
		points++;					
		if (pfm_data[y*cols+x]== shadecol)
			sum++;		
		y++;
		deltay= y-start_y;
		x= start_x+int(tan(M_PI/2-a)*float(deltay));
   	}
	while ((x<=max_x) && (y<=max_y));
	}
else if ((a>M_PI/2) && (a<3*(M_PI/4.0))){
	do{	
		#ifdef DEBUG_PRG
//		cout<<"Teste Punkt (="<<x<<","<<y<<")"<<endl;
		#endif		
		points++;					
		if (pfm_data[y*cols+x]== shadecol)
			sum++;		
		y++;
		deltay= y-start_y;
		x= start_x-int(tan(a-M_PI/2)*float(deltay));
   	}
	while ((x>=min_x) && (y<=max_y));
	}
else if ((a>3*(M_PI/4)) && (a<M_PI)){
	do{	
		#ifdef DEBUG_PRG
//		cout<<"Teste Punkt (="<<x<<","<<y<<")"<<endl;
		#endif		
		points++;					
		if (pfm_data[y*cols+x]== shadecol)
			sum++;		
		x--;
		deltax= abs(x-start_x);
		y= start_y+int(tan(M_PI-a)*float(deltax));
   	}
	while ((x>=min_x) && (y<=max_y));
	}

return (sum);
}

/** No descriptions */
void PicWin::sundir()
{
  if (!pfm_data) return;
  double f=double(pm.width())/pmScaled.width();	//	factor	original - scaled image
  int tllx=int(llx*f);	// true llx
  int tlly=int(lly*f);
  int turx=int(urx*f);
  int tury=int(ury*f);
  if (tllx>turx)
 	qSwap(tllx,turx);
  if (tury>tlly) qSwap(tlly,tury);
  if (turx>=image.width() || tlly>=image.height()) return;	//	markierter Bereich groesser als Original

  int cols=image.width();
//  int rows=image.height();
  float value[181];
  float shadecol=1.0;
  float accsum=0.0;
  int sum;
  for (int i=0; i<181; i++) value[i]=0;

  // test sun from directions alpha 0 and 180
	int count=0;
	int points=0;
	for (int y=tury; y<=tlly; y++){
		sum=0;
   	 for (int x=tllx; x<=turx; x++){
			points++;
			if (pfm_data[y*cols+x]== shadecol)
				sum++;
			}
			if (sum > 0)
				count++;
			accsum+= float (sum);
		}
	value[0]= accsum / (float(count)*float(points));
	value[180]= value[0];	
	#ifdef DEBUG_PRG
	cout<<"alpha=0 points="<<points<<" sum="<<accsum<<" value[0]="<<value[0]<<endl;
	#endif

	// test sun from directions alpha 1...89	
	for (int alpha=1; alpha<=89; alpha+=1){
		accsum=0.0;
   	count=0;
		points=0;
   	 for (int x=tllx; x<=turx; x++){			
			sum= follow_ray (x, tury, pfm_data, alpha, cols, turx, tllx, tlly, shadecol, points);		
			accsum+= sum;
			if (sum > 0)
				count++;
			}
		for (int y=tury+1; y<=tlly; y++){
			sum= follow_ray (tllx, y, pfm_data, alpha, cols, turx, tllx, tlly, shadecol, points);		
			accsum+= sum;
			if (sum > 0)
				count++;
			}
		value[alpha]= accsum / (float(count)*float(points));

		#ifdef DEBUG_PRG
		cout<<"alpha="<<alpha<<" points="<<points<<" sum="<<accsum<<" value["<<alpha<<"]="<<value[alpha]<<endl;
		#endif
		}

	//	alpha == 90
	count=0;
	points=0;
	accsum=0.0;
  	for (int x=tllx; x<=turx; x++){
		sum=0;
		for (int y=tury; y<=tlly; y++){
			points++;
			if (pfm_data[y*cols+x]== shadecol)			
				sum++;
			}
			if (sum > 0)
				count++;
			accsum+= float (sum);
		}
	value[90]= accsum / (float(count)*float(points));

	#ifdef DEBUG_PRG
	cout<<"alpha=90 points="<<points<<" sum="<<accsum<<" value[90]="<<value[90]<<endl;
	#endif

	// test sun from directions alpha 91...179
	for (int alpha=91; alpha<=179; alpha+=1){
		accsum=0.0;
   	count=0;
		points=0;
   	 for (int x=tllx; x<=turx; x++){			
			sum= follow_ray (x, tury, pfm_data, alpha, cols, turx, tllx, tlly, shadecol, points);		
			accsum+= sum;
			if (sum > 0)
				count++;
			}
		for (int y=tury+1; y<=tlly; y++){
			sum= follow_ray (turx, y, pfm_data, alpha, cols, turx, tllx, tlly, shadecol, points);		
			accsum+= sum;
			if (sum > 0)
				count++;
			}
	value[alpha]= accsum / (float(count)*float(points));

		#ifdef DEBUG_PRG
		cout<<"alpha="<<alpha<<" points="<<points<<" sum="<<accsum<<" value["<<alpha<<"]="<<value[alpha]<<endl;
		#endif
		}

	float maxvalue=0.0;
  int maxcl=0;
	for (int i=0; i<181; i++)
		if (value[i] > maxvalue){
			maxvalue=value[i];
			maxcl= i;
			}
	//	norm values
	for (int i=0; i<181; i++){
		value[i]= value[i]/maxvalue;
		value[i]*=100;
		}

	int maxcount= 100;
	//	visualisation
	float min=0, max=181;
  float range=max-min;
  const int win_height=512;
  const int win_width=182;
  QPainter p;
  p.begin(&histogramm_);
  p.setPen(red);
  QString s;
  s.sprintf("shade direction from 0°...180°");
  p.drawText(5,+15,s);
	int sundir= maxcl-90;
	if (sundir < 0)
		sundir =maxcl+90;
  s.sprintf("sundir=%i°", sundir);
  p.drawText(5,35,s);
  s.sprintf("maxcount=%d at sundir=%d",maxcount,maxcl);
  qDebug("picwin::sundir %d",maxcount);
  p.drawText(5,55,s);
//  p.clear();
  p.setWindow(0,-100,win_width,win_height+101);
//  p.scale(1,-1);
//  p.translate(0,win_height+1);
  p.setPen(blue);
  p.drawLine(0,-win_height,win_width,-win_height);
  p.drawLine(0,0,win_width,0);
  p.setPen(blue);
  p.drawLine(0,-win_height,win_width,-win_height);
  p.drawLine(0,0,win_width,0);
  if (range!=0.0) {// Draw COS
    float step=pow(10,floor(log10(maxcount)));
    p.setPen(red);
    for (float base=step; base<maxcount; base+=step) {
      p.drawLine(0,int(win_height-base/maxcount*win_height),
                 win_width,int(win_height-base/maxcount*win_height));
    }
  }
  p.setPen(black);
//  histogramm_.fill();
//  p.clear();
  bool flag=true;
  QBrush brush(SolidPattern);
  for (int cl=0; cl<=181; cl++) {
    if (flag) brush.setColor(blue);
    else brush.setColor(black);
    flag=!flag;
    int h=int(double(value[cl])/maxcount*win_height);
    if (!h && value[cl]) {
      h=5;
      brush.setColor(red);
    }
    p.fillRect(cl,win_height,2,-h,brush);
  }
  p.end();
  if (histDialog_) {
    delete histDialog_;
    histDialog_=0;
  }
  if (!histDialog_) histDialog_=new Plot2D(0,"Histogram");
  histDialog_->set(histogramm_);
  histDialog_->resize(win_width+1,win_height);
  histDialog_->show();
  histDialog_->raise();

}


/** No descriptions */
void PicWin::shape()
{
  if (!pfm_data) return;
  int w1=pm.width();
  int w2=pmScaled.width();
  double f=double(w1)/double(w2);
  int tllx=int(llx*f);
  int tlly=int(lly*f);
  int turx=int(urx*f);
  int tury=int(ury*f);
//  qDebug("f=%f %d %d %d %d",f,tllx,tlly,turx,tury);
  if (turx>=image.width() || tlly>=image.height()) return;
  int lenx=turx-tllx;
  int leny=tury-tlly;
  int len=(abs(lenx)>abs(leny) ? abs(lenx) : abs(leny));
  float data[len];
  float min=FLT_MAX;
  float max=FLT_MIN;
  int cols=image.width();
  if (abs(lenx)>abs(leny)) {
    int threshold=abs(lenx);
    int offset=abs(leny);
    int count=0;
//    if (lenx<0) { swap(tllx,turx); swap(tlly,tury); lenx=-lenx; leny=-leny;}
    int x=tllx;
    int y=tlly;
    int offsx=(lenx>0 ? 1 : -1);
    int offsy=(leny>0 ? 1 : -1);
    int index=0;
    while (x!=turx) {
//      qDebug("x=%5d y=%5d index=%5d %f",x,y,index,pfm_data[y*cols+x]);
      if (x<image.width() && y<image.height() && index<len) {
        float v=pfm_data[y*cols+x];
        data[index]=v;
        if (v<min) min=v;
        if (v>max) max=v;
        index++;
      }
      x+=offsx;
      count+=offset;
      if (count>=threshold) {
        y+=offsy;
        count-=threshold;
      }
    }
  }
  else {
    int threshold=abs(leny);
    int offset=abs(lenx);
    int count=0;
//    if (leny<0) { swap(tllx,turx); swap(tlly,tury); lenx=-lenx; leny=-leny;}
    int x=tllx;
    int y=tlly;
    int offsx=(lenx>0 ? 1 : -1);
    int offsy=(leny>0 ? 1 : -1);
    int index=0;
    while (y!=tury) {
//      qDebug("x=%5d y=%5d index=%5d %f",x,y,index,pfm_data[y*cols+x]);
      if (x<image.width() && y<image.height() && index<len) {
        float v=pfm_data[y*cols+x];
        data[index]=v;
        if (v<min) min=v;
        if (v>max) max=v;
        index++;
      }
      y+=offsy;
      count+=offset;
      if (count>=threshold) {
        x+=offsx;
        count-=threshold;
      }
    }
  }
  float range=max-min;
  int win_width=len;
  const int win_height=512;
//  shape_.resize(win_width,win_height);
//  shape_.fill();
  QPainter p;
  p.begin(&shape_);
  p.setPen(red);
  QString s;
  s.sprintf("max=%f",max);
  p.drawText(5,+15,s);
  s.sprintf("min=%f",min);
  p.drawText(5,35,s);
//  p.clear();
  p.setWindow(0,-win_height-100,win_width+1,win_height+101);
//  p.scale(1,-1);
//  p.translate(0,win_height+1);
  p.setPen(blue);
  p.drawLine(0,-win_height,win_width,-win_height);
  p.drawLine(0,0,win_width,0);
  if (range!=0.0) {// Draw COS
    float step=pow(10,floor(log10(range)));
    float base=ceil(min/step)*step;
    p.setPen(red);
    for ( ; base<max; base+=step) {
     p.drawLine(0,int(-(base-min)/range*win_height),
                win_width,int(-(base-min)/range*win_height));
    }
  }
  p.setPen(black);
  p.moveTo(0,int(-(data[0]-min)/range*win_height));
  p.lineTo(1,int(-(data[0]-min)/range*win_height));
  for (int index=1; index<win_width; index++) {
    p.lineTo(index,int(-(data[index]-min)/range*win_height));
    p.lineTo(index+1,int(-(data[index]-min)/range*win_height));
  }
  p.end();
  if (win_width>1024) win_width=1024;
  if (!shapeDialog_) shapeDialog_=new Plot2D(0,"Shape");
  shapeDialog_->set(shape_);
  shapeDialog_->resize(win_width+1,win_height);
//  shapeDialog_->setBackgroundPixmap(shape_);
  shapeDialog_->show();
  shapeDialog_->raise();
}

/** shift the image by one column (cylindrical) */
void PicWin::shiftImage()
{
	if (!pfm_data)
		return;
	
	int block = image.width() / 64;
	if (!block) block = 1;

	float tmpval[block];
   
  for (int y = 0; y < image.height(); y ++)
	{
		memcpy(tmpval, pfm_data + y * image.width(), sizeof(float) * block);
		memcpy(pfm_data + y * image.width(), pfm_data + y * image.width() + block, (image.width() - block) * sizeof(float));
		memcpy(pfm_data + (y + 1) * image.width() - block, tmpval, sizeof(float) * block);
	}

  qDebug("PicWin::shiftImage() min=%f max=%f",pfm_min_val,pfm_max_val);
	slotScaleImg(pfm_min_val, pfm_max_val);
}


/**
  This function is the slot for processing the Open menu item.
*/
void PicWin::reload()
{
  loadImage(filename);
}

/**
  This function is the slot for processing the Open menu item.
*/
void PicWin::openFile()
{
  static QFileDialog fd ((QWidget*)0, "xxpgm", this);
  const char* filter[50] = {"*.[p][fnpgbl][mg]"," JPG files (*.jpg)","*.[x][bp][m]",
  "PFM files (*.pfm)","PGM files (*.pgm)","PPM files (*.ppm)","PBM files (*.pbm)",
  "PNG files (*.png)","XBM files (*.xbm)","XPM files (*.xpm)"," ALL (*.*)",0};
  fd.setFilters (filter);
  if ( fd.exec() == QDialog::Accepted ) {
  	QString newfilename = fd.selectedFile();

  	if ( !newfilename.isEmpty() ) {
  		loadImage( newfilename ) ;
	  	repaint();			// show image in widget
  	}
  }
}

/**
  This function decompress gzip and bzip files.
*/
void PicWin::decompress(char* filename, int& compressed) {
  // if the file is bzipped, extract the little bugger ...
  FILE *infile;
  infile = fopen(filename,"r");
  if (!infile) {
       fprintf(stderr,"Can't open file %s for reading!\n", filename);
       return ;
  }

  char cmd[2048], bfilename[256];
  int ret=0;

  int mag_nr1 = fgetc(infile);
  int mag_nr2 = fgetc(infile);
  int mag_nr3 = fgetc(infile);
  fclose(infile);
  compressed = 0;

  strcpy(bfilename, "/tmp/.pfm_view-");
  if(strrchr(filename, '/')) strcat(bfilename, strrchr(filename, '/') + 1);
	else strcat(bfilename, filename);
  *strrchr(bfilename, '.') = '\0';

  // Check for bzip2 "magicnumber": BZh
  if (mag_nr1 == 'B' && mag_nr2 == 'Z' && mag_nr3 == 'h') {
    sprintf(cmd, "bunzip2 -d -q -c %s | cat >%s", filename, bfilename);
    ret = system(cmd);
    compressed = 1;
    } else // Check for gzip "magicnumber": 31 139 8
    if (mag_nr1 == 31 && mag_nr2 == 139 && mag_nr3 == 8 ) {
    	sprintf(cmd, "gzip -d -q -c %s | cat >%s", filename, bfilename);
    	ret = system(cmd);
    	compressed = 1;
    }

		if (compressed) {
    	if (ret == 127 || ret == -1) {
      	fprintf(stderr,"Error unzipping %s!\n", filename);
      	compressed = 0;
      	return;
			}
		strcpy(filename, bfilename);	
		}
}

/**
  This function loads an image from a file and resizes the widget to
  exactly fit the image size. If the file was not found or the image
  format was unknown it will resize the widget to fit the errorText
  message (see above) displayed in the current font.

  Returns TRUE if the image was successfully loaded.
*/

bool PicWin::loadImage( const char *fileName )
{
  strcpy(filename, fileName);
  bool ok = FALSE;
  if ( filename ) {
	QApplication::setOverrideCursor( waitCursor ); // this might take time
  if(pfm_data) delete pfm_data;  //reset PFM data
  editimg->setEnabled(FALSE);
  pfm_data = (float*)NULL;
  pfm_min_val = pfm_max_val = 0.0;

  int compressed = 0;
	//fprintf(stderr,"open file %s (%d)!\n", filename,compressed);
	decompress(filename, compressed);
  //fprintf(stderr,"open file %s (%d)..\n", filename,compressed);

  ok = image.load(filename, 0);
  std::cerr << "bool PicWin::loadImage ... ok = " << (ok ? "true" : "false") << "\n";
	pickx = -1;
	clickx = -1;
	if ( ok ) {
		paintWidget->setFixedSize(image.width(),image.height());
	 ok = reconvertImage();
#ifdef DEBUG_MSG
   qDebug("PicWin::loadImage: format=%s",image.imageFormat(filename));
#endif
	 if ((strcmp(image.imageFormat(filename),"PFM")==0)
     || (strcmp(image.imageFormat(filename),"PGMRAW")==0)
     || (strcmp(image.imageFormat(filename),"PGMASCII")==0))
       loadpfm(filename);
	}
	
	if (compressed) {
		char cmd[2048];
	  sprintf(cmd, "rm -f %s", filename);
    int ret = system(cmd);

    if (ret == 127 || ret == -1) {
      fprintf(stderr,"Error deleting temporary file %s!\n", filename);
      return !ok;
    }
  }

	if ( ok ) {
	    setCaption( filename );			// set window caption
	    int w = pm.width();
	    int h = pm.height();

	    const int reasonable_width = 128;
	    if ( w < reasonable_width ) {
		// Integer scale up to something reasonable
		int multiply = ( reasonable_width + w - 1 ) / w;
		w *= multiply;
		h *= multiply;
	    }

			paintWidget->setFixedSize(w,h);
			resize(w+10,h+menubar->height()+status->height()+10);
//	    paintWidget->resize( w, h );				// we resize to fit image
	    //	    update();
	    paintWidget->setPixmap(pmScaled);
	    paintWidget->adjustSize();
	    paintWidget->repaint();
	} else {
	    pm.resize(0,0);				// couldn't load image
	    paintWidget->update();
	}
	QApplication::restoreOverrideCursor();	// restore original cursor
    }
    updateStatus();
    setMenuItemFlags();
//		if(pfm_data)  editimg->setEnabled(TRUE);
		if (image.depth()==8) editimg->setEnabled(TRUE);
    return ok;
}

bool PicWin::reconvertImage()
{
  bool success = FALSE;

  if ( image.isNull() ) return FALSE;

  if ( alloc_context ) {
    QColor::destroyAllocContext( alloc_context );
    alloc_context = 0;
  }
  if ( useColorContext() ) {
    alloc_context = QColor::enterAllocContext();
    // Clear the image to hide flickering palette
#if 0	
    QPainter painter(this);
    painter.eraseRect(0, menubar->heightForWidth( width() ), width(), height());
#endif	
  }

  QApplication::setOverrideCursor( waitCursor ); // this might take time
  if ( pm.convertFromImage(image, conversion_flags) ) {
    pmScaled = QPixmap();
    scale();
    resize( width(), height() );
    success = TRUE;				// load successful
  }
  else {
    pm.resize(0,0);				// couldn't load image
  }
  updateStatus();
  setMenuItemFlags();
  QApplication::restoreOverrideCursor();	// restore original cursor

  if ( useColorContext() )
    QColor::leaveAllocContext();

  return success;				// TRUE if loaded OK
}

bool PicWin::smooth() const
{
    return options->isItemChecked(ss);
}

bool PicWin::useColorContext() const
{
    return options->isItemChecked(cc);
}

void  PicWin::scaleImgSize(float fx, float fy) {
	paintWidget->setFixedSize(int(pm.width()*fx),int(pm.height()*fy));
};

/*
  This functions scales the pixmap in the member variable "pm" to fit the
  widget size and  puts the resulting pixmap in the member variable "pmScaled".
*/

void PicWin::scale()
{
	int h = paintWidget->height();
  if ( image.isNull() ) return;

  QApplication::setOverrideCursor( waitCursor ); // this might take time
  if ( paintWidget->width() == pm.width() && h == pm.height() )
    {						// no need to scale if widget
			pmScaled = pm;				// size equals pixmap size
    } else {
			if (smooth()) {
		    pmScaled.convertFromImage(image.smoothScale(paintWidget->width(), h),
				conversion_flags);
		} else {
	    QWMatrix m;				// transformation matrix
	    m.scale(((double)paintWidget->width())/pm.width(),// define scale factors
		    ((double)h)/pm.height());
	    pmScaled = pm.xForm( m );		// create scaled pixmap
		}
  }
  paintWidget->setPixmap(pmScaled);
  QApplication::restoreOverrideCursor();	// restore original cursor
}

/*
  The resize event handler, if a valid pixmap was loaded it will call
  scale() to fit the pixmap to the new widget size.
*/

void PicWin::myResizeEvent( QResizeEvent * )
{
    if ( pm.size() == QSize( 0, 0 ) )		// we couldn't load the image
	return;

    int h = paintWidget->height();
    if ( paintWidget->width() != pmScaled.width() || h != pmScaled.height())
    {						// if new size,
	scale();				// scale pmScaled to window
	updateStatus();
    }
}

bool PicWin::convertEvent( QMouseEvent* e, int& x, int& y)
{
  if ( pm.size() != QSize( 0, 0 ) ) {
		int h = paintWidget->height();
		int nx = e->x() * image.width() / paintWidget->width();
		int ny = (e->y()) * image.height() / h;
		if (nx != x || ny != y ) {
	    x = nx;
	    y = ny;
	    updateStatus();
	    return TRUE;
		}
  }
  return FALSE;
}

void PicWin::myMousePressEvent( QMouseEvent *e )
{
	button=e->button();
  switch (button) {
  case LeftButton: {
      QPainter painter(paintWidget);
      painter.setPen(white);
      painter.setRasterOp(XorROP);
      int w=urx-llx; if (w>0) w++; else w--;
      int h=ury-lly; if (h>0) h++; else h--;
      painter.drawRect(llx,lly,w,h);
      painter.drawLine(llx,lly,urx,ury);
      llx=urx=e->x();
      lly=ury=e->y();
#ifdef DEBUG_MSG
      qDebug("PicWin::myMousePressEvent: llx=%d lly=%d urx=%d ury=%d",llx,lly,urx,ury);
#endif
    }
    break;
  case MidButton:
  		clickx=e->x();
  		clicky=e->y();
    break;
  default:
    break;
	}
}

void PicWin::myMouseReleaseEvent( QMouseEvent * )
{
	button=NoButton;
}

/*
  Record the pixel position of interest.
*/
void PicWin::myMouseMoveEvent( QMouseEvent *e )
{
	switch (button) {
  case LeftButton: {
      QPainter painter(paintWidget);
      painter.setPen(white);
      painter.setRasterOp(XorROP);
      int w=urx-llx; if (w>0) w++; else w--;
      int h=ury-lly; if (h>0) h++; else h--;
      painter.drawRect(llx,lly,w,h);
      painter.drawLine(llx,lly,urx,ury);
      urx=e->x();
      ury=e->y();
#ifdef DEBUG_MSG
      qDebug("PicWin::myMouseMoveEvent: llx=%d lly=%d urx=%d ury=%d",llx,lly,urx,ury);
#endif
      w=urx-llx; if (w>0) w++; else w--;
      h=ury-lly; if (h>0) h++; else h--;
      painter.drawRect(llx,lly,w,h);
      painter.drawLine(llx,lly,urx,ury);
    }
    break;
	case MidButton: {
		int dx=-(e->x()-clickx);
		int dy=-(e->y()-clicky);
		if (dx && scrollView->horizontalScrollBar()->isVisible()) {
			scrollView->horizontalScrollBar()->setValue(scrollView->horizontalScrollBar()->value()+dx);
		}
		if (dy && scrollView->verticalScrollBar()->isVisible()) {
			scrollView->verticalScrollBar()->setValue(scrollView->verticalScrollBar()->value()+dy);
		}
		}
		break;
	default:
    if (convertEvent(e,pickx,picky)) {
			updateStatus();
	  }
	}
}

/*
  Draws the portion of the scaled pixmap that needs to be updated or prints
  an error message if no legal pixmap has been loaded.
*/

void PicWin::myPaintEvent( QPaintEvent *e )
{
  if ( pm.size() != QSize( 0, 0 ) ) {		// is an image loaded?
#if 0
  paintWidget->setPixmap(pmScaled);
#endif
	QPainter painter(paintWidget);
#ifdef DEBUG_MSG
  qDebug("PicWin::myPaintEvent: llx=%d lly=%d urx=%d ury=%d",llx,lly,urx,ury);
  qDebug("PicWin::myPaintEvent: rect(%d %d %d %d)",
         e->rect().left(),
         e->rect().top(),
         e->rect().right(),
         e->rect().bottom());
#endif
	painter.setClipRect(e->rect());
	painter.drawPixmap(0, 0, pmScaled);
  painter.setPen(white);
  painter.setRasterOp(XorROP);
  int w=urx-llx; if (w>0) w++; else w--;
  int h=ury-lly; if (h>0) h++; else h--;
  painter.drawRect(llx,lly,w,h);
  painter.drawLine(llx,lly,urx,ury);
//  painter.drawRect(llx,lly,urx-llx,ury-lly);
    }
}


/*
  Explain anything that might be confusing.
*/
void PicWin::giveHelp()
{
    if (!helpmsg) {
	QString helptext = "Show images";
	QStrList support = QImage::inputFormats();
	helptext += "\n\nSupported input formats:\n";
	int lastnl = helptext.length();
	helptext += "  ";
	const char* f = support.first();
	helptext += f;
	f = support.next();
	for (; f; f = support.next()) {
	    helptext += ',';
	    if ( helptext.length() - lastnl > 40 ) {
		helptext += "\n  ";
		lastnl = helptext.length() - 2;
	    } else {
		helptext += ' ';
	    }
	    helptext += f;
	}
  helptext += "\n\nTNT, Uni - Hannover, Jürgen Bückner\nbueckner@tnt.uni-hannover.de";
  helpmsg = new QMessageBox( "Help", helptext,
  QMessageBox::Information, QMessageBox::Ok, 0, 0, 0, 0, FALSE );
    }
    helpmsg->show();
    helpmsg->raise();
}

void PicWin::quit( ) {
  qApp->quit();
}

void PicWin::close( ) {
  destroy (this);
}


PicWin* PicWin::other = 0;



EditImg::EditImg ( QWidget *parent, const char *name, float min_, float max_ )
                  : QDialog(parent, name)
{
  layout = new QVBoxLayout( this, 5 );

  setIconText("Scale gray values");
  setName("Scale gray values");
  setCaption("Scale gray values");

  mainList = new QVBoxLayout();
  slider = new combiSlider("gray value range", this, "", min_, max_);
  layout->addLayout(mainList);
  layout->addWidget(slider);
	layout->addSpacing(5);
	
  QFrame* seperator = new QFrame(this);
  seperator->setFrameStyle(QFrame::Box | QFrame::Sunken);
  seperator->setLineWidth(2);
  seperator->setFixedHeight(4);
  layout->addWidget(seperator);
  layout->addSpacing(5);

  footer = new QHBoxLayout();
  layout->addLayout(footer);

  cancel = new QPushButton( "Cancel", this );
	connect( cancel, SIGNAL(clicked()), SLOT(reject()) );
  footer->addWidget( cancel );

  footer->addStretch();

  apply = new QPushButton( "Apply", this );
  connect( apply, SIGNAL(clicked()), this, SLOT(slotGetVal()));
  footer->addWidget( apply );

  connect(this, SIGNAL(sigVal(float, float)), parent, SLOT(slotScaleImg(float, float)));

  //connect( this, SIGNAL(sigNewEntry(QString)), parent, SLOT(slotNewEntry(QString)));
	// And reconvert...
	//  reconvertImage();
	//  repaint(image.hasAlphaBuffer());	// show image in widget
}

void EditImg::slotGetVal() {
 emit sigVal(slider->value1(), slider->value2());
}

static void convertLabel(QImage& img, float* data, float min, float max)
{
 int cols=img.width();
 int rows=img.height();
 for (int y=0; y<rows; y++)
   for (int x=0; x<cols; x++)
      img.setPixel(x, y, int(fabs(data[y*cols+x]+min))%256);
}

static void convertScalar(QImage& img, float* data, float min, float max)
{
 int cols=img.width();
 int rows=img.height();
 float factor = max-min;
 for (int y=0; y<rows; y++)
   for (int x=0; x<cols; x++) {
    float v=data[y*cols+x];
    setPixel(img,x, y, int((v-min)/factor*254+1));
   }
}

static void calcMinMax(const QImage &img, float *data, float &min, float &max)
{
 max = min = data[0];
 for (int x=0; x<img.height()*img.width(); x++){
   if (data[x]>max) max = data[x];
   if (data[x]<min) min = data[x];
 }
}

// PFM .. PFM ..PFM ..PFM ..PFM ..PFM ..PFM ..PFM ..PFM ..PFM ..
// PFM .. PFM ..PFM ..PFM ..PFM ..PFM ..PFM ..PFM ..PFM ..PFM ..

 static void
read_pfm_image( QImageIO *iio ) {
 //char* fileName;
 //fileName = (const char*)(iio->fileName());
 const char* format=iio->imageFormat(iio->fileName());
 FILE *infile;
 infile = fopen((iio->fileName()),"r");
 if (!infile) {
      fprintf(stderr,"Can't open file %s for reading!\n", (iio->fileName().latin1()));
      return; //exit(0);
 }

 int cols, rows;
 float minval,maxval;
 float* pic;
 std::cerr << "static void read_pfm_image...\n";
 pic=pfm_readpfm(infile,&cols,&rows,&minval,&maxval);
 fclose(infile);

 QImage img(cols, rows, 8, 256);
 for (int i=0; i<256; i++) img.setColor( i, qRgb(i,i,i) );

 printf("(%s) minval=%f maxval=%f\n",format,minval,maxval);
 convertScalar(img,pic,minval,maxval);
 iio->setFormat(format);
 iio->setImage(img);
 iio->setStatus(0);
};

static void write_pfm_image( QImageIO * ) {
  // write the image, using the image->ioDevice()
  std::cerr << "The sense escapes me - i have no float values"<<std::endl;
};

/** No descriptions */
void PicWin::selectAll()
{
  urx=pmScaled.width()-1;
  lly=pmScaled.height()-1;
  llx=0;
  ury=0;
}

class pfm_IOHandler {
 public:
 pfm_IOHandler(){
 // add the PFM image handler
 QImageIO::defineIOHandler("PFM",
                           "^F[0-7]",
                           (char*) NULL,
                           read_pfm_image,
                           write_pfm_image );
 QImageIO::defineIOHandler("PGMRAW",
                           "^P5",
                           (char*) NULL,
                           read_pfm_image,
                           0 );
 QImageIO::defineIOHandler("PGMASCII",
                           "^P2",
                           (char*) NULL,
                           read_pfm_image,
                           0 );
 }
};

static pfm_IOHandler mach_mal; //init before main
// PFM .. PFM ..PFM ..PFM ..PFM ..PFM ..PFM ..PFM ..PFM ..PFM ..

