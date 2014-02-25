#include <string>

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_gfxPrimitives.h>

#include "ttffont.h"
#include "directory.h"
#include "somecolors.h"
#include "ColorHelper.h"

#include "viewdirectory.h"
#include "filehelper.h"
#include "t_print.h"
#include "property.h"
#include "global.h"
#include "updaterect.h"
#include "quality.h"

// -----------------------------------------------------------------------------

bool ViewDirectory::isUpdateNeed()
{
    // this told us, to really repaint the list
    // if (m_pPlayList && m_pPlayList->isUpToDate() == false)
    // {
    //     m_bUpdateIsNeed = true;
    //     m_pPlayList->setUpToDate();
    // }
    return m_nUpdateIsNeed > 0 ? true : false;
}

void ViewDirectory::setUpdateIsNeed(bool _bValue)
{
    if (_bValue == true)
    {
        m_nUpdateIsNeed = 2;
    }
    else
    {
        m_nUpdateIsNeed--;
    }
}

// -----------------------------------------------------------------------------
// ViewDirectory::ViewDirectory(SDL_Surface *_pDisplay, TTFFont *_pFont, DirectoryHelper & _aDir)
ViewDirectory::ViewDirectory(TTFFont *_pFont, DirectoryHelper * _pDirHelper)
        :
         m_nLines(6),
         m_x(10),
         m_y(0 /* 287 */ ),
         m_w(800 - 80),
         /* m_nFiles(0), */
         m_nOffset(0),
         m_yUpdate(-1),
         m_pDisplay(NULL),
         m_pBackgrounds(NULL),
         m_pTextSurfaces(NULL),
         m_pFont(_pFont),
         m_pPlayList(NULL),
         m_pPlayListIterator(NULL),
         m_pDirectoryHelper(_pDirHelper),
         m_pLock(SDL_CreateMutex()),
         m_eType(VIEW_TYPE_NOTHING),
         m_nUpdateIsNeed(0), 
         m_nCurrentRealSelectionPos(-1),
         m_nNextScrollerPosition(0)
{
    // refreshScroller();
    setUpdateIsNeed(true);
    m_y = 300; // (600 - m_nLines * m_pFont->getH()) / 2; 

        
    if (m_pFont != NULL)
    {
        initBackground();
    }
}

void ViewDirectory::initBackground()
{
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
    
    // rmask = 0x0000000a;
    // gmask = 0x00000a00;
    // bmask = 0x000a0000;
    // amask = 0xff000000;
#endif
    m_pBackgrounds = new SDL_Surface*[m_nLines];
    m_pTextSurfaces = new SDL_Surface*[m_nLines];

    int nFontH = m_pFont->getH();
    for (int i=0;i<m_nLines;i++)
    {
        SDL_Surface* pSurface = SDL_AllocSurface(SDL_SWSURFACE | SDL_RLEACCEL, /* SDL_HWSURFACE | SDL_RLEACCEL | SDL_HWACCEL,*/
                                                 m_w, nFontH, 32,
                                                 rmask,gmask,bmask,amask);
        // SDL_SetAlpha(pSurface, SDL_SRCCOLORKEY | SDL_SRCALPHA, 128);
        // SDL_SetAlpha(pSurface,
        //              SDL_SRCCOLORKEY | SDL_SRCALPHA,
        //              SDL_ALPHA_OPAQUE);
        // Uint32 nColorKey = SDL_MapRGB(pSurface->format, Color::transparent.r, Color::transparent.g, Color::transparent.b);
        // int n = SDL_SetColorKey(pSurface, SDL_SRCCOLORKEY, nColorKey);
        clearSurface(pSurface);
        m_pTextSurfaces[i] = pSurface;

        pSurface = SDL_AllocSurface(SDL_SWSURFACE | SDL_RLEACCEL, /* SDL_HWSURFACE | SDL_RLEACCEL | SDL_HWACCEL,*/
                                    m_w, nFontH, 32,
                                    rmask,gmask,bmask,amask);
        m_pBackgrounds[i] = pSurface;        
    }
}


