/*-*-c++-*-
 *
 *    $RCSfile: global.h,v $
 *     Project:
 * Description:
 *
 *   Author(s): Lars Langhans
 *       $Date: 2005/07/06 20:05:13 $
 *   Copyright: (c)1996 by Lars Langhans
 *   $Revision: 1.3 $
 */

/* ==========================================================================
 *
 *               HISTORY
 *               -------
 *       $Log: global.h,v $
 *       Revision 1.3  2005/07/06 20:05:13  langha_l
 *       better output for documentation of keys
 *
 *       Revision 1.2  2005/05/23 20:01:29  langha_l
 *       add some more information in createUserEvent
 *
 *       Revision 1.1  2005/01/25 18:23:51  langha_l
 *       add new filter system
 *
 */

#ifndef _GLOBAL_H
#define _GLOBAL_H

#include <string>
#include <SDL_keysym.h>
#include <SDL_events.h>

namespace Global
{
    bool shiftKey(SDL_Event const& _aEvent);
    void createUserEvent(std::string const& _sUserEvent);
    void documentKey(int _nKey, int _nModifier, std::string const& _sDocumentationForKey, int _nToKey=0);

    // convert key numbers to strings for better documentations
    std::string showKey(int _nKey);
    std::string showModifier(int _nModifier);

} // namespace Global

#endif
