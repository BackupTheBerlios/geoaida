/***************************************************************************
                          stack.h  -  description
                             -------------------
    begin                : Wed Aug 1 2001
    copyright            : (C) 2001 by Martin Pahl, Jürgen Bückner
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


#ifndef STACK_H
#define STACK_H

#include <qstring.h>
#ifdef WIN32
#include <qtextstream.h>
#include <qstack.h>
#define  QPtrStack QStack
#else
#include <qptrstack.h>
#endif
#include "stackelem.h"
#include <qmap.h>
#include <MLParser.h>

class NodeList;
class Node;

template<class type> class QPtrStackIterator	
: public QGListIterator	
{ 
public:
//WIN32:Compiler-Warnung (Stufe 1) C4243
//'Konvertierungstyp' Konvertierung von 'Typ1' nach 'Typ2' existiert, ist aber nicht verfügbar
//Ein Zeiger auf eine abgeleitete Klasse soll in einen Zeiger auf die Basisklasse konvertiert werden. 
//Der Zugriff auf diese Konvertierung ist jedoch unzulässig, da die Ableitung mit private oder protected vorgenommen wurde.
    QPtrStackIterator(const QPtrStack<type> &l) :QGListIterator((QGList &)l) {}
   ~QPtrStackIterator()        {}
    uint  count()   const     { return list->count(); }
    bool  isEmpty() const     { return list->count() == 0; }
    bool  atFirst() const     { return QGListIterator::atFirst(); }
    bool  atLast()  const     { return QGListIterator::atLast(); }
    type *toFirst()           { return (type *)QGListIterator::toFirst(); }
    type *toLast()            { return (type *)QGListIterator::toLast(); }
    operator type *() const   { return (type *)QGListIterator::get(); }
    type *operator*()         { return (type *)QGListIterator::get(); }

    // No good, since QPtrStack<char> (ie. QStrList fails...
    //
    // MSVC++ gives warning
    // Sunpro C++ 4.1 gives error
    //    type *operator->()        { return (type *)QGListIterator::get(); }

    type *current()   const   { return (type *)QGListIterator::get(); }
    type *operator()()        { return (type *)QGListIterator::operator()();}
    type *operator++()        { return (type *)QGListIterator::operator++(); }
    type *operator+=(uint j)  { return (type *)QGListIterator::operator+=(j);}
    type *operator--()        { return (type *)QGListIterator::operator--(); }
    type *operator-=(uint j)  { return (type *)QGListIterator::operator-=(j);}
    QPtrStackIterator<type>& operator=(const QPtrStackIterator<type>&it)
                              { QGListIterator::operator=(it); return *this; }
};


/**A stack holding objects of type StackElem
  *@author Martin Pahl
  */

class Stack : public QPtrStack<StackElem>  {
public:
	Stack(NodeList* nl=0, QString filename="");
  Stack(const Stack& stack);
	~Stack();
  Stack& operator=(const Stack& stack);
  Stack& copy(const Stack& stack);
  /** Duplicate the top most element */
  bool dup();
  /** Drop the topmost element */
  bool drop();
  /** Swap the two topmost stack elements */
  bool swap();
  /** Filter all the nodelist in elem 2 with the classname in elemen 1 */
  bool selectClass();
  bool select();
  bool selectInvers();
  /** calculate attribute for all member in nodelist */
  bool calcAttribute();
  /** numeric function to mange the stack (add, div, mul, sub) */
  bool numfkt(double f(double,double));
/**addition of two stack elements.
  * possible types:
  * scalar, scalar, push the result back to stack
  * scalar, list - adds each list element (local stack entry) with scalar, push the list back to stack
  * list, list - adds list element with list element using the local stack in node, lower list push back to stack
  */
  bool add();
/** substraction of two stack elements.
  * possible types:
  * scalar, scalar, push the result back to stack
  * scalar, list - subtracts each list element (local stack entry) with scalar, push the list back to stack
  * list, list - subtracts list element with list element using the local stack in node, lower list push back to stack
  */
  bool sub();
/** multiplication of two stack elements.
  * possible types:
  * scalar, scalar, push the result back to stack
  * scalar, list - multiply each list element (local stack entry) with scalar, push the list back to stack
  * list, list - multiply list element with list element using the local stack in node, lower list push back to stack
  */
  bool mul();
  /** division of two stack elements.
  * possible types:
  * scalar, scalar, push the result back to stack
  * scalar, list - divide each list element (local stack entry) with scalar, push the list back to stack
  * list, list - divide list element with list element using the local stack in node, lower list push back to stack
  */
  bool div();

