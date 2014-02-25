/*-*-c++-*-
 *
 *    $RCSfile: cliprect.cc,v $
 *     Project:
 * Description:
 *
 *   Author(s): Lars Langhans
 *       $Date: 2004/09/11 11:11:56 $
 *   Copyright: (c)1996 by Lars Langhans
 *   $Revision: 1.1 $
 */

// damit kann RCS im executeable noch die Version feststellen
// static char rcsid[] = "$Id: cliprect.cc,v 1.1 2004/09/11 11:11:56 langha_l Exp $";

#include "cliprect.h"

// clip auf 800x600 pixel
void clipRect(int nXPos, int nYPos, int nWidth, int nHeight, SDL_Rect& _aOutRect)
{
    if (nXPos < 0)
    {
        nWidth += nXPos;
        nXPos = 0;
    }
    if (nYPos < 0)
    {
        nHeight += nYPos;
        nYPos = 0;
    }
    if (nXPos + nWidth > 800)
    {
        nWidth = 800 - nXPos;
    }
    if (nYPos + nHeight > 600)
    {
        nHeight = 600 - nYPos;
    }
    
    _aOutRect.x = nXPos;
    _aOutRect.y = nYPos;
    _aOutRect.w = nWidth;
    _aOutRect.h = nHeight;
}

