/*-*-c++-*-
 *
 *    $RCSfile: ttffont.h,v $
 *     Project:
 * Description:
 *
 *   Author(s): Lars Langhans
 *       $Date: 2005/11/03 08:43:36 $
 *   Copyright: (c)1996 by Lars Langhans
 *   $Revision: 1.11 $
 */

/* ==========================================================================
 *
 *               HISTORY
 *               -------
 *       $Log: ttffont.h,v $
 *       Revision 1.11  2005/11/03 08:43:36  langha_l
 *       add X_RIGHT
 *
 *       Revision 1.10  2005/10/14 19:19:55  langha_l
 *       move get font height into ttffont
 *
 *       Revision 1.9  2005/08/13 10:54:04  langha_l
 *       add RESIZE_WIDTH for variable Buttons
 *
 *       Revision 1.8  2005/07/26 19:52:59  langha_l
 *       some font problems fixed, some more tests
 *
 *       Revision 1.7  2005/06/23 07:38:48  langha_l
 *       start with -Weffc++ cleanups
 *
 *       Revision 1.6  2004/09/14 19:33:31  langha_l
 *       Font changes, some bugs
 *
 *       Revision 1.5  2004/08/29 16:41:44  langha_l
 *       cleanups and repairs of some bugs
 *
 *       Revision 1.4  2004/07/11 18:58:32  langha_l
 *       add better handling for directories
 *
 *       Revision 1.3  2004/03/19 18:23:58  langha_l
 *       mplayer start exported, better font support, some problems with restart removed
 *
 *       Revision 1.2  2004/02/16 19:34:50  langha_l
 *       Changes for new SDL_Mixer
 *
 *       Revision 1.1  2003/05/12 16:25:58  langha_l
 *       lot of changes
 *
 */

#ifndef _ttffont_h
#define _ttffont_h

#include <SDL.h>
#include <SDL_ttf.h>

#define X_LEFT 1
#define X_CENTER 2
#define Y_CENTER 4
#define STRING_SHORTEN 8
#define RESIZE_WIDTH 16
#define X_RIGHT 32

#define FONT_TYPE_NOTHING  0
#define FONT_TYPE_NORMAL   1
#define FONT_TYPE_BOLD     2
#define FONT_TYPE_KURSIV   4
#define FONT_TYPE_OUTLINE  8
#define FONT_TYPE_SHADOWED 16

class TTFFont
{
    int        m_nSize;
    int        m_nStyle;
    int        m_w;
    int        m_h;
    SDL_Color  m_aTextColor;
    TTF_Font  *m_pFont;
    bool       m_bUpdate;
    int        m_nWidth;
    int        m_nHeight;
    int        m_nMode;                          // CENTER, LEFT, RIGHT, RESIZE_WIDTH

    typedef int FontType;
    FontType   m_eFontType;

    SDL_Rect m_aRect;

private:    
    TTFFont(const TTFFont&);
    TTFFont& operator=(const TTFFont&);

    void initRect();
    SDL_Rect getRect() {return m_aRect;}
    
    bool checkIfFontExist(std::string const& _s);

    SDL_Rect       setNormalText(SDL_Surface *display, SDL_Color const& pColor, int x, int y, std::string const& _sText);
    SDL_Rect         setBoldText(SDL_Surface *display, SDL_Color const& pColor, int x, int y, std::string const& _sText);
    SDL_Rect     setShadowedText(SDL_Surface *display, SDL_Color const& pColor, int x, int y, std::string const& _sText);
    SDL_Rect     setOutlinedText(SDL_Surface *display, SDL_Color const& pColor, int x, int y, std::string const& _sText);
    SDL_Rect setBoldOutlinedText(SDL_Surface *display, SDL_Color const& pColor, int x, int y, std::string const& _sText);

public:
    TTFFont(int _nSize, bool _nUpdate = true);
    TTFFont(int _nSize, bool _nUpdate, std::string const& _s);
    ~TTFFont();

    void setWidth(int _w) {m_nWidth = _w;}
    void setHeight(int _h) {m_nHeight = _h;}
    void setMode(int _n) {m_nMode = _n;}
    void addMode(int _n) {m_nMode |= _n;}
    
/*
    possible values
    FONT_TYPE_NOTHING  
    FONT_TYPE_NORMAL   
    FONT_TYPE_BOLD     
    FONT_TYPE_KURSIV   
    FONT_TYPE_OUTLINE  
    FONT_TYPE_SHADOWED 
 */
    void setFontType(FontType _e) {m_eFontType = _e;}
    FontType getFontType() {return m_eFontType;}
    
    SDL_Rect             setText(SDL_Surface *display, SDL_Color const& pColor, int x, int y, std::string const& _sText);

    int getW() {return m_w;}
    int getH();
    void setUpdate(bool _bValue) {m_bUpdate = _bValue;}
};

// -----------------------------------------------------------------------------
class FontHeightTable
{
    int m_aHeight[35];

private:
    FontHeightTable(FontHeightTable const&);
    FontHeightTable& operator=(FontHeightTable const&);
public:
    FontHeightTable(SDL_Surface *_pDisplay, std::string const& _sFontPath);
    int getHeight(int _nValue);
};


#endif
