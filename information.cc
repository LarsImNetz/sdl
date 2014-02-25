#include <string>
#include <SDL.h>
#include <SDL_gfxPrimitives.h>

#include "ColorHelper.h"
#include "somecolors.h"
#include "rectangleWithRoundVertex.h"
#include "cliprect.h"
#include "property.h"
#include "information.h"
#include "updaterect.h"

void clipPercent(int &_nPercent)
{
    if (_nPercent < 0)
    {
        _nPercent = 0;
    }
    if (_nPercent > 100)
    {
        _nPercent = 100;
    }
}

Uint32 ColorCalculator(Uint32 _nStartColor, Uint32 _nEndColor, int _n, int _nMaxCount)
{
    // return _nEndColor;
    if (_n > _nMaxCount)
    {
        _n = _nMaxCount;
    }
    
    int r = ((_nEndColor & 0xff000000) >> 24) * _n / _nMaxCount;
    int g = ((_nEndColor & 0x00ff0000) >> 16) * _n / _nMaxCount;
    int b = ((_nEndColor & 0x0000ff00) >> 8)  * _n / _nMaxCount;
    int a = _nEndColor & 0x000000ff;

    Uint32 nNewColor = ((r & 0xff) << 24) | ((g & 0xff) << 16) | ((b & 0xff) << 8) | a;
    return nNewColor;
}

void Information::drawProgressBar(int _nPercent)
{
    clipPercent(_nPercent);

// Old position on the botton down position
//     int x = 50; // 147 - 396
//     int y = 580; // - 70 artist
// 
//     int nXSizeMax = 800 - 2 * x;
//     
//     int xsize = nXSizeMax * _nPercent / 100;
// 
//     boxColor(m_pDisplay, x,         y, x + xsize, y + 5, ColorToUint32(Color::orange));
//     boxColor(m_pDisplay, x + xsize, y, x + nXSizeMax,y + 5, ColorToUint32(Color::grey30));
//     SDL_Rect drect;
//     clipRect(x, y, nXSizeMax, 5, drect);
//     mySDL_UpdateRects(m_pDisplay, 1, &drect);

    int nMaxHeight = 100;
    int nMaxWidth = 64;

    int nPercentToColor = _nPercent * nMaxHeight / 100;
// new position on the right side
    int xm = 800 - 40 + 4 ; // 147 - 396
    int xw = nMaxWidth;
    
    int ystep = 5;
    int y = (600 - (nMaxHeight * ystep)) / 2; // - 70 artist

    Uint32 nOrangeColor = ColorToUint32(Color::orange);
    Uint32 nGrayColor = ColorToUint32(Color::grey30);

    for (int i = 0;i<nMaxHeight;i++)
    {
        Uint32 nColor = nGrayColor;
        if (nPercentToColor >= (nMaxHeight - i))
        {
            nColor = ColorCalculator(0x0, nOrangeColor, (nMaxHeight - i), nMaxHeight);
        }
        
        int xwhalf = xw / 2;
        boxColor(m_pDisplay, 
                 xm - xwhalf,
                 y + ystep * i,
                 xm + xwhalf,
                 y + ystep * i,
                 nColor);
        xw = nMaxWidth - (i * nMaxWidth / nMaxHeight);
    }

    SDL_Rect drect;
    clipRect(xm - (nMaxWidth / 2),
             y,
             nMaxWidth,
             nMaxHeight * ystep,
             drect);
    mySDL_UpdateRects(m_pDisplay, 1, &drect);
    
}

// void Information::clearArtist()
// {
//     int x = 147; // 147 - 396
//     int y = 45; // - 70 artist
// // 73 title
// // 101 album
// // 129 year
//     boxColor(m_pDisplay, x, y, x + 249, y + 25, ColorToUint32(Color::grey30));
//     SDL_Rect drect;
//     clipRect(x, y, 249, 25, drect);
//     mySDL_UpdateRects(m_pDisplay, 1, &drect);
// }

#define INFO_XPOS 10
#define INFO_YPOS 15
#define INFO_HEIGHT (37 + 2)

#define INFO_WIDTH (800 - 80)

#define COLOR_NUMBER_5 5
#define COLOR_NUMBER_3 3

