/*-*-c++-*-
 *
 *    $RCSfile: i_handlecontroller.h,v $
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
 *       $Log: i_handlecontroller.h,v $
 *       Revision 1.2  2005/01/25 18:23:51  langha_l
 *       add new filter system
 *
 *       Revision 1.1  2005/01/19 12:13:58  langha_l
 *       much changes for version 2, more modulisation
 *
 */

#ifndef _I_HANDLECONTROLLER_H
#define _I_HANDLECONTROLLER_H

/**
   handleContoller interface
 */
class Controller;
class interface_handleController
{
public:
    
    virtual bool handleController(Controller *_pController) = 0;
};


#endif
