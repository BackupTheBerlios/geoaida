/***************************************************************************
                          stack.cpp  -  description
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
//  #define DEBUGMSG

using namespace std;
#include <iostream>

#include "stack.h"
#include "stackelemnodelist.h"
#include "stackelemnumber.h"
#include "stackelemstring.h"
#include "stackelemstack.h"
#include "regionsensor.h"
#include "qregexp.h"
#include <qmap.h>
#include <cstdio>
#include <iostream>



//######### NUMERIC FUNCTION  #########
bool Stack::numfkt(double f(double, double))
{
  StackElem *el1 = 0;
  StackElem *el2 = 0;
  try {
    if (count() < 2)
      throw CleanUp(false);     //sind noch genug da?
    el1 = pop();                //von stack holen
    el2 = pop();                //von stack holen
    if (el1->type() == StackElem::NUMBER && el2->type() == StackElem::NODELIST) {       //value + list
      double d = ((StackElemNumber *) el1)->data();     //wert holen
      NodeList & nl = ((StackElemNodeList *) el2)->data();      //liste holen
      QDictIterator < Node > it(nl);
      while (it.current()) {
        Node *node = it.current();      //nl.find(*it);
        if (node->stackCount() < 1)
          throw CleanUp(false); //sind noch genug da?
        StackElem *nse = node->stackPop();      //pop value from node
        if (nse->type() != StackElem::NUMBER)
          throw CleanUp(false);
        double nd = ((StackElemNumber *) nse)->data();
        node->stackPush(new StackElemNumber(f(nd, d))); //push value to node
#ifdef DEBUGMSG
        printf("<<< >>> %f * %f = %f\n", nd, d, f(nd, d));
#endif //DEBUGMSG
        ++it;
      }
      push(new StackElemNodeList(nl));
      throw CleanUp(true);
    }
    else if (el1->type() == StackElem::NODELIST && el2->type() == StackElem::NODELIST) {        // 2 lists
      NodeList & nl1 = ((StackElemNodeList *) el1)->data();     //liste holen
      NodeList & nl2 = ((StackElemNodeList *) el2)->data();     //liste holen
      if (nl1.size() != nl2.size())
        throw CleanUp(false);   //unterschiedliche Listengroesse
      QDictIterator < Node > it1(nl1);
      QDictIterator < Node > it2(nl2);
      while (it1.current()) {
        Node *node1 = it1.current();    //nl.find(*it);
        Node *node2 = it2.current();    //nl.find(*it);
        if (node1->stackCount() < 1)
          throw CleanUp(false); //sind noch genug da?
        if (node2->stackCount() < 1)
          throw CleanUp(false); //sind noch genug da?
        StackElem *nse1 = node1->stackPop();    //pop value from node
        StackElem *nse2 = node2->stackPop();    //pop value from node
        if (nse1->type() != StackElem::NUMBER
            || nse2->type() != StackElem::NUMBER)
          throw CleanUp(false);
        double nd1 = ((StackElemNumber *) nse1)->data();
        double nd2 = ((StackElemNumber *) nse2)->data();
        node2->stackPush(new StackElemNumber(f(nd2, nd1)));     //push value to node
#ifdef DEBUGMSG
        printf("<<< >> %f * %f = %f\n", nd2, nd1, f(nd2, nd1));
#endif //DEBUGMSG
        ++it1;
        ++it2;
      }
      push(new StackElemNodeList(nl2));
      throw CleanUp(true);
    }
    else if (el1->type() == StackElem::NUMBER && el2->type() == StackElem::NUMBER) {    //value + value
      double d1 = ((StackElemNumber *) el1)->data();
      double d2 = ((StackElemNumber *) el2)->data();
      push(new StackElemNumber(f(d1, d2)));
#ifdef DEBUGMSG
      printf("<< >> %f (?) %f = %f\n", d1, d2, f(d2, d1));
#endif //DEBUGMSG
      throw CleanUp(true);
    }
    else
      throw CleanUp(false);
  }
  catch(CleanUp e) {
    if (el1)
      delete el1;
    if (el2)
      delete el2;
    return e.status_;
  }
}

static inline double add_(double d1, double d2)
{
  return d1 + d2;
}
static inline double sub_(double d1, double d2)
{
  return d1 - d2;
}
static inline double mul_(double d1, double d2)
{
  return d1 * d2;
}
static inline double div_(double d1, double d2)
{
  return d1 / d2;
}
static inline double max_(double d1, double d2)
{
  return (d1 > d2) ? d1 : d2;
}
static inline double min_(double d1, double d2)
{
  return (d1 < d2) ? d1 : d2;
}

/** return the max. value of the both topmost stack elements
    1) ... [value] [value] -> [value] : compare the both topmost stack values
    2) ... [nodelist] [value] -> [nodelist] : compare the local stack value with given value
*/
bool Stack::numMax()
{
  return numfkt(max_);
}

/** return the min. value of the both topmost stack elements
    1) ... [value] [value] -> [value] : compare the both topmost stack values
    2) ... [nodelist] [value] -> [nodelist] : compare the local stack value with given value
*/
bool Stack::numMin()
{
  return numfkt(min_);
}

/**addition of two stack elements.
  * possible types:
  * scalar, scalar, push the result back to stack
  * scalar, list - adds each list element (local stack entry) with scalar, push the list back to stack
  * list, list - adds list element with list element using the local stack in node, lower list push back to stack
  */
bool Stack::add()
{
  return numfkt(add_);
}

/** substraction of two stack elements.
  * possible types:
  * scalar, scalar, push the result back to stack
  * scalar, list - subtracts each list element (local stack entry) with scalar, push the list back to stack
  * list, list - subtracts list element with list element using the local stack in node, lower list push back to stack
  */
bool Stack::sub()
{
  return numfkt(sub_);
}

/** multiplication of two stack elements.
  * possible types:
  * scalar, scalar, push the result back to stack
  * scalar, list - multiply each list element (local stack entry) with scalar, push the list back to stack
  * list, list - multiply list element with list element using the local stack in node, lower list push back to stack
  */
bool Stack::mul()
{
  return numfkt(mul_);
}

/** division of two stack elements.
  * possible types:
  * scalar, scalar, push the result back to stack
  * scalar, list - divide each list element (local stack entry) with scalar, push the list back to stack
  * list, list - divide list element with list element using the local stack in node, lower list push back to stack
  */
bool Stack::div()
{
  return numfkt(div_);
}

