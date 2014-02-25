/*-*-c++-*-
 *
 *    $RCSfile: cliprect.h,v $
 *     Project:
 * Description:
 *
 *   Author(s): Lars Langhans
 *       $Date: 2004/09/11 11:11:56 $
 *   Copyright: (c)1996 by Lars Langhans
 *   $Revision: 1.1 $
 */

#ifndef _CLIPRECT_H
#define _CLIPRECT_H

#include <SDL.h>

// clip auf 800x600 pixel
void clipRect(int nXPos, int nYPos, int nWidth, int nHeight, SDL_Rect& _aOutRect);

#endif
