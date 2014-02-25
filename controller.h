/*-*-c++-*-
 *
 *    $RCSfile: controller.h,v $
 *     Project:
 * Description:
 *
 *   Author(s): Lars Langhans
 *       $Date: 2005/06/23 07:38:48 $
 *   Copyright: (c)1996 by Lars Langhans
 *   $Revision: 1.5 $
 */

/* ==========================================================================
 *
 *               HISTORY
 *               -------
 *       $Log: controller.h,v $
 *       Revision 1.5  2005/06/23 07:38:48  langha_l
 *       start with -Weffc++ cleanups
 *
 *       Revision 1.4  2005/05/23 20:00:46  langha_l
 *       add MutexGuard
 *
 *       Revision 1.3  2005/02/17 19:48:58  langha_l
 *       remove PropertyValue
 *
 *       Revision 1.2  2005/01/25 18:23:51  langha_l
 *       add new filter system
 *
 *       Revision 1.1  2005/01/19 12:13:58  langha_l
 *       much changes for version 2, more modulisation
 *
 */

#ifndef _CONTROLLER_H
#define _CONTROLLER_H

#include <string>
#include <map>
#include "mutexguard.h"

typedef std::string KeyType;

class SingleContainer
{
    std::string m_sName;

protected:
    // must be protected to suppress the possibility to set an Object back to a
    // SingleContainer
    // sample: set a Double to a SingleContainer and so on.
    SingleContainer(SingleContainer const& _aOther)
            :m_sName(_aOther.m_sName)
        {
            // m_sName = _aOther.m_sName;
        }
    
    SingleContainer& operator=(SingleContainer const& _aOther)
        {
            if (this != &_aOther)
            {
                m_sName = _aOther.m_sName;
            }
            return *this;
        }
    
public:
    SingleContainer()
            :m_sName("Empty")
        {}
    
    virtual ~SingleContainer() {}
    SingleContainer(std::string const& _sName)
            :m_sName(_sName)
        {}
    std::string getTypeName() const
        {
            return m_sName;
        }

    bool isInstanceOf(SingleContainer const& _aOtherSingleContainer) const
        {
            if (this->getTypeName() == _aOtherSingleContainer.getTypeName())
            {
                return true;
            }
            return false;
        }
    
};

class Integer : public SingleContainer
{
    int m_nValue;
public:
    Integer(int _nValue)
            :SingleContainer("Integer"),
             m_nValue(_nValue)
        {}
    int intValue() const
        {
            return m_nValue;
        }    

    
};

class Double : public SingleContainer
{
    double m_nValue;
public:
    Double(double _nValue)
            :SingleContainer("Double"),
             m_nValue(_nValue)
        {}
    double doubleValue() const
        {
            return m_nValue;
        }
};

class String: public SingleContainer
{
    std::string m_sValue;
public:
    String(std::string const& _sValue)
            :SingleContainer("String"),
             m_sValue(_sValue)
        {}
    std::string stringValue() const
        {
            return m_sValue;
        }
};

class Boolean: public SingleContainer
{
    bool m_bValue;
public:
    Boolean(bool _bValue)
            :SingleContainer("Boolean"),
             m_bValue(_bValue)
        {}
    bool booleanValue() const
        {
            return m_bValue;
        }
};

// -----------------------------------------------------------------------------

Integer* getIntegerType();
Double* getDoubleType();
String* getStringType();
Boolean* getBooleanType();

// -----------------------------------------------------------------------------

typedef std::map<KeyType, SingleContainer*> MapStringSingleContainer;

// -----------------------------------------------------------------------------
class Controller
{
    MapStringSingleContainer m_aMap;
    SDL_mutex *m_pLock;
    
private:
    Controller(Controller const&);
    Controller& operator=(Controller const&);
public:
    Controller();
    ~Controller();

    void setPropertyValue(KeyType const& _sKey, SingleContainer* _pValue);
    void setPropertyValue(KeyType const& _sKey, SingleContainer* _pValue, bool _bRefresh);
    const SingleContainer* getPropertyValue(KeyType const& _sKey);

    int         getPropertyAsIntValue(KeyType const& _sKey);
    double      getPropertyAsDoubleValue(KeyType const& _sKey);
    std::string getPropertyAsStringValue(KeyType const& _sKey);
    bool        getPropertyAsBooleanValue(KeyType const& _sKey);

    bool hasPropertyChanged()
        {
            return true;
        }

    // returns true, if the given property exists
    bool hasPropertyValue(KeyType const& _sKey);
    void removePropertyValue(KeyType const& _sKey);
};

// -----------------------------------------------------------------------------
class ControllerHelper
{
    Controller *m_pController;
private:
    ControllerHelper(ControllerHelper const&);
    ControllerHelper& operator=(ControllerHelper const&);
protected:
    Controller* getController() const {return m_pController;}
    bool hasAController() const
        {
            return (m_pController != NULL) ? true : false;
        }
    
public:
    ControllerHelper() 
            :m_pController(NULL)
        {}

    virtual ~ControllerHelper()
        {
            m_pController = NULL;
        }
    
    void setController(Controller *_pController)
        {
            m_pController = _pController;
        }

    bool hasPropertyChanged()
        {
            bool bHasChanged = false;
            if (m_pController)
            {
                bHasChanged = m_pController->hasPropertyChanged();
            }
            
            return bHasChanged;
        }

};
#endif