//######### LOGIC FUNCTION  #########
/** logic operator for numeric values to mange the stack (eq, neq, gt, lt, get, let) */
bool Stack::lofkt(bool f(double, double), bool sf(QString, QString))
{
  StackElem *el1 = 0;
  StackElem *el2 = 0;
  try {
    if (count() < 2)
      throw CleanUp(false);     //sind noch genug da?
    el1 = pop();                //von stack holen
    el2 = pop();                //von stack holen
    if (el1->type() == StackElem::NUMBER && el2->type() == StackElem::NODELIST) {       //value + list
      cout << "Stack::lofkt: el1 =" << el1->type() << " el2=" << el2->
        type() << endl;
      double d = ((StackElemNumber *) el1)->data();     //wert holen
      NodeList & nl = ((StackElemNodeList *) el2)->data();      //liste holen
      //NodeList* selected=new NodeList;// new nodelist for result
      QDictIterator < Node > it(nl);
      while (it.current()) {
        Node *node = it.current();
        if (node->stackCount() < 1) {   //sind noch genug da?
          cerr << "Not enough elements on local stack " << endl;
          throw CleanUp(false);
        }
        StackElem *nse = node->stackPop();      //pop value from node
        if (nse->type() != StackElem::NUMBER) {
          cerr << "Wrong element type (" << nse->type()
            << ") on local stack - expected " << StackElem::NUMBER << endl;
          throw CleanUp(false);
        }
        double nd = ((StackElemNumber *) nse)->data();
        //if ((f(nd,d)))
        node->stackPush(new StackElemNumber(f(nd, d))); //push value to node
        //selected->insert(it.currentKey(),node);
#ifdef DEBUGMSG
        printf(".<< > (lofkt nu-no) %f ? %f = %d\n", d, nd, f(nd, d));
#endif //DEBUGMSG
        ++it;
      }
#ifdef DEBUGMSG
      cout << "SIZE nl (lofkt nu-no): " << nl.size() << endl;
      //nl.info();
#endif //DEBUGMSG
      push(new StackElemNodeList(nl));
      throw CleanUp(true);
    }
    else if (el1->type() == StackElem::NODELIST && el2->type() == StackElem::NODELIST) {        //value + list
      NodeList & nl1 = ((StackElemNodeList *) el1)->data();     //liste holen
      NodeList & nl2 = ((StackElemNodeList *) el2)->data();     //liste holen
      if (nl1.size() != nl2.size())
        throw CleanUp(false);   //unterschiedliche Listengroesse
      QDictIterator < Node > it1(nl1);
      QDictIterator < Node > it2(nl2);
      while (it1.current()) {
        Node *node1 = it1.current();    //nl.find(*it);
        Node *node2 = it2.current();    //nl.find(*it);
        if (node1->stackCount() < 1)
          throw CleanUp(false); //sind noch genug da?
        if (node2->stackCount() < 1)
          throw CleanUp(false); //sind noch genug da?
        StackElem *nse1 = node1->stackPop();    //pop value from node
        StackElem *nse2 = node2->stackPop();    //pop value from node
        if (nse1->type() != StackElem::NUMBER
            || nse2->type() != StackElem::NUMBER)
          throw CleanUp(false);
        double nd1 = ((StackElemNumber *) nse1)->data();
        double nd2 = ((StackElemNumber *) nse2)->data();
        //if ((f(nd1,nd2)))
        node2->stackPush(new StackElemNumber((f) (nd2, nd1)));  //push value to node
#ifdef DEBUGMSG
        printf(".<<< >> (lofkt no-no) %f ? %f = %d\n", nd2, nd1,
               (*f) (nd2, nd1));
#endif //DEBUGMSG
        ++it1;
        ++it2;
      }
      push(new StackElemNodeList(nl2));
      throw CleanUp(true);
    }
    else if (el1->type() == StackElem::NUMBER && el2->type() == StackElem::NUMBER) {    //value + value
      double d1 = ((StackElemNumber *) el1)->data();
      double d2 = ((StackElemNumber *) el2)->data();
      push(new StackElemNumber((*f) (d2, d1)));
#ifdef DEBUGMSG
      printf(".<< >> (lofkt no-no) %f * %f = %d\n", d2, d1, (f) (d2, d1));
#endif //DEBUGMSG
      throw CleanUp(true);
    }
    else if (el1->type() == StackElem::STRING && el2->type() == StackElem::NODELIST) {  //string + list
      QString d = ((StackElemString *) el1)->data();    //wert holen
      NodeList & nl = ((StackElemNodeList *) el2)->data();      //liste holen
      NodeList *selected = new NodeList;        // new nodelist for result
      QDictIterator < Node > it(nl);
      while (it.current()) {
        Node *node = it.current();
        QString name = node->classname();
        node->stackPush(new StackElemNumber(sf(name, d)));      //push value to node
        // if ((sf(node->classname(),d)))
        selected->insert(it.currentKey(), node);        //XYXY
#ifdef DEBUGMSG
        printf(".<< > (lofkt s-no) %s ? %s = %d\n", d.latin1(),
               node->classname().latin1(), sf(node->classname(), d));
#endif //DEBUGMSG
        ++it;
      }
#ifdef DEBUGMSG
      cout << "SIZE (lofkt s-no) selected: " << selected->size() << endl;
      //nl.info();
#endif //DEBUGMSG
      push(new StackElemNodeList(*selected));
      throw CleanUp(true);
    }
    else
      throw CleanUp(false);
  }
  catch(CleanUp e) {
    if (el1)
      delete el1;
    if (el2)
      delete el2;
    return e.status_;
  }
}

// logic fkt for nummeric values
static inline bool eq_(double d1, double d2)
{
  return d1 == d2;
}

static inline bool neq_(double d1, double d2)
{
  return !(d1 == d2);
}

static inline bool gt_(double d1, double d2)
{
  return d1 > d2;
}

static inline bool get_(double d1, double d2)
{
  return d1 >= d2;
}

static inline bool lt_(double d1, double d2)
{
  return d1 < d2;
}

static inline bool let_(double d1, double d2)
{
  return d1 <= d2;
}

static inline bool and_(double d1, double d2)
{
  return ((d1!=0.0) && (d2!=0.0));
}

static inline bool nand_(double d1, double d2)
{
  return (!((d1!=0.0) && (d2!=0.0)));
}

static inline bool or_(double d1, double d2)
{
  return ((d1!=0.0) || (d2!=0.0));
}

static inline bool nor_(double d1, double d2)
{
  return (!((d1!=0.0) || (d2!=0.0)));
}



// logic fkt for string values
/**Returns TRUE if s1 is equal to s2 or FALSE if they are different. */
static inline bool seq_(QString d1, QString d2)
{
  return d1 == d2;
}

/**Returns TRUE if s1 is not equal to s2 or FALSE if they are equal.  */
static inline bool sneq_(QString d1, QString d2)
{
  return d1 != d2;
}

/**Returns TRUE if s1 is lexically greater than s2 or FALSE if it is not. */
static inline bool sgt_(QString d1, QString d2)
{
  return d1 > d2;
}

/**Returns TRUE if s1 is lexically greater than or equal to s2 or FALSE if it is not.  */
static inline bool sget_(QString d1, QString d2)
{
  return d1 >= d2;
}

/**Returns TRUE if s1 is lexically less than s2 or FALSE if it is not.  */
static inline bool slt_(QString d1, QString d2)
{
  return d1 < d2;
}

/**Returns TRUE if s1 is lexically less than or equal to s2 or FALSE if it is not.  */
static inline bool slet_(QString d1, QString d2)
{
  return d1 <= d2;
}

/** equality of two stack elements.
  * possible types:
  * scalar, scalar - push the result [0,1] back to stack
  * scalar, list - push new list back to stack where the comparison 'local stack in node' and 'scalar' is true
  * list, list - push new list back to stack where the comparison 'local stack in node1' and 'local stack in node2' is true
  */
bool Stack::eq()
{
  return lofkt(eq_, seq_);
}

/** inequality of two stack elements.
  * possible types:
  * scalar, scalar - push the result [0,1] back to stack
  * scalar, list - push new list back to stack where the comparison 'local stack in node' and 'scalar' is true
  * list, list - push new list back to stack where the comparison 'local stack in node1' and 'local stack in node2' is true
  */
bool Stack::neq()
{
  return lofkt(neq_, sneq_);
}

/** 'greater than' of two stack elements.
  * possible types:
  * scalar, scalar - push the result [0,1] back to stack
  * scalar, list - push new list back to stack where the comparison 'local stack in node' and 'scalar' is true
  * list, list - push new list back to stack where the comparison 'local stack in node1' and 'local stack in node2' is true
  */
bool Stack::gt()
{
  return lofkt(gt_, sgt_);
}

/** 'greater or equal than' of two stack elements.
  * possible types:
  * scalar, scalar - push the result [0,1] back to stack
  * scalar, list - push new list back to stack where the comparison 'local stack in node' and 'scalar' is true
  * list, list - push new list back to stack where the comparison 'local stack in node1' and 'local stack in node2' is true
  */
bool Stack::get()
{
  return lofkt(get_, sget_);
}

/** 'lower than' of two stack elements.
  * possible types:
  * scalar, scalar - push the result [0,1] back to stack
  * scalar, list - push new list back to stack where the comparison 'local stack in node' and 'scalar' is true
  * list, list - push new list back to stack where the comparison 'local stack in node1' and 'local stack in node2' is true
  */
bool Stack::lt()
{
  return lofkt(lt_, slt_);
}

/** 'lower or equal than' of two stack elements.
  * possible types:
  * scalar, scalar - push the result [0,1] back to stack
  * scalar, list - push new list back to stack where the comparison 'local stack in node' and 'scalar' is true
  * list, list - push new list back to stack where the comparison 'local stack in node1' and 'local stack in node2' is true
  */
bool Stack::let()
{
  return lofkt(let_, slet_);
}

/** and-operation on two stack elements.
  * possible types:
  * scalar, scalar - push the result [0,1] back to stack
  * scalar, list - push new list back to stack where the comparison 'local stack in node' and 'scalar' is true
  * list, list - push new list back to stack where the comparison 'local stack in node1' and 'local stack in node2' is true
  * does not work for strings 
  */

