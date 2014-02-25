/*-*-c++-*-
 *
 *    $RCSfile: listfilter.cc,v $
 *     Project:
 * Description:
 *
 *   Author(s): Lars Langhans
 *       $Date: 2005/07/03 18:39:25 $
 *   Copyright: (c)1996 by Lars Langhans
 *   $Revision: 1.2 $
 */

#include <string>
#include <SDL.h>
#include "listfilter.h"
#include "t_print.h"
#include "property.h"
#include "global.h"

bool ListFilter::dispatch(SDL_Event const& _aEvent)
{
    t_print(T_FLOWCONTROL, "ListFilter::dispatch");
    bool bReturn = false;
    
    switch( _aEvent.type )
    {
    case SDL_USEREVENT:
        Global::documentKey(SDLK_a, 0, "Handle a-z as a filter.", SDLK_z);
        Global::documentKey(SDLK_BACKSPACE, 0, "Remove one char out of the file filter.");
        Global::documentKey(SDLK_SPACE, 0, "Remove the whole file filter.");
        break;

    case SDL_KEYDOWN:
        if ( _aEvent.key.keysym.sym >= SDLK_a && _aEvent.key.keysym.sym <= SDLK_z )
        {
            t_print(T_FLOWCONTROL, "KEY: alpha\n");
            m_sCurrentFilter += static_cast<char>(_aEvent.key.keysym.sym);
            bReturn = true;
        }
        else if (_aEvent.key.keysym.sym == SDLK_BACKSPACE)
        {
            t_print(T_FLOWCONTROL, "KEY: back space\n");

            if (m_sCurrentFilter.length() > 0)
            {
                m_sCurrentFilter = m_sCurrentFilter.substr(0, m_sCurrentFilter.length() - 1);
            }
            bReturn = true;
        }
        else if (_aEvent.key.keysym.sym == SDLK_SPACE)
        {
            t_print(T_FLOWCONTROL, "KEY: space\n");
            if (m_sCurrentFilter.length() > 1)
            {
                m_sCurrentFilter = std::string();
            }
            bReturn = true;
        }
        break;
    }

    if (bReturn == true)
    {
        if (hasAController())
        {
            getController()->setPropertyValue(P_FILTER_STRING, new String(m_sCurrentFilter));
        }
    }
    
    return bReturn;
}
