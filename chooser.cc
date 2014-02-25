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

#include "i_dispatch.h"
#include "i_view.h"
#include "mvc_dispatcher.h"
#include "selection.h"
#include "controller.h"

// -----------------------------------------------------------------------------
char** environment;



/*
class Controller
{
    int m_nCurrentSelection;
    int m_nMaxSelection;
public:
    Controller()
            :m_nCurrentSelection(0),
             m_nMaxSelection(0)
        {}
    
    int getCurrentSelection() {return m_nCurrentSelection;}
    void setCurrentSelection(int _nSelection) {m_nCurrentSelection = _nSelection;}
    int getMaxSelection() {return m_nMaxSelection;}
    void setMaxSelection(int _nMaxSelection) {m_nMaxSelection = _nMaxSelection;}
    void select(int _nCurrentSelection)
        {
            m_nCurrentSelection = _nCurrentSelection;
        }    

};
*/

// -----------------------------------------------------------------------------
// Idea
struct Position
{
    int m_nX;
    int m_nY;
    Position(int _nX, int _nY)
            :m_nX(_nX),
             m_nY(_nY)
        {}
    
};

class Cursor
{
    Position m_nXY;
    void* m_pObject; // the current object, where the cursor is
public:
    Cursor()
            :m_nXY(0,0),
             m_pObject(NULL)
        {}
    // Cursor movement
    void up();
    void down();
    void left();
    void right();

    Position getPosition() {return m_nXY;}
    
};
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