void Information::showInfo(std::string const &_sText)
{
    int x = INFO_XPOS;
    int y = INFO_YPOS;
    // clearArtist();
    // m_pFont->setText(m_pDisplay, Color::orange , x + 2, y + 4, _sText);
    SetTextWithRectangle a(m_pDisplay, x, y, INFO_WIDTH, COLOR_NUMBER_5, Color::orange, _sText);
    a.setHeight(36);
    a.setRadius(11);
    m_pFont->setMode( Y_CENTER);
    m_pFont->setFontType(FONT_TYPE_BOLD | FONT_TYPE_OUTLINE);
    a.setFont(m_pFont);
    SDL_Rect drect = a.paint();
    mySDL_UpdateRects(m_pDisplay, 1, &drect);
}
void Information::showArtist(std::string const &_sText)
{
    int x = INFO_XPOS;
    int y = INFO_YPOS + INFO_HEIGHT;
    // clearArtist();
    // m_pFont->setText(m_pDisplay, Color::orange , x + 2, y + 4, _sText);
    SDL_Color aColor = Color::black;
    if (_sText.length() > 0)
    {
        aColor = Color::orange;
    }
    SetTextWithRectangle a(m_pDisplay, x, y, 120, COLOR_NUMBER_5, aColor, "artist");
    a.setHeight(36);
    a.setRadius(11);
    m_pFont->setMode(X_CENTER | Y_CENTER);
    m_pFont->setFontType(/* FONT_TYPE_BOLD | */ FONT_TYPE_OUTLINE);
    a.setFont(m_pFont);
    SDL_Rect drect = a.paint();
    mySDL_UpdateRects(m_pDisplay, 1, &drect);

    SetTextWithRectangle b(m_pDisplay, x + 122, y, INFO_WIDTH - 122, COLOR_NUMBER_3, Color::greyc0, _sText);
    b.setHeight(36);
    b.setRadius(11);
    m_pFont->setMode(Y_CENTER | STRING_SHORTEN);
    m_pFont->setFontType( FONT_TYPE_OUTLINE);
    b.setFont(m_pFont);
    SDL_Rect drect2 = b.paint();
    mySDL_UpdateRects(m_pDisplay, 1, &drect2);
}
void Information::showTitle(std::string const &_sText)
{
    int x = INFO_XPOS;
    int y = INFO_YPOS + 2 * INFO_HEIGHT;
//    clearArtist();
    // m_pFont->setText(m_pDisplay, Color::orange , x + 2, y + 4, _sText);
    SDL_Color aColor = Color::black;
    if (_sText.length() > 0)
    {
        aColor = Color::orange;
    }
    SetTextWithRectangle a(m_pDisplay, x, y, 120, COLOR_NUMBER_5, aColor, "title");
    a.setHeight(36);
    a.setRadius(11);
    m_pFont->setMode(X_CENTER | Y_CENTER);
    m_pFont->setFontType(/* FONT_TYPE_BOLD | */ FONT_TYPE_OUTLINE);
    a.setFont(m_pFont);
    SDL_Rect drect = a.paint();
    mySDL_UpdateRects(m_pDisplay, 1, &drect);

    SetTextWithRectangle b(m_pDisplay, x + 122, y, INFO_WIDTH - 122, COLOR_NUMBER_3, Color::greyc0, _sText);
    b.setHeight(36);
    b.setRadius(11);
    m_pFont->setMode(Y_CENTER);
    m_pFont->setFontType( FONT_TYPE_OUTLINE);
    b.setFont(m_pFont);
    SDL_Rect drect2 = b.paint();
    mySDL_UpdateRects(m_pDisplay, 1, &drect2);
}
void Information::showAlbum(std::string const &_sText)
{
    int x = INFO_XPOS;
    int y = INFO_YPOS + 3 * INFO_HEIGHT;
//    clearArtist();
    // m_pFont->setText(m_pDisplay, Color::orange , x + 2, y + 4, _sText);
    SDL_Color aColor = Color::black;
    if (_sText.length() > 0)
    {
        aColor = Color::orange;
    }
    SetTextWithRectangle a(m_pDisplay, x, y, 120, COLOR_NUMBER_5, aColor, "album");
    a.setHeight(36);
    a.setRadius(11);
    m_pFont->setMode(X_CENTER | Y_CENTER);
    m_pFont->setFontType( /* FONT_TYPE_BOLD | */ FONT_TYPE_OUTLINE);
    a.setFont(m_pFont);
    SDL_Rect drect = a.paint();
    mySDL_UpdateRects(m_pDisplay, 1, &drect);

    SetTextWithRectangle b(m_pDisplay, x + 122, y, INFO_WIDTH - 122, COLOR_NUMBER_3, Color::greyc0, _sText);
    b.setHeight(36);
    b.setRadius(11);
    m_pFont->setMode(Y_CENTER);
    m_pFont->setFontType( FONT_TYPE_OUTLINE);
    b.setFont(m_pFont);
    SDL_Rect drect2 = b.paint();
    mySDL_UpdateRects(m_pDisplay, 1, &drect2);
}
void Information::showYear(std::string const &_sText)
{
    int x = INFO_XPOS;
    int y = INFO_YPOS + 4 * INFO_HEIGHT;
//    clearArtist();
    // m_pFont->setText(m_pDisplay, Color::orange , x + 2, y + 4, _sText);
    SDL_Color aColor = Color::black;
    if (_sText.length() > 0)
    {
        aColor = Color::orange;
    }
    SetTextWithRectangle a(m_pDisplay, x, y, 120, COLOR_NUMBER_5, aColor, "year");
    a.setHeight(36);
    a.setRadius(11);
    m_pFont->setMode(X_CENTER | Y_CENTER);
    m_pFont->setFontType( /* FONT_TYPE_BOLD | */ FONT_TYPE_OUTLINE);
    a.setFont(m_pFont);
    SDL_Rect drect = a.paint();
    mySDL_UpdateRects(m_pDisplay, 1, &drect);

    SetTextWithRectangle b(m_pDisplay, x + 122, y, INFO_WIDTH - 122, COLOR_NUMBER_3, Color::greyc0, _sText);
    b.setHeight(36);
    b.setRadius(11);
    m_pFont->setMode(Y_CENTER);
    m_pFont->setFontType( FONT_TYPE_OUTLINE);
    b.setFont(m_pFont);
    SDL_Rect drect2 = b.paint();
    mySDL_UpdateRects(m_pDisplay, 1, &drect2);
}

