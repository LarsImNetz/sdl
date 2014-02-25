/*-*-c++-*-
 *
 *    $RCSfile: somecolors.h,v $
 *     Project:
 * Description:
 *
 *   Author(s): Lars Langhans
 *       $Date: 2005/10/14 19:19:00 $
 *   Copyright: (c)1996 by Lars Langhans
 *   $Revision: 1.8 $
 */

/* ==========================================================================
 *
 *               HISTORY
 *               -------
 *       $Log: somecolors.h,v $
 *       Revision 1.8  2005/10/14 19:19:00  langha_l
 *       rebuild of viewdirectory
 *
 *       Revision 1.7  2004/10/13 10:02:16  langha_l
 *       add some colors
 *
 *       Revision 1.6  2004/09/15 18:55:39  langha_l
 *       add new color slide in rectangle
 *
 *       Revision 1.5  2004/09/06 17:43:04  langha_l
 *       Hintergrund weg, jetzt mit rectangle selbst erstellt
 *
 *       Revision 1.4  2004/08/29 16:41:05  langha_l
 *       add some grey colors
 *
 *       Revision 1.3  2004/02/13 19:44:51  langha_l
 *       no more a rectangle, not other color for a selected value
 *
 *       Revision 1.2  2004/02/04 06:52:35  langha_l
 *       lot of changes for better handling
 *
 *       Revision 1.1  2003/05/12 16:25:58  langha_l
 *       lot of changes
 *
 */

#ifndef _somecolors_h
#define _somecolors_h

#include <SDL.h>

class Color
{
public:
    
    static SDL_Color white;
    static SDL_Color greyf0;
    static SDL_Color greye0;
    static SDL_Color greyd0;
    static SDL_Color greyc0;
    static SDL_Color greyb0;
    static SDL_Color greya0;
    static SDL_Color grey90;
    static SDL_Color grey80;
    static SDL_Color grey70;
    static SDL_Color grey60;
    static SDL_Color grey50;
    static SDL_Color grey40;
    static SDL_Color grey30;
    static SDL_Color grey20;
    static SDL_Color grey10;
    static SDL_Color black;
    static SDL_Color orange;
    static SDL_Color bunt;
    static SDL_Color red20;
    static SDL_Color red;
    static SDL_Color green;
    static SDL_Color blue;
    static SDL_Color grey23;
    static SDL_Color transparent;
};

#endif