  /** logic operator for numeric values to mange the stack () */
  bool lofkt(bool f(double,double),bool sf (QString, QString));
  bool eq();
  bool neq();
  bool let();
  bool gt();
  bool lt();
  bool get();
  /** Push the nodelist on the stack*/
  bool pushNodeList();

  /** Execute the given cmd */
  bool run(QString cmd);


  class GeoPos {//help class to calc distance
  public:
    GeoPos(Node* n, float x, float y) {
      node_=n;
			x_ = x;
			y_ = y;
    }
    float qDist(GeoPos *p){
      return (x_-p->x_)*(x_-p->x_)+(y_-p->y_)*(y_-p->y_);
    }
    QString key(){return key_;}
    void key(QString k){key_ = k;}
    Node* node(){return node_;}
    QString key_;
    float x_;
    float y_;
    Node* node_;
  };

  class CleanUp {
  public:
    bool status_;
    CleanUp(bool status) {status_=status;}
  };
public:
  void initFunctionTable() const;
  /** concatenate the both topmost lists in stack */
  bool concat();
  /** write the topmost stackelement, which should be a nodelist or a stack of nodlist, to the given textstream */
  bool write(QTextStream& fp, QString keyword="node");
  /** write the topmost stackelement to the given filename */
  void write(QString filename, QString keyword="node");
  /** push n lists from stack to a 'seperate stack' on stack */
  bool list2stack();
  /** push n lists from the 'seperate stack' and the numbers of lists to stack  */
  bool stack2list();
  /** set an attribute */
  bool setAttrib();
  /** get an attribute */
  bool getAttrib();
  /** push a stackelement on the local stacks */
  bool pushLocal();
  /** run a command on the local stacks */
  bool runLocal();
  /** group nodes with distance < maxDist by using 'Minimum Spanning Tree'.
      input: [nodelist] [value]
      output: [stack of lists] */
  bool nlRun();
  bool nlPush();
  bool nlPop();
  bool nlSet();
  bool nlGet();
  bool maxDist();
  bool nodeSet();
  bool nodeGet();
  /** register a new function */
  void registerFunction(QString name, bool function(Stack&));
  /** return the max. value of the both topmost stack elements */
  bool numMax();
  /** return the min. value of the both topmost stack elements */
  bool numMin();
  bool merge(); //merge using 'p'
  bool merge_nn(); //merge with New Nodes using 'p'
  /** set the judgement for the 'stack' see also function 'average'
 */
  bool set_judgement();
/** calculate average using all nodes (menmber of list). Take all top local stack
values from node and write the result in the 'nodelist' stack.
see also function 'set_judgement' of class 'stack' */
  bool nl_average();
/** calculate max over all nodes (menmber of list). Take all top local stack
values from node and write the result in the 'nodelist' stack.
see also function 'set_judgement' of class 'stack' */
  bool nl_max();
/** calculate sum over all nodes (menmber of list). Take all top local stack
values from node and write the result in the 'nodelist' stack.
see also function 'set_judgement' of class 'stack' */
  bool nl_sum();
  bool nl_div();
  bool nl_count();
  bool st_max();
protected:
  static QMap<QString,bool (Stack::*)()> functionTable_;
  static QMap<QString,bool (*)(Stack&)> externFunctionTable_;
  static bool initFunctionTable_;
  static NodeList* nodeList_;
  static QString filename_;
  static ArgDict attribList_;
};

#endif