bool Stack::andop()
{
  return lofkt(and_, seq_);
}

bool Stack::nandop()
{
  return lofkt(nand_, seq_);
}

/** or-operation on two stack elements.
  * possible types:
  * scalar, scalar - push the result [0,1] back to stack
  * scalar, list - push new list back to stack where the comparison 'local stack in node' and 'scalar' is true
  * list, list - push new list back to stack where the comparison 'local stack in node1' and 'local stack in node2' is true
  * does not work for strings 
  */
bool Stack::orop()
{
  return lofkt(or_, seq_);
}

bool Stack::norop()
{
  return lofkt(nor_, seq_);
}



//#########  FUNCTION  #########

QMap < QString, bool(Stack::*)() > Stack::functionTable_;
QMap < QString, bool(*)(Stack &) > Stack::externFunctionTable_;

bool Stack::initFunctionTable_ = true;
NodeList *Stack::nodeList_ = 0;
QString Stack::filename_;
ArgDict Stack::attribList_;

void Stack::initFunctionTable() const
{
  functionTable_.insert(QString("=="), &Stack::eq);
  functionTable_.insert(QString(">="), &Stack::get);
  functionTable_.insert(QString(">"), &Stack::gt);
  functionTable_.insert(QString("<"), &Stack::lt);
  functionTable_.insert(QString("nl.count"), &Stack::nl_count);
  functionTable_.insert(QString("<="), &Stack::let);
  functionTable_.insert(QString("!="), &Stack::neq);
  functionTable_.insert(QString("dup"), &Stack::dup);
  functionTable_.insert(QString("drop"), &Stack::drop);
  functionTable_.insert(QString("swap"), &Stack::swap);
  functionTable_.insert(QString("selectInvers"), &Stack::selectInvers);
  functionTable_.insert(QString("selectClass"), &Stack::selectClass);
  functionTable_.insert(QString("select"), &Stack::select);
  functionTable_.insert(QString("*"), &Stack::mul);
  functionTable_.insert(QString("/"), &Stack::div);
  functionTable_.insert(QString("+"), &Stack::add);
  functionTable_.insert(QString("concat"), &Stack::concat);
  functionTable_.insert(QString("-"), &Stack::sub);
  functionTable_.insert(QString("&"), &Stack::andop);
  functionTable_.insert(QString("&&"), &Stack::andop);
  functionTable_.insert(QString("and"), &Stack::andop);
  functionTable_.insert(QString("|"), &Stack::orop);
  functionTable_.insert(QString("||"), &Stack::orop);
  functionTable_.insert(QString("or"), &Stack::orop);
  functionTable_.insert(QString("calc"), &Stack::calcAttribute);
  functionTable_.insert(QString("nodelist"), &Stack::pushNodeList);
  functionTable_.insert(QString("list2stack"), &Stack::list2stack);
  functionTable_.insert(QString("stack2list"), &Stack::stack2list);
  functionTable_.insert(QString("set"), &Stack::setAttrib);
  functionTable_.insert(QString("get"), &Stack::getAttrib);
  functionTable_.insert(QString("node.set"), &Stack::nodeSet);
  functionTable_.insert(QString("node.get"), &Stack::nodeGet);
  functionTable_.insert(QString("runlocal"), &Stack::runLocal);
  functionTable_.insert(QString("node.run"), &Stack::runLocal);
  functionTable_.insert(QString("pushlocal"), &Stack::pushLocal);
  functionTable_.insert(QString("node.push"), &Stack::pushLocal);
  functionTable_.insert(QString("maxDist"), &Stack::maxDist);
  functionTable_.insert(QString("min"), &Stack::numMin);
  functionTable_.insert(QString("max"), &Stack::numMax);
  functionTable_.insert(QString("merge"), &Stack::merge);
  functionTable_.insert(QString("merge_nn"), &Stack::merge_nn);
  functionTable_.insert(QString("set_judgement"), &Stack::set_judgement);
  functionTable_.insert(QString("nl_average"), &Stack::nl_average);
  functionTable_.insert(QString("nl_sum"), &Stack::nl_sum);
  functionTable_.insert(QString("nl_div"), &Stack::nl_div);
  functionTable_.insert(QString("nl.average"), &Stack::nl_average);
  functionTable_.insert(QString("nl.sum"), &Stack::nl_sum);
  functionTable_.insert(QString("nl.div"), &Stack::nl_div);
  functionTable_.insert(QString("nl_max"), &Stack::nl_max);
  functionTable_.insert(QString("node.max"), &Stack::nl_max);
  functionTable_.insert(QString("nl.average"), &Stack::nl_average);
  functionTable_.insert(QString("nl.sum"), &Stack::nl_sum);
  functionTable_.insert(QString("nl.div"), &Stack::nl_div);
  functionTable_.insert(QString("nl.run"), &Stack::nlRun);
  functionTable_.insert(QString("nl.push"), &Stack::nlPush);
  functionTable_.insert(QString("nl.pop"), &Stack::nlPop);
  functionTable_.insert(QString("nl.set"), &Stack::nlSet);
  functionTable_.insert(QString("nl.get"), &Stack::nlGet);
}

/** register a new function */
void Stack::registerFunction(QString name, bool function(Stack &))
{
  externFunctionTable_.insert(name, function);
}

Stack::Stack(NodeList * nl, QString filename)
{
  if (initFunctionTable_) {
    initFunctionTable();
    initFunctionTable_ = false;
  }
  if (nl)
    nodeList_ = nl;
  if (!filename.isEmpty())
    filename_ = filename;
}

Stack::Stack(const Stack & stack)
{
  copy(stack);
}

Stack::~Stack()
{
  while (!isEmpty())
    delete pop();
}

Stack & Stack::operator = (const Stack & stack) {
  return copy(stack);
}

Stack & Stack::copy(const Stack & stack)
{
  if (this != &stack) {
    clear();
    QPtrStackIterator < StackElem > it(stack);
    for (; it.current(); ++it) {
      push((*it)->copy());
    }
  }
  return *this;

}

/** Duplicate the top most element */
bool Stack::dup()
{
  if (isEmpty())
    return false;
  StackElem *el = pop();
  push(el);
  el = el->copy();
  push(el);
  return true;
}

/** Drop the topmost element
 */
bool Stack::drop()
{
  if (isEmpty())
    return false;
  StackElem *el = pop();
  if (el)
    delete el;
  return true;
}

/** Swap the two topmost stack elements */
bool Stack::swap()
{
  if (count() < 2)
    return false;
  StackElem *el1 = pop();
  StackElem *el2 = pop();
  push(el1);
  push(el2);
  return true;

}

/** Filter the nodes in nodelist with local stack entry 1=TRUE */
bool Stack::select()
{
  StackElem *el1 = 0;
  StackElem *el2 = 0;
  try {
    if (count() < 1)
      throw CleanUp(false);
    el1 = pop();
    if (el1->type() == StackElem::NODELIST) {
      NodeList & nl = ((StackElemNodeList *) el1)->data();
      NodeList *selected = new NodeList;        // new nodelist for result
      QDictIterator < Node > it(nl);
      while (it.current()) {
        Node *node = it.current();
        if (node->stackCount() < 1)
          throw CleanUp(false); //sind noch genug da?
        StackElem *nse = node->stackPop();      //pop value from node
        if (nse->type() != StackElem::NUMBER)
          throw CleanUp(false);
        //double nd=((StackElemNumber*)nse)->data();
        if (((StackElemNumber *) nse)->data() == 1)
          selected->insert(it.currentKey(), node);      //XYXY
#ifdef DEBUGMSG
        printf("(select) %f\n", ((StackElemNumber *) nse)->data());
#endif //DEBUGMSG
        ++it;
      }
#ifdef DEBUGMSG
      cout << "SIZE selected (select.): " << selected->size() << endl;
#endif //DEBUGMSG
      push(new StackElemNodeList(*selected));
      throw CleanUp(true);
    }
    else if (el1->type() == StackElem::NUMBER
             || el2->type() == StackElem::NODELIST) {
      double d1 = ((StackElemNumber *) el1)->data();
      NodeList & nl = ((StackElemNodeList *) el2)->data();
      NodeList *selected = new NodeList;        // new nodelist for result
      QDictIterator < Node > it(nl);
      while (it.current()) {
        Node *node = it.current();
        if (node->stackCount() < 1)
          throw CleanUp(false); //sind noch genug da?
        StackElem *nse = node->stackPop();      //pop value from node
        if (nse->type() != StackElem::NUMBER)
          throw CleanUp(false);
        //double nd=((StackElemNumber*)nse)->data();
        if (((StackElemNumber *) nse)->data() == d1)
          selected->insert(it.currentKey(), node);      //XYXY
#ifdef DEBUGMSG
        printf("(select) %f %f\n", d1, ((StackElemNumber *) nse)->data());
#endif //DEBUGMSG
        ++it;
      }
#ifdef DEBUGMSG
      cout << "SIZE selected (select..): " << selected->size() << endl;
#endif //DEBUGMSG
      push(new StackElemNodeList(*selected));
      throw CleanUp(true);
    }
    else
      throw CleanUp(false);
  }
  catch(CleanUp e) {
    if (el1)
      delete el1;
    if (el2)
      delete el2;
    return e.status_;
  }
}

