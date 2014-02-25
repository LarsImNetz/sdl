/*-*-c++-*-
 *
 *    $RCSfile: mvc_dispatcher.cc,v $
 *     Project:
 * Description:
 *
 *   Author(s): Lars Langhans
 *       $Date: 2008/10/24 17:54:55 $
 *   Copyright: (c)1996 by Lars Langhans
 *   $Revision: 1.13 $
 */

// damit kann RCS im executeable noch die Version feststellen
// static char rcsid[] = "$Id: mvc_dispatcher.cc,v 1.13 2008/10/24 17:54:55 langha_l Exp $";

#include <vector>
#include <SDL_gfxPrimitives.h>
#include "updaterect.h"
#include "mvc_dispatcher.h"
#include "t_print.h"
#include "global.h"
#include "property.h"
#include "ColorHelper.h"
#include "somecolors.h"

const int PAINT_WAIT_IN_MS = 30;
// void Dispatcher::setController(Controller* _pController)
// {
//     m_pController = _pController;
// }
void MVC_Dispatcher::addToDispatch(interface_Dispatch* _aToDispatch)
{
    m_aDispatcherList.push_back(_aToDispatch);
}
void MVC_Dispatcher::addToView(interface_View* _aToView)
{
    m_aViewList.push_back(_aToView);
}

void MVC_Dispatcher::addToHandleController(interface_handleController* _aToHandle)
{
    m_aControllerList.push_back(_aToHandle);
}

bool MVC_Dispatcher::handleController(Controller* _pController)
{
    bool bReturn = false;
    for (std::vector<interface_handleController*>::const_iterator it=m_aControllerList.begin();
         it != m_aControllerList.end();
         it++)
    {
        interface_handleController* p = *it;
        bReturn |= p->handleController(_pController);
    }
    return bReturn;
}

// -----------------------------------------------------------------------------
void TimerTest::run()
{
    TimerTest *pHelper = this;
    // t_print("Timer occured\n");
    MVC_Dispatcher *pDispatcher = pHelper->getDispatcher();
    if (pDispatcher == NULL)
    {
        return;
    }
    MutexGuard pv(pDispatcher->m_pLock);

    // t_print(T_FLOWCONTROL, "MVC_Dispatcher::paintAllViews\n");
    // t_print("MVC_Dispatcher::paintAllViews\n");
    // for (std::vector<interface_View*>::const_iterator it=m_pViewList->begin();
    //      it != m_pViewList->end();
    //      it++)
    // {
    //     interface_View* p = *it;
    //     p->paint(pHelper->getDisplay());
    // }
}

// TimerTest::~TimerTest()
// {
// }


Uint32 TimerFkt(Uint32 _nInterval, void* param)
{
    t_print("Paint Values.\n");
    if (param)
    {
        TimerTest* pHelper = static_cast<TimerTest*>(param);
        if (!pHelper)
        {
            return _nInterval;
        }
        
        if (pHelper->isStopped() == false)
        {
            // t_print("\nPaint Values.\n");
            pHelper->run();
        }
        // else
        // 
        if (1)
        // if (pHelper->m_bMachsNocheinmalSam)
        // {
        //     pHelper->m_bMachsNocheinmalSam = false;
        // }
        // else
        {
            // t_print("\nTimer stopped and removed.\n");
            if (pHelper->nID != 0)
            {
                SDL_RemoveTimer(pHelper->nID);
                pHelper->nID = 0;
            }
            pHelper->getDispatcher()->m_pTimerTest = NULL;
        }
    }
    return _nInterval;
}

void MVC_Dispatcher::paintAllViews(SDL_Surface* _pSurface)
{
    // MutexGuard pv(m_pLock);
    // t_print(T_DEBUG, "start paintAllViews");
    // if (m_pTimerTest == NULL)
    // {
        // create and start the paint timer, wait 50ms, then start painting
//        m_pTimerTest = new TimerTest(_pSurface, this, &m_aViewList);
//        m_pTimerTest->nID = SDL_AddTimer(PAINT_WAIT_IN_MS, TimerFkt, m_pTimerTest);
        // t_print(T_DEBUG, " ok.\n");

    // t_print(T_FLOWCONTROL, "MVC_Dispatcher::paintAllViews\n");
    // t_print("MVC_Dispatcher::paintAllViews\n");
    for (std::vector<interface_View*>::const_iterator it=m_aViewList.begin();
         it != m_aViewList.end();
         it++)
    {
        interface_View* p = *it;
        p->paint(_pSurface);
    }
    // }
    // else
    // {
    //     t_print(T_DEBUG, " already started.\n");
    // }
}