// -----------------------------------------------------------------------------
void ViewDirectory::setList(Audio::PlayList * _pPlayList, bool _bFilterChange)
{
    // m_aPlayList.clear();
    // m_aPlayList.setNewList( _aPlayList );
    m_pPlayList = _pPlayList;
    unsigned int nOldPosition = 0;
    if (m_pPlayListIterator != NULL)
    {
        nOldPosition = m_pPlayListIterator->getCursorPosition();
        delete m_pPlayListIterator;
    }
    m_pPlayListIterator = new Audio::PlayListIterator(m_pPlayList, nOldPosition, m_sCurrentFilter);

    // create new scroller
    int nSize = m_pPlayListIterator->getSize();

    int nFilterPosition = 0;
    if (_bFilterChange == true)
    {
        nFilterPosition = m_pPlayListIterator->findFilteredPosition(m_sCurrentFilename);
        if (nFilterPosition < 0)
        {
            nFilterPosition = 0;
            m_nNextScrollerPosition = 0;
        }
        else
        {
            m_nNextScrollerPosition = m_nCurrentRealSelectionPos;
            if (nSize < nFilterPosition)
            {
                nFilterPosition = nSize;
            }
        }
    }
    m_aScroller.refreshScroller(nSize, nFilterPosition, m_nLines);
    setUpdateIsNeed(true);
}

// -----------------------------------------------------------------------------
void ViewDirectory::changeDir(std::string const& _sDir, std::string const& _sLastDirName)
{
    m_pDirectoryHelper->changeDir(_sDir);
    setList( m_pDirectoryHelper->getList(), false /* NO_FILTER_CHANGE */ );

    // jump to last directory entry name, it's only for nicer editing
    if (_sLastDirName.size() > 0)
    {
        bool bFound = false;
        std::string sLastName;
        std::string sCurrentPath;
        const char* pLastDirName = basename(_sLastDirName.c_str());
        std::string sLastDirName(pLastDirName);
        
        // t_print("LastDir %s\n", sLastDirName.c_str());
        while(!bFound)
        {
            sLastName = sCurrentPath;
            sCurrentPath = m_pPlayList->getElement(m_aScroller.getSelectionPos()).getFilename();
            // t_print("%s\n", sCurrentPath.c_str());
            if (sCurrentPath != sLastDirName)
            {
                up();
            }
            else
            {
                bFound = true;
            }
            // emergency break:
            if (sLastName == sCurrentPath)
            {
                break;
            }
        }
    }

    if (hasAController())
    {
        getController()->setPropertyValue(P_INFO_NEXTPLAY, new String(_sDir));
    }
}

// -----------------------------------------------------------------------------
// bool ViewDirectory::isDirectory(std::string const& _aName)
// {
//     return m_pPlayList->isDirectory(_aName);
// }
// -----------------------------------------------------------------------------
ViewDirectory::~ViewDirectory()
{
    MutexGuard pv(m_pLock);
    if (m_pTextSurfaces)
    {
        for (int i=0;i<m_nLines;i++)
        {
            SDL_FreeSurface(m_pTextSurfaces[i]);
        }
        delete [] m_pTextSurfaces;
        m_pTextSurfaces = NULL;
    }
    if (m_pBackgrounds)
    {
        for (int i=0;i<m_nLines;i++)
        {
            SDL_FreeSurface(m_pBackgrounds[i]);
        }
        delete [] m_pBackgrounds;
        m_pBackgrounds = NULL;
    }
}

void ViewDirectory::clearSurface(SDL_Surface* _pSurface)
{
    if (_pSurface != NULL)
    {
        // int nColorKeyOther = SDL_MapRGB(_pSurface->format, 0, 0, 1);
        // int n = SDL_SetColorKey(_pSurface, SDL_SRCCOLORKEY | SDL_RLEACCEL, nColorKeyOther);
        // SDL_SetAlpha(_pSurface, SDL_RLEACCEL, 0);
        
        boxColor(_pSurface, 0, 0, _pSurface->w, _pSurface->h, ColorToUint32(Color::black ) );

        int nColorKeyBlack = SDL_MapRGB(_pSurface->format, 0, 0, 0);
        int dummy = SDL_SetColorKey(_pSurface, SDL_SRCCOLORKEY | SDL_RLEACCEL, nColorKeyBlack);
        SDL_SetAlpha(_pSurface, SDL_RLEACCEL, 0);
    }
}