void Information::showFilter(std::string const &_sText)
{
    int x = INFO_XPOS;
    int y = INFO_YPOS + 5 * INFO_HEIGHT;
//    clearArtist();
    // m_pFont->setText(m_pDisplay, Color::orange , x + 2, y + 4, _sText);
    SDL_Color aColor = Color::black;
    if (_sText.length() > 0)
    {
        aColor = Color::greyc0;
    }
    SetTextWithRectangle a(m_pDisplay, x, y, 120, COLOR_NUMBER_5, aColor, "filter");
    a.setHeight(36);
    a.setRadius(11);
    m_pFont->setMode(X_CENTER | Y_CENTER);
    m_pFont->setFontType( /* FONT_TYPE_BOLD | */ FONT_TYPE_OUTLINE);
    a.setFont(m_pFont);
    SDL_Rect drect = a.paint();
    mySDL_UpdateRects(m_pDisplay, 1, &drect);

    SetTextWithRectangle b(m_pDisplay, x + 122, y, INFO_WIDTH - 122, COLOR_NUMBER_3, Color::greyc0, _sText);
    b.setHeight(36);
    b.setRadius(11);
    m_pFont->setMode(Y_CENTER);
    m_pFont->setFontType( FONT_TYPE_OUTLINE);
    b.setFont(m_pFont);
    SDL_Rect drect2 = b.paint();
    mySDL_UpdateRects(m_pDisplay, 1, &drect2);
}

void Information::showCurrentDirectory(std::string const& _sText)
{
    int x = INFO_XPOS;
    int y = INFO_YPOS + 6 * INFO_HEIGHT;
    // clearArtist();
    // m_pFont->setText(m_pDisplay, Color::orange , x + 2, y + 4, _sText);
    SetTextWithRectangle a(m_pDisplay, x, y, INFO_WIDTH, COLOR_NUMBER_5, Color::orange, _sText);
    a.setHeight(36);
    a.setRadius(11);
    m_pFont->setMode(Y_CENTER | STRING_SHORTEN);
    m_pFont->setFontType(FONT_TYPE_BOLD | FONT_TYPE_OUTLINE);
    a.setFont(m_pFont);
    SDL_Rect drect = a.paint();
    mySDL_UpdateRects(m_pDisplay, 1, &drect);
}

int Information::showKey(std::string const &_sText, int _nX)
{
    int x = INFO_XPOS + _nX;
    int y = 599 - 1*INFO_HEIGHT - 10;
    // clearArtist();
    // m_pFont->setText(m_pDisplay, Color::orange , x + 2, y + 4, _sText);
    SetTextWithRectangle a(m_pDisplay, x, y, 80, COLOR_NUMBER_5, Color::greyc0, _sText);
    a.setHeight(36);
    a.setRadius(11);
    a.setResizeWidth(true);
    m_pKeyFont->setMode( Y_CENTER);
    m_pKeyFont->setFontType(FONT_TYPE_NORMAL );
    a.setFont(m_pKeyFont);
    SDL_Rect drect = a.paint();
    if (drect.x + drect.w < 799 )                // simple check if draw is ok.
    {
        mySDL_UpdateRects(m_pDisplay, 1, &drect);
    }
    
    return _nX + drect.w + 1;
}

/*
void Information::handleAnEvent(int _nValue)
{
}

void Information::handleAnEvent(std::string const& _sStr)
{
    setCurrentDirectory(_sStr);
}
*/

