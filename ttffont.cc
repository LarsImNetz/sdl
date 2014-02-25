/*-*-c++-*-
 *
 *    $RCSfile: ttffont.cc,v $
 *     Project:
 * Description:
 *
 *   Author(s): Lars Langhans
 *       $Date: 2008/10/24 17:49:55 $
 *   Copyright: (c)1996 by Lars Langhans
 *   $Revision: 1.22 $
 */

// damit kann RCS im executeable noch die Version feststellen
// static char rcsid[] = "$Id: ttffont.cc,v 1.22 2008/10/24 17:49:55 langha_l Exp $";

#include <string>
#include "ttffont.h"
#include "somecolors.h"
#include "stringhelper.h"
#include "t_print.h"
#include "updaterect.h"
#include "ColorHelper.h"

// -----------------------------------------------------------------------------
#define NO_UPDATE false
#define TTF_DEFAULT_WIDTH 0
#define TTF_DEFAULT_HEIGHT -1


bool TTFFont::checkIfFontExist(std::string const& _sFontPath)
{
    FILE *f = fopen(_sFontPath.c_str(), "r");
    if (! f)
    {
        return false;
    }
    fclose(f);
    return true;
}

// -----------------------------------------------------------------------------
TTFFont::TTFFont(int _nSize, bool _nUpdate)
        :m_nSize(_nSize),
         m_nStyle(TTF_STYLE_NORMAL), /* TTF_STYLE_BOLD */
         m_w(TTF_DEFAULT_WIDTH),
         m_h(TTF_DEFAULT_HEIGHT), /* we try to build m_h */
         m_aTextColor(Color::white),
//!!! we must be sure, that this font exist, if not, we gett an error.
         m_pFont(NULL),
         m_bUpdate(_nUpdate),
         m_nWidth(0), m_nHeight(0),
         m_nMode(X_LEFT | STRING_SHORTEN),
         m_eFontType(0)
         // m_aRect()
{
//    std::string sFont("/usr/share/fonts/truetype/arial.ttf");
    std::string sCurrentDir(get_current_dir_name());
    std::string sFont = sCurrentDir + "/fonts/comic.ttf";
    if (!checkIfFontExist(sFont))
    {
        t_print(T_ERROR, "Font not found: %s\n", sFont.c_str());
    }
    else
    {
        m_pFont = TTF_OpenFont(sFont.c_str(), _nSize);
    }
    getH();
}

// -----------------------------------------------------------------------------
TTFFont::TTFFont(int _nSize, bool _nUpdate, std::string const& _sFontPath)
        :m_nSize(_nSize),
         m_nStyle(TTF_STYLE_NORMAL), /* TTF_STYLE_BOLD */
         m_w(TTF_DEFAULT_WIDTH),
         m_h(TTF_DEFAULT_HEIGHT),
         m_aTextColor(Color::white),
//!!! we must be sure, that this font exist, if not, we gett an error.
         m_pFont(NULL),
         m_bUpdate(_nUpdate),
         m_nWidth(0), m_nHeight(0),
         m_nMode(X_LEFT | STRING_SHORTEN),
         m_eFontType(0)
         // m_aRect()
{
    if (!checkIfFontExist(_sFontPath))
    {
        t_print(T_ERROR, "Font not found: %s\n", _sFontPath.c_str());
    }
    m_pFont = TTF_OpenFont(_sFontPath.c_str(), _nSize);
}

// -----------------------------------------------------------------------------
TTFFont::~TTFFont()
{
    TTF_CloseFont(m_pFont);
}

void TTFFont::initRect()
{
    static SDL_Rect aCleanRect = {0,0,0,0};
    m_aRect = aCleanRect;
}

SDL_Rect TTFFont::setBoldText(SDL_Surface *_pDisplay, SDL_Color const& _pColor, int x, int y, std::string const& _sText)
{
    initRect();
    // SDL_Rect aRect = {0,0,0,0};
    int nRestoreStyle = m_nStyle;
    m_nStyle = TTF_STYLE_BOLD;
    setNormalText(_pDisplay, _pColor, x, y, _sText);
    m_nStyle = nRestoreStyle;
    return getRect();
}