// void Get32BitPixel(SDL_Surface* _pSurface)
// {
//     // source is out of SDL_PixelFormat
//     /* Lock the surface */
//         SDL_PixelFormat *fmt;
//        Uint32 temp, pixel;
//        Uint8 red, green, blue, alpha;
// 
//        fmt = _pSurface->format;
//        SDL_LockSurface(_pSurface);
//        pixel = *((Uint32*)_pSurface->pixels);
//        SDL_UnlockSurface(_pSurface);
// 
//        /* Get Red component */
//        temp = pixel&fmt->Rmask; /* Isolate red component */
//        temp = temp>>fmt->Rshift;/* Shift it down to 8-bit */
//        temp = temp<<fmt->Rloss; /* Expand to a full 8-bit number */
//        red = (Uint8)temp;
// 
//        /* Get Green component */
//        temp = pixel&fmt->Gmask; /* Isolate green component */
//        temp = temp>>fmt->Gshift;/* Shift it down to 8-bit */
//        temp = temp<<fmt->Gloss; /* Expand to a full 8-bit number */
//        green = (Uint8)temp;
// 
//        /* Get Blue component */
//        temp = pixel&fmt->Bmask; /* Isolate blue component */
//        temp = temp>>fmt->Bshift;/* Shift it down to 8-bit */
//        temp = temp<<fmt->Bloss; /* Expand to a full 8-bit number */
//        blue = (Uint8)temp;
// 
//        /* Get Alpha component */
//        temp = pixel&fmt->Amask; /* Isolate alpha component */
//        temp = temp>>fmt->Ashift;/* Shift it down to 8-bit */
//        temp = temp<<fmt->Aloss; /* Expand to a full 8-bit number */
//        alpha = (Uint8)temp;
// 
//        printf("Pixel Color -> R: %d,  G: %d,  B: %d,  A: %d \n", red, green, blue, alpha);
// }

    // if (m_pOldBackground == NULL)
    // {
    // 
    //     SDL_Rect drect        = {0, 0, 0, 0};
    //     SDL_BlitSurface(m_pDisplay, 0, m_pOldBackground, &drect);
    //     int dummy = 0;
    // }
    // 
    // SDL_Rect drect        = {0, 0, 0, 0};
    // int nBlitBack = SDL_BlitSurface(m_pOldBackground, 0, m_pDisplay, &drect);
    // 
    // // hard update for test only
    // SDL_Rect drect2 = {0, 0, 799, 599 };
    // mySDL_UpdateRects(m_pDisplay, 1, &drect2);

// boxColor(m_pTextSurfaces, m_x, m_y, m_x + m_w, ymax, ColorToUint32(Color::black ) );

    // hard update for test only
    // SDL_Rect drect3 = {0, 0, 799, 599 };
    // mySDL_UpdateRects(m_pDisplay, 1, &drect3);

void ViewDirectory::copyBackground()
{
    int nH = m_pFont->getH();

    int y = m_y;
    for(int i=0;i<m_nLines;i++)
    {
        // rescue background
        SDL_Surface* pBackground = m_pBackgrounds[i];
        SDL_Rect srect;
        srect.x = m_x;
        srect.y = y;
        srect.w = pBackground->w;
        srect.h = pBackground->h;

        SDL_Rect drect;
        drect.x = 0;
        drect.y = 0;
        drect.w = 0; // pBackground->w;
        drect.h = 0; // pBackground->h;

        SDL_BlitSurface(m_pDisplay, &srect, pBackground, &drect);
        
        SDL_Surface *pTextSurface = m_pTextSurfaces[i];
        
        // paint the normal text
        srect.x = 0;
        srect.y = 0;
        srect.w = pBackground->w;
        srect.h = pBackground->h;

        drect.x = m_x;
        drect.y = y;
        drect.w = 0;
        drect.h = 0;

        SDL_BlitSurface(pTextSurface, &srect, m_pDisplay, &drect);

        // update the whole sense
        drect.x = m_x;
        drect.y = y;
        drect.w = pTextSurface->w;
        drect.h = pTextSurface->h;

        // SDL_Rect drect2 = {m_x, y, pTextSurface->w, pTextSurface->h };
        mySDL_UpdateRects(m_pDisplay, 1, &drect);

        // restore background
        srect.x = 0;
        srect.y = 0;
        srect.w = pBackground->w;
        srect.h = pBackground->h;

        drect.x = m_x;
        drect.y = y;
        drect.w = 0;
        drect.h = 0;

        SDL_BlitSurface(pBackground, &srect, m_pDisplay, &drect);
        
        // update the whole sense
//         drect.x = m_x;
//         drect.y = y;
//         drect.w = pTextSurface->w;
//         drect.h = pTextSurface->h;
//         mySDL_UpdateRects(m_pDisplay, 1, &drect);

        y += nH;
    }
}

