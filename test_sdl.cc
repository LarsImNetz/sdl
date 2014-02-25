#include <map>
#include <string>
#include <vector>
#include <queue>
#include <fstream>

#include <unistd.h>
#include <errno.h>
#include <sys/wait.h>
#include <sys/socket.h>

#include <SDL.h>
#include <SDL_image.h>

#include <SDL_gfxPrimitives.h>

#include "stringhelper.h"
#include "somecolors.h"
#include "t_print.h"
#include "rgb2hsv.h"
#include "ioextend.h"
#include "playlist.h"
#include "ttffont.h"
#include "pathmax.h"

#include "rectangleWithRoundVertex.h"
#include "ColorHelper.h"

#include "controller.h"
#include "global.h"

#include "stopwatch.hxx"

// -----------------------------------------------------------------------------
char** environment;

// -----------------------------------------------------------------------------

long linear(long _nMaxValue, long _nSmallerMaxValue, long _nCurrentValue)
{
    // you have Values from 0 to 90 but need values from 0 to 255
    // this will help you
    double nMax = _nMaxValue;
    double nSmaller = _nSmallerMaxValue;
    double nFactor = nMax / nSmaller;
    return long(double(_nCurrentValue * nFactor));
}


// -----------------------------------------------------------------------------
// #f48104 Orange 
// #82817f hell
// #231f1e dark gray
// #303030 

