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
/// \file       configurator.cpp
/// \brief      Implementation of class "Configurator"
///
/// \date       2010-03-16
/// \author     Torsten Bueschenfeld (bfeld@tnt.uni-hannover.de)
///
////////////////////////////////////////////////////////////////////////////////

#include "configurator.h"

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Constructor
///
///////////////////////////////////////////////////////////////////////////////
Configurator::Configurator()
{
    METHOD_ENTRY("Configurator::Configurator");
    CTOR_CALL("Configurator::Configurator");

    METHOD_EXIT("Configurator::Configurator");
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Returns value for a given key
///
/// \param _strKey Key of parameter
///
/// \return Returns the value
///
///////////////////////////////////////////////////////////////////////////////
double Configurator::getFlt(const std::string& _strKey)
{
    METHOD_ENTRY("Configurator::getFlt");
    
    std::string strValue = m_Parameters[_strKey];
    double fValue;
    
    std::istringstream iss(strValue);
    iss >> fValue;
    
    if (!iss.eof())
    {
        WARNING_MSG("Configurator", "Value for \"" << _strKey << "\" is not a valid float value.")
    }
    DOM_VAR(DEBUG_MSG("Configurator", "Returned value: " << fValue))
    
    METHOD_EXIT("Configurator::getFlt");
    return fValue;
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Returns value for a given key
///
/// \param _strKey Key of parameter
///
/// \return Returns the value
///
///////////////////////////////////////////////////////////////////////////////
int Configurator::getInt(const std::string& _strKey)
{
    METHOD_ENTRY("Configurator::getInt");
    
    std::string strValue = m_Parameters[_strKey];
    int nValue;
    
    std::istringstream iss(strValue);
    iss >> nValue;
    
    if (!iss.eof())
    {
        WARNING_MSG("Configurator", "Value for \"" << _strKey << "\" is not a valid integer value.")
    }
    
    DOM_VAR(DEBUG_MSG("Configurator", "Returned value: " << nValue))
    
    METHOD_EXIT("Configurator::getInt");
    return nValue;
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Returns value for a given key
///
/// \param _strKey Key of parameter
///
/// \return Returns the value
///
///////////////////////////////////////////////////////////////////////////////
std::string Configurator::getStr(const std::string& _strKey)
{
    METHOD_ENTRY("Configurator::getInt");
    
    METHOD_EXIT("Configurator::getInt");
    return m_Parameters[_strKey];
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Loads configuration parameters
///
/// \param _strFilename Filename of configuration file
///
/// \return Returns if loading was succesful
///
///////////////////////////////////////////////////////////////////////////////
bool Configurator::loadConfig(const std::string& _strFilename)
{
    METHOD_ENTRY("Configurator::loadConfig");

    INFO_MSG("Configurator", "Loading configuration file " << _strFilename << ".")

    std::ifstream infile;
    infile.open(_strFilename.c_str());
    if (infile.fail())
    {
        ERROR_MSG("Configurator", "Can't open " << _strFilename)
        METHOD_EXIT("Configurator::loadConfig");
        return false;
    }
    
    // Delete a previously loaded configuration
    m_Parameters.clear();
    
    bool bFailed = false;
    std::string strKey;
    std::string strValue;
    while (!infile.eof())
    {
        bFailed = false;
        infile >> strKey;
        if (strKey.at(0) == '#')
        {
            getline(infile, strValue);
        }
        else
        {
            if (infile.fail()) bFailed = true;
            infile >> strValue;
            if (infile.fail()) bFailed = true;
            
            if (!bFailed)
            {
                m_Parameters[strKey] = strValue;
                DOM_VAR(DEBUG_MSG("Configurator", "Key, Value: \"" << strKey << "\" \"" << strValue << "\""))
            }
        }
    }
    
    infile.close();
    
//     if (bFailed)
//     {
//         ERROR_MSG("Configurator", "Errors in configuration file. ", LOG_DOMAIN_FILEIO);
//         METHOD_EXIT("Configurator::loadConfig");
//         return false;
//     }
    
    METHOD_EXIT("Configurator::loadConfig");
    return true;
}