bool Information::paint(SDL_Surface* _pSurface)
{
    m_pDisplay = _pSurface;

    if (m_bInfo)
    {
        showInfo(m_sInfo);
        m_bInfo = false;
    }
    if (m_bArtist)
    {
        showArtist(m_sArtist);
        m_bArtist = false;
    }
    if (m_bTitle)
    {
        showTitle(m_sTitle);
        m_bTitle = false;
    }
    if (m_bAlbum)
    {
        showAlbum(m_sAlbum);
        m_bAlbum = false;
    }
    if (m_bYear)
    {
        showYear(m_sYear);
        m_bYear = false;
    }    
    if (m_bCurrentDirectory)
    {
        showCurrentDirectory(m_sCurrentDirectory);
        m_bCurrentDirectory = false;
    }
    if (m_bFilter)
    {
        showFilter(m_sFilter);
        m_bFilter = false;
    }

    int x;
    x = showKey("F10 new playlist", 0);
    x = showKey("+ next", x);
    x = showKey("- prev", x);
    x = showKey("B up +10 sec", x);
    x = showKey("B down -10 sec", x);
    x = showKey("TAB toggle view", x);
    x = showKey("ESC Quit", x);
    
    //  x = showKey("F5 test far", x);
    //  x = showKey("F6 test", x);
    //  x = showKey("F7 test foo", x);
    //  x = showKey("F8 test bah", x);
    //  x = showKey("F9 test 123456789", x);
    
    drawProgressBar(m_nPercent);
    m_pDisplay = NULL;
    return true;
}
bool Information::setInfo(std::string const& _sText)
{
    if (m_sInfo != _sText)
    {
        m_sInfo = _sText;
        m_bInfo = true;
    }
    return m_bInfo;
}

bool Information::setArtist(std::string const &_sText)
{
    if (m_sArtist != _sText)
    {
        m_sArtist = _sText;
        m_bArtist = true;
    }
    return m_bArtist;
}

bool Information::setTitle(std::string const &_sText)
{
    if (m_sTitle != _sText)
    {
        m_sTitle = _sText;
        m_bTitle = true;
    }
    return m_bTitle;
}

bool Information::setAlbum(std::string const &_sText)
{
    if (m_sAlbum != _sText)
    {
        m_sAlbum = _sText;
        m_bAlbum = true;
    }
    return m_bAlbum;
}

bool Information::setYear(std::string const &_sText)
{
    if (m_sYear != _sText)
    {
        m_sYear = _sText;
        m_bYear = true;
    }
    return m_bYear;
}

bool Information::setFilter(std::string const& _sText)
{
    if (m_sFilter != _sText)
    {
        m_sFilter = _sText;
        m_bFilter = true;
    }
    return m_bFilter;
}
bool Information::setCurrentDirectory(std::string const &_sText)
{
    if (m_sCurrentDirectory != _sText)
    {
        m_sCurrentDirectory = _sText;
        m_bCurrentDirectory = true;
    }
    return m_bCurrentDirectory;
}
bool Information::setProgressBar(int _nPercent)
{
    if (m_nPercent != _nPercent)
    {
        m_nPercent = _nPercent;
        return true;
    }
    return false;
}

// interface handleController
bool Information::handleController(Controller *_pController)
{
    bool bReturn = false;
    if (_pController != NULL)
    {
        if (_pController->hasPropertyValue(P_INFO_MUSIC_ARTIST))
        {
            std::string sArtist = _pController->getPropertyAsStringValue(P_INFO_MUSIC_ARTIST);
            bReturn |= setArtist(sArtist);
        }
        if (_pController->hasPropertyValue(P_INFO_PROGRESSBAR))
        {
            int nProgressBarValue = _pController->getPropertyAsIntValue(P_INFO_PROGRESSBAR);
            bReturn |= setProgressBar(nProgressBarValue);
        }
        // if (_pController->hasPropertyValue(P_INFO_CURRENT_DIRECTORY))
        // {
        //     std::string sNewDirectory = _pController->getPropertyAsStringValue(P_INFO_CURRENT_DIRECTORY);
        //     bReturn |= setCurrentDirectory(sNewDirectory);
        // }
        if (_pController->hasPropertyValue(P_INFO_NEXTPLAY))
        {
            std::string sNextPlay = _pController->getPropertyAsStringValue(P_INFO_NEXTPLAY);
            bReturn |= setCurrentDirectory(sNextPlay);
        }
        if (_pController->hasPropertyValue(P_FILTER_STRING))
        {
            std::string sFilter = _pController->getPropertyAsStringValue(P_FILTER_STRING);
            bReturn |= setFilter(sFilter);
        }
    }
    return bReturn;
}