/** Filter the nodes in nodelist with local stack entry 1=FALSE */
bool Stack::selectInvers()
{
  StackElem *el1 = 0;
  StackElem *el2 = 0;
  try {
    if (count() < 1)
      throw CleanUp(false);
    el1 = pop();
    if (el1->type() == StackElem::NODELIST) {
      NodeList & nl = ((StackElemNodeList *) el1)->data();
      NodeList *selected = new NodeList;        // new nodelist for result
      QDictIterator < Node > it(nl);
      while (it.current()) {
        Node *node = it.current();
        if (node->stackCount() < 1)
          throw CleanUp(false); //sind noch genug da?
        StackElem *nse = node->stackPop();      //pop value from node
        if (nse->type() != StackElem::NUMBER)
          throw CleanUp(false);
        //double nd=((StackElemNumber*)nse)->data();
        if (((StackElemNumber *) nse)->data() != 1)
          selected->insert(it.currentKey(), node);      //XYXY
#ifdef DEBUGMSG
        printf("(selectInvers) %f\n", ((StackElemNumber *) nse)->data());
#endif //DEBUGMSG
        ++it;
      }
#ifdef DEBUGMSG
      cout << "SIZE selected (selectInvers): " << selected->size() << endl;
#endif //DEBUGMSG
      push(new StackElemNodeList(*selected));
      throw CleanUp(true);
    }
    else if (el1->type() == StackElem::NUMBER
             || el2->type() == StackElem::NODELIST) {
      double d1 = ((StackElemNumber *) el1)->data();
      NodeList & nl = ((StackElemNodeList *) el2)->data();
      NodeList *selected = new NodeList;        // new nodelist for result
      QDictIterator < Node > it(nl);
      while (it.current()) {
        Node *node = it.current();
        if (node->stackCount() < 1)
          throw CleanUp(false); //sind noch genug da?
        StackElem *nse = node->stackPop();      //pop value from node
        if (nse->type() != StackElem::NUMBER)
          throw CleanUp(false);
        //double nd=((StackElemNumber*)nse)->data();
        if (((StackElemNumber *) nse)->data() != d1)
          selected->insert(it.currentKey(), node);      //XYXY
#ifdef DEBUGMSG
        printf("(selectInvers) %f %f\n", d1,
               ((StackElemNumber *) nse)->data());
#endif //DEBUGMSG
        ++it;
      }
#ifdef DEBUGMSG
      cout << "SIZE selected (selectInvers): " << selected->size() << endl;
#endif //DEBUGMSG
      push(new StackElemNodeList(*selected));
      throw CleanUp(true);
    }
    else
      throw CleanUp(false);
  }
  catch(CleanUp e) {
    if (el1)
      delete el1;
    if (el2)
      delete el2;
    return e.status_;
  }
}


/** Filter all the nodelist in elem 2 with the classname in elemen 1 */
bool Stack::selectClass()
{
  StackElem *el1 = 0;
  StackElem *el2 = 0;
  try {
    if (count() < 2)
      throw CleanUp(false);
    el1 = pop();
    el2 = pop();
    if (el1->type() != StackElem::STRING
        || el2->type() != StackElem::NODELIST)
      throw CleanUp(false);
    QString s = ((StackElemString *) el1)->data();
    NodeList & nl = ((StackElemNodeList *) el2)->data();
    NodeList *selected = nl.selectClass(s);
    if (!selected)
      throw CleanUp(false);
    //selected->info();
    push(new StackElemNodeList(*selected));

    //XXXXXXXXXXXXX delete selected;
    throw CleanUp(true);
  }
  catch(CleanUp e) {
    if (el1)
      delete el1;
    if (el2)
      delete el2;
    return e.status_;
  }
}

/** calculate attribute for all member in nodelist and put it on the local stack
  * exist funktion:
  * size - region size in pixel
  * area - region size in m^2
  * x_center - region center x-value in image coordinates
  * y_center - region center y-value in image coordinates
  * llx - lower left x-point of enclose rectangle
  * lly - lower left y-point of enclose rectangle
  * urx - upper right x-point of enclose rectangle
  * ury - upper right y-point of enclose rectangle
  * circumference - circumference of the label [0,...,1]
  * roundness - roundness of the label [0,...,1]
  * squareness - squareness of the label [0,...,1]
  * compactness - compactness of the label [0,...,1]
  * thickness - thickness of the label = count shrinking steps in pixel
  * M11 - momentum M11 of the label
  * M02 - momentum M02 of the label
  * M20 - momentum M20 of the label
  * orthogonality - orthogonality of the label using hought (take a while)
*/
bool Stack::calcAttribute()
{
  StackElem *el1 = 0;
  StackElem *el2 = 0;
  try {
    RegionSensor *rs = new RegionSensor();
    if (count() < 2)
      throw CleanUp(false);
    el1 = pop();
    el2 = pop();
    if (el1->type() != StackElem::STRING
        && el2->type() != StackElem::NODELIST)
      throw CleanUp(false);
    QString s = ((StackElemString *) el1)->data();
    NodeList & nl = ((StackElemNodeList *) el2)->data();
    nl.calcForSelect("ALL", s, rs);
    push(new StackElemNodeList(nl));
    if (rs)
      delete rs;
    throw CleanUp(true);
  }
  catch(CleanUp e) {
    if (el1)
      delete el1;
    if (el2)
      delete el2;
    return e.status_;
  }
}



typedef bool(Stack::*StackFunction) ();
typedef bool ExternFunction(Stack &);

/** Execute the given cmd */
bool Stack::run(QString cmd)
{
  cmd = cmd.simplifyWhiteSpace();
  QStringList cmdList = QStringList::split(" ", cmd);
  for (QStringList::Iterator it = cmdList.begin(); it != cmdList.end(); ++it) {
    if (externFunctionTable_.find(*it) != externFunctionTable_.end()){
      QMap < QString, bool(*)(Stack &) >::const_iterator efit =
	externFunctionTable_.find(*it);

      // Debug Stack types
      cout << "(" << count() << "): ";
      {
	QPtrStackIterator < StackElem > it(*this);
	for (; it.current(); ++it) {
	  cout << (*it)->typeName() << " ";
	}
	cout << endl;
      }

      if (efit.data()) {
	cout << "(" << count() << ") Calling extern function " << *it << endl;
	ExternFunction *func = efit.data();
	if (!(*func) (*this))
	  return false;
	continue;
      }
    }
    
    if (functionTable_.find(*it) != functionTable_.end()){
      QMap < QString, bool(Stack::*)() >::const_iterator fit =
	functionTable_.find(*it);
    
      if (fit.data()) {
	cout << "(" << count() << ") Calling function " << *it << endl;
	StackFunction func = fit.data();
	if (!(*this.*func) ()) {
	  cerr << "function " << (*it) << " failed!" << endl;
	  return false;
	}
	continue;
      }
    }
    if (((*it).length() > 0) && (*it)[0] == '\"') {
      QString s = (*it);
      while (it != cmdList.end()) {

        if ((s[s.length() - 1] == '\"') && (s.length() >= 2)
            && (s[s.length() - 2] != '\\'))

          break;
        ++it;
        s += " " + (*it);
      }
      if ((s[(int) s.length() - 1] != '\"') || (s.length() >= 2)
          && (s[(int) s.length() - 2] == '\\'))
        return false;

      s = s.mid(1, s.length() - 2);
      s = s.replace(QRegExp("\\\\\""), "\"");
      cout << "(" << count() << ") String:" << s << endl;
      push(new StackElemString(s));
      continue;
    }
    bool ok;
    double v = (*it).toDouble(&ok);
    if (!ok){
      cerr << "Stack::run Error: " << *it << " neither command nor argument. Exiting. " << endl; 
      return false;
    }
    cout << "(" << count() << ") Num:" << *it << endl;
    push(new StackElemNumber(v));
  }
  return true;
}

