/*-*-c++-*-
 *
 *    $RCSfile: listener.h,v $
 *     Project:
 * Description:
 *
 *   Author(s): Lars Langhans
 *       $Date: 2005/01/19 12:13:58 $
 *   Copyright: (c)1996 by Lars Langhans
 *   $Revision: 1.2 $
 */


#ifndef _LISTENER_H
#define _LISTENER_H

#include <vector>
#include "listenerabfaenger.h"

// template <typename T>
// #define T ListenerAbfaenger

class Listener
{
protected:
    std::vector< ListenerAbfaenger* > m_aListener;
public:
    Listener(){}

    virtual ~Listener()
        {
            m_aListener.clear();
        }
    

    virtual void addListener( ListenerAbfaenger * _t) 
        {
            m_aListener.push_back(_t);
        }
    virtual void removeListener( ListenerAbfaenger * _t)
        {
            // std::vector<ListenerAbfaenger>::iterator it = m_aListener.find(_t);
            // if (it != m_aListener.end())
            // {
            //     m_aListener.erase(it);
            // }
        }
    virtual void actionPerformed(int _a)
        {
            for(std::vector< ListenerAbfaenger* >::const_iterator it = m_aListener.begin();
                it != m_aListener.end();
                ++it)
            {
                (*it)->handleAnEvent(_a);
            }
        }   
    virtual void actionPerformed(std::string const& _s)
        {
            for(std::vector< ListenerAbfaenger* >::const_iterator it = m_aListener.begin();
                it != m_aListener.end();
                ++it)
            {
                (*it)->handleAnEvent(_s);
            }
        }
};

// -----------------------------------------------------------------------------

// template <typename T>
/*
class ChangeDirectoryListener : public Listener
{
public:
    ChangeDirectoryListener() {}
    
};
*/

#endif
