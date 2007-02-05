
#ifndef FORMVIEWWIDGET_H
#define FORMVIEWWIDGET_H

#include <qwidget.h>
#include <qmultilineedit.h>

class FormViewWidget : public QMultiLineEdit
{
  Q_OBJECT
public:
  FormViewWidget( QWidget *parent=0, const char *name=0 );
  
public: // signals

private: // members
  
  public slots:
	      void changeCurs(const QString&);
private: // methods
};

#endif //MULTILINE_H
