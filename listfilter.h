/*-*-c++-*-
 *
 *    $RCSfile: listfilter.h,v $
 *     Project:
 * Description:
 *
 *   Author(s): Lars Langhans
 *       $Date: 2005/01/25 18:23:51 $
 *   Copyright: (c)1996 by Lars Langhans
 *   $Revision: 1.1 $
 */

/* ==========================================================================
 *
 *               HISTORY
 *               -------
 *       $Log: listfilter.h,v $
 *       Revision 1.1  2005/01/25 18:23:51  langha_l
 *       add new filter system
 *
 */

#ifndef _LISTFILTER_H
#define _LISTFILTER_H

#include <string>
#include "i_dispatch.h"
#include "controller.h"

class ListFilter : public interface_Dispatch, public ControllerHelper
{
    std::string m_sCurrentFilter;
public:
    ListFilter() {}
    virtual ~ListFilter() {}
    
    // handle Key Events
    virtual bool dispatch(SDL_Event const& _aEvent);
};


#endif
