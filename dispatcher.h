/*-*-c++-*-
 *
 *    $RCSfile: dispatcher.h,v $
 *     Project:
 * Description:
 *
 *   Author(s): Lars Langhans
 *       $Date: 2005/11/28 20:39:37 $
 *   Copyright: (c)1996 by Lars Langhans
 *   $Revision: 1.9 $
 */

#ifndef _dispatcher_h
#define _dispatcher_h

#include <string>
#include <SDL_mutex.h>
#include <queue>
#include "mutexguard.h"

class EventContainer
{
    int m_nEvent;
    double      m_nAddValue;
    std::string m_sStringValue;

public:
    EventContainer();
    explicit EventContainer(int _nEvent);
    EventContainer(int _nEvent, double _nAddValue);
    EventContainer(int _nEvent, std::string const& _sString);
    
    int    getEvent();
    double getValue();
    std::string getString();
    bool   isValueValid();
};

// -----------------------------------------------------------------------------

class Dispatcher
{
    SDL_mutex *m_pLock;
    
private:
    Dispatcher(Dispatcher const&);
    Dispatcher& operator=(Dispatcher const&);

//     std::queue<EventValue> m_aDispatchFifo;
    std::vector<EventContainer> m_aDispatchFifo;
protected:
    EventContainer peekEvent();
    void popEvent();
    bool hasANewEvent();
    virtual void showEvent(int _nCurrentEvent) =0;
    void showEvents();
public:
    Dispatcher();
    virtual ~Dispatcher();
    void LockInsertEvent();
    void UnLockInsertEvent();
    void insertNewEvent(EventContainer const& _nNewEvent);
    void insertNewEventNoLock(EventContainer const& _nNewEvent);
    void insertEventAtFront(EventContainer const& _nNewEvent);
    virtual void handleAnEvent() =0;
};


#endif
