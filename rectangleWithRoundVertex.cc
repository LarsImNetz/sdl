#include <string>
#include <SDL_gfxPrimitives.h>
#include "ttffont.h"
#include "ColorHelper.h"
#include "rectangleWithRoundVertex.h"
#include "updaterect.h"

SDL_Rect filledRectangleWithRoundVertex(SDL_Surface *_pDisplay, int x, int y, int w, int h, int nRadius, Uint32 nColor)
{
    SDL_Rect aRect = {x, y, w, h};
    filledCircleColor(_pDisplay, x + nRadius,     y + nRadius    , nRadius, nColor);
    filledCircleColor(_pDisplay, x + nRadius,     y + h - nRadius, nRadius, nColor);
    filledCircleColor(_pDisplay, x + w - nRadius, y + nRadius    , nRadius, nColor);
    filledCircleColor(_pDisplay, x + w - nRadius, y + h - nRadius, nRadius, nColor);

    int w2 = w - 2*nRadius;
    if (w2 > 0)
    {
        boxColor(_pDisplay, x + nRadius, y          , x + nRadius + w2, y + h, nColor);
    }
    
    int h2 = h - 2*nRadius;
    if (h2 > 0)
    {
        boxColor(_pDisplay, x              , y + nRadius, (x) + nRadius, (y + nRadius) + h2, nColor);
        boxColor(_pDisplay, x + w - nRadius, y + nRadius, (x + w - nRadius) + nRadius, (y + nRadius) + h2, nColor);
    }
    return aRect;
}

SDL_Rect filledRectangleWithRoundVertexWithColorSlide(SDL_Surface *_pDisplay, int _x, int _y, int _w, int _h, int _nRadius, Uint32 _nColor[], int _nColorCount)
{
    SDL_Rect aRect = {_x, _y, _w, _h};

    for (int i=0;i<_nColorCount;i++)
    {
        int nRadius = _nRadius - i;
        int x = _x + i;
        int y = _y + i;
        int w = _w - 2*i;
        int h = _h - 2*i;
        int nColor = _nColor[i];

        filledCircleColor(_pDisplay, x + nRadius,     y + nRadius    , nRadius, nColor);
        filledCircleColor(_pDisplay, x + nRadius,     y + h - nRadius, nRadius, nColor);
        filledCircleColor(_pDisplay, x + w - nRadius, y + nRadius    , nRadius, nColor);
        filledCircleColor(_pDisplay, x + w - nRadius, y + h - nRadius, nRadius, nColor);
        
        int w2 = w - 2*nRadius;
        if (w2 > 0)
        {
            boxColor(_pDisplay, x + nRadius, y          , x + nRadius + w2, y + h, nColor);
        }
        
        int h2 = h - 2*nRadius;
        if (h2 > 0)
        {
            boxColor(_pDisplay, x              , y + nRadius, (x) + nRadius, (y + nRadius) + h2, nColor);
            boxColor(_pDisplay, x + w - nRadius, y + nRadius, (x + w - nRadius) + nRadius, (y + nRadius) + h2, nColor);
        }
        // mySDL_UpdateRects(_pDisplay, 1, &aRect);
    }
    
    return aRect;
}

//  TODO: Rectangle with antialising color effects 
    
SDL_Rect mySetTextWithRectangle(SDL_Surface *_pDisplay, int _x, int _y, int _w, int _nRectColorNumber, SDL_Color _nTextColor, std::string const& _sText)
{
    SetTextWithRectangle a(_pDisplay, _x, _y, _w, _nRectColorNumber, _nTextColor, _sText);

    SDL_Rect drect={0,0,800,600};
    
    // clipRect(0,0,799,599, drect);
    SDL_UpdateRects(_pDisplay, 1, &drect);

    a.setHeight(37);
    a.setRadius(11);
    a.setFontSize(22);
    return a.paint();
}