/** Push the nodelist on the stack */
bool Stack::pushNodeList()
{
  if (nodeList_) {
    push(new StackElemNodeList(*nodeList_));
    return true;
  }
  else
    return false;

}

/** write the topmost stackelement, which should be a nodelist or a stack of nodlist, to the given textstream */
bool Stack::write(QTextStream & fp, QString keyword)
{
  StackElem *el1 = 0;
  try {
    if (count() < 1)
      throw CleanUp(false);
    el1 = pop();
    if (el1->type() == StackElem::NODELIST) {
      NodeList & nl = ((StackElemNodeList *) el1)->data();
      //nl.merge(false);
      if (keyword == "node") {
        nl.genGroupImage();
        nl.write(fp, filename_ + ".bu.plm");
      }
      else {
        nl.writeRegionFile(fp);
      }

      throw CleanUp(true);
    }
    if (el1->type() == StackElem::STACK) {
      Stack & stack = ((StackElemStack *) el1)->data();
      while (stack.count()) {
        if (!stack.write(fp, keyword))
          throw CleanUp(false);
      }
      throw CleanUp(true);
    }
    throw CleanUp(false);
  }
  catch(CleanUp e) {
    if (el1)
      delete el1;
    return e.status_;
  }
}

/** write the topmost stackelement to the given filename */
void Stack::write(QString filename, QString keyword)
{
  QFile fp(filename);           // 'XML' - description
  nodeList_->setImgName(filename + ".bu.node.plm");
  if (!fp.open(IO_WriteOnly)) {
    fprintf(stderr, "Stack::write: file not accesable to %s\n",
            filename.latin1());
    return;
  }
  QTextStream str(&fp);
  write(str, keyword);
}

/** concatenate the both topmost lists in stack */
bool Stack::concat()
{
  StackElem *el1 = 0;
  StackElem *el2 = 0;

  try {
    if (count() < 2)
      throw CleanUp(false);
    el1 = pop();
    el2 = pop();
    if (el1->type() != StackElem::NODELIST
        && el2->type() != StackElem::NODELIST)
      throw CleanUp(false);
    NodeList & nl1 = ((StackElemNodeList *) el1)->data();
    NodeList & nl2 = ((StackElemNodeList *) el2)->data();
    nl2 += nl1;
    push(new StackElemNodeList(nl2));
    throw CleanUp(true);
  }
  catch(CleanUp e) {
    if (el1)
      delete el1;
    if (el2)
      delete el2;
    return e.status_;
  }
}

/** push n lists from stack to a 'seperate stack' on stack */
bool Stack::list2stack()
{
  StackElem *el1 = 0;
  StackElem *el2 = 0;
  try {
    if (count() < 1)
      throw CleanUp(false);
    el1 = pop();
    if (el1->type() != StackElem::NUMBER)
      throw CleanUp(false);
    int d1 = int (((StackElemNumber *) el1)->data());   //Anzahl der Listen
    if (d1 < 0)
      throw CleanUp(false);
    Stack *se = new Stack();
    for (int i = 0; i < d1; i++) {      //Listen umsetzen
      if (count() < 1)
        throw CleanUp(false);
      el2 = pop();
      if (el2->type() != StackElem::NODELIST)
        throw CleanUp(false);
      se->push(el2);
      //se->insertAt(0,new StackElemList(el2));
    }
    insertAt(0, new StackElemStack(*se));       //stack mag kein insertAt!
    throw CleanUp(true);
  }
  catch(CleanUp e) {
    if (el1)
      delete el1;
    if (el2)
      delete el2;
    return e.status_;
  }
}

/** push n lists from the 'seperate stack' and the numbers of lists to stack  */
bool Stack::stack2list()
{
  StackElem *el1 = 0;
  StackElem *el2 = 0;
  try {
    if (count() < 1)
      throw CleanUp(false);
    el1 = pop();
    if (el1->type() != StackElem::STACK)
      throw CleanUp(false);
    Stack & st = ((StackElemStack *) el1)->data();      //stack vom stack
    int v(st.count());
    QPtrStackIterator < StackElem > it(st);
    for (; it.current(); ++it) {
      push((*it)->copy());
    }
    /*
       while (!st.isEmpty()) { //über alle elemente
       if (st.count()<1) throw CleanUp(false);
       el2=st.pop();
       if (el2->type()!=StackElem::NODELIST) throw CleanUp(false);
       push(el2);
       }
     */
    push(new StackElemNumber(v));
    throw CleanUp(true);
  }
  catch(CleanUp e) {
    if (el1)
      delete el1;
    if (el2)
      delete el2;
    return e.status_;
  }
}

/** set an attribute; stack -> node
  make only a copy - don't change the local stack*/
bool Stack::nodeSet()
{
  StackElem *el1 = 0;
  StackElem *el2 = 0;
  try {
    if (count() < 2)
      throw CleanUp(false);     //sind noch genug da?
    el1 = pop();                //von stack holen
    el2 = pop();                //von stack holen
    if (el1->type() == StackElem::STRING && el2->type() == StackElem::NODELIST) {       //value + list
      QString attribName = ((StackElemString *) el1)->data();   //wert holen
      NodeList & nl = ((StackElemNodeList *) el2)->data();      //liste holen
      QDictIterator < Node > it(nl);
      while (it.current()) {
        Node *node = it.current();      //nl.find(*it);
        if (node->stackCount() < 1)
          throw CleanUp(false); //sind noch genug da?
        StackElem *nse = node->stackPop();      //pop value from node
        QString val;
        switch (nse->type()) {
        case StackElem::STRING:
          val = ((StackElemString *) nse)->data();
          break;
        case StackElem::NUMBER:{
            double nd = ((StackElemNumber *) nse)->data();
            val.sprintf("%f", nd);
          }
          break;
        default:
          throw CleanUp(false);
          break;
        }
        node->replace(attribName, new QString(val));
        delete nse;
//                  node->stackPush(nse);
        ++it;
      }
      push(new StackElemNodeList(nl));
      throw CleanUp(true);
    }
    else
      throw CleanUp(false);
  }
  catch(CleanUp e) {
    if (el1)
      delete el1;
    if (el2)
      delete el2;
    return e.status_;
  }
}

bool Stack::nlSet()
{
  StackElem *el1 = 0;
  StackElem *el2 = 0;
  try {
    if (count() < 2)
      throw CleanUp(false);     //sind noch genug da?
    el1 = pop();                //von stack holen
    el2 = pop();                //von stack holen
    if (el1->type() == StackElem::STRING && el2->type() == StackElem::NODELIST) {       //value + list
      QString attribName = ((StackElemString *) el1)->data();   //wert holen
      NodeList & nl = ((StackElemNodeList *) el2)->data();      //liste holen
      if (nl.stackCount() < 1)
        throw CleanUp(false);   //sind noch genug da?
      StackElem *nse = nl.stackPop();   //pop value from node
      QString val;
      switch (nse->type()) {
      case StackElem::STRING:
        val = ((StackElemString *) nse)->data();
        break;
      case StackElem::NUMBER:{
          double nd = ((StackElemNumber *) nse)->data();
          val.sprintf("%f", nd);
        }
        break;
      default:
        throw CleanUp(false);
        break;
      }
      nl.set(attribName, val);
      delete nse;
//          nl.stackPush(nse);
      push(new StackElemNodeList(nl));
      throw CleanUp(true);
    }
    else
      throw CleanUp(false);
  }
  catch(CleanUp e) {
    if (el1)
      delete el1;
    if (el2)
      delete el2;
    return e.status_;
  }
}