// -----------------------------------------------------------------------------
#include "audio_icon.xpm"
#include "video_icon.xpm"

void ViewDirectory::paintValues()
{
    // t_print(T_FLOWCONTROL, "ViewDirectory::paintValues()\n");
    if (isUpdateNeed() == false)
    {
        return;
    }
    
    int y = m_y;
    // SelectedText aSelection;
    // m_pFont->setUpdate(false);

    m_pFont->setMode(X_LEFT | STRING_SHORTEN);
    m_pFont->setWidth(690);
    
    // clearBackground();

    // we have to do this fake, or it's not possible to know the getH() value.
    // m_pFont->setFontType(FONT_TYPE_NORMAL);
    // m_pFont->setText(m_pTextSurfaces, Color::white, m_x, m_y, "..");
    int nH = m_pFont->getH();

    int ymax = 599;

    int i = 0;

    SDL_Surface *pVideoIconSurface = IMG_ReadXPMFromArray((char **)video_icon_xpm);
    SDL_Surface *pAudioIconSurface = IMG_ReadXPMFromArray((char **)audio_icon_xpm);
    
    // int nStartIndex = m_aScroller.getMinPos() + m_aScroller.getGlobalPos();
    // CursorPos nSize = aIterator.getSize();
    // m_aScroller.refreshScroller(nSize);
    if (m_nNextScrollerPosition != 0)
    {
        m_pPlayListIterator->resetPosition(m_nCurrentRealSelectionPos);
        m_nNextScrollerPosition = 0;
    }
    else
    {
        m_pPlayListIterator->resetPosition(m_aScroller.getGlobalPos());
    }
    
    // 
//     t_print("\n");
//     t_print("m_nCurrentRealSelectionPos : %d\n", m_nCurrentRealSelectionPos);
//     t_print("\n");
//     t_print("\n");
//     t_print("\n");
//     t_print("\n");
//     t_print("\n");
//     t_print("\n");

    int nCurrentSurface = 0;
    for(i = m_aScroller.getMinPos();
        i <= m_aScroller.getMaxPos();
        i++)
    {
        SDL_Surface* pTextSurface = m_pTextSurfaces[nCurrentSurface];
        clearSurface(pTextSurface);
        
        int nCurrentIndex = i + m_aScroller.getGlobalPos();
//         t_print("nCurrentIndex %d ", nCurrentIndex);
        // Audio::Entry aEntry = m_pPlayList->getElement(nCurrentIndex);
        Audio::Entry aEntry = m_pPlayListIterator->getCurrentElement();

        std::string sFilename = aEntry.getFilename();
        std::string sFullFilename = aEntry.getPath() + "/" + sFilename;
        // std::string sFilename = m_aFileDirList[nCurrentIndex];
        // t_print("%s\n", sFilename.c_str());
        
        SDL_Surface *pIconSurface = NULL;
        if (FileHelper::isVideo(sFilename))
        {
            pIconSurface = pVideoIconSurface;
        }
        if (FileHelper::isAudio(sFilename))
        {
            pIconSurface = pAudioIconSurface;
        }

        std::string sFilenameShortend = FileHelper::removeFileExt(sFilename);
        if (aEntry.getSystemType() == ENTRY_ISDIRECTORY)
        {
            if (sFilenameShortend != "..")
            {
                sFilenameShortend += "/";
            }
        }
        else
        {
            // sFilenameShortend = FileHelper::getLastPath(aEntry.getPath()) + "/" + FileHelper::removeFileExt(sFilename);
            sFilenameShortend = FileHelper::removeFileExt(sFilename);
        }
        
        // t_print(" Name %s\n", sFilenameShortend.c_str() );
      

        nH = m_pFont->getH();
        // boxColor(pTextSurface, m_x, y, m_x + m_w, y + nH - 2, ColorToUint32(Color::grey50, 0x80) );

        if (m_sSpecialFilename.size() > 0 &&
            sFullFilename == m_sSpecialFilename)
        {
#warning here we want to show to current playing song
            m_pFont->setFontType(FONT_TYPE_BOLD | FONT_TYPE_OUTLINE);
            SDL_Rect aRect = m_pFont->setText(pTextSurface, Color::blue, 0, 0, sFilenameShortend);
            if (hasAController())
            {
                getController()->setPropertyValue(P_VIEW_CURRENT_FILENAME, new String(sFullFilename), false);
            }
        }
        else
        {
            // change the text color of the current cursor position
            if (nCurrentIndex == m_aScroller.getSelectionPos())
            {
                // aSelection = SelectedText(pTextSurface, m_x, y, m_pFont->getW(), m_pFont->getH(), sFilenameShortend);
                m_pFont->setFontType(FONT_TYPE_OUTLINE);
                SDL_Rect aRect = m_pFont->setText(pTextSurface, Color::red20, 0, 0, sFilenameShortend);
                // this is the current real position in the playlist array
                m_nCurrentRealSelectionPos = m_pPlayListIterator->getCursorPosition();
                m_sCurrentFilename = getPlayList()->getElement( m_nCurrentRealSelectionPos ).getFilename();
                if (hasAController())
                {
                    getController()->setPropertyValue(P_VIEW_CURRENT_FILENAME, new String(sFullFilename), false);
                }
            }
            else
            {
// #error show where the current title is in blue
                // check, if near by selected position
                if (nCurrentIndex - 1 == m_aScroller.getSelectionPos() || 
                    nCurrentIndex + 1 == m_aScroller.getSelectionPos())
                {
                    m_pFont->setFontType( FONT_TYPE_OUTLINE);
                    m_pFont->setText(pTextSurface, Color::greyc0, 0, 0, sFilenameShortend);
                }
                else
                {
                    // absolutly normal
                    m_pFont->setFontType( FONT_TYPE_OUTLINE);
                    m_pFont->setText(pTextSurface, Color::grey80, 0, 0, sFilenameShortend);
                }
            }
        }
        
        if (pIconSurface)
        {
            int x = m_w - pIconSurface->w /* 24 */ /* width of picture */ - 5 /* frame */;
            int newy = ((nH - pIconSurface->h /* 16*/ ) / 2);
            SDL_Rect drect        = {x, newy, 0, 0};
            SDL_BlitSurface(pIconSurface, 0, pTextSurface, &drect);
        }
        
        int nQuality = m_pQuality ? m_pQuality->getQuality(sFullFilename) : 0;
        if (nQuality > 0)
        {
            int x = m_w - 46;
            int w = 12;
            // int newy = y;
            int nQH = 20 - ( 2 * nQuality);
            int y2 = nH - 9;
            boxColor(pTextSurface,x ,y2,x + w, y2 - nQH, ColorToUint32(Color::greyc0));
            nQH = 20;
            rectangleColor(pTextSurface,x ,y2, x + w, y2 - nQH, ColorToUint32(Color::grey60));
        }
        
        m_pPlayListIterator->next();
        y += nH - 1;
        nCurrentSurface++;
    }
    // t_print("--------------------------------------------------------------------------------\n");
    
    // clear the rest
    for (int j = i; j < m_nLines;++j)
    {
        SDL_Surface* pTextSurface = m_pTextSurfaces[nCurrentSurface];
        clearSurface(pTextSurface);
        // boxColor(pTextSurface, m_x, y, m_x + m_w, y + nH - 2, ColorToUint32(Color::grey40, 0x80) );
        y += nH - 1;
        nCurrentSurface ++;
    }
    
    if (y > ymax)
        y = ymax;
    m_yUpdate = y;

    copyBackground();
//    aSelection.paintRectAngle();
    // m_pFont->setUpdate(true);
}
// -----------------------------------------------------------------------------
void ViewDirectory::update()
{
    // t_print(T_FLOWCONTROL, "ViewDirectory::update()\n");
    if (isUpdateNeed() == false) return;
    // if (m_yUpdate < 0) return;
    // 
    // SDL_Rect drect = {m_x, m_y, m_w, m_yUpdate - m_y /* 600 - m_y */ };
    // mySDL_UpdateRects(m_pDisplay, 1, &drect);
    // 
    setUpdateIsNeed(false);
}

