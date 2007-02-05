/***************************************************************************
                          gashareddata.h  -  description
                             -------------------
    begin                : Wed Feb 21 2001
    copyright            : (C) 2001 by Martin Pahl
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

#ifndef GASHAREDDATA_H
#define GASHAREDDATA_H

namespace Ga {

/**class to manage shared data
  *@author Oliver Stahlhut
  */

class SharedData {
public: 
	SharedData() {
		count_=1;
	}
	~SharedData() {}
	int ref() { return ++count_; }
	bool deref() {return ! --count_;}
	int count() {return count_; }
private:
	int count_;
	
};

} // namespace Ga

#endif