// std::string getFile()
// {
//     std::string aBackStr;
//     
//     SDL_Event event;
//     // SDL_EventFilter aFilter = SDL_GetEventFilter();
// 
//     int quit = 0;
//     // Event-Handling
//     int nWaitInNanoSec = 100000000;
//     // int nHandleEvents = 3;
//     // int nDontHandleAnEvent = 0;
// 
//     int nWaitKeyEndprell = 0;
//     while( quit == 0)
//     {    
//         struct timespec delay;
//         delay.tv_sec = 0;
//         delay.tv_nsec = nWaitInNanoSec;
//         
//         nanosleep(&delay, 0); // tenth second
//         do
//         {
//             // bHandleEvent = false;
//             
//             switch( event.type )
//             {
//             case SDL_QUIT:
//                 quit = 1;
//                 break;
//             case SDL_KEYUP:
//                 nWaitKeyEndprell = 0;
//                 break;
//             case SDL_KEYDOWN:
//                 // t_print("Key:%d mod:%d \n", event.key.keysym.sym, event.key.keysym.mod);
//                 nWaitInNanoSec =  50000000;
//                 
//                 // t_print("event.key.state %d\n", event.key.state);
// 
//                 // HANDLE QUIT
//                 if (event.key.keysym.sym == SDLK_ESCAPE ||
//                     event.key.keysym.sym == SDLK_q || 
//                     event.key.keysym.sym == 'Q')
//                 {
//                     quit = 1;
//                     // nHandleEvents++;
//                 }
// 
//                 // LOUDNESS
//                 else if (event.key.keysym.mod == 1 && event.key.keysym.sym == SDLK_DOWN)
//                 {
//                     // shift down
//                     // t_print("leiser\n");
//                     // _aDispatcher.setNewEvent(EventValue(IMM_AUDIO_EVENT_LOUDNESS_LOWER));
//                 }
//                 else if (event.key.keysym.mod == 1 && event.key.keysym.sym == SDLK_UP)
//                 {
//                     // shift up
//                     // _aDispatcher.setNewEvent(EventValue(IMM_AUDIO_EVENT_LOUDNESS_HIGHER));
//                 }
// 
//                 else if (event.key.keysym.mod == 1 && event.key.keysym.sym == SDLK_LEFT)
//                 {
//                     // shift left
//                     // _aDispatcher.setNewEvent(EventValue(IMM_AUDIO_EVENT_JUMP_BACKWARD, 10.0));
//                 }
// 
//                 else if (event.key.keysym.mod == 1 && event.key.keysym.sym == SDLK_RIGHT)
//                 {
//                     // shift right
//                     // _aDispatcher.setNewEvent(EventValue(IMM_AUDIO_EVENT_JUMP_FORWARD, 10.0));
//                 }
// 
//                 // 
//                 else if (event.key.keysym.sym == SDLK_n ||
//                          event.key.keysym.sym == 'N')
//                 {
//                     // t_print("next\n");
//                     // _aDispatcher.setNewEvent(EventValue(IMM_AUDIO_EVENT_PLAY_NEXT));
//                 }
//                 else if (event.key.keysym.sym == SDLK_DOWN)
//                 {
//                     // _aView.up();
//                 }
//                 else if (event.key.keysym.sym == SDLK_UP)
//                 {
//                     // _aView.down();
//                 }
//                 else if (event.key.keysym.sym == SDLK_LEFT)
//                 {
//                     // _aView.changeDir("..", _aView.getCurrentPath());
//                 }
//                 else if (event.key.keysym.sym == SDLK_RIGHT)
//                 {
//                     // t_print("Key right\n");
//                     // std::string aStr = _aView.getFileName( _aView.getSelectionPos());
//                     // if (_aView.isDirectory(aStr) && aStr != "..")
//                     // {
//                     //     _aView.changeDir(aStr, "");
//                     // }
//                 }
//                 else if (event.key.keysym.sym == SDLK_c)
//                 {
//                     t_print("Clear\n");
//                     // _aDispatcher.setNewEvent(EventValue(IMM_AUDIO_EVENT_CLEAR_LIST));
//                 }
//                 
//                 else if (event.key.keysym.sym == SDLK_RETURN)
//                 {
//                     // std::string aStr = _aView.getFileName( _aView.getSelectionPos());
//                     // if (_aView.isDirectory(aStr))
//                     // {
//                     //     std::string sLastDirName;
//                     //     if (aStr == "..")
//                     //     {
//                     //         sLastDirName = _aView.getCurrentPath();
//                     //     }                        
//                     //     _aView.changeDir(aStr, sLastDirName);
//                     // }
//                     // else
//                     // {
//                     //     aBackStr = _aView.getCurrentPath() + "/" + aStr;
//                     //     quit = 1;
//                     // }
//                     quit = 1;
//                 }
//                 else if (event.key.keysym.sym == 'S' ||
//                          event.key.keysym.sym == SDLK_s)
//                 {
//                     // t_print("stop\n");
//                     // _aDispatcher.setNewEvent(EventValue(IMM_AUDIO_EVENT_STOP));
//                 }
//                 nWaitKeyEndprell++;
//                 break;
//             default:
//                 break;
//                 nWaitInNanoSec = 100000000;
//                 nWaitKeyEndprell = 0;
//             }
//             // _aView.paintValues();
//             // _aView.update();
// 
//             // _aDispatcher.handleAnEvent();
// 
//             // t_print("%d\n", nWaitKeyEndprell);
//             if (nWaitKeyEndprell == 1)
//             {
//                 int nWait = 6;
//                 while (nWait--)
//                 {
//                     // t_print("Entprell...\n");
//                     struct timespec delay;
//                     delay.tv_sec = 0;
//                     delay.tv_nsec = nWaitInNanoSec;
//                     
//                     nanosleep(&delay, 0); // tenth second
//                     SDL_PollEvent( &event );
//                     if (event.type == SDL_KEYUP)
//                     {
//                         // t_print("SDL_KEYUP get.\n");
//                         nWaitKeyEndprell = 0;
//                         break;
//                     }
//                 }
//             }
//         }
//         while( SDL_PollEvent( &event ) );
//     }
//     return aBackStr;
// }

