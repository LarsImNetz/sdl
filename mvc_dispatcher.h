/*-*-c++-*-
 *
 *    $RCSfile: mvc_dispatcher.h,v $
 *     Project:
 * Description:
 *
 *   Author(s): Lars Langhans
 *       $Date: 2006/04/18 17:54:40 $
 *   Copyright: (c)1996 by Lars Langhans
 *   $Revision: 1.7 $
 */

/* ==========================================================================
 *
 *               HISTORY
 *               -------
 *       $Log: mvc_dispatcher.h,v $
 *       Revision 1.7  2006/04/18 17:54:40  langha_l
 *       comment
 *
 *       Revision 1.6  2006/01/25 17:55:17  langha_l
 *       mvc_dispatcher.cc
 *
 *       Revision 1.5  2005/07/20 07:03:14  langha_l
 *       little window movements, new loudness view
 *
 *       Revision 1.4  2005/06/23 07:38:48  langha_l
 *       start with -Weffc++ cleanups
 *
 *       Revision 1.3  2005/05/23 20:07:01  langha_l
 *       add a Timer to repaint all
 *
 *       Revision 1.2  2005/01/25 18:23:51  langha_l
 *       add new filter system
 *
 *       Revision 1.1  2005/01/19 12:13:58  langha_l
 *       much changes for version 2, more modulisation
 *
 */

#ifndef _MVC_DISPATCHER_H
#define _MVC_DISPATCHER_H

// -----------------------------------------------------------------------------
// This is the Model-View-Control Dispatcher
// maybe there is a better name

#include "i_dispatch.h"
#include "i_view.h"
#include "controller.h"
#include "i_handlecontroller.h"
#include "mutexguard.h"

class MVC_Dispatcher;

class TimerHelper
{
    bool bStopTimer;
    SDL_Surface *m_pDisplay;

private:
    TimerHelper(TimerHelper const&);
    TimerHelper& operator=(TimerHelper const&);

public:
    SDL_TimerID nID;

    TimerHelper(SDL_Surface *_pDisplay)
            : bStopTimer(false),
              m_pDisplay(_pDisplay),
              nID(0)
        {}

    // virtual ~TimerHelper();
    SDL_Surface* getDisplay() {return m_pDisplay;}
    
    void stopTimer() {bStopTimer = true;}
    bool isStopped() {return bStopTimer;}

    virtual void run() =0;
};

// -----------------------------------------------------------------------------
class TimerTest : public TimerHelper
{
    friend class MVC_Dispatcher;
    std::vector< interface_View* > const* m_pViewList;
    MVC_Dispatcher *m_pDispatcher;
public:
    
    bool m_bMachsNocheinmalSam;

private:
    // no copy ctor
    TimerTest(TimerTest const&);
    // no assignment operator
    TimerTest& operator=(TimerTest const&);
public:

    TimerTest(SDL_Surface* _pDisplay, MVC_Dispatcher* _pDispatcher, std::vector< interface_View* > const* _pViewList )
            :TimerHelper(_pDisplay),
             m_pViewList(_pViewList),
             m_pDispatcher(_pDispatcher),
             m_bMachsNocheinmalSam(true)
        {}
    
    MVC_Dispatcher* getDispatcher() {return m_pDispatcher;}
    virtual void run();

    // virtual TimerTest::~TimerTest();
};

// -----------------------------------------------------------------------------
class MVC_Dispatcher : public ControllerHelper
{
    std::vector< interface_Dispatch* > m_aDispatcherList;
    std::vector< interface_View* > m_aViewList;
    std::vector< interface_handleController* > m_aControllerList;

    SDL_Surface* m_pSurface;
    // Controller *m_pController;

    friend class TimerTest;

    SDL_mutex *m_pLock;
private:
    MVC_Dispatcher(MVC_Dispatcher const&);
    MVC_Dispatcher& operator=(MVC_Dispatcher const&);


    void setProgress(int _nPercentage);
public:
    TimerTest *m_pTimerTest;

    MVC_Dispatcher()
            :m_pSurface(NULL),
             m_pLock(SDL_CreateMutex()),
             m_pTimerTest(NULL)
        {}
    
    ~MVC_Dispatcher();
    void addToDispatch(interface_Dispatch* _pToDispatch);
    void addToView(interface_View* _pToView);
    void addToHandleController(interface_handleController* _pToHandleController);

    void testLoop();
    void loop();

    void setDisplay(SDL_Surface* _pSurface);
    void paintAllViews(SDL_Surface* _pSurface);
    bool handleController(Controller* _pController);

    void documentAllKeys();
};


#endif
