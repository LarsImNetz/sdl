/*-*-c++-*-
 *
 *    $RCSfile: i_view.h,v $
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
 *       $Log: i_view.h,v $
 *       Revision 1.1  2005/01/19 12:13:58  langha_l
 *       much changes for version 2, more modulisation
 *
 */

#ifndef _IVIEW_H
#define _IVIEW_H

/**
   View interface
 */
class interface_View
{
public:
    
    virtual bool paint(SDL_Surface* _pSurface) = 0;
};


#endif