// -----------------------------------------------------------------------------
class mySDL : public interface_Dispatch
{
    SDL_Surface *m_pImage;
    SDL_Surface *m_pDisplay;
    TTFFont     *m_pFont;

    void screenUpdate();
public:
    mySDL()
            :m_pImage(NULL),
             m_pDisplay(NULL),
             m_pFont(NULL)
        {
        }

    virtual ~mySDL()
        {}
    
    SDL_Surface* getDisplay() {return m_pDisplay;}
    
    void initialize();
    void destroy();

    // void testPrimitives4();

    // void setInfo(std::string const &_sText);
    // void show();
    
    // void prepareToQuit();
    bool dispatch(SDL_Event const& _aEvent);

    // void paintValues(int _nSeletion);
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
    char* egal = getcwd (cDirName, sizeof (cDirName));
    // t_print(T_DEBUG, "Current Directory: %s\n", cDirName);
    std::string sCurrentPath = cDirName;
    std::string sFontPath = sCurrentPath + "/" + "fonts/comic.ttf";

    FontHeightTable aFontHeight(m_pDisplay, sFontPath);
    
    // m_pFont12 = new TTFFont(12);

    boxColor(m_pDisplay, 0, 0, 799, 599, toUint32(Color::black) );

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
// -----------------------------------------------------------------------------


bool mySDL::dispatch(SDL_Event const& _aEvent)
{
    t_print(T_FLOWCONTROL, "mySDL::dispatch\n");
    bool bReturn = false;
    
    switch( _aEvent.type )
    {
    case SDL_KEYDOWN:
        // printf("Key:%d mod:%d \n", event.key.keysym.sym, event.key.keysym.mod);
        // HANDLE QUIT
        if (_aEvent.key.keysym.sym == SDLK_ESCAPE ||
            _aEvent.key.keysym.sym == SDLK_q || 
            _aEvent.key.keysym.sym == 'Q')
        {
            // prepareToQuit();
            SDL_Event* pQuitEvent = static_cast<SDL_Event*>(malloc(sizeof (SDL_QuitEvent)));
            if (pQuitEvent)
            {
                t_print(T_FLOWCONTROL, "KEY: quit\n");
                pQuitEvent->type = SDL_QUIT;
                SDL_PushEvent(pQuitEvent);
            }
            else
            {
                t_print(T_ERROR, "can't get memory for Quit Event\n");
                // exit(1);
            }
            
            bReturn = true;
        }
        break;
        // default:
        // bReturn = false;
    }
    return bReturn;
}

// void mySDL::prepareToQuit()
// {
// }

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

// #include "test.xpm"

// void mySDL::testPrimitives4()
// {
//     // test the gfxPrimitives
// 
//     // LLA: ok, works
//     char **xpm;
//     xpm = test_xpm;
//     SDL_Surface *pSurface = IMG_ReadXPMFromArray(xpm);
// 
//     // LLA: ok also works
//     // SDL_Surface *pSurface = IMG_Load("test.xpm");
//     
//     int x=100;
//     int y=100;
// 
//     SDL_Rect drect        = {x, y, 0, 0};
//     SDL_BlitSurface(pSurface, 0, m_pDisplay, &drect);
//             
//     SDL_UpdateRects(m_pDisplay, 1, &drect);
// }
// -----------------------------------------------------------------------------

// void mySDL::setInfo(std::string const &_sText)
// {
//     int x = INFO_XPOS;
//     int y = INFO_YPOS;
//     // clearArtist();
//     // m_pFont->setText(m_pDisplay, Color::orange , x + 2, y + 4, _sText);
//     SetTextWithRectangle a(m_pDisplay, x, y, INFO_WIDTH, COLOR_NUMBER_5, Color::orange, _sText);
//     a.setHeight(42);
//     a.setRadius(11);
//     m_pFont->setMode( Y_CENTER);
//     m_pFont->setFontType(FONT_TYPE_BOLD | FONT_TYPE_OUTLINE);
//     a.setFont(m_pFont);
//     SDL_Rect drect = a.paint();
//     SDL_UpdateRects(m_pDisplay, 1, &drect);
// }
// 
// void mySDL::show()
// {
//     setInfo("test1");
// }




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

#define INFO_XPOS 15
#define INFO_YPOS 15
#define INFO_HEIGHT (37 + 2)

#define INFO_WIDTH (800 - 2 * INFO_XPOS - 50)

#define COLOR_NUMBER_5 5
#define COLOR_NUMBER_3 3

    

// -----------------------------------------------------------------------------
class Viewer : public interface_View
{
    // SDL_Surface *m_pDisplay;
    Controller *m_pController;
    // TTFFont *m_pFont;
    int m_nXWidth;
    int m_nYHeight;
    int m_nCaption;