void MVC_Dispatcher::documentAllKeys()
{
    // t_print("run through dispatch list\n");
    SDL_Event event;                             // fake event
    event.type = SDL_USEREVENT;

    for (std::vector<interface_Dispatch*>::const_iterator it=m_aDispatcherList.begin();
         it != m_aDispatcherList.end();
         it++)
    {
        interface_Dispatch* p = *it;
        p->dispatch(event);
    }
}


void MVC_Dispatcher::loop()
{
    SDL_Event event;
    event.type = SDL_NOEVENT;

    while( SDL_WaitEvent( &event ) )
    {
        MutexGuard pv(m_pLock);

        if ( event.type == SDL_QUIT)
        {
            t_print(T_INFO, "SDL_Quit occured.\n");
            // nQuit = 1;
            // leave this while loop
            break;
        }
        
        if (event.type == SDL_USEREVENT)
        {
            //  TODO: this has to be replaced by a better method to paint
            //        At the moment every time a SDL_USEREVENT is inserted, paintAllViews()
            //        is called. This is very expensive, because if some SDL_USEREVENTs occur
            //        in a short time frame, we should only handle the last one.
            //        But here it isn't possible to say which is the last one.
            
            t_print(T_FLOWCONTROL, "MVC_Dispatcher::loop() SDL_UserEvent occured.\n");
            if (hasAController())
            {
                if (handleController(getController()))
                {
                    paintAllViews(m_pSurface);
                    continue;
                }
            }
        }
        
        if (event.type == SDL_KEYDOWN)
        {
            // t_print("run through dispatch list\n");
            if (event.key.keysym.sym == SDLK_F1)
            {
                t_print("\nHelp: All keys\n");
                documentAllKeys();
                t_print("\n");
            }
            else
            {
                bool bEventHandled = false;
                for (std::vector<interface_Dispatch*>::const_iterator it=m_aDispatcherList.begin();
                     it != m_aDispatcherList.end();
                     it++)
                {
                    interface_Dispatch* p = *it;
                    bEventHandled |= p->dispatch(event);
                    if (bEventHandled)
                    {
                        // handleController(getController());
                        paintAllViews(m_pSurface);
                        break;
                    }
                }
                if (bEventHandled == false)
                {
                    switch( event.type )
                    {
                    case SDL_KEYDOWN:
                        t_print(T_DEBUG, "Key not handled : Key: %s Key: %d Mod: %s\n",
                                Global::showKey(event.key.keysym.sym).c_str(),
                                event.key.keysym.sym,
                                Global::showModifier(event.key.keysym.mod).c_str());
                        break;
                    }
                }
            }
        }
    }
}

void MVC_Dispatcher::setDisplay(SDL_Surface* _pSurface)
{
    m_pSurface = _pSurface;
}

MVC_Dispatcher::~MVC_Dispatcher()
{
    if (m_pSurface)
    {
        double nWait = ((PAINT_WAIT_IN_MS + 5) * 1000) / 32;
        for (int i=0;i<32;i++)
        {
            Uint32 nColor = ColorToUint32(Color::black, 8 );
            boxColor(m_pSurface, 0, 0, m_pSurface->w, m_pSurface->h, nColor );
            SDL_Rect drect        = {0, 0, m_pSurface->w, m_pSurface->h};
            mySDL_UpdateRects(m_pSurface, 1, &drect);
            usleep(int(nWait));                         // little bit longer as the timer is!
        }
    }
    
    // usleep((PAINT_WAIT_IN_MS + 10) * 1000);                         // little bit longer as the timer is!
    SDL_DestroyMutex(m_pLock);
}

// --------------------------------- some tests ---------------------------------

void MVC_Dispatcher::setProgress(int _nPercentage)
{
    getController()->setPropertyValue(P_INFO_PROGRESSBAR, new Integer(_nPercentage));
    if (handleController(getController()))
    {
        paintAllViews(m_pSurface);
    }
//    usleep(200);
}

void MVC_Dispatcher::testLoop()
{
    if (hasAController())
    {
        for (int i=0;i<100;i++)
        {
            setProgress(i);
        }
        for (int i=100;i>50;i--)
        {
            setProgress(i);
        }        
        // for (int i=0;i<50;i++)
        // {
        //     setProgress(i);
        // }
    }
}
