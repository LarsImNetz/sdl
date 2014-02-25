/*-*-c++-*-
 *
 *    $RCSfile: quality.h,v $
 *     Project:
 * Description:
 *
 *   Author(s): Lars Langhans
 *       $Date: 2005/11/20 13:57:13 $
 *   Copyright: (c)1996 by Lars Langhans
 *   $Revision: 1.3 $
 */

/* ==========================================================================
 *
 *               HISTORY
 *               -------
 *       $Log: quality.h,v $
 *       Revision 1.3  2005/11/20 13:57:13  langha_l
 *       right handling of quality
 *
 *       Revision 1.2  2005/04/18 17:48:14  langha_l
 *       add right Quality handling
 *
 *       Revision 1.1  2005/02/12 12:32:30  langha_l
 *       last changes
 *
 *       Revision 1.1  2005/01/25 18:23:51  langha_l
 *       add new filter system
 *
 */

#ifndef _QUALITY_H
#define _QUALITY_H

#include <string>
#include "i_dispatch.h"
#include "i_handlecontroller.h"
#include "controller.h"

class Profile;

class Quality
    : public interface_Dispatch,
      public ControllerHelper,
      public interface_handleController
{
    std::string m_sPropertyFile;
    Profile* m_pProfile;

public:
    Quality(std::string const& _sPropertyFile);
    virtual ~Quality();
    
    Profile* getProfile();

    // handle Key Events
    virtual bool dispatch(SDL_Event const& _aEvent);
    int  getQuality(std::string const& _sFilename);
    void setQuality(std::string const& _sFilename, int _nQuality);

    // interface_handleController
    virtual bool handleController(Controller *_pController);
    
};


#endif