// clip auf 800x600 pixel
void clipRect(int nXPos, int nYPos, int nWidth, int nHeight, SDL_Rect& _aOutRect)
{
    if (nXPos < 0)
    {
        nWidth -= nXPos;
        nXPos = 0;
    }
    if (nYPos < 0)
    {
        nHeight -= nYPos;
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

SDL_Surface *m_pDisplay = NULL;

void drawProgressBar(int _nPercent)
{
    clipPercent(_nPercent);

    int x = 50; // 147 - 396
    int y = 580; // - 70 artist

    int nXSizeMax = 800 - 2 * x;
    
    int xsize = nXSizeMax * _nPercent / 100;

    boxColor(m_pDisplay, x,         y, x + xsize, y + 5, toUint32(Color::orange));
    boxColor(m_pDisplay, x + xsize, y, x + nXSizeMax,y + 5, toUint32(Color::grey30));
    SDL_Rect drect;
    clipRect(x, y, nXSizeMax, 5, drect);
    SDL_UpdateRects(m_pDisplay, 1, &drect);
}

void clearArtist()
{
    int x = 147; // 147 - 396
    int y = 45; // - 70 artist
// 73 title
// 101 album
// 129 year
    boxColor(m_pDisplay, x, y, x + 249, y + 25, toUint32(Color::grey30));
    SDL_Rect drect;
    clipRect(x, y, 249, 25, drect);
    SDL_UpdateRects(m_pDisplay, 1, &drect);
}




std::string getFile()
{
    std::string aBackStr;
    
    SDL_Event event;
    // SDL_EventFilter aFilter = SDL_GetEventFilter();

    int quit = 0;
    // Event-Handling
    int nWaitInNanoSec = 100000000;
    // int nHandleEvents = 3;
    // int nDontHandleAnEvent = 0;

    int nWaitKeyEndprell = 0;
    while( quit == 0)
    {    
        struct timespec delay;
        delay.tv_sec = 0;
        delay.tv_nsec = nWaitInNanoSec;
        
        nanosleep(&delay, 0); // tenth second
        do
        {
            // bHandleEvent = false;
            
            switch( event.type )
            {
            case SDL_QUIT:
                quit = 1;
                break;
            case SDL_KEYUP:
                nWaitKeyEndprell = 0;
                break;
            case SDL_KEYDOWN:
                // t_print("Key:%d mod:%d \n", event.key.keysym.sym, event.key.keysym.mod);
                nWaitInNanoSec =  50000000;
                
                // t_print("event.key.state %d\n", event.key.state);

                // HANDLE QUIT
                if (event.key.keysym.sym == SDLK_ESCAPE ||
                    event.key.keysym.sym == SDLK_q || 
                    event.key.keysym.sym == 'Q')
                {
                    quit = 1;
                    // nHandleEvents++;
                }

                // LOUDNESS
                else if (Global::shiftKey(event) && event.key.keysym.sym == SDLK_DOWN)
                {
                    // shift down
                    // t_print("leiser\n");
                    // _aDispatcher.setNewEvent(EventValue(IMM_AUDIO_EVENT_LOUDNESS_LOWER));
                }
                else if (Global::shiftKey(event) && event.key.keysym.sym == SDLK_UP)
                {
                    // shift up
                    // _aDispatcher.setNewEvent(EventValue(IMM_AUDIO_EVENT_LOUDNESS_HIGHER));
                }

                else if (Global::shiftKey(event) && event.key.keysym.sym == SDLK_LEFT)
                {
                    // shift left
                    // _aDispatcher.setNewEvent(EventValue(IMM_AUDIO_EVENT_JUMP_BACKWARD, 10.0));
                }

                else if (Global::shiftKey(event) && event.key.keysym.sym == SDLK_RIGHT)
                {
                    // shift right
                    // _aDispatcher.setNewEvent(EventValue(IMM_AUDIO_EVENT_JUMP_FORWARD, 10.0));
                }

                // 
                else if (event.key.keysym.sym == SDLK_n ||
                         event.key.keysym.sym == 'N')
                {
                    // t_print("next\n");
                    // _aDispatcher.setNewEvent(EventValue(IMM_AUDIO_EVENT_PLAY_NEXT));
                }
                else if (event.key.keysym.sym == SDLK_DOWN)
                {
                    // _aView.up();
                }
                else if (event.key.keysym.sym == SDLK_UP)
                {
                    // _aView.down();
                }
                else if (event.key.keysym.sym == SDLK_LEFT)
                {
                    // _aView.changeDir("..", _aView.getCurrentPath());
                }
                else if (event.key.keysym.sym == SDLK_RIGHT)
                {
                    // t_print("Key right\n");
                    // std::string aStr = _aView.getFileName( _aView.getSelectionPos());
                    // if (_aView.isDirectory(aStr) && aStr != "..")
                    // {
                    //     _aView.changeDir(aStr, "");
                    // }
                }
                else if (event.key.keysym.sym == SDLK_c)
                {
                    t_print("Clear\n");
                    // _aDispatcher.setNewEvent(EventValue(IMM_AUDIO_EVENT_CLEAR_LIST));
                }
                
                else if (event.key.keysym.sym == SDLK_RETURN)
                {
                    // std::string aStr = _aView.getFileName( _aView.getSelectionPos());
                    // if (_aView.isDirectory(aStr))
                    // {
                    //     std::string sLastDirName;
                    //     if (aStr == "..")
                    //     {
                    //         sLastDirName = _aView.getCurrentPath();
                    //     }                        
                    //     _aView.changeDir(aStr, sLastDirName);
                    // }
                    // else
                    // {
                    //     aBackStr = _aView.getCurrentPath() + "/" + aStr;
                    //     quit = 1;
                    // }
                    quit = 1;
                }
                else if (event.key.keysym.sym == 'S' ||
                         event.key.keysym.sym == SDLK_s)
                {
                    // t_print("stop\n");
                    // _aDispatcher.setNewEvent(EventValue(IMM_AUDIO_EVENT_STOP));
                }
                nWaitKeyEndprell++;
                break;
            default:
                break;
                nWaitInNanoSec = 100000000;
                nWaitKeyEndprell = 0;
            }
            // _aView.paintValues();
            // _aView.update();

            // _aDispatcher.handleAnEvent();

            // t_print("%d\n", nWaitKeyEndprell);
            if (nWaitKeyEndprell == 1)
            {
                int nWait = 6;
                while (nWait--)
                {
                    // t_print("Entprell...\n");
                    struct timespec delay;
                    delay.tv_sec = 0;
                    delay.tv_nsec = nWaitInNanoSec;
                    
                    nanosleep(&delay, 0); // tenth second
                    SDL_PollEvent( &event );
                    if (event.type == SDL_KEYUP)
                    {
                        // t_print("SDL_KEYUP get.\n");
                        nWaitKeyEndprell = 0;
                        break;
                    }
                }
            }
        }
        while( SDL_PollEvent( &event ) );
    }
    return aBackStr;
}

// -----------------------------------------------------------------------------
class mySDL
{
    SDL_Surface *m_pImage;
    SDL_Surface *m_pDisplay;

    void screenUpdate();
public:
    mySDL()
            :m_pImage(NULL),
             m_pDisplay(NULL)
        {
        }

    ~mySDL()
        {}
    
    SDL_Surface* getDisplay() {return m_pDisplay;}
    
    void initialize();
    void destroy();

    void testPrimitives();
    void testPrimitives2();
    void testPrimitives3();
    void testPrimitives4(); // set a picture (xpm)
    void testPrimitives5(); // timer
    void testPrimitives6(); // Schwabbeltest
    void testPrimitives7(); // timetest
    void testPrimitives5_maxspeed();
    void testPrimitives8();
};
// -----------------------------------------------------------------------------
void mySDL::initialize()
{
  if ( SDL_Init(SDL_INIT_VIDEO /* | SDL_INIT_AUDIO */ | SDL_INIT_TIMER ) < 0 )
    {
        t_print(T_DEBUG, "SDL konnte nicht initialisiert werden:  %s\n",
                SDL_GetError());
        exit(1);
    }
    
    //## if ( Sound_Init() == 0)
    //## {
    //##     t_print(T_DEBUG, "no sound:  %s\n", Sound_GetError());
    //##     exit(1);
    //## }

    SDL_EnableKeyRepeat(SDL_DEFAULT_REPEAT_DELAY, SDL_DEFAULT_REPEAT_INTERVAL);
    // SDL_EnableKeyRepeat(0 /* disabled */, SDL_DEFAULT_REPEAT_INTERVAL);

    // m_pImage = IMG_Load("mp3box.jpg");
    // 
    // if (m_pImage == NULL)
    // {
    //     t_print(T_DEBUG, "Das Bild konnte nicht geladen werden:%s\n",
    //             SDL_GetError());
    //     exit(-1);
    // }

    
    m_pDisplay = SDL_SetVideoMode( 800, 600, 16, SDL_SWSURFACE );
    // HWSURFACE | SDL_RLEACCEL | SDL_HWACCEL);
    // SDL_SWSURFACE );

    if ( m_pDisplay == NULL )
    {
        t_print(T_DEBUG, "Konnte kein Fenster 800x600px oeffnen: %s\n",
                SDL_GetError());
        exit(1);
    }

    if (TTF_Init() < 0)
    {
        t_print(T_DEBUG, "Konnte kein TTF initialisieren: %s\n",
                SDL_GetError());
        exit(1);
    }

    char cDirName[PATH_MAX];
    getcwd (cDirName, sizeof (cDirName));
    // t_print(T_DEBUG, "Current Directory: %s\n", cDirName);
    std::string sCurrentPath = cDirName;
    std::string sFontPath = sCurrentPath + "/" + "fonts/comic.ttf";
//    FontHeightTable aFontHeight(m_pDisplay, sFontPath);
    
    // m_pFont12 = new TTFFont(12);
    // m_pFont18 = new TTFFont(18);

    // boxColor(m_pDisplay, 0, 0, 799, 599, toUint32(Color::black) );

    // kopiere das Bild-Surface auf das m_pDisplay-surface
//    SDL_BlitSurface(m_pImage, NULL, m_pDisplay, NULL);
    
    // srect.x = 0;
    // srect.y = 0;
    // srect.w = (m_pImage->w)/2;  // das gesamte Bild
    // srect.h = m_pImage->h;   // das gesamte Bild


// den veraenderten Bereich des m_pDisplay-surface auffrischen
    SDL_Flip(m_pDisplay);
    SDL_ShowCursor(0);

//     SDL_Delay(3000);
}



void mySDL::destroy()
{
    // Das Bitmap-Surface l?schen
    if (m_pImage)
    {
        SDL_FreeSurface(m_pImage);
    }
    if (m_pDisplay)
    {
        SDL_FreeSurface(m_pDisplay);
    }
    
    // delete m_pFont18;
    // delete m_pFont12;

    TTF_Quit();
    //## Sound_Quit();
}

// -----------------------------------------------------------------------------

void ColorWheel_InnerWhite(SDL_Surface *_pDisplay, int _x, int _y, int _nRadius)
{
    int nMaxRadius = _nRadius;
    int nHalfRadius = (nMaxRadius / 2);
    
    HSV hsv;
    RGB rgb;
    for (int r=nMaxRadius;r>5;r-=1)
    {
        for(int i=0;i<180;i++)
        {
            long nH = linear(GRADZAHL * 360, 180, i);
            //           t_print("H:%d ", nH / GRADZAHL);
            hsv.h = nH;                       // 0-359

            if (r > nHalfRadius)
            { 
                int n = nHalfRadius - (r - nHalfRadius);
                if (n < 0) n = 0;

                hsv.v = linear(256 * 255, nHalfRadius, n); // 0-99
                hsv.s = 255 * 256;                // 0-99
            }
            else
            {
                hsv.v = 255 * 256;                // 0-99
                hsv.s = linear(256 * 255, nHalfRadius, r); // 0-99
            }
            
            changeHSV2RGB(&hsv, &rgb);
            
//             t_print("R:%d G:%d B:%d\n", rgb.r >> 8, rgb.g >> 8, rgb.b >> 8);
            filledPieColor(_pDisplay, _x, _y, r, i * 2, (i+2) * 2, toUint32(rgb.r >> 8, rgb.g >> 8, rgb.b >> 8));
        }
        SDL_Rect drect;
        clipRect(_x -r ,_y - r, _x + r, _y + r, drect);
        // clipRect(0,0,799,599, drect);
        SDL_UpdateRects(_pDisplay, 1, &drect);
    }
}

void ColorWheel_InnerBlack(SDL_Surface *_pDisplay, int _x, int _y, int _nRadius)
{
    int nMaxRadius = _nRadius;
    int nHalfRadius = (nMaxRadius / 2);
    
    HSV hsv;
    RGB rgb;
    int nOffset = 90 / 2;
    for (int r=nMaxRadius;r>5;r-=1)
    {
        for(int i=0;i<180;i++)
        {
            int n = (i + nOffset) % 180;
            long nH = linear(GRADZAHL * 360, 180, n);
            //           t_print("H:%d ", nH / GRADZAHL);
            hsv.h = nH;                       // 0-359

            if (r > nHalfRadius)
            {
                int n = nHalfRadius - (r - nHalfRadius);
                if (n < 0) n = 0;

                hsv.v = 255 * 256;                // 0-99
                hsv.s = linear(256 * 255, nHalfRadius, n); // 0-99
            }
            else
            {
                hsv.v = linear(256 * 255, nHalfRadius, r); // 0-99
                hsv.s = 255 * 256;                // 0-99
            }
            
            changeHSV2RGB(&hsv, &rgb);
            
//             t_print("R:%d G:%d B:%d\n", rgb.r >> 8, rgb.g >> 8, rgb.b >> 8);
            filledPieColor(_pDisplay, _x, _y, r, i * 2, (i+2) * 2, toUint32(rgb.r >> 8, rgb.g >> 8, rgb.b >> 8));
        }
        SDL_Rect drect;
        clipRect(_x -r ,_y - r, _x + r, _y + r, drect);
        SDL_UpdateRects(_pDisplay, 1, &drect);
    }
}

// -----------------------------------------------------------------------------
void mySDL::screenUpdate()
{
    SDL_Rect drect;
    clipRect(0,0,799,599, drect);
    SDL_UpdateRects(m_pDisplay, 1, &drect);
}

// -----------------------------------------------------------------------------
void mySDL::testPrimitives()
{
    // test the gfxPrimitives

    ColorWheel_InnerWhite(m_pDisplay, 600, 150, 149);
    screenUpdate();
    ColorWheel_InnerBlack(m_pDisplay, 600, 300 + 150, 149);
    screenUpdate();

    rectangleColor(m_pDisplay, 0,0,20,30,toUint32(Color::white));

    for (int i=1;i<71;i++)
    {
        aacircleColor(m_pDisplay, 80, 80, i, toUint32(Color::white));
    }
    screenUpdate();
}


// -----------------------------------------------------------------------------
void setAText(SDL_Surface *_pDisplay, std::string const& _sFontPath, int _y, std::string const& _sFontname)
{
    TTFFont *pFont2 = new TTFFont(22, true, _sFontPath);
//    m_pFont->setUpdate(false);
    
    pFont2->setMode(/*X_CENTER | */Y_CENTER);
    int w = 0;
    int x = 0;
    int h = 35;
    pFont2->setWidth(w);
    pFont2->setHeight(h);
        
    std::string sText = _sFontname + " abcdefghijklmNOPQRSTUVWXYZ 0123456789 +-*/ $@¤#_";
    SDL_Rect aRect = pFont2->setText(_pDisplay, Color::orange , x, _y, sText);
}
// -----------------------------------------------------------------------------
void mySDL::testPrimitives3()
{
    std::string sFonts[] = 
        {
            "/usr/local/handmade/media-box/fonts/Vera.ttf",
            "/usr/local/handmade/media-box/fonts/VeraBI.ttf",
            "/usr/local/handmade/media-box/fonts/VeraBd.ttf",
            "/usr/local/handmade/media-box/fonts/VeraIt.ttf",
            "/usr/local/handmade/media-box/fonts/VeraMoBI.ttf",
            "/usr/local/handmade/media-box/fonts/VeraMoBd.ttf",
            "/usr/local/handmade/media-box/fonts/VeraMoIt.ttf",
            "/usr/local/handmade/media-box/fonts/VeraMono.ttf",
            "/usr/local/handmade/media-box/fonts/VeraSe.ttf",
            "/usr/local/handmade/media-box/fonts/VeraSeBd.ttf",
            "",
            "/usr/X11R6/lib/X11/fonts/TTF/luximbi.ttf",
            "/usr/X11R6/lib/X11/fonts/TTF/luximr.ttf",
            "/usr/X11R6/lib/X11/fonts/TTF/luximri.ttf",
            "/usr/X11R6/lib/X11/fonts/TTF/luxirb.ttf",
            "/usr/X11R6/lib/X11/fonts/TTF/luxirbi.ttf",
            "/usr/X11R6/lib/X11/fonts/TTF/luxirr.ttf",
            "/usr/X11R6/lib/X11/fonts/TTF/luxirri.ttf",
            "/usr/X11R6/lib/X11/fonts/TTF/luxisb.ttf",
            "/usr/X11R6/lib/X11/fonts/TTF/luxisbi.ttf",
            "/usr/X11R6/lib/X11/fonts/TTF/luxisr.ttf",
            "/usr/X11R6/lib/X11/fonts/TTF/luxisri.ttf",
            "/usr/X11R6/lib/X11/fonts/truetype/andalemo.ttf",
"",
            "8514fix.fon",
            "8514oem.fon",
            "8514sys.fon",
            "app850.fon",
            "arialbd.ttf",
            "arialbi.ttf",
            "ariali.ttf",
            "arial.ttf",
            "ariblk.ttf",
            "comicbd.ttf",
            "comic.ttf",
            "courbd.ttf",
            "courbi.ttf",
            "coure.fon",
            "courf.fon",
            "couri.ttf",
            "cour.ttf",
            "desktop.ini",
            "impact.ttf",
            "lucon.ttf",
            "marlett.ttf",
            "modern.fon",
            "serife.fon",
            "seriff.fon",
            "smalle.fon",
            "smallf.fon",
            "sserife.fon",
            "sseriff.fon",
            "symbole.fon",
            "symbolf.fon",
            "symbol.ttf",
            "tahomabd.ttf",
            "tahoma.ttf",
            "timesbd.ttf",
            "timesbi.ttf",
            "timesi.ttf",
            "times.ttf",
            "verdanab.ttf",
            "verdanai.ttf",
            "verdana.ttf",
            "verdanaz.ttf",
            "vga850.fon",
            "vgafix.fon",
            "vgaoem.fon",
            "vgasys.fon",
            "webdings.ttf",
            "wingding.ttf",
            "0"
        };

    std::string sDir = "/dos/c/win98se/fonts/";
    int nYPos = 0;
    t_print("Anzahl fonts %d\n", sizeof(sFonts));
    int i = 0;
    while(1)
    {
        std::string sFontname = sFonts[i++];

        if (sFontname.length() == 1) break;      // CANCEL CONDITION

        std::string aFontName;
        if (sFontname.c_str()[0] != '/')
        {
            aFontName = sDir + sFontname;
        }
        else
        {
            aFontName = sFontname;
        }
        
        if (sFontname.length() > 1)
        {
            setAText(m_pDisplay, aFontName, nYPos, sFontname);
            nYPos += 30;
        }
        
        if (nYPos >= 570 or sFontname.length() == 0)
        {
            getFile();
            nYPos = 0;
            boxColor(m_pDisplay, 0,0,799,599, toUint32(Color::black));
            screenUpdate();
        }
    }
}

void mySDL::testPrimitives2()
{
    // test the gfxPrimitives

    rectangleColor(m_pDisplay, 0,0,20,30,toUint32(Color::white));

    // for (int i=1;i<71;i++)
    // {
    //     aacircleColor(m_pDisplay, 80, 80, i, toUint32(Color::white));
    // }
    screenUpdate();

    int x = 200;
    int y = 200;
    int w = 200;
    int h = 50;
    int nRadius = 10;
    
    filledRectangleWithRoundVertex(m_pDisplay, x, y, w, h, nRadius, toUint32(Color::grey40));

    TTFFont *pFont = new TTFFont(22);
//    m_pFont->setUpdate(false);

    pFont->setMode(X_CENTER | Y_CENTER);
    pFont->setWidth(w);
    pFont->setHeight(h);
    pFont->setFontType( FONT_TYPE_SHADOWED );
    /* SDL_Rect aRect = */ pFont->setText(m_pDisplay, Color::orange , x, y, "Dies ist text.");
    // rectangleColor(m_pDisplay, x - 1, y - 25, x + aRect.w,y - 25 + aRect.y ,toUint32(Color::white));

//    m_pFont->setUpdate(true);
    screenUpdate();

#define COLOR_NUMBER_4 4
    SDL_Rect aRect = mySetTextWithRectangle(m_pDisplay, x, y + 100, w, COLOR_NUMBER_4, Color::orange, "Dies ist 2. text");
    /* SDL_Rect */ aRect = mySetTextWithRectangle(m_pDisplay, x + 50 , y + 152, w, COLOR_NUMBER_4, Color::orange, "Dies ist 3. text");
    screenUpdate();
    
}
// -----------------------------------------------------------------------------

// /* XPM */
// static char * test_xpm[] = {
// "8 8 8 1",
// " 	c None",
// ".	c #FFFFFF",
// "+	c #C7C7C7",
// "@	c #555555",
// "#	c #393939",
// "$	c #727272",
// "%	c #8E8E8E",
// "&	c #AAAAAA",
// "        ",
// "        ",
// "        ",
// "    +@@#",
// "    +  $",
// "    +@@$",
// "    @  %",
// "    &##$"};

#include "test.xpm"

void mySDL::testPrimitives4()
{
    // test the gfxPrimitives

    // LLA: ok, works
    char **xpm;
    xpm = test_xpm;
    SDL_Surface *pSurface = IMG_ReadXPMFromArray(xpm);

    // LLA: ok also works
    // SDL_Surface *pSurface = IMG_Load("test.xpm");
    
    int x=100;
    int y=100;

    SDL_Rect drect        = {x, y, 0, 0};
    SDL_BlitSurface(pSurface, 0, m_pDisplay, &drect);
            
    SDL_UpdateRects(m_pDisplay, 1, &drect);
}


// -----------------------------------------------------------------------------
class TimerHelper
{
    bool bStopTimer;
    SDL_Surface *m_pDisplay;
public:
    SDL_TimerID nID;

    TimerHelper(SDL_Surface *_pDisplay)
            : bStopTimer(false),
              m_pDisplay(_pDisplay),
              nID(0)
        {}
    SDL_Surface* getDisplay() {return m_pDisplay;}
    void stopTimer() {bStopTimer = true;}
    bool isStopped() {return bStopTimer;}

    virtual void run() =0;
};

// -----------------------------------------------------------------------------
class SchwippSchwapp
{
protected:
    int m_nValue;
    int m_nValueAdd;
    int m_nMin;
    int m_nMax;
public:
    SchwippSchwapp(int _nStartValue, int _nStartCounter, int _nMin, int _nMax)
            :m_nValue(_nStartValue),
             m_nValueAdd(_nStartCounter),
             m_nMin(_nMin),
             m_nMax(_nMax)
        {}
    
    virtual void checkBounds()
        {
            if (m_nValue > m_nMax)
            {
                m_nValue = m_nMax;
                m_nValueAdd = -m_nValueAdd;
            }
            if (m_nValue < m_nMin)
            {
                m_nValue = m_nMin;
                m_nValueAdd = -m_nValueAdd;
            }
        }
    
    int getValue()
        {
            return m_nValue;
        }
    void setValue(int _nValue)
        {
            m_nValue = _nValue;
            checkBounds();
        }
    virtual int nextValue()
        {
            m_nValue += m_nValueAdd;
            checkBounds();
            return m_nValue;
        }
};

// -----------------------------------------------------------------------------

class SchwippSchwappSinus : public SchwippSchwapp
{
public:
    SchwippSchwappSinus(int _nStartValue, int _nStartCounter, int _nMin, int _nMax)
            :SchwippSchwapp(_nStartValue, _nStartCounter, _nMin, _nMax)
        {}
    
    virtual int nextValue()
        {
            int nMaxHalf = m_nMax / 2;
            int j = int(double ( sin( double( m_nValue ) * 2 * 3.141592653589 / m_nMax ) * nMaxHalf )) + nMaxHalf;
            m_nValue += m_nValueAdd;
            checkBounds();
            return j;
        }
    
};
class SchwippSchwappCosinus : public SchwippSchwapp
{
public:
    SchwippSchwappCosinus(int _nStartValue, int _nStartCounter, int _nMin, int _nMax)
            :SchwippSchwapp(_nStartValue, _nStartCounter, _nMin, _nMax)
        {}
    
    virtual int nextValue()
        {
            int nMaxHalf = m_nMax / 2;
            int j = int(double ( cos( double( m_nValue ) * 2 * 3.141592653589 / m_nMax ) * nMaxHalf )) + nMaxHalf;
            m_nValue += m_nValueAdd;
            checkBounds();
            return j;
        }
    
};

// -----------------------------------------------------------------------------
class TimerTest1 : public TimerHelper
{
public:
    
    SchwippSchwapp m_aSchwabbel;

    TimerTest1(SDL_Surface* _pDisplay)
            :TimerHelper(_pDisplay),
             m_aSchwabbel(1,1,0,255)
        {}
    
    virtual void run();
};
// -----------------------------------------------------------------------------
void TimerTest1::run()
{
// TODO: call this as a function in pHelper

    int nXPos = 20;
    int nYPos = 20;
    int nHeight = 300;
    int nWidth = 300;

    TimerTest1 *pHelper = this;

    int nAlphaColor = pHelper->m_aSchwabbel.nextValue();

    boxColor( pHelper->getDisplay(), 
              nXPos, nYPos,
              nXPos + nWidth, nYPos + nHeight,
              ColorToUint32(Color::red, nAlphaColor)  );

    nXPos += 100;
    nYPos += 100;
    
    boxColor( pHelper->getDisplay(), 
              nXPos, nYPos,
              nXPos + nWidth, nYPos + nHeight,
              ColorToUint32(Color::green, nAlphaColor)  );

    nXPos += 100;
    nYPos += 100;
    
    boxColor( pHelper->getDisplay(), 
              nXPos, nYPos,
              nXPos + nWidth, nYPos + nHeight,
              ColorToUint32(Color::blue, nAlphaColor)  );

//                 
//             // filledCircleColor(pHelper->display, 
//             //                   nXPos, nYPos,
//             //                   nWidth,
//             //                   intAsColortoUint32(nColor) 
//             //                   );
//             
    SDL_Rect drect;
    clipRect(20, 20, 520 + 1, 520 + 1, drect);
    SDL_UpdateRects(pHelper->getDisplay(), 1, &drect);
    
    t_print("%d\n", pHelper->m_aSchwabbel.getValue());
    fflush(stdout);
}
// -----------------------------------------------------------------------------
class ColorSchwabbel
{
public:
    SchwippSchwapp m_aSchwabbel_Red;
    SchwippSchwapp m_aSchwabbel_Green;
    SchwippSchwapp m_aSchwabbel_Blue;
    ColorSchwabbel(int _nRed, int _nGreen, int _nBlue)
            :m_aSchwabbel_Red  (_nRed, 1, 0, 255),
             m_aSchwabbel_Green(_nGreen, 2, 0, 255),
             m_aSchwabbel_Blue (_nBlue, 3, 0, 255)
        {
        }
    short getRed() {return m_aSchwabbel_Red.nextValue();}
    short getGreen() {return m_aSchwabbel_Green.nextValue();}
    short getBlue() {return m_aSchwabbel_Blue.nextValue();}    
};
// -----------------------------------------------------------------------------


class TimerTest2 : public TimerHelper
{
public:
    SchwippSchwappCosinus m_aSchwabbelX;
    SchwippSchwappSinus m_aSchwabbelY;

    SchwippSchwappSinus m_aSchwabbelW;
    SchwippSchwappCosinus m_aSchwabbelH;
    SchwippSchwapp m_aSchwabbelA;

    ColorSchwabbel m_aColorSchwabbel;

    TimerTest2(SDL_Surface * _pDisplay)
            :TimerHelper(_pDisplay),
             m_aSchwabbelX(1,3,1,800 - 20 - 100 /* 2*max_width/2 */ ),
             m_aSchwabbelY(1,2,1,600 - 20 - 100 /* 2*max_height/2 */ ),
             m_aSchwabbelW(1,1,1,90),
             m_aSchwabbelH(1,2,1,90),
             m_aSchwabbelA(1,1,1,64),
             m_aColorSchwabbel(255,0,0)
        {}
    
    virtual void run();
};
// -----------------------------------------------------------------------------
class TimerTest2_maxspeed:public TimerTest2
{
public:
    TimerTest2_maxspeed(SDL_Surface * _pDisplay)
            :TimerTest2(_pDisplay)
        {
        }
};
// -----------------------------------------------------------------------------
void TimerTest2::run()
{
    TimerTest2 *pHelper = this;

    int nAlphaColor = pHelper->m_aSchwabbelA.nextValue();

    int nWidth =  pHelper->m_aSchwabbelW.nextValue() + 9;
    int nHeight = pHelper->m_aSchwabbelH.nextValue() + 9;

    int nXPos = pHelper->m_aSchwabbelX.nextValue() + nWidth;
    int nYPos = pHelper->m_aSchwabbelY.nextValue() + nHeight;

    boxColor( pHelper->getDisplay(), 
              nXPos, nYPos,
              nXPos + nWidth, nYPos + nHeight,
              /* ColorToUint32(Color::red, nAlphaColor)*/
              /* ColorToUint32(Color::red, nAlphaColor)*/ 
              toUint32(m_aColorSchwabbel.getRed(), 
                       m_aColorSchwabbel.getGreen(), 
                       m_aColorSchwabbel.getBlue(), 
                       nAlphaColor)
              );

    //     t_print("x:%d y:%d w:%d h:%d a:%d\n", nXPos, nYPos, nWidth, nHeight, nAlphaColor);

    SDL_Rect drect;
    //     clipRect(nXPos, nYPos, 100, 100, drect);
    clipRect(nXPos, nYPos, 100, 100, drect);
    SDL_UpdateRects(pHelper->getDisplay(), 1, &drect);
    
    // t_print("%d\n", pHelper->m_aSchwabbelX.getValue());
    // fflush(stdout);
}
// -----------------------------------------------------------------------------
Uint32 TimerFkt(Uint32 _nInterval, void* param)
{
    if (param)
    {
        TimerHelper* pHelper = static_cast<TimerTest1*>(param);
        if (!pHelper)
        {
            return _nInterval;
        }
        
        if (pHelper->isStopped() == false)
        {
            pHelper->run();
        }
        else
        {
            t_print("\nTimer stopped and removed.\n");
            if (pHelper->nID != 0)
            {
                SDL_RemoveTimer(pHelper->nID);
                pHelper->nID = 0;
            }
        }
    }
    return _nInterval;
}

void mySDL::testPrimitives6()
{
    SchwippSchwappSinus m_aSchwabbel(1,1,1,90);
    for(int i=0;i<90;i++)
    {
        // int j = int(double ( sin( double( i ) * 3.141592653589 / 90.0 ) * 90.0));
        t_print("value:%d\n", m_aSchwabbel.nextValue());
    }
}

// -----------------------------------------------------------------------------
void mySDL::testPrimitives5()
{
    // test the gfxPrimitives

    // TimerHelper *pTimerHelper = new TimerHelper(m_pDisplay);
    TimerTest2 *pTimerTest = new TimerTest2(m_pDisplay);
    t_print("Start timer\n");
    pTimerTest->nID = SDL_AddTimer(50, TimerFkt, pTimerTest);

    getFile();

    pTimerTest->stopTimer();
    usleep(2000 * 1000);                         // 2 seconds!
    delete pTimerTest;
}

void mySDL::testPrimitives7()
{
    LLibrary::StopWatch sp;

    int x = 0;
    sp.start();
    for(int i=0;i<100;i++)
    {
        Uint32 nColor = ColorToUint32(Color::black, i * 2 );
        boxColor(m_pDisplay, x, 0, x + 7, m_pDisplay->h, nColor );
        x += 8;
        SDL_Rect drect        = {0, 0,m_pDisplay->w, m_pDisplay->h};
        mySDL_UpdateRects(m_pDisplay, 1, &drect);
        sp.stop();
        double sec = sp.getSec();
        printf("time: %f\n",sec);
    }
    usleep(5 * 1000000);
}

void mySDL::testPrimitives8()
{
    TTFFont *pFont = new TTFFont(22);
    int w = 800;
    int h = 600;

    pFont->setMode(X_CENTER | Y_CENTER);
    pFont->setWidth(w);
    pFont->setHeight(h);
    pFont->setFontType( FONT_TYPE_SHADOWED );
    pFont->setText(m_pDisplay, Color::orange , 0, 0, "X_CENTER | Y_CENTER");

    pFont->setText(m_pDisplay, Color::orange , 25, 25, "X_CENTER | Y_CENTER");

    pFont->setMode(X_LEFT);
    pFont->setText(m_pDisplay, Color::orange , 0, 0, "X_LEFT");

    pFont->setMode(X_LEFT);
    pFont->setText(m_pDisplay, Color::orange , 0, 30, "X_LEFT y:=30");

    pFont->setMode(X_LEFT | Y_CENTER);
    pFont->setText(m_pDisplay, Color::orange , 0, 0, "X_LEFT | Y_CENTER");

    pFont->setMode(X_RIGHT);
    pFont->setText(m_pDisplay, Color::orange , 0, 0, "X_RIGHT");

    pFont->setMode(X_RIGHT);
    pFont->setText(m_pDisplay, Color::orange , 0, 50, "X_RIGHT y:=50");

    pFont->setMode(X_RIGHT);
    pFont->setText(m_pDisplay, Color::orange , 20, 90, "X_RIGHT x:=20 y:=90");

    pFont->setMode(X_RIGHT);
    pFont->setText(m_pDisplay, Color::orange , -30, 130, "X_RIGHT x:=-30 y:=130");

    pFont->setMode(X_RIGHT | Y_CENTER);
    pFont->setText(m_pDisplay, Color::orange , 0, 0, "X_RIGHT | Y_CENTER");

    w = 350;
    h = 150;
    int x = 50;
    int y = 100;
    // boxColor(m_pDisplay, x, y, w, h, toUint32(Color::orange));
    rectangleColor(m_pDisplay, x,y,x + w,y + h,toUint32(Color::white));
    SDL_Rect drect;
    clipRect(x ,y, w + 1, h + 1, drect);
    SDL_UpdateRects(m_pDisplay, 1, &drect);

    pFont->setMode(X_CENTER | Y_CENTER);
    pFont->setWidth(w);
    pFont->setHeight(h);
    pFont->setFontType( FONT_TYPE_SHADOWED );
    pFont->setText(m_pDisplay, Color::white , x, y, "X_CENTER | Y_CENTER");

    pFont->setMode(X_LEFT);
    pFont->setText(m_pDisplay, Color::white , x, y, "X_LEFT");
}

// -----------------------------------------------------------------------------
bool shouldQuit()
{
    SDL_Event event;
    int nQuit = 0;
    SDL_PollEvent( &event );
    switch( event.type )
    {
    case SDL_QUIT:
        // nQuit = 1;
        break;
    case SDL_KEYDOWN:
        // HANDLE QUIT
        if (event.key.keysym.sym == SDLK_ESCAPE ||
            event.key.keysym.sym == SDLK_q || 
            event.key.keysym.sym == 'Q')
        {
            nQuit = 1;
            // nHandleEvents++;
        }
        // default:
    }
    if (nQuit == 1) return true;
    return false;        
}

// -----------------------------------------------------------------------------
void mySDL::testPrimitives5_maxspeed()
{
    // test the gfxPrimitives

    // TimerHelper *pTimerHelper = new TimerHelper(m_pDisplay);
    TimerTest2_maxspeed *pTimerTest = new TimerTest2_maxspeed(m_pDisplay);
    t_print("Start timer test without timer\n");
    // pTimerTest->nID = SDL_AddTimer(50, TimerFkt, pTimerTest);
    
    while (!shouldQuit())
    {
        pTimerTest->run();
    }
    
    t_print("Test done.\n");
    delete pTimerTest;
}

// -----------------------------------------------------------------------------
void typeidtest()
{
    if ( typeid(int) == typeid(long) )
    {
        t_print("int and long are equal.\n");
    }

    std::string aStr;
    if (typeid(aStr) == typeid(std::string))
    {
        t_print("a String.\n");
    }
    int a;
    if (typeid(a) == typeid(int))
    {
        t_print("a Integer (int).\n");
    }
}


/*
void test()
{
    std::ifstream iss("/tmp/firsttest", std::ios::in);
    std::string sType;
    std::vector<std::string> _aVector;

    void* pType = readObject(iss, sType);
    std::string *pt = static_cast<std::string*>(pType);
    _aVector.push_back(*pt);
    delete pt;                                            
}
*/

// in ioextend
// CreateFunc_writeVector( std::string );
// CreateFunc_readVector( std::string );

CreateFunc_writeVector( int );
CreateFunc_readVector( int );

void teststringvector()
{    
    if (1) {
        std::ofstream oss("/tmp/firsttest", std::ios::out);
        
        std::vector< std::string > aVector;
        aVector.push_back(std::string("first"));
        aVector.push_back(std::string("second"));
        aVector.push_back(std::string("third"));
        writeVector(oss, aVector);
    }

    if (1) {
        std::ifstream iss("/tmp/firsttest", std::ios::in);

        std::vector<std::string> aVector;
        int nCount = readVector(iss, aVector);
        if (nCount != 3)
        {
            std::cerr << "BUG:" << std::endl;
        }
        else
        {
            writeVector(std::cout, aVector);
        }
    }
}

void testintvector()
{    
    if (1) {
        std::ofstream oss("/tmp/secondtest", std::ios::out);
        
        std::vector< int > aVector;
        aVector.push_back(1);
        aVector.push_back(2);
        aVector.push_back(3);
        writeVector(oss, aVector);
    }

    if (1) {
        std::ifstream iss("/tmp/secondtest", std::ios::in);

        std::vector< int > aVector;
        int nCount = readVector(iss, aVector);
        if (nCount != 3)
        {
            std::cerr << "BUG:" << std::endl;
        }
        else
        {
            writeVector(std::cout, aVector);
        }
    }
}

void showListOnConsole(Audio::PlayList & _aList)
{
    int n = _aList.getSize();
    Audio::Entry aElement;
    for (int i=0;i<n;i++)
    {
        aElement = _aList.getElement(i);
        aElement.write(std::cout); //t_print("%s\n", aElement.c_str());
    }
}

void testPlayList()
{
    std::vector< Audio::Entry > aVector;
    aVector.push_back(Audio::Entry("", "first", "", 0));
    aVector.push_back(Audio::Entry("", "second", "", 0));
    aVector.push_back(Audio::Entry("", "third", "", 0));

    Audio::PlayList aList;
//     aList.setList(aVector);

    showListOnConsole(aList);
    aList.append(Audio::Entry("", "fourth", "", 0));
    showListOnConsole(aList);

    if (1) {
        std::ofstream oss("/tmp/thirdtest", std::ios::out);
        aList.write(oss);
    }
    
    std::ifstream iss("/tmp/thirdtest", std::ios::in);
    Audio::PlayList aList2;
    aList2.read(iss);
    if (1) {
        showListOnConsole(aList2);
    }
    
}

void testVector()
{
    std::vector< std::string > aVector;
    aVector.push_back("first");
    aVector.push_back("second");
    aVector.push_back("third");

    int n = aVector.size();
    for (int i = 0;i<n;i++)
    {
        std::string aElement = aVector[i];
        t_print("%s\n", aElement.c_str());
    }
    

}

std::string getLastDir(std::string const& _sString)
{
    // we want to go one directory back.
    std::string sNew;
    int nPos = _sString.rfind("/");
    if (nPos == 0)
    {
        sNew = "/";
    }
    else
    {
        sNew = _sString.substr(0, nPos);
    }
    // t_print("%s\n", m_sCurrentPath.c_str());
    return sNew;
}

void testDir()
{
    std::string sTest;
    sTest = getLastDir("/dir/name");
    sTest = getLastDir("/dir");
    sTest = getLastDir("/");
    sTest = getLastDir("");
    
}


// -----------------------------------------------------------------------------
void test_map()
{
    typedef std::map<std::string, std::string> MapStringString;
    MapStringString myMap;

    myMap["a"] = "1";
    myMap["b"] = "2";
    myMap["c"] = "3";

    myMap["a"] = "4";
    
    std::cout << myMap["a"] << std::endl;

    for (MapStringString::const_iterator it=myMap.begin();
         it != myMap.end();
         it++)
    {
        std::pair<std::string, std::string> aPair = *it;
        std::cout << "first: " << aPair.first << "  second: " << aPair.second << std::endl;
    }

    // std::map<std::string, std::string>::iterator aFound = myMap.find("d");
    if (myMap.find("d") == myMap.end())
    {
        std::cout << "Key 'd' not found." << std::endl;
    }
    else
    {
        std::cout << "Key 'd' found." << std::endl;
    }
    
    myMap["d"] = "12345";
    
    // std::map<std::string, std::string>::iterator aFound = myMap.find("d");
    MapStringString::const_iterator aFind = myMap.find("d");
        
    std::pair<std::string, std::string> aPair = *aFind;
    std::cout << "first: " << aPair.first << "  second: " << aPair.second << std::endl;
}

void test_controller()
{
    std::string aStr = "..";
    std::string sFind = "misc";

    // find returns -1 if the value isn't found in the string
    // int n = aStr.find(sFind);

    String aString(aStr);
    
    std::string aStr2 = "abcdefgh";
    String aString2(aStr2);

    if (aString.isInstanceOf(aString2))
    {
        std::cout << "Strings are same type" << std::endl;
    }
    else
    {
        std::cout << "ERROR" << std::endl;
    }
    
    int n1 = 10;
    Integer aInt(n1);
    Integer aInt2(n1);
    
    if (aInt.isInstanceOf(aInt2))
    {
        std::cout << "Integers are same type" << std::endl;
    }
    else
    {
        std::cout << "ERROR" << std::endl;
    }

    double n2 = 10;
    Double aDouble(n2);
    Double aDouble2(n2 + 1);
    Double aDouble3(aDouble);

    if (aDouble.isInstanceOf(aDouble2))
    {
        std::cout << "Doubles are same type" << std::endl;
    }
    else
    {
        std::cout << "ERROR" << std::endl;
    }

    if (! aInt.isInstanceOf(aDouble ))
    {
        std::cout << "Integers and Doubles are not the same type" << std::endl;
    }
    else
    {
        std::cout << "ERROR" << std::endl;
    }
    
    int n3 = aInt.intValue();
    if (n3 == n1)
    {
        std::cout << "Integer value is ok" << std::endl;
    }

    // LLA: here we lost the content of the double value.
    //      so we set the copy ctor and the assign operator of the SingleContainer to private.
    // SingleContainer aContainer;
    // aContainer = aDouble;

    SingleContainer *pContainer;
    pContainer = &aDouble;

    std::cout << "SingleContainer typename " << pContainer->getTypeName() << std::endl;

    Double *pDoubleNew = static_cast<Double*>(pContainer);
    
    std::cout << "aDouble Value " << aDouble.doubleValue() << " aDoubleNew Value: " << pDoubleNew->doubleValue() << std::endl;
    
}

// void testFill(mySDL const& _aSDL)
// {
// }

// -----------------------------------------------------------------------------
int main(int argc, char* argv[], char* env[])
{
//     test_controller();
//    test_map();
//     exit(0);
    // std::string sStr = "/space/local/Media/MpegVideo/Films/divx";
    // 
    // std::string sNew = sStr;
    // while (sNew.length() > 7)
    // {
    //     sNew = shortString(sNew);
    //     t_print("%s\n", sNew.c_str());
    // }    
    // exit(1);

    // store the environment variables
    environment = env;

    
    // testDir();
    // testVector();
    // typeidtest();

    // testPlayList();

//    exit(0);

    // if we are root, we want to run in real time
    int nUID = getuid();
    if (nUID == 0)
    {
        nice(-20);
    }

    mySDL aSDL;
    aSDL.initialize();

    int n = SDL_WasInit(0);
    if (n & SDL_INIT_TIMER)
    {
        t_print( T_DEBUG, "Timer initialized.\n");
    }
    if (n & SDL_INIT_AUDIO)
    {
        t_print( T_DEBUG, "Audio initialized.\n");
    }
    if (n & SDL_INIT_VIDEO)
    {
        t_print( T_DEBUG, "Video initialized.\n");
    }
    
    // while(1)
    // {
//    aSDL.testPrimitives2();
//     aSDL.testPrimitives4();
    // aSDL.testPrimitives7();
    aSDL.testPrimitives8();
//    aSDL.testPrimitives5_maxspeed();
//    aSDL.testPrimitives6();

//     aSDL.testPrimitives6();
//    aSDL.testPrimitives();
//    aSDL.testPrimitives3();
    // teststringvector();
    // testintvector();
    
    getFile();
        // }
    
    aSDL.destroy();
    atexit(SDL_Quit);
}
