/*-*-c++-*-
 *
 *    $RCSfile: updaterect.h,v $
 *     Project:
 * Description:
 *
 *   Author(s): Lars Langhans
 *       $Date: 2005/07/20 07:03:14 $
 *   Copyright: (c)1996 by Lars Langhans
 *   $Revision: 1.1 $
 */

/* ==========================================================================
 *
 *               HISTORY
 *               -------
 *       $Log: updaterect.h,v $
 *       Revision 1.1  2005/07/20 07:03:14  langha_l
 *       little window movements, new loudness view
 *
 */

#ifndef _updaterect_h
#define _updaterect_h

#include <SDL.h>

void mySDL_UpdateRects(SDL_Surface *_pDisplay, int numrects, SDL_Rect *_pRect);

#endif
