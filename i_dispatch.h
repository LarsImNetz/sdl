/*-*-c++-*-
 *
 *    $RCSfile: i_dispatch.h,v $
 *     Project:
 * Description:
 *
 *   Author(s): Lars Langhans
 *       $Date: 2005/01/25 18:23:51 $
 *   Copyright: (c)1996 by Lars Langhans
 *   $Revision: 1.2 $
 */

/* ==========================================================================
 *
 *               HISTORY
 *               -------
 *       $Log: i_dispatch.h,v $
 *       Revision 1.2  2005/01/25 18:23:51  langha_l
 *       add new filter system
 *
 *       Revision 1.1  2005/01/19 12:13:58  langha_l
 *       much changes for version 2, more modulisation
 *
 */

#ifndef _IDISPATCH_H
#define _IDISPATCH_H

/**
   Dispatcher for SDL_Events
 */

#include <SDL.h>
 
class interface_Dispatch
{
public:
    virtual bool dispatch(SDL_Event const& _aEvent) = 0;
};


#endif
