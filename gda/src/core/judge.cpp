/***************************************************************************
                          judge.cpp  -  description
                             -------------------
    begin                : Thu Oct 19 2000
    copyright            : (C) 2000 by Jürgen Bückner
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

/*
 * $Source: /data/cvs/gda/gda/core/judge.cpp,v $
 * $Revision: 1.2 $
 * $Date: 2001/11/27 16:35:30 $
 * $Author: pahl $
 * $Locker:  $
 */

#include "judge.h"

Judge::Judge()
{
}

Judge::~Judge()
{
}

/** compare two judgement; return [TRUE, FALSE] */
bool
Judge::operator < (Judge & j)
{
  return (this->value_ < j.value_);
}

/** compare two judgement; return [TRUE, FALSE] */
bool
Judge::operator > (Judge & j)
{
  return (this->value_ > j.value_);
}

/** compare two judgement; return [TRUE, FALSE] */
bool
  Judge::operator == (Judge & j) {
  return (this->value_ == j.value_);
}

/** calculate the judgement */
void
Judge::calc(void)
{
  value_ = 0.5;
}

/** provided the judgemnet. A scalar value between 0...1. [0..1] */
float
Judge::value(void)
{
  return value_;
}

/** set the judgement. A scalar value between [0...1] */
void
Judge::value(float v)
{
  value_ = v;
}
