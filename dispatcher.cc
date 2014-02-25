/*-*-c++-*-
 *
 *    $RCSfile: dispatcher.cc,v $
 *     Project:
 * Description:
 *
 *   Author(s): Lars Langhans
 *       $Date: 2005/11/28 20:39:37 $
 *   Copyright: (c)1996 by Lars Langhans
 *   $Revision: 1.5 $
 */

// damit kann RCS im executeable noch die Version feststellen
// static char rcsid[] = "$Id: dispatcher.cc,v 1.5 2005/11/28 20:39:37 langha_l Exp $";


#include "dispatcher.h"
#include "t_print.h"

#define MAGIC_NAN 3.1415926

EventContainer::EventContainer()
        :m_nEvent(0),
         m_nAddValue(MAGIC_NAN)
{}
    
EventContainer::EventContainer(int _nEvent)
        :m_nEvent(_nEvent),
         m_nAddValue(MAGIC_NAN)
{}
EventContainer::EventContainer(int _nEvent, double _nAddValue)
        :m_nEvent(_nEvent),
         m_nAddValue(_nAddValue)
{}

EventContainer::EventContainer(int _nEvent, std::string const& _sString)
        :m_nEvent(_nEvent),
         m_sStringValue(_sString)
{}

int    EventContainer::getEvent() {return m_nEvent;}
double EventContainer::getValue() {return m_nAddValue;}
std::string EventContainer::getString() {
    return m_sStringValue;
}

bool   EventContainer::isValueValid()
{
    if (m_nAddValue == MAGIC_NAN)
    {
        return false;
    }
    return true;
}

// -----------------------------------------------------------------------------

EventContainer Dispatcher::peekEvent()
{
    MutexGuard pv(m_pLock);
    // SDL_mutexP(m_pLock);
    EventContainer nEvent;
    if (m_aDispatchFifo.empty() == false)
    {
        nEvent = m_aDispatchFifo.front();
    }
    // SDL_mutexV(m_pLock);
    return nEvent;
}
bool Dispatcher::hasANewEvent()
{
    MutexGuard pv(m_pLock);
    if (m_aDispatchFifo.empty() == true)
    {
        return false;
    }
    return true;
}

void Dispatcher::popEvent()
{
    t_print(T_DEBUG, "Dispatcher::popEvent()\n");
    MutexGuard aGuard(m_pLock);
    // SDL_mutexP(m_pLock);
    // m_aDispatchFifo.pop();
    t_print(T_DEBUG, "Size of fifo %d\n", m_aDispatchFifo.size());
    
    std::vector<EventContainer>::iterator it = m_aDispatchFifo.begin();
    m_aDispatchFifo.erase(it);
    // SDL_mutexV(m_pLock);
}

Dispatcher::Dispatcher()
{
    m_pLock = SDL_CreateMutex();
}

Dispatcher::~Dispatcher()
{
    SDL_DestroyMutex(m_pLock);
}


void Dispatcher::LockInsertEvent()
{
    SDL_mutexP(m_pLock);
}
void Dispatcher::insertNewEventNoLock(EventContainer const& _nNewEvent)
{
    m_aDispatchFifo.push_back(_nNewEvent);
}

void Dispatcher::insertNewEvent(EventContainer const& _nNewEvent)
{
    MutexGuard pv(m_pLock);
    // SDL_mutexP(m_pLock);
    insertNewEventNoLock(_nNewEvent);
    // SDL_mutexV(m_pLock);
}
void Dispatcher::UnLockInsertEvent()
{
    SDL_mutexV(m_pLock);
}

void Dispatcher::insertEventAtFront(EventContainer const& _nNewEvent)
{
    MutexGuard pv(m_pLock);
    // SDL_mutexP(m_pLock);
    m_aDispatchFifo.insert(m_aDispatchFifo.begin(), _nNewEvent);
    // SDL_mutexV(m_pLock);
}
    

void Dispatcher::showEvents()
{
    if (! m_aDispatchFifo.empty())
    {
        t_print(T_AUDIOPLAYCONTROL, "List events\n");
    
        for (std::vector<EventContainer>::iterator it = m_aDispatchFifo.begin();
             it != m_aDispatchFifo.end();
             it++)
        {
            EventContainer aCurrentEvent = *it;
            t_print(T_AUDIOPLAYCONTROL, "   ");
            showEvent(aCurrentEvent.getEvent());
        }
    }
    
}