// -----------------------------------------------------------------------------
SDL_Rect TTFFont::setBoldOutlinedText(SDL_Surface *_pDisplay, SDL_Color const& _pColor, int x, int y, std::string const& _sText)
{
    SDL_Rect aRect = {0,0,0,0};
    int nRestoreStyle = m_nStyle;
    m_nStyle = TTF_STYLE_BOLD;
    aRect = setOutlinedText(_pDisplay, _pColor, x, y, _sText);
    m_nStyle = nRestoreStyle;
    return aRect;
}
// -----------------------------------------------------------------------------
SDL_Rect TTFFont::setText(SDL_Surface *_pDisplay, SDL_Color const& _pColor, int x, int y, std::string const& _sText)
{
    SDL_Rect aRect = {0,0,0,0};
    FontType e = getFontType();
    // int e = static_cast<int>(getFontType());

    if (e == FONT_TYPE_NORMAL)
    {
        aRect = setNormalText(_pDisplay, _pColor, x, y, _sText);
    }
    else if ((e & FONT_TYPE_BOLD) &&
             (e & FONT_TYPE_OUTLINE))
    {
        aRect = setBoldOutlinedText(_pDisplay, _pColor, x, y, _sText);
    }
    else if (e & FONT_TYPE_BOLD)
    {
        aRect = setBoldText(_pDisplay, _pColor, x, y, _sText);
    }
    else if (e & FONT_TYPE_SHADOWED)
    {
        aRect = setShadowedText(_pDisplay, _pColor, x, y, _sText);
    }
    else if (e & FONT_TYPE_OUTLINE)
    {
        aRect = setOutlinedText(_pDisplay, _pColor, x, y, _sText);
    }
    else
    {
        t_print(T_ERROR, "Unhandled FONT_TYPE");
    }
    return aRect;
}


// -----------------------------------------------------------------------------
SDL_Rect TTFFont::setNormalText(SDL_Surface *_pDisplay, SDL_Color const& _pColor, int x, int y, std::string const& _sText)
{
    SDL_Rect aRect = {0,0,0,0};
    if (m_pFont)
    {
        TTF_SetFontStyle(m_pFont, m_nStyle);

        std::string sText = _sText;
        bool bRepeatBecauseTooWide = false;
        SDL_Surface *pText = NULL;
        do
        {
            bRepeatBecauseTooWide = false;

            pText = TTF_RenderText_Blended(m_pFont, sText.c_str(), _pColor);

            if ((m_nMode & STRING_SHORTEN) == 0)
            {
                // nix string kuerzer machen!
                break;
            }
            
            if (pText)
            {
                if (pText->w > m_nWidth && sText.length() > 8)
                {
                    bRepeatBecauseTooWide = true;
                    SDL_FreeSurface(pText);
                    pText = NULL;
                    sText = StringHelper::shortString(sText);
                }
            }
        } while (bRepeatBecauseTooWide == true);

        if (pText)
        {
            // if ((m_nMode & RESIZE_WIDTH) == RESIZE_WIDTH)
            // {
            // }
            m_w = pText->w;
            m_h = pText->h;
            aRect.x = 0;
            aRect.y = 0;
            aRect.w = m_w;
            aRect.h = m_h;
            
            if ((m_nMode & X_LEFT) == X_LEFT)
            {
            }
            if ((m_nMode & X_RIGHT) == X_RIGHT)
            {
                x = x + (m_nWidth - m_w);
            }

            if ((m_nMode & X_CENTER) == X_CENTER)
            {
                x = x + (m_nWidth - m_w) / 2;
            }
            if ((m_nMode & Y_CENTER) == Y_CENTER)
            {
                y = y + (m_nHeight - m_h) / 2;
            }
            
            SDL_Rect drect        = {x, y, 0, 0};
            SDL_BlitSurface(pText, 0, _pDisplay, &drect);
            
            if (m_bUpdate)
            {
                mySDL_UpdateRects(_pDisplay, 1, &drect);
            }
            SDL_FreeSurface(pText);
        }
    }
    
    return aRect;
}
// -----------------------------------------------------------------------------

SDL_Rect TTFFont::setShadowedText(SDL_Surface *display, SDL_Color const& _pColor, int x, int y, std::string const& _sText)
{
    SDL_Rect aRect = {0,0,0,0};
    m_aTextColor = Color::white;
    aRect = setNormalText(display, Color::black, x + 2, y + 2, _sText);
    aRect = setNormalText(display, _pColor, x, y, _sText);
    return aRect;
}

// -----------------------------------------------------------------------------
SDL_Rect TTFFont::setOutlinedText(SDL_Surface *display, SDL_Color const& _pColor, int x, int y, std::string const& _sText)
{
    SDL_Color aColor = Color::black; // Color::white;
    setNormalText(display, aColor, x - 1, y - 1, _sText);
    setNormalText(display, aColor, x - 1, y    , _sText);
    setNormalText(display, aColor, x - 1, y + 1, _sText);
    setNormalText(display, aColor, x + 1, y - 1, _sText);
    setNormalText(display, aColor, x + 1, y    , _sText);
    setNormalText(display, aColor, x + 1, y + 1, _sText);

    SDL_Rect aRect = {0,0,0,0};
    aRect = setNormalText(display, _pColor, x, y, _sText);
    return aRect;
}

// -----------------------------------------------------------------------------

