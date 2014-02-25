/*-*-c++-*-
 *
 *    $RCSfile: controller.cc,v $
 *     Project:
 * Description:
 *
 *   Author(s): Lars Langhans
 *       $Date: 2005/05/23 20:00:46 $
 *   Copyright: (c)1996 by Lars Langhans
 *   $Revision: 1.4 $
 */

// damit kann RCS im executeable noch die Version feststellen
// static char rcsid[] = "$Id: controller.cc,v 1.4 2005/05/23 20:00:46 langha_l Exp $";

/* ==========================================================================
 *
 *               HISTORY
 *               -------
 *       $Log: controller.cc,v $
 *       Revision 1.4  2005/05/23 20:00:46  langha_l
 *       add MutexGuard
 *
 *       Revision 1.3  2005/02/17 19:48:45  langha_l
 *       remove PropertyValue
 *
 *       Revision 1.2  2005/01/25 18:23:51  langha_l
 *       add new filter system
 *
 *       Revision 1.1  2005/01/19 12:13:58  langha_l
 *       much changes for version 2, more modulisation
 *
 */

#include "global.h"
#include "controller.h"
#include "t_print.h"


Controller::Controller()
        :m_pLock(SDL_CreateMutex())
{
}

Controller::~Controller()
{
    t_print(T_DEBUG, "Controller is going away.\n");

    // empty the map
    for (MapStringSingleContainer::const_iterator it=m_aMap.begin();
         it != m_aMap.end();
         it++)
    {
        std::pair<std::string, SingleContainer*> aPair = *it;
        delete aPair.second;
    }
    SDL_DestroyMutex(m_pLock);
}

void Controller::setPropertyValue(KeyType const& _sKey, SingleContainer* _pValue)
{
    setPropertyValue(_sKey, _pValue, true);
}

void Controller::setPropertyValue(KeyType const& _sKey, SingleContainer* _pValue, bool _bSendUserEvent)
{
    MutexGuard pv(m_pLock);
    MapStringSingleContainer::const_iterator it = m_aMap.find(_sKey);
    if (it != m_aMap.end())
    {
        // An entry already exist.
        // get it, delete it, erase the map entry.
        
        SingleContainer* pOldValue = m_aMap[ _sKey ];
        // SingleContainer* pOldValue2 = *it.second;
        // if (pOldValue2 != pOldValue)
        // {
        //     t_print("setPropertyValue already set and both get methods are equal.\n");
        // }
        
        delete pOldValue;
        // m_aMap.erase(it);
    }
    // set new value.
    m_aMap[_sKey] = _pValue;

    if (_bSendUserEvent == true)
    {
        // this is a need, because we only handle things if there occur an event
        // most the time key events, but here it is a simple empty user event.
        Global::createUserEvent(_sKey);
    }
}


bool Controller::hasPropertyValue(KeyType const& _sKey)
{
    MutexGuard pv(m_pLock);
    if (m_aMap.find(_sKey) == m_aMap.end())
    {
        return false;
    }
    return true;
}

void Controller::removePropertyValue(KeyType const& _sKey)
{
    MutexGuard pv(m_pLock);
    MapStringSingleContainer::iterator it = m_aMap.find(_sKey);
    if (it != m_aMap.end())
    {
        // An entry already exist.
        // get it, delete it, erase the map entry.
        
        SingleContainer* pOldValue = m_aMap[ _sKey ];
        delete pOldValue;
        m_aMap.erase(it);
    }
}

const SingleContainer* Controller::getPropertyValue(KeyType const& _sKey)
{
    MutexGuard pv(m_pLock);
    if (m_aMap.find(_sKey) == m_aMap.end())
    {
        t_print(T_DEBUG, "WARNING: key %s not set, before used, it is allowed but not really useful.\n", _sKey.c_str());
        return NULL;
    }
    
    const SingleContainer* pValue = m_aMap[ _sKey ];
    return pValue;
}

int Controller::getPropertyAsIntValue(KeyType const& _sKey)
{
    MutexGuard pv(m_pLock);
    int nValue = 0;
    const SingleContainer* pProperty = getPropertyValue(_sKey);
    if (pProperty != NULL)
    {
        if (getIntegerType()->isInstanceOf(*pProperty))
        {
            const Integer* pIntValue = static_cast<const Integer*>(pProperty);
            nValue = pIntValue->intValue();
        }
    }
    return nValue;
}

double Controller::getPropertyAsDoubleValue(KeyType const& _sKey)
{
    MutexGuard pv(m_pLock);
    double nValue = 0;
    const SingleContainer* pProperty = getPropertyValue(_sKey);
    if (pProperty != NULL)
    {
        if (getDoubleType()->isInstanceOf(*pProperty))
        {
            const Double* pDoubleValue = static_cast<const Double*>(pProperty);
            nValue = pDoubleValue->doubleValue();
        }
    }
    return nValue;
}

std::string Controller::getPropertyAsStringValue(KeyType const& _sKey)
{
    MutexGuard pv(m_pLock);
    std::string sValue;
    const SingleContainer* pProperty = getPropertyValue(_sKey);
    if (pProperty != NULL)
    {
        if (getStringType()->isInstanceOf(*pProperty))
        {
            const String* pStringValue = static_cast<const String*>(pProperty);
            sValue = pStringValue->stringValue();
        }
    }
    return sValue;
}

bool Controller::getPropertyAsBooleanValue(KeyType const& _sKey)
{
    MutexGuard pv(m_pLock);
    bool bValue = false;
    const SingleContainer* pProperty = getPropertyValue(_sKey);
    if (pProperty != NULL)
    {
        if (getBooleanType()->isInstanceOf(*pProperty))
        {
            const Boolean* pBooleanValue = static_cast<const Boolean*>(pProperty);
            bValue = pBooleanValue->booleanValue();
        }
    }
    return bValue;
}
// ------------------------------- type functions -------------------------------
Integer* getIntegerType()
{
    static Integer* p = NULL;
    if (p == NULL)
    {
        p = new Integer(0);
    }
    return p;
}

Double* getDoubleType()
{
    static Double* p = NULL;
    if (p == NULL)
    {
        p = new Double(0.0);
    }
    return p;
}

String* getStringType()
{
    static String* p = NULL;
    if (p == NULL)
    {
        p = new String(std::string());
    }
    return p;
}

Boolean* getBooleanType()
{
    static Boolean* p = NULL;
    if (p == NULL)
    {
        p = new Boolean(false);
    }
    return p;
}