bool Stack::setAttrib()
{
  StackElem *el1 = 0;
  StackElem *el2 = 0;
  try {
    if (count() < 2)
      throw CleanUp(false);     //sind noch genug da?
    el1 = pop();                //von stack holen
    el2 = pop();                //von stack holen
    if (el1->type() == StackElem::STRING) {     //value + list
      QString attribName = ((StackElemString *) el1)->data();   //wert holen
      QString val;
      switch (el2->type()) {
      case StackElem::STRING:
        val = ((StackElemString *) el2)->data();
        break;
      case StackElem::NUMBER:{
          double nd = ((StackElemNumber *) el2)->data();
          val.sprintf("%f", nd);
        }
        break;
      default:
        throw CleanUp(false);
        break;
      }
      attribList_.replace(attribName, val);
//          push(el2);
      throw CleanUp(true);
    }
    else
      throw CleanUp(false);
  }
  catch(CleanUp e) {
    if (el1)
      delete el1;
    if (el2)
      delete el2;
    return e.status_;
  }
}

/** get an attribute */
bool Stack::nodeGet()
{
  StackElem *el1 = 0;
  StackElem *el2 = 0;

  try {
    if (count() < 2)
      throw CleanUp(false);     //sind noch genug da?
    el1 = pop();                //von stack holen
    el2 = pop();                //von stack holen
    if (el1->type() == StackElem::STRING && el2->type() == StackElem::NODELIST) {       //value + list
      QString attribName = ((StackElemString *) el1)->data();   //wert holen
      NodeList & nl = ((StackElemNodeList *) el2)->data();      //liste holen
      QDictIterator < Node > it(nl);
      while (it.current()) {
        Node *node = it.current();      //nl.find(*it);
	QString v;
        QString *val = node->find(attribName);
        
	if (val) v=*val;
	else {
	  cerr << "Warning: Attribute not set: " 
	       << node->classname() << "::" 
	       << node->name() << "(" << attribName << ")" << endl;
        }
        bool ok;
        double d = v.toDouble(&ok);
        if (ok)
          node->stackPush(new StackElemNumber(d));
        else
          node->stackPush(new StackElemString(v));
        ++it;
      }
      push(new StackElemNodeList(nl));
     throw CleanUp(true);
    }
    else
      throw CleanUp(false);
  }
  catch(CleanUp e) {
    if (el1)
      delete el1;
    if (el2)
      delete el2;

    return e.status_;
  }
}

/** get an attribute */
bool Stack::nlGet()
{
  StackElem *el1 = 0;
  StackElem *el2 = 0;
  try {
    if (count() < 2)
      throw CleanUp(false);     //sind noch genug da?
    el1 = pop();                //von stack holen
    el2 = pop();                //von stack holen
    if (el1->type() == StackElem::STRING && el2->type() == StackElem::NODELIST) {       //value + list
      QString attribName = ((StackElemString *) el1)->data();   //wert holen
      NodeList & nl = ((StackElemNodeList *) el2)->data();      //liste holen
      QString val = nl.get(attribName);
#ifdef DEBUGMSG
      cout << "val="<<val<<endl;
#endif      
      bool ok;
      double d = val.toDouble(&ok);
      if (ok)
        nl.stackPush(new StackElemNumber(d));
      else
        nl.stackPush(new StackElemString(val));
      push(new StackElemNodeList(nl));
      throw CleanUp(true);
    }
    else
      throw CleanUp(false);
  }
  catch(CleanUp e) {
    if (el1)
      delete el1;
    if (el2)
      delete el2;
    return e.status_;
  }
}

/** get an attribute */
bool Stack::getAttrib()
{
  StackElem *el1 = 0;
  try {
    if (count() < 2)
      throw CleanUp(false);     //sind noch genug da?
    el1 = pop();                //von stack holen
    if (el1->type() == StackElem::STRING) {     //value + list
      QString attribName = ((StackElemString *) el1)->data();   //wert holen
      QString val;
      MLParser::setString(val, &attribList_, attribName);
      bool ok;
      double d = val.toDouble(&ok);
      if (ok)
        push(new StackElemNumber(d));
      else
        push(new StackElemString(val));
      throw CleanUp(true);
    }
    else
      throw CleanUp(false);
  }
  catch(CleanUp e) {
    if (el1)
      delete el1;
    return e.status_;
  }
}

/** merge all region - using 'p' weighting
... [nodelist] -> [nodelist]
 - set the variable 'nodelist' new to the returned list
 - put 'nodelist' to the stack
*/
bool Stack::merge()
{
  StackElem *el1 = 0;
  try {
    if (count() < 1)
      throw CleanUp(false);     //sind noch genug da?
    el1 = pop();                //von stack holen
    if (el1->type() == StackElem::NODELIST) {   //value + list
      NodeList & nl = ((StackElemNodeList *) el1)->data();      //liste holen
      nodeList_ = nl.merge(false, filename_ + ".bu.node.plm");
      push(new StackElemNodeList(*nodeList_));
      throw CleanUp(true);
    }
    else if (el1->type() == StackElem::STACK) { //      value + stack
#ifdef DEBUGMSG
      cout << "merge mit stack" << endl;
#endif //DEBUGMSG
      Stack & st = ((StackElemStack *) el1)->data();    //      stack holen
      Stack *backst_ = new Stack;       // Ergebnis stack anlegen
      NodeList res_list;
      while (st.count() >= 1) {
        StackElem *el = st.pop();
        if (el->type() != StackElem::NODELIST)
          throw(CleanUp(false));
        NodeList & nl = ((StackElemNodeList *) el)->data();     //      liste holen;
        NodeList *part_list = nl.merge(false, filename_ + ".bu.node.plm");
        res_list += *part_list;
        backst_->push(new StackElemNodeList(*part_list));
       delete el;
       delete part_list;
      }
      push(new StackElemStack(*backst_));
      *nodeList_ = res_list;
      delete backst_;
      throw CleanUp(true);
    }
    else
      throw CleanUp(false);
  }
  catch(CleanUp e) {
    if (el1)
      delete el1;
    return e.status_;
  }
}

/** merge all region - using 'p' weighting
  generate new nodes
  ... [nodelist] -> [nodelist]
 - set the variable 'nodelist' new to the returned list
 - put 'nodelist' on the stack

*/
bool Stack::merge_nn()          //merge with New Nodes
{
  StackElem *el1 = 0;
  try {
    if (count() < 1)
      throw CleanUp(false);     //sind noch genug da?
    el1 = pop();                //von stack holen
    if (el1->type() == StackElem::NODELIST) {   //value + list
      NodeList & nl = ((StackElemNodeList *) el1)->data();      //liste holen
      //NodeList* newNL = nl.merge(true,filename_+".bu.node.plm");
      nodeList_ = nl.merge(true, filename_ + ".bu.node.plm");
      push(new StackElemNodeList(*nodeList_));
      //push(new StackElemNodeList(*newNL));
      throw CleanUp(true);
    }
    else
      throw CleanUp(false);
  }
  catch(CleanUp e) {
    if (el1)
      delete el1;
    return e.status_;
  }
}

/** run a command on the local stacks */
bool Stack::runLocal()
{
  StackElem *el1 = 0;
  StackElem *el2 = 0;
  try {
    if (count() < 2)
      throw CleanUp(false);     //sind noch genug da?
    el1 = pop();                //von stack holen
    el2 = pop();                //von stack holen
    if (el1->type() == StackElem::STRING && el2->type() == StackElem::NODELIST) {       //value + list
      QString command = ((StackElemString *) el1)->data();      //wert holen
      NodeList & nl = ((StackElemNodeList *) el2)->data();      //liste holen
      QDictIterator < Node > it(nl);
      while (it.current()) {
        Node *node = it.current();      //nl.find(*it);
        (node->stack()).run(command);
        ++it;
      }
      push(new StackElemNodeList(nl));
      throw CleanUp(true);
    }
    else
      throw CleanUp(false);
  }
  catch(CleanUp e) {
    if (el1)
      delete el1;
    if (el2)
      delete el2;
    return e.status_;
  }
}


