/*-*-c++-*-
 *
 *    $RCSfile: selection.cc,v $
 *     Project:
 * Description:
 *
 *   Author(s): Lars Langhans
 *       $Date: 2005/05/23 20:05:57 $
 *   Copyright: (c)1996 by Lars Langhans
 *   $Revision: 1.2 $
 */

// damit kann RCS im executeable noch die Version feststellen
// static char rcsid[] = "$Id: selection.cc,v 1.2 2005/05/23 20:05:57 langha_l Exp $";

#include "selection.h"
#include "controller.h"
#include "t_print.h"

const char* SELECTION_MAX_SELECTION = "MaxSelection";
const char* SELECTION_CURRENT_SELECTION = "CurrentSelection";

void Selection::updateSelectionInController()
{
   if (m_pController)
   {
       // m_pController->setCurrentSelection(m_nCurrentPosition);
       m_pController->setPropertyValue(SELECTION_CURRENT_SELECTION, new Integer(m_nCurrentPosition), false);
   }
}

void Selection::up()
{
    ++m_nCurrentPosition;
    if (m_nCurrentPosition >= m_nMaxPosition)
    {
        m_nCurrentPosition = m_nMaxPosition - 1;
    }
    updateSelectionInController();
}

void Selection::down()
{
    --m_nCurrentPosition;
    if (m_nCurrentPosition < 0)
    {
        m_nCurrentPosition = 0;
    }
    updateSelectionInController();
}

void Selection::select()
{
    if (m_pController)
    {
        // m_pController->setCurrentSelection(m_nCurrentPosition);
        // m_pController->select(m_nCurrentPosition);
        m_pController->setPropertyValue(SELECTION_CURRENT_SELECTION, new Integer(m_nCurrentPosition), false);
    }
}

bool Selection::dispatch(SDL_Event const& _aEvent)
{
    t_print(T_FLOWCONTROL, "Selection::dispatch\n");
    bool bReturn = false;
    
    switch( _aEvent.type )
    {
    case SDL_KEYDOWN:
        // t_print("Key:%d mod:%d \n", event.key.keysym.sym, event.key.keysym.mod);
        if (_aEvent.key.keysym.sym == SDLK_DOWN)
        {
            t_print(T_FLOWCONTROL, "KEY: down\n");
            up();
            bReturn = true;
        }
        else if (_aEvent.key.keysym.sym == SDLK_UP)
        {
            t_print(T_FLOWCONTROL, "KEY: up\n");
            down();
            bReturn = true;
        }
        else if (_aEvent.key.keysym.sym == SDLK_RETURN)
        {
            t_print(T_FLOWCONTROL, "KEY: return\n");
            select();
            // down();
            bReturn = true;
        }
        
        break;

        // default:
        // bReturn = false;
    }
    return bReturn;
}

    // Communication port to outside
void Selection::setController(Controller *_pController)
{
    m_pController = _pController;
    if (m_pController)
    {
        // m_pController->setMaxSelection( getMax()); 
        m_pController->setPropertyValue(SELECTION_MAX_SELECTION, new Integer( getMax() ), false);
        m_pController->setPropertyValue(SELECTION_CURRENT_SELECTION, new Integer(m_nCurrentPosition), false);
    }
}
