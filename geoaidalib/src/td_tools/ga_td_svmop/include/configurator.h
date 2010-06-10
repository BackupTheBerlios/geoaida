/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

////////////////////////////////////////////////////////////////////////////////
///
/// \file       configurator.h
/// \brief      Prototype of class "Configurator"
///
/// \date       2010-03-16
/// \author     Torsten Bueschenfeld (bfeld@tnt.uni-hannover.de)
///
////////////////////////////////////////////////////////////////////////////////

#ifndef CONFIGURATOR_H
#define CONFIGURATOR_H

//--- Program header ---------------------------------------------------------//
#include "log.h"

//--- Standard header --------------------------------------------------------//
#include <fstream>
#include <map>
#include <sstream>

//--- Misc. header -----------------------------------------------------------//

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Class to read configuration from textfiles
///
////////////////////////////////////////////////////////////////////////////////
class Configurator
{
    
    public:

        //--- Constructor/Destructor -----------------------------------------//
        Configurator();
        
        //--- Const Methods --------------------------------------------------//
        
        //--- Methods --------------------------------------------------------//
//         void add(const std::string&, const double&);
//         void add(const std::string&, const int&);
//         void add(const std::string&, const std::string&);

        double      getFlt(const std::string&);        
        int         getInt(const std::string&);
        std::string getStr(const std::string&);
        
        bool loadConfig(const std::string&);
        
    private:
    
        //--- Methods --------------------------------------------------------//
        
        //--- Variables ------------------------------------------------------//
        std::map<std::string, std::string> m_Parameters;       

};

//--- Implementation is done here for inlining -------------------------------//

#endif