/** run a command on the local stacks */
bool Stack::nlRun()
{
  StackElem *el1 = 0;
  StackElem *el2 = 0;
  try {
    if (count() < 2)
      throw CleanUp(false);     //sind noch genug da?
    el1 = pop();                //von stack holen
    el2 = pop();                //von stack holen
    if (el1->type() == StackElem::STRING && el2->type() == StackElem::NODELIST) {       //value + list
      QString command = ((StackElemString *) el1)->data();      //wert holen
      NodeList & nl = ((StackElemNodeList *) el2)->data();      //liste holen
      nl.stack().run(command);
      push(new StackElemNodeList(nl));
      throw CleanUp(true);
    }
    else
      throw CleanUp(false);
  }
  catch(CleanUp e) {
    if (el1)
      delete el1;
    if (el2)
      delete el2;
    return e.status_;
  }
}


/** push a stackelement on the local stacks */
bool Stack::pushLocal()
{
  StackElem *el1 = 0;
  StackElem *el2 = 0;
  try {
    if (count() < 2)
      throw CleanUp(false);     //sind noch genug da?
    el1 = pop();                //von stack holen
    el2 = pop();                //von stack holen
    if (el2->type() == StackElem::NODELIST) {   //value + list
      NodeList & nl = ((StackElemNodeList *) el2)->data();      //liste holen
      QDictIterator < Node > it(nl);
      while (it.current()) {
        Node *node = it.current();      //nl.find(*it);
        (node->stack()).push(el1->copy());
        ++it;
      }
      push(new StackElemNodeList(nl));
      throw CleanUp(true);
    }
    else
      throw CleanUp(false);
  }
  catch(CleanUp e) {
    if (el1)
      delete el1;
    if (el2)
      delete el2;
    return e.status_;
  }
}


/** push a stackelement on the local stacks */
bool Stack::nlPush()
{
  StackElem *el1 = 0;
  StackElem *el2 = 0;
  try {
    if (count() < 2)
      throw CleanUp(false);     //sind noch genug da?
    el1 = pop();                //von stack holen
    el2 = pop();                //von stack holen
    if (el2->type() == StackElem::NODELIST) {   //value + list
      NodeList & nl = ((StackElemNodeList *) el2)->data();      //liste holen
      nl.stackPush(el1);
      push(new StackElemNodeList(nl));
      throw CleanUp(true);
    }
    else
      throw CleanUp(false);
  }
  catch(CleanUp e) {
    if (el2)
      delete el2;
    return e.status_;
  }
}

/** pop a stackelement on the local stacks */
bool Stack::nlPop()
{
  StackElem *el1 = 0;
  try {
    if (count() < 1)
      throw CleanUp(false);     //sind noch genug da?
    el1 = pop();                //von stack holen
    if (el1->type() == StackElem::NODELIST) {   //value + list
      NodeList & nl = ((StackElemNodeList *) el1)->data();      //liste holen
      StackElem *el = nl.stackPop();
      push(new StackElemNodeList(nl));
      push(el);
      throw CleanUp(true);
    }
    else
      throw CleanUp(false);
  }
  catch(CleanUp e) {
    if (el1)
      delete el1;
    return e.status_;
  }
}


/** group nodes with distance < maxDist by using 'Minimum Spanning Tree'.
input: [nodelist] [value]
output: [stack of lists] */
bool Stack::maxDist()
{
  StackElem *el1 = 0;
  StackElem *el2 = 0;
  try {
    if (count() < 2)
      throw CleanUp(false);     //sind noch genug da?
    el1 = pop();                //von stack holen
    el2 = pop();                //von stack holen
    if (el1->type() != StackElem::NUMBER
        || el2->type() != StackElem::NODELIST)
      throw CleanUp(false);
    double maxdist = ((StackElemNumber *) el1)->data(); //wert holen
    NodeList & nl = ((StackElemNodeList *) el2)->data();        //liste holen
    RegionSensor *rs = new RegionSensor();
    nl.calcForSelect("ALL", "xGeoCenter", rs);
    nl.calcForSelect("ALL", "yGeoCenter", rs);
    QPtrList < GeoPos > *gpList = new QPtrList < GeoPos >;;
    QDictIterator < Node > it(nl);
    while (it.current()) {      //hilfsliste fuellen
      Node *node = it.current();        //nl.find(*it);
      GeoPos *gp =
        new GeoPos(node, (node->getValue("xGeoCenter"))->toFloat(),
                   (node->getValue("yGeoCenter"))->toFloat());
      gp->key(it.currentKey());
      gpList->append(gp);
      ++it;
    }

    maxdist *= maxdist;         //quadratischen abstand verwenden
    QPtrList < QPtrList < GeoPos > >gpll;       //liste von listen - das Ergebnis
    QPtrList < GeoPos > *gpt = new QPtrList < GeoPos >; //neue tmp-liste zum zwischenspeichern
    GeoPos *gpo, *gpot;
    int flag;
    QPtrList < GeoPos > *gpin = gpList;
    QPtrList < GeoPos > *gpout = gpt;
    QPtrList < GeoPos > *gpswitch;
    while (!gpin->isEmpty()) {
      gpo = gpin->take();       //object holen
      QPtrList < GeoPos > *gpl = new QPtrList < GeoPos >;       //neue liste
      gpll.append(gpl);         //liste in ergebnis liste einfügen
      gpl->append(gpo);         //objekt in ergebnis liste einfügen
      flag = 1;
      if (!gpin->isEmpty())
        while (flag) {
          if (gpin->isEmpty())
            flag = 0;
          if (!gpin->isEmpty()) {
            gpot = gpin->take();        //test-object holen
#ifdef DEBUGMSG
            cout << " ## " << maxdist << " <?< " << gpo->
              qDist(gpot) << " : " << (maxdist < gpo->qDist(gpot)) << endl;
#endif //DEBUGMSG
            if (maxdist < gpo->qDist(gpot))
              gpout->append(gpot);      //abstand zu gross
            else {
              gpl->append(gpot);        //in ergebnis liste einfügen
              flag = 1;
            }
          }
        }                       //while (flag)
      gpswitch = gpin;          //listen umtauschen
      gpin = gpout;
      gpout = gpswitch;
    }                           //while (!gpin->isEmpty())

    Stack *st = new Stack;      //ergebnis auf den stack schreiben
#ifdef DEBUGMSG
    cout << "## Anz. Listen: " << gpll.count() << endl;
#endif //DEBUGMSG
    while (!gpll.isEmpty()) {
      NodeList *rnl = new NodeList;
      QPtrList < GeoPos > *grl = gpll.take();
#ifdef DEBUGMSG
      cout << " ## Ele.Anz.: " << grl->count() << endl;
#endif //DEBUGMSG
      while (!grl->isEmpty()) {
        GeoPos *gx = grl->take();
        rnl->insert(gx->key(), gx->node());
      }
      st->push(new StackElemNodeList(*rnl));
    }
    if (gpList)
      delete gpList;
    if (gpt)
      delete gpt;
    if (rs)
      delete rs;
    push(new StackElemStack(*st));
    throw CleanUp(true);
  }
  catch(CleanUp e) {
    if (el1)
      delete el1;
    if (el2)
      delete el2;
    return e.status_;
  }
}

/** set the judgement for the 'stack' see also function 'nl_average' and 'nl_max'
 */
bool Stack::set_judgement()
{
  StackElem *el1 = 0;
  StackElem *el2 = 0;
  try {
    if (count() < 1)
      throw CleanUp(false);     //sind noch genug da?
    el1 = pop();                //von stack holen
    if (el1->type() == StackElem::NODELIST) {   //value
      NodeList & nl = ((StackElemNodeList *) el1)->data();      //liste holen
      el2 = nl.stackPop();      //ele. vom listenstack holen
      //nl.judgement(nl.stackPop());
      if (el2->type() == StackElem::NUMBER) {
        float f = ((StackElemNumber *) el2)->data();    //Wert holen
        nl.set("p", QString().sprintf("%f", f));
#if 0
        nl.judgement(f);
#endif
      }
      else
        throw CleanUp(false);   //wrong type
      push(new StackElemNodeList(nl));  // nodelist back to stack
      throw CleanUp(true);
    }
    else
      throw CleanUp(false);
  }
  catch(CleanUp e) {
    if (el1)
      delete el1;
    if (el2)
      delete el2;
    return e.status_;
  }
}

