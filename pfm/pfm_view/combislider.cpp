/***************************************************************************
                          combislider.cpp  -  description
                             -------------------
    begin                : Thu Sep 7 2000
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

#include "combislider.h"
//#include "iostream.h"

combiSlider::combiSlider(QString labelText, QWidget* parent, const char* name,
                         float min__, float max__ ) : QFrame(parent, name),
                         min_(min__), max_(max__)
{
  setFrameStyle(Panel | Plain);
  diff_ = max_ - min_;
  QLabel* label = new QLabel(labelText,this);
  label->setFixedSize(label->sizeHint());
  label->setFixedWidth(100);

  vali1 = new QDoubleValidator(min_, max_, 2, this);   //von, bis, anz. Nachkomma
  vali2 = new QDoubleValidator(min_, max_, 2, this);   //von, bis, anz. Nachkomma

  box1 = new mySpinBox(min_, max_, (max_-min_)/100, this); //min,max,step, Tasten rauf und runter - links
  box2 = new mySpinBox(min_, max_, (max_-min_)/100, this); //min,max,step, Tasten rauf und runter - rechts
  box1->setValue(int(min_*100));
  box2->setValue(int(max_*100));
  box1->setValidator(vali1);
  box2->setValidator(vali2);
  box1->setFixedSize(box1->sizeHint());
  box2->setFixedSize(box2->sizeHint());

  slider = new doubleslider(min_, max_, this);
  slider->setWert1(min_);
  slider->setWert2(max_);

  connect(box1, SIGNAL(valueChanged(const QString &)),
	  slider, SLOT(setWert1(const QString&)));
  connect(box2, SIGNAL(valueChanged(const QString&)),
	  slider, SLOT(setWert2(const QString&)));
  connect(slider, SIGNAL(value1Changed(float)), this, SLOT(setValue1(float)) );
  connect(slider, SIGNAL(value2Changed(float)), this, SLOT(setValue2(float)) );

  connect(box1, SIGNAL(valueChanged(int)), this, SLOT(valiAnpassen1(int)) );
  connect(box2, SIGNAL(valueChanged(int)), this, SLOT(valiAnpassen2(int)) );


  QHBoxLayout* layout = new QHBoxLayout(this);
  layout->addWidget(label);
  layout ->addWidget(box1);
  layout->addSpacing(10);
  layout->addWidget(slider);
  layout->addSpacing(10);
  layout->addWidget(box2);
}

void combiSlider::valiAnpassen1(int wert){
  box2->setRange(wert, int(max_*100));
}

void combiSlider::valiAnpassen2(int wert){
  box1->setRange(int(min_*100), wert);
}

void combiSlider::setValue1(float val1){
  float temp1 = val1 * diff_ + min_;
  int temp2 = int(temp1);
  box1->setValue(temp2*100);
}

void combiSlider::setValue2(float val1){
  float temp1 = val1 * diff_ + min_;
  int temp2 = int(temp1);
  box2->setValue(temp2*100);
}

float combiSlider::value1(){
  float temp1 = box1->value();
  return (temp1/100);
}

float combiSlider::value2(){
  float temp1 = box2->value();
  return (temp1/100);
}
