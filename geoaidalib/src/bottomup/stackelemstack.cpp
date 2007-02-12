/***************************************************************************
                          stackelemstack.cpp  -  description
                             -------------------
    begin                : Tue Nov 13 2001
    copyright            : (C) 2001 by Martin Pahl
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

#include "stackelemstack.h"


StackElemStack::StackElemStack(const Stack& stack){
  //stack_=stack;
  QPtrStackIterator<StackElem> it(stack);
  for (;it.current();++it) {
    stack_.push((*it)->copy());
  }
}

StackElemStack::StackElemStack(const StackElemStack& elem){
  //stack_=elem.stack_;
  QPtrStackIterator<StackElem> it(elem.stack_);
  for (;it.current();++it) {
    stack_.push((*it)->copy());
  }
}

StackElemStack::~StackElemStack(){
}

Stack& StackElemStack::data(){
  return stack_;
}

int StackElemStack::type(){
  return STACK;
}

const char* StackElemStack::typeName()
{
  return "stack";
}

StackElem* StackElemStack::copy() {
  Stack val=((StackElemStack*)this)->data(); //liste holen
  return (new StackElemStack(val));
}