// -----------------------------------------------------------------------------
void ViewDirectory::up() 
{
    // t_print(T_FLOWCONTROL, "ViewDirectory::up()\n");
    
    m_aScroller.up(); //++
    setUpdateIsNeed(true);
}
void ViewDirectory::down()
{
    // t_print(T_FLOWCONTROL, "ViewDirectory::down()\n");

    m_aScroller.down(); // --
    setUpdateIsNeed(true);
}
// int ViewDirectory::getSelectionPos()
// {
//     return m_pScroller->getSelectionPos();
// }
// void ViewDirectory::setSelectionPos(int _n)
// {
//     for(int i=0;i<_n;i++)
//     {
//         up();
//     }
// }

bool ViewDirectory::dispatch(SDL_Event const& _aEvent)
{
    // t_print(T_FLOWCONTROL, "ViewDirectory::dispatch\n");
    bool bReturn = false;
        
    switch( _aEvent.type )
    {
    case SDL_USEREVENT:
        Global::documentKey(SDLK_DOWN, 0,  "Scroll down.");
        Global::documentKey(SDLK_UP,   0,  "Scroll up.");
        Global::documentKey(SDLK_LEFT, 0,  "Previous directory.");
        Global::documentKey(SDLK_RIGHT, 0, "Jump to current directory, if one under cursor.");            
        break;

    case SDL_KEYDOWN:
        if (_aEvent.key.keysym.sym == SDLK_DOWN)
        {
            // t_print(T_FLOWCONTROL, "KEY: down()\n");
            up();
            bReturn = true;
        }
        else if (_aEvent.key.keysym.sym == SDLK_UP)
        {
            // t_print(T_FLOWCONTROL, "KEY: up()\n");
            down();
            bReturn = true;
        }
        else if (getType() == VIEW_TYPE_DIRECTORY && _aEvent.key.keysym.sym == SDLK_LEFT)
        {
            // t_print(T_FLOWCONTROL, "KEY: left()\n");
            // Audio::Entry aEntry = getPlayList()->getElement( m_aScroller.getSelectionPos());
            Audio::Entry aEntry = getCurrentSelectionValue();
            std::string sLastDir = DirectoryHelper::removeLastDirEntry(aEntry.getPath());
            
            changeDir(sLastDir, aEntry.getPath());
            bReturn = true;
        }
        else if (getType() == VIEW_TYPE_DIRECTORY && _aEvent.key.keysym.sym == SDLK_RIGHT)
        {
            // t_print(T_FLOWCONTROL, "KEY: right()\n");
            Audio::Entry aEntry = getCurrentSelectionValue();
            std::string aStr = aEntry.getFilename();

            // Audio::Entry aEntry = getPlayList()->getElement( m_aScroller.getSelectionPos() );
            // std::string aStr = aEntry.getFilename();
            if (getPlayList()->isDirectory(aStr) && aStr != "..")
            {
                std::string sNewDir = FileHelper::getAbsoluteFile(aEntry);
                changeDir(sNewDir, "");
                up();
            }
            bReturn = true;
        }
        break;
        // default:
        // bReturn = false;
    }
    
    return bReturn;
}

