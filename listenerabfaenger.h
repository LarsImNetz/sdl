/*-*-c++-*-
 *
 *    $RCSfile: listenerabfaenger.h,v $
 *     Project:
 * Description:
 *
 *   Author(s): Lars Langhans
 *       $Date: 2004/09/13 17:55:34 $
 *   Copyright: (c)1996 by Lars Langhans
 *   $Revision: 1.1 $
 */

/* ==========================================================================
 *
 *               HISTORY
 *               -------
 *       $Log: listenerabfaenger.h,v $
 *       Revision 1.1  2004/09/13 17:55:34  langha_l
 *       Changes for using Listener design
 *
 */

#ifndef _LISTENERABFAENGER_H
#define _LISTENERABFAENGER_H

#include <string>

class ListenerAbfaenger
{
public:
    virtual void handleAnEvent(int _n) =0;
    virtual void handleAnEvent(std::string const& _s) =0;
};


#endif
