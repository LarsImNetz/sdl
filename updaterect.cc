/*-*-c++-*-
 *
 *    $RCSfile: updaterect.cc,v $
 *     Project:
 * Description:
 *
 *   Author(s): Lars Langhans
 *       $Date: 2005/11/28 20:42:05 $
 *   Copyright: (c)1996 by Lars Langhans
 *   $Revision: 1.3 $
 */

// damit kann RCS im executeable noch die Version feststellen
// static char rcsid[] = "$Id: updaterect.cc,v 1.3 2005/11/28 20:42:05 langha_l Exp $";

/* ==========================================================================
 *
 *               HISTORY
 *               -------
 *       $Log: updaterect.cc,v $
 *       Revision 1.3  2005/11/28 20:42:05  langha_l
 *       rectangle via parameter
 *
 *       Revision 1.2  2005/10/17 18:09:34  langha_l
 *       handle update rect right
 *
 *       Revision 1.1  2005/07/20 07:03:14  langha_l
 *       little window movements, new loudness view
 *
 */

#include <SDL_gfxPrimitives.h>

#include "updaterect.h"
#include "ColorHelper.h"
#include "somecolors.h"
#include "t_print.h"

void mySDL_UpdateRects(SDL_Surface *_pDisplay, int numrects, SDL_Rect *_pRect)
{
  // paint an orange rectangle to debug the surfaces
	if ( aGlobalStructure.isBit( Bit_Flag_RECTANGLE ) )
    {
        rectangleColor(_pDisplay,
                       _pRect->x,
                       _pRect->y,
                       _pRect->x + _pRect->w - 1,
                       _pRect->y + _pRect->h - 1,
                       ColorToUint32(Color::orange));
        
        lineColor(_pDisplay,
                  _pRect->x,
                  _pRect->y,
                  _pRect->x + _pRect->w - 1,
                  _pRect->y + _pRect->h - 1,
                  ColorToUint32(Color::orange));
    }
    
    // _pRect->w = _pRect->w;
    // _pRect->h = _pRect->h;

    SDL_UpdateRects(_pDisplay, numrects, _pRect);
}
