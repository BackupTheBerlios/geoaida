// GAD - Library 
// Copyright (C) 2000 Martin Pahl, Universität Hannover
//
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Library General Public
// License as published by the Free Software Foundation; either
// version 2 of the License, or (at your option) any later version.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Library General Public License for more details.
//
// You should have received a copy of the GNU Library General Public
// License along with this library; if not, write to the Free
// Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.

/*
 * $Source: /data/cvs/gad/gad/WildcardReplace.cpp,v $
 * $Revision: 1.2 $
 * $Date: 2002/05/24 06:14:37 $
 * $Author: pahl $
 * $Locker:  $
 */

#include "WildcardReplace.h"
#include <qstringlist.h>
#include <qregexp.h>

QString wreplace(QString src, QString pattern, QString mask)
{
  int i, j;
  QString s;
  QStringList arg;
  qDebug("wreplace(%s, %s,%s)",
         (const char *) src, (const char *) pattern, (const char *) mask);
  if (QString(src).find(QRegExp(pattern, true, true)) != 0)
    goto error;

  for (i = 0; i < pattern.length(); i++) {
    switch (pattern[i].latin1()) {
    case '*':
      {
        while ((i + 1 < pattern.length()) && (pattern[i + 1] == '*'))
          i++;
        if (i + 1 == pattern.length()) {
          arg += src;
          src = "";
          continue;
        }
        QString p = pattern.mid(i + 1);
        p.replace(QRegExp("\\."), "\\.");
        p.replace(QRegExp("\\?"), ".");
        p.replace(QRegExp("\\*"), ".*");
        p += "$";
        j = src.find(QRegExp(p));
        //      qDebug("search(%d) %s -> %s\n",
        //             j,(const char*)p,(const char*)src);
        if (j == -1)
          goto error;
        arg += src.left(j);
        src.remove(0, j);
      }
      break;
    case '?':
      if (src.length() == 0)
        goto error;
      arg += src.left(1);
      src.remove(0, 1);
      break;
    default:
      if (src.length() == 0)
        goto error;
      if (pattern[i] != src[0])
        goto error;
      src.remove(0, 1);
    }
  }
  if (src.length() != 0)
    goto error;
  //  qDebug("Success\n");
  {
    QStringList::Iterator it;
    for (i = 1, it = arg.begin(); it != arg.end(); ++it, i++) {
      mask.replace(QRegExp("#" + QString().setNum(i)), (*it));
      //      qDebug("a:%s\n",(const char*)(*it));
    }
  }
  //  qDebug("result=%s\n",(const char*)mask);
  return mask;

error:
  //  qDebug("Error\n");
  return "";
#ifdef zzz
  qDebug("Error\n");
  QStringList::Iterator it;
  for (it = arg.begin(); it != arg.end(); ++it)
    qDebug("a:%s\n", (const char *) (*it));
  return "";
#endif
}

#ifdef zzz
int main(int argc, char *argv[])
{
  qDebug("%s\n",
         (const char *) wreplace("hallotesthallo20", "*hallo*test*",
                                 "#2n#3eu#1"));
  qDebug("%s\n", (const char *) wreplace("adad5", "*ad?", "#2neu#1"));
  qDebug("%s\n", (const char *) wreplace("ababdfdf5", "*ab*df?", "#2neu#1"));

  qDebug("%d\n", QString("adad5").find(QRegExp("ad.$", true, false)));
  qDebug("%d\n", QString("ababdfdf5").find(QRegExp("*ab*df?", true, true)));
}
#endif