    void paintValues(SDL_Surface* _pSurface, int _nSelection);
    
    Viewer(Viewer const&);

public:
    
    Viewer()
            :m_pController(NULL)
        {
            // m_pFont = new TTFFont(32);
        }
    
    void setSize(int _nXWidth, int _nYHeight, int _nCaption = 5)
        {
            m_nXWidth = _nXWidth;
            m_nYHeight = _nYHeight;
            m_nCaption = _nCaption;
        }
    
    void setController(Controller* _pController) {m_pController = _pController;}
    
    bool paint(SDL_Surface* _pSurface);
};

bool Viewer::paint(SDL_Surface* _pSurface)
{
    // int nSelection = 0;
    if (m_pController)
    {
        int nSelection = m_pController->getPropertyAsIntValue(SELECTION_CURRENT_SELECTION);
        paintValues(_pSurface, nSelection);
        return true;
    }
    return false;
}

void Viewer::paintValues(SDL_Surface* _pSurface, int _nSeletion)
{
    t_print("draw selection: %d\n", _nSeletion);
    
    int x = (800 - m_nXWidth) / 2;
    // int nYHeight = 16;
    int yadd = m_nYHeight + m_nCaption;
    int y = (600 - (m_pController->getPropertyAsIntValue(SELECTION_MAX_SELECTION) * yadd)) / 2;
    int ny = y;
    for (int i=0;i<m_pController->getPropertyAsIntValue(SELECTION_MAX_SELECTION);i++)
    {
        if (i == _nSeletion)
        {
            rectangleColor(_pSurface, x, ny, x + m_nXWidth, ny + m_nYHeight, toUint32(Color::white) );
        }
        else
        {
            rectangleColor(_pSurface, x, ny, x + m_nXWidth, ny + m_nYHeight, toUint32(Color::grey40) );
        }
        
        ny += yadd;
    }
    SDL_Rect drect = {x, y, m_nXWidth + 1, m_pController->getPropertyAsIntValue(SELECTION_MAX_SELECTION) * yadd};
    SDL_UpdateRects(_pSurface, 1, &drect);
}




// -----------------------------------------------------------------------------
int main(int argc, char* argv[], char* env[])
{
    // store the environment variables
    environment = env;
    
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
    
    // don't remove this if, it is for a new scope, to handle right, 
    // the descruction of object elements.
    if (1)
    {
        Controller aController;
            
        Selection aSelection(10);
        aSelection.setController(&aController);
        
        Viewer aViewer;
        aViewer.setSize(600, 32, 4);
        aViewer.setController(&aController);
        
        MVC_Dispatcher mvc;
        mvc.setDisplay(aSDL.getDisplay());

        mvc.addToDispatch(&aSDL);
        mvc.addToDispatch(&aSelection);
        
        mvc.addToView(&aViewer);
        // aViewer.paint( aSDL.getDisplay() );
        // first we must paint all views for initialisation
        mvc.paintAllViews(aSDL.getDisplay());

        mvc.loop();
    }
    
    // aSDL.show();
    
    aSDL.destroy();
    atexit(SDL_Quit);
}