bool ViewDirectory::paint(SDL_Surface* _pSurface)
{
    MutexGuard pv(m_pLock);
//    t_print(T_FLOWCONTROL, "ViewDirectory::paint()\n");
    t_print(T_AUDIOPLAYCONTROL, "Paint start\n");
    m_pDisplay = _pSurface;

    // t_print(T_DEBUG, "Size                    :%d\n", m_pPlayListIterator->getSize());
    // t_print(T_DEBUG, "Cursor Scroller         :%d\n", m_aScroller.getSelectionPos());
    // t_print(T_DEBUG, "Scroller global pos     :%d\n\n", m_aScroller.getGlobalPos());

    paintValues();
    update();
    m_pDisplay = NULL;
    t_print(T_AUDIOPLAYCONTROL, "Paint stop\n");
    return false;
}

void ViewDirectory::setCursorPos(int _nCursorPos)
{
    // Set the last known Cursor position, don't start at position '..'
    if (_nCursorPos != 0)
    {
        m_aScroller.setSelectionPos(_nCursorPos);
    }
    else
    {
        // up();
    }
}

int ViewDirectory::getCurrentSelectionPos()
{
    // int nCurrentSelection = m_aScroller.getSelectionPos();
    // return nCurrentSelection;
    return m_nCurrentRealSelectionPos;
}

