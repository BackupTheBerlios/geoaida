#include <stdio.h>

#include "formviewwidget.h"


FormViewWidget::FormViewWidget( QWidget *parent, const char *name ):
QMultiLineEdit(parent, name )
{

}
void FormViewWidget::changeCurs(const QString& str)
{
  int x, y;
  insert(str);
  getCursorPosition( &x, &y );
  setCursorPosition( x, y-2, FALSE );
return;
}
 
