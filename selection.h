/*-*-c++-*-
 *
 *    $RCSfile: selection.h,v $
 *     Project:
 * Description:
 *
 *   Author(s): Lars Langhans
 *       $Date: 2005/01/19 12:13:58 $
 *   Copyright: (c)1996 by Lars Langhans
 *   $Revision: 1.1 $
 */

/* ==========================================================================
 *
 *               HISTORY
 *               -------
 *       $Log: selection.h,v $
 *       Revision 1.1  2005/01/19 12:13:58  langha_l
 *       much changes for version 2, more modulisation
 *
 */

#ifndef _SELECTION_H
#define _SELECTION_H

#include "i_dispatch.h"

class Controller;

extern const char* SELECTION_MAX_SELECTION;
extern const char* SELECTION_CURRENT_SELECTION;

class Selection : public interface_Dispatch
{    
    int m_nCurrentPosition;
    int m_nMaxPosition;
    Controller *m_pController;

    void up();
    void down();
    void select();
    void updateSelectionInController();
public:
    Selection(int _nPossibleSelections)
            :m_nCurrentPosition(0),
             m_nMaxPosition(_nPossibleSelections),
             m_pController(NULL)
        {}

    // Communication port to outside
    void setController(Controller *_pController);

    // int getCurrent() { return m_nCurrentPosition; }
    int getMax() {return m_nMaxPosition; }
    
    // return true, is Event was handled, else false
    bool dispatch(SDL_Event const& _aEvent);
};


#endif