Audio::Entry ViewDirectory::getCurrentSelectionValue()
{
    return getPlayList()->getElement( getCurrentSelectionPos() );

    // LLA: this is a stupid method to run through the array and search for the current selection
    
    // static Audio::Entry aEmptyEntry = Audio::Entry();
    // 
    // m_pPlayListIterator->resetPosition(m_aScroller.getGlobalPos());
    // int nRealCursor = -1;
    // 
    // int i = 0;
    // for(i = m_aScroller.getMinPos();
    //     i <= m_aScroller.getMaxPos();
    //     i++)
    // {
    //     int nRealIndex = i + m_aScroller.getGlobalPos();
    //     if (nRealIndex == m_aScroller.getSelectionPos())
    //     {
    //         nRealCursor = m_pPlayListIterator->getCursorPosition();
    //         break;
    //     }
    //     m_pPlayListIterator->next();
    // }
    // 
    // if (nRealCursor != -1)
    // {
    //     return m_pPlayList->getElement(nRealCursor);
    // }
    // return aEmptyEntry;
}

void ViewDirectory::setFilter(std::string const& _sFilter)
{
    if (_sFilter != m_sCurrentFilter)
    {
        m_sCurrentFilter = _sFilter;
        // t_print(T_DEBUG, "Filter : %s\n", m_sCurrentFilter.c_str());
        setList(m_pPlayList, true /* FILTER_CHANGED */ );
    }
}

void ViewDirectory::setSpecialFilename(std::string const& _sName)
{
    m_sSpecialFilename = _sName;
}


// interface handleController
bool ViewDirectory::handleController(Controller *_pController)
{
    bool bReturn = false;
    if (_pController != NULL)
    {
        if (_pController->hasPropertyValue(P_FILTER_STRING))
        {
            std::string sFilter = _pController->getPropertyAsStringValue(P_FILTER_STRING);
            setFilter(sFilter);
            bReturn = true;
        }
        if (_pController->hasPropertyValue(P_PLAY_CURRENT_FILENAME))
        {
            std::string sFilename = _pController->getPropertyAsStringValue(P_PLAY_CURRENT_FILENAME);
            setSpecialFilename(sFilename);
            bReturn = true;
        }
        if (_pController->hasPropertyValue(P_VIEW_REPAINT_DIRECTORY))
        {
            bool bValue = _pController->getPropertyAsBooleanValue(P_VIEW_REPAINT_DIRECTORY);
            setUpdateIsNeed(bValue);
            _pController->removePropertyValue(P_VIEW_REPAINT_DIRECTORY);
            bReturn = true;
        }
    }
    
    return bReturn;
}
// -----------------------------------------------------------------------------
// refresh the scroller with a new size, values will reset to 0
void DoubleScroller::refreshScroller(int _nSize, int _nGlobalStartPos, int _nMaxSize)
{
    // remove old scroller
    delete m_pScroller;
    delete m_pMainFieldScroller;

    if (_nGlobalStartPos >= _nSize)
    {
        _nGlobalStartPos = _nSize - 1;
    }
    if (_nGlobalStartPos < 0)
    {
        _nGlobalStartPos = 0;
    }
    
    m_pMainFieldScroller = new ScrollViewer(NULL, 0, _nSize - 1, _nGlobalStartPos);

    int nFiles = _nMaxSize;
    if (_nSize < nFiles)
    {
        nFiles = _nSize;
    }
    
    m_pScroller = new ScrollViewer(m_pMainFieldScroller, 0, nFiles - 1);
}

void DoubleScroller::up() 
{
    m_pScroller->up(); //++
}
void DoubleScroller::down()
{
    m_pScroller->down(); // --
}
int DoubleScroller::getSelectionPos()
{
    return m_pScroller->getSelectionPos();
}
void DoubleScroller::setSelectionPos(int _n)
{
    for(int i=0;i<_n;i++)
    {
        up();
    }
}
DoubleScroller::~DoubleScroller()
{
    delete m_pScroller;
    delete m_pMainFieldScroller;
}
int DoubleScroller::getGlobalPos()
{
    return m_pMainFieldScroller->getSelectionPos();
}

int DoubleScroller::getMinPos()
{
    return m_pScroller->getMinPos();
}

int DoubleScroller::getMaxPos()
{
    return m_pScroller->getMaxPos();
}