/*
  void TTFFont::setfont(SDL_Surface *_pDisplay, int x, int y)
  {
  std::string aStr;
  aStr = "Dies ist ein Text-test.";
  if (m_pFont)
  {
  TTF_SetFontStyle(m_pFont, TTF_STYLE_BOLD);

  SDL_Surface *pText_shadow = TTF_RenderText_Blended(m_pFont, aStr.c_str(), black);
        
  SDL_Rect drect_shadow = {x+2, y+2, 0, 0};
  t_print("w:%d, h:%d\n", pText->w, pText->h);

  // SDL_BlitSurface(image, &srect, _pDisplay, &drect);
  SDL_BlitSurface(pText_shadow, 0, _pDisplay, &drect_shadow);
  mySDL_UpdateRects(_pDisplay,1,&drect_shadow);

  SDL_FreeSurface(pText);
  }
  }
*/
int TTFFont::getH()
{
    if (m_h == TTF_DEFAULT_HEIGHT)
    {
        // Build a new surface
        Uint32 rmask, gmask, bmask, amask;
        
        /* SDL interprets each pixel as a 32-bit number, so our masks must depend
           on the endianness (byte order) of the machine */
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
        rmask = 0xff000000;
        gmask = 0x00ff0000;
        bmask = 0x0000ff00;
        amask = 0x000000ff;
#else
        rmask = 0x000000ff;
        gmask = 0x0000ff00;
        bmask = 0x00ff0000;
        amask = 0xff000000;
#endif
        SDL_Surface* pSurface = SDL_AllocSurface(SDL_HWSURFACE, 
                                                 100, 100, 32,
                                                 rmask,gmask,bmask,amask);
        if (pSurface != NULL)
        {
            FontType aCurrentFont = getFontType();
            setFontType(FONT_TYPE_NORMAL);
            setText(pSurface, Color::white, 0, 0, "..");
            SDL_FreeSurface(pSurface);
            setFontType(aCurrentFont);
        }
        // int nH = getH();
    }
    return m_h;
}


FontHeightTable::FontHeightTable(SDL_Surface *_pDisplay, std::string const& _sFontPath)
{
    m_aHeight[0] = 0;
    m_aHeight[1] = 0;
    m_aHeight[2] = 0;
    m_aHeight[3] = 0;

    int y = 10;
    int yadd = 0;
    int i;
    for (i=4;i<28;i++)
    {
        TTFFont aFont(i, true/* UPDATE */, _sFontPath );
        aFont.setFontType(FONT_TYPE_NORMAL);
        aFont.setText(_pDisplay, Color::grey50, 0,y, "TestString");
        yadd = aFont.getH();
        m_aHeight[i] = yadd;
        y += yadd;
    }

    int j = 0;
    for (i=4;i<600;i++)
    {
        TTFFont aFont(i, true/* UPDATE */, _sFontPath );
        aFont.setFontType(FONT_TYPE_NORMAL);
        aFont.setMode(X_CENTER | Y_CENTER);
        aFont.setWidth(700);
        aFont.setHeight(200);
        // SDL_Rect aRect = aFont.setText(_pDisplay, Color::grey50, 50,200, "TestString");
        SDL_Color aColor = {i,i,i,0};
        SDL_Rect aRect = aFont.setText(_pDisplay, aColor, 50,200, "TestString");
        if (aRect.w > 700)
        {
            break;
        }
        i += (++j) / 4;
        // yadd = aFont.getH();
        // m_aHeight[i] = yadd;
        // y += yadd;
    }

    j = 0;
    int c = i + 5;
    for (i=4;i<600;i++)
    {
        TTFFont aFont(i, true/* UPDATE */, _sFontPath );
        aFont.setFontType(FONT_TYPE_NORMAL);
        aFont.setMode(X_CENTER | Y_CENTER);
        aFont.setWidth(700);
        aFont.setHeight(200);
        // SDL_Rect aRect = aFont.setText(_pDisplay, Color::grey50, 50,200, "TestString");
        SDL_Color aColor = {c,c,c,0};
        c--;
        SDL_Rect aRect = aFont.setText(_pDisplay, aColor, 50,200, "TestString");
        if (aRect.w > 680)
        {
            break;
        }

        i += (++j) / 4;
        
        // yadd = aFont.getH();
        // m_aHeight[i] = yadd;
        // y += yadd;
    }
}

int FontHeightTable::getHeight(int _nIndex)
{
    if (_nIndex >= 0 && _nIndex < 20)
        return m_aHeight[_nIndex];
    return 0;
}

void showListOnConsole(StringHelper::StringList const& _aList)
{
    for (StringHelper::StringList::const_iterator it = _aList.begin();
         it != _aList.end();
         ++it)
    {
        std::string aStr = *it;
        t_print(T_INFO, "%s\n", aStr.c_str());
    }
}
