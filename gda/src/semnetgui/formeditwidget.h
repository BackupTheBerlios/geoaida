/****************************************************************
**       
** formedit.h    created by Ulrike Waschkowski
**
****************************************************************/

#ifndef FORMEDITWIDGET_H
#define FORMEDITWIDGET_H

#include <qwidget.h>
#include "formviewwidget.h"


class FormEditWidget : public QWidget
{
  Q_OBJECT
public:
  FormEditWidget( QWidget *parent=0, const char *name=0 );
  
public: // signals

private: // members
  FormViewWidget *formview;
  
  public slots:
private: // methods
};

#endif //FORMEDIT_H
