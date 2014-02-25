#ifndef RECTANGLEWITHROUNDVERTEX_H
#define RECTANGLEWITHROUNDVERTEX_H

#include <string>
#include <SDL.h>
#include "ttffont.h"
#include "ColorHelper.h"
#include "somecolors.h"
#include "t_print.h"
#include "updaterect.h"

SDL_Rect filledRectangleWithRoundVertex(SDL_Surface *_pDisplay, int x, int y, int w, int h, int nRadius, Uint32 nColor);
SDL_Rect mySetTextWithRectangle(SDL_Surface *_pDisplay, int x, int y, int w, int _nRectColorNumber, SDL_Color _nTextColor, std::string const& _sText);
SDL_Rect filledRectangleWithRoundVertexWithColorSlide(SDL_Surface *_pDisplay, int _x, int _y, int _w, int _h, int _nRadius, Uint32 _nColor[], int _nColorCount);

// -----------------------------------------------------------------------------
class SetTextWithRectangle
{
    SDL_Surface *m_pSurface;
    int m_x, m_y;
    int m_w, m_h;
    int m_nRadius;
    // SDL_Color m_nRectColor;
    int m_nRectColorNumber;
    SDL_Color m_nTextColor;
    std::string m_sText;
    TTFFont *m_pFont;
    int m_nFontSize;
    bool m_bResizeWidth;

private:
    SetTextWithRectangle(SetTextWithRectangle const&);
    SetTextWithRectangle& operator=(SetTextWithRectangle const&);
public:
    
    SetTextWithRectangle(SDL_Surface *_pDisplay, int x, int y, int w, int _nRectColorNumber, SDL_Color _nTextColor, std::string const& _sText)
            :m_pSurface(_pDisplay),
             m_x(x),
             m_y(y),
             m_w(w),
             m_h(50),
             m_nRadius(10),
             m_nRectColorNumber(_nRectColorNumber),
             m_nTextColor(_nTextColor),
             m_sText(_sText),
             m_pFont(NULL),
             m_nFontSize(-1),
             m_bResizeWidth(false)
        {
        }
    void setResizeWidth(bool _bSetWidth)
        {
            m_bResizeWidth = _bSetWidth;
        }
    void setRadius(int _nRadius)
        {
            m_nRadius = _nRadius;
        }
    void setHeight(int _nHeight)
        {
            m_h = _nHeight;
        }
    void setFontSize(int _nFontSize)
        {
            m_nFontSize = _nFontSize;
            m_pFont = new TTFFont( m_nFontSize );
            m_pFont->setFontType(FONT_TYPE_BOLD);
        }
    void setFont(TTFFont *_pFont)
        {
            m_pFont = _pFont;
            m_nFontSize = -1;
        }
    
//  TODO: BOLD, outline, normal 
//     void setFontType(/*BOLD, normal, OUTLINE, ...*/)
//         {
//         }
    
    SDL_Rect paint()
        {
            SDL_Rect empty={0,0,0,0};
            if (m_pFont == NULL)
            {
                t_print(T_DEBUG, "NO FONT SET!\n");
                return empty;
            }
            
            // First draw the text, then draw the outline

            // m_pFont->setUpdate(false);
            
            m_pFont->setWidth(m_w - 2 * m_nRadius);
            m_pFont->setHeight(m_h);
            /* SDL_Rect aRect = */
            // pFont->setShadowedText( m_pSurface, m_nTextColor , m_x, m_y, m_sText.c_str() );
            // update not need here, too early
            m_pFont->setUpdate(false);
            // if (m_bResizeWidth == true)
            // {
            //     m_pFont->addMode( RESIZE_WIDTH );
            // }
            
            SDL_Rect aRectText = m_pFont->setText( m_pSurface, m_nTextColor , m_x + m_nRadius, m_y, m_sText.c_str() );
            // SDL_Rect aRect = {x,y,w,h};
            aRectText.x = m_x + m_nRadius;
            aRectText.y = m_y + m_nRadius;
            
            // mySDL_UpdateRects(m_pSurface, 1, &aRectText);

            if (m_bResizeWidth == true)
            {
                m_w = aRectText.w + 2 * m_nRadius;
            }
            
            // this is a little bit dry
//             if (m_nFontSize != -1)
//             {
//                 delete m_pFont;
//                 m_pFont = NULL;
//                 return empty;
//             }

            // SDL_Rect aRect = filledRectangleWithRoundVertex(m_pSurface, m_x, m_y, m_w, m_h, m_nRadius, toUint32(m_nRectColor));

            Uint32 aColorList[] = 
                {
                    ColorToUint32(Color::grey10),
                    ColorToUint32(Color::grey20),
                    ColorToUint32(Color::grey30),
                    ColorToUint32(Color::grey40),
                    ColorToUint32(Color::grey50),
                    ColorToUint32(Color::grey60),
                    ColorToUint32(Color::grey70),
                    ColorToUint32(Color::grey80),
                    ColorToUint32(Color::grey90),
                    ColorToUint32(Color::greya0)
                };
            
            SDL_Rect aRect = filledRectangleWithRoundVertexWithColorSlide(m_pSurface, m_x, m_y, m_w, m_h, m_nRadius, aColorList, m_nRectColorNumber);

            // set text again, draw over all other
            /* SDL_Rect aRectText = */
            m_pFont->setText( m_pSurface, m_nTextColor , m_x + m_nRadius, m_y, m_sText.c_str() );
            return aRect;
        }
    
};
#endif
