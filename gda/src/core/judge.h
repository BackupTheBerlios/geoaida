/***************************************************************************
                          judge.h  -  description
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
 * $Source: /data/cvs/gda/gda/core/judge.h,v $
 * $Revision: 1.2 $
 * $Date: 2001/11/27 16:35:30 $
 * $Author: pahl $
 * $Locker:  $
 */

#ifndef JUDGE_H
#define JUDGE_H


/**basisclass for the weighing of objects like INode or the whole interpretation
  *@author Jürgen Bückner
  */

class Judge
{
public:
  /** default constructor */
  Judge();

  /** destructor */
  virtual ~ Judge();

  /** compare two judgement; return [TRUE, FALSE] */
  virtual bool operator < (Judge & j);

  /** compare two judgement; return [TRUE, FALSE] */
  virtual bool operator > (Judge & j);

  /** compare two judgement; return [TRUE, FALSE] */
  virtual bool operator == (Judge & j);

  /** calculate the judgement */
  virtual void calc(void);

  /** set the judgement. A scalar value between [0...1] */
  virtual void value(float v);

  /** provided the judgemnet. A scalar value between 0...1. [0..1] */
  virtual float value(void);

protected:                     // Protected attributes
  /** the scalar judgemnet */
  float value_;

};

#endif