/** calculate average over all nodes (menmber of list). Take all top local stack
values from node and write the result in the 'nodelist' stack.
see also function 'set_judgement' of class 'stack' */
//... [nodelist] -> [nodelist]
bool Stack::nl_average()
{
  StackElem *el1 = 0;
  StackElem *el2 = 0;
  try {
    if (count() < 1)
      throw CleanUp(false);     //sind noch genug da?
    el1 = pop();                //von stack holen
    if (el1->type() == StackElem::NODELIST) {   //list
      NodeList & nl = ((StackElemNodeList *) el1)->data();      //liste holen
      NodeList *selected = new NodeList;        // new nodelist for result
      QDictIterator < Node > it(nl);
      float sum = 0.0, val = 0.0;
      int count = 0;
      while (it.current()) {
        Node *node = it.current();      //nl.find(*it);
        selected->insert(it.currentKey(), node);
        if (node->stackCount() < 1)
          throw CleanUp(false); //sind noch genug da?
        el2 = node->stackPop();
        if (el2->type() == StackElem::NUMBER) {
          val = ((StackElemNumber *) el2)->data();      //Wert holen
          count++;
          sum += val;
        }
        ++it;
      }
      //Y nl.stackPush(new StackElemNumber(sum/float(val))); //average to nodelist stack
      selected->stackPush(new StackElemNumber(sum / float (count)));    //average to nodelist stack
      push(new StackElemNodeList(*selected));
      //Y push(new StackElemNodeList(nl));// nodelist back to stack
      throw CleanUp(true);
    }
    else
      throw CleanUp(false);
  }
  catch(CleanUp e) {
    if (el1)
      delete el1;
    if (el2)
      delete el2;
    return e.status_;
  }
}

/** calculate max over all nodes (menmber of list). Take all top local stack
values from node and write the result in the 'nodelist' stack.
see also function 'set_judgement' of class 'stack' */
//... [nodelist] -> [nodelist]
bool Stack::nl_max()
{
  StackElem *el1 = 0;
  StackElem *el2 = 0;
  try {
    if (count() < 1)
      throw CleanUp(false);     //sind noch genug da?
    el1 = pop();                //von stack holen
    if (el1->type() == StackElem::NODELIST) {   //list
      NodeList & nl = ((StackElemNodeList *) el1)->data();      //liste holen
      NodeList *selected = new NodeList;        // new nodelist for result
      QDictIterator < Node > it(nl);
      float val = 0.0, max = -9999999.0;
      while (it.current()) {
        Node *node = it.current();      //nl.find(*it);
        selected->insert(it.currentKey(), node);
        if (node->stackCount() < 1)
          throw CleanUp(false); //sind noch genug da?
        el2 = node->stackPop();
        if (el2->type() == StackElem::NUMBER) {
          val = ((StackElemNumber *) el2)->data();      //Wert holen
          if (val > max)
            max = val;
        }
        ++it;
      }
      //Y nl.stackPush(new StackElemNumber(max));//max to nodelist stack
      selected->stackPush(new StackElemNumber(max));    //max to nodelist stack
      push(new StackElemNodeList(*selected));
      //Y push(new StackElemNodeList(nl));// nodelist back to stack
      throw CleanUp(true);
    }
    else
      throw CleanUp(false);
  }
  catch(CleanUp e) {
    if (el1)
      delete el1;
    if (el2)
      delete el2;
    return e.status_;
  }
}

/** calculate sum over all nodes (menmber of list). Take all top local stack
values from node and write the result in the 'nodelist' stack.
see also function 'set_judgement' of class 'stack' */
//... [nodelist] -> [nodelist]
bool Stack::nl_sum()
{
  StackElem *el1 = 0;
  StackElem *el2 = 0;
  try {
    if (count() < 1)
      throw CleanUp(false);     //sind noch genug da?
    el1 = pop();                //von stack holen
    if (el1->type() == StackElem::NODELIST) {   //list
      NodeList & nl = ((StackElemNodeList *) el1)->data();      //liste holen
      NodeList *selected = new NodeList;        // new nodelist for result
      QDictIterator < Node > it(nl);
      float sum = 0.0, val = 0.0;
      int count = 0;
      while (it.current()) {
        Node *node = it.current();      //nl.find(*it);
        selected->insert(it.currentKey(), node);
        if (node->stackCount() < 1)
          throw CleanUp(false); //sind noch genug da?
        el2 = node->stackPop();
        if (el2->type() == StackElem::NUMBER) {
          val = ((StackElemNumber *) el2)->data();      //Wert holen
          count++;
          sum += val;
        }
        ++it;
      }
#ifdef DEBUGMSG
      cout << "sum= " << sum << endl;
#endif //DEBUGMSG

      selected->stackPush(new StackElemNumber(sum));    //       sum to nodelist stack
      push(new StackElemNodeList(*selected));
      throw CleanUp(true);
    }
    else
      throw CleanUp(false);
  }
  catch(CleanUp e) {
    if (el1)
      delete el1;
    if (el2)
      delete el2;
    return e.status_;
  }
}

/** calculate sum over all nodes (menmber of list). Take all top local stack
values from node and write the result in the 'nodelist' stack.
see also function 'set_judgement' of class 'stack' */
//... [nodelist] -> [nodelist]
bool Stack::nl_div()
{
  StackElem *el1 = 0;
  StackElem *el2 = 0;
  StackElem *el3 = 0;
  StackElem *el4 = 0;
  try {
    if (count() < 2)
      throw CleanUp(false);     //sind noch genug da?
    el1 = pop();                //von stack holen
    el2 = pop();                //von stack holen        
    if ((el1->type() == StackElem::NODELIST) && (el1->type() == StackElem::NODELIST)) { // 2 lists
      NodeList & nl1 = ((StackElemNodeList *) el1)->data();     //       liste 1 holen
      NodeList & nl2 = ((StackElemNodeList *) el2)->data();     //       liste 2 holen
      NodeList *selected = new NodeList;        // new nodelist for result
      //      2 values from nodelists
      float div1, div2, div;
      el3 = nl1.stackPop();
      el4 = nl2.stackPop();
      if (el3->type() == StackElem::NUMBER)
        div1 = ((StackElemNumber *) el3)->data();
      else
        throw CleanUp(false);
      if (el4->type() == StackElem::NUMBER)
        div2 = ((StackElemNumber *) el4)->data();
      else
        throw CleanUp(false);
      //      nodelists empty
      if (div1 + div2 == 0.0)
        div = 0.0;
		else
        div = div1 / (div1+div2);
#ifdef DEBUGMSG
      cout << div1 << " / " << div1+div2 << "=Ergebnis= " << div << endl;
#endif
      //      compose result
      selected->stackPush(new StackElemNumber(div));

      QDictIterator < Node > it1(nl1);
      while (it1.current()) {
        Node *node1 = it1.current();
        selected->insert(it1.currentKey(), node1);
        ++it1;
      }
      QDictIterator < Node > it2(nl2);
      while (it2.current()) {
        Node *node2 = it2.current();
        selected->insert(it2.currentKey(), node2);
        ++it2;
      }

      push(new StackElemNodeList(*selected));
      throw CleanUp(true);
    }
    else
      throw CleanUp(false);
  }
  catch(CleanUp e) {
    if (el1)
      delete el1;
    if (el2)
      delete el2;
    return e.status_;
  }
}

/** count number of nodes in nodelist and
write result to nodelist stack */
//... [nodelist] -> [nodelist]
bool Stack::nl_count()
{
  StackElem *el1 = 0;
  try {
    if (count() < 1)
      throw CleanUp(false);     //sind noch genug da?
    el1 = pop();                //von stack holen
    if (el1->type() == StackElem::NODELIST) {   // 1 lists
      NodeList & nl1 = ((StackElemNodeList *) el1)->data();     //       liste 1 holen
      NodeList *selected = new NodeList;        // new nodelist for result
      //       values from nodelists
      int sum = 0;

      QDictIterator < Node > it1(nl1);
      while (it1.current()) {
        Node *node1 = it1.current();
        selected->insert(it1.currentKey(), node1);
        sum++;
        ++it1;
      }

      selected->stackPush(new StackElemNumber(sum));

#ifdef DEBUGMSG
      cout << "in nl_count: Anzahl=" << sum << endl;
#endif

      push(new StackElemNodeList(*selected));
      throw CleanUp(true);
    }
    else
      throw CleanUp(false);
  }
  catch(CleanUp e) {
    if (el1)
      delete el1;
    return e.status_;
  }
}
