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

#include "i_dispatch.h"

#include "playaudio.h"
// #include "dispatcher.h"
#include "scrollviewer.h"
#include "stringhelper.h"
#include "ttffont.h"
#include "somecolors.h"
#include "directory.h"
#include "information.h"
#include "t_print.h"
#include "filehelper.h"
#include "ioextend.h"
#include "ColorHelper.h"
#include "rectangleWithRoundVertex.h"
#include "cliprect.h"
#include "viewdirectory.h"
#include "mvc_dispatcher.h"
#include "controller.h"
#include "listfilter.h"
#include "property.h"
#include "global.h"
#include "quality.h"
#include "profile.h"
#include "updaterect.h"
#include "pathmax.h"

// -----------------------------------------------------------------------------
char** environment;

// -----------------------------------------------------------------------------
#define ADD_TO_LIST 1
#define REPLACE_ALL_WITH_CURRENT_DIR 2
#define PLAY_NEXT 3


// struct FileElement
// {
//     Audio::Entry aEntry;
//     SDL_Event    aLastEvent;
// 
//     FileElement(){}
//     
//     void setEntry(Audio::Entry const& _aEntry){aEntry = _aEntry;}
//     void setEvent(SDL_Event const& _aEvent){aLastEvent = _aEvent;}
// };

// -----------------------------------------------------------------------------

// no longer need FileElement getFile(ViewDirectory & _aView, Audio::PlayAudio & _aPlayer )
// no longer need {
// no longer need     // _aPlayer.showList();
// no longer need 
// no longer need     _aView.paintValues();
// no longer need     _aView.update();
// no longer need 
// no longer need     FileElement aBackEntry;
// no longer need     
// no longer need     SDL_Event event;
// no longer need     event.type = SDL_NOEVENT;
// no longer need 
// no longer need     // SDL_EventFilter aFilter = SDL_GetEventFilter();
// no longer need 
// no longer need     int quit = 0;
// no longer need     // Event-Handling
// no longer need     int nWaitInNanoSec = 100000000;
// no longer need 
// no longer need     // remove all open events
// no longer need     // while( SDL_PollEvent( &event ) == 1);
// no longer need 
// no longer need     int nWaitKeyEndprell = 0;
// no longer need     while( quit == 0)
// no longer need     {
// no longer need         struct timespec delay;
// no longer need         delay.tv_sec = 0;
// no longer need         delay.tv_nsec = nWaitInNanoSec;
// no longer need         
// no longer need         nanosleep(&delay, 0); // tenth second
// no longer need         do
// no longer need         {
// no longer need             aBackEntry.setEvent(event);
// no longer need             switch( event.type )
// no longer need             {
// no longer need             case SDL_QUIT:
// no longer need                 //                quit = 1;
// no longer need                 break;
// no longer need             case SDL_KEYUP:
// no longer need                 nWaitKeyEndprell = 0;
// no longer need                 break;
// no longer need             case SDL_KEYDOWN:
// no longer need                 // t_print("Key:%d mod:%d \n", event.key.keysym.sym, event.key.keysym.mod);
// no longer need                 nWaitInNanoSec =  30000000;
// no longer need                 
// no longer need                 // t_print("event.key.state %d\n", event.key.state);
// no longer need 
// no longer need                 // HANDLE QUIT
// no longer need // mySDL                if (event.key.keysym.sym == SDLK_ESCAPE)
// no longer need // mySDL                {
// no longer need // mySDL                    quit = 1;
// no longer need // mySDL                }
// no longer need                 // toggle Play List
// no longer need // SimpleController                if (event.key.keysym.sym == SDLK_F1)
// no longer need // SimpleController                {
// no longer need // SimpleController                    quit = 1;
// no longer need // SimpleController                }
// no longer need // SimpleController                else if (event.key.keysym.sym == SDLK_F2)
// no longer need // SimpleController                {
// no longer need // SimpleController                    quit = 1;
// no longer need // SimpleController                }
// no longer need // SimpleController                else if (event.key.keysym.sym == SDLK_F3)
// no longer need // SimpleController                {
// no longer need // SimpleController                    quit = 1;
// no longer need // SimpleController                }
// no longer need 
// no longer need // PlayAudio                // LOUDNESS
// no longer need // PlayAudio                else if (event.key.keysym.mod == 1 && event.key.keysym.sym == SDLK_DOWN)
// no longer need // PlayAudio                {
// no longer need // PlayAudio                    // shift down
// no longer need // PlayAudio                    // t_print("leiser\n");
// no longer need // PlayAudio                    _aPlayer.insertNewEvent(EventContainer(IMM_AUDIO_EVENT_LOUDNESS_LOWER));
// no longer need // PlayAudio                }
// no longer need // PlayAudio                else if (event.key.keysym.mod == 1 && event.key.keysym.sym == SDLK_UP)
// no longer need // PlayAudio                {
// no longer need // PlayAudio                    // shift up
// no longer need // PlayAudio                    _aPlayer.insertNewEvent(EventContainer(IMM_AUDIO_EVENT_LOUDNESS_HIGHER));
// no longer need // PlayAudio                }
// no longer need // PlayAudio
// no longer need // PlayAudio                // else if (event.key.keysym.mod == 1 && event.key.keysym.sym == SDLK_LEFT)
// no longer need // PlayAudio                else if (event.key.keysym.sym == SDLK_PAGEDOWN)
// no longer need // PlayAudio                {
// no longer need // PlayAudio                    // shift left
// no longer need // PlayAudio                    _aPlayer.insertNewEvent(EventContainer(IMM_AUDIO_EVENT_JUMP_BACKWARD, 10.0));
// no longer need // PlayAudio                }
// no longer need // PlayAudio
// no longer need // PlayAudio                // else if (event.key.keysym.mod == 1 && event.key.keysym.sym == SDLK_RIGHT)
// no longer need // PlayAudio                else if (event.key.keysym.sym == SDLK_PAGEUP)
// no longer need // PlayAudio                {
// no longer need // PlayAudio                    // shift right
// no longer need // PlayAudio                    _aPlayer.insertNewEvent(EventContainer(IMM_AUDIO_EVENT_JUMP_FORWARD, 10.0));
// no longer need // PlayAudio                }
// no longer need // PlayAudio                
// no longer need // PlayAudio
// no longer need // PlayAudio                else if (event.key.keysym.sym == SDLK_SPACE)
// no longer need // PlayAudio                {
// no longer need // PlayAudio                    // shift left
// no longer need // PlayAudio                    _aPlayer.insertNewEvent(EventContainer(IMM_AUDIO_EVENT_TOGGLE_PAUSE));
// no longer need // PlayAudio                }
// no longer need // PlayAudio                else if (event.key.keysym.sym == SDLK_HOME)
// no longer need // PlayAudio                {
// no longer need // PlayAudio                    // shift left
// no longer need // PlayAudio                    _aPlayer.insertNewEvent(EventContainer(IMM_AUDIO_EVENT_JUMP_BACKWARD, AUDIO_START));
// no longer need // PlayAudio                }
// no longer need // PlayAudio
// no longer need // PlayAudio                // else if (event.key.keysym.mod == 1 && event.key.keysym.sym == SDLK_RIGHT)
// no longer need // PlayAudio                else if (event.key.keysym.sym == SDLK_END)
// no longer need // PlayAudio                {
// no longer need // PlayAudio                    // shift right
// no longer need // PlayAudio                    _aPlayer.insertNewEvent(EventContainer(IMM_AUDIO_EVENT_JUMP_FORWARD, AUDIO_END));
// no longer need // PlayAudio                }
// no longer need // PlayAudio
// no longer need // PlayAudio                // 
// no longer need // PlayAudio                else if (event.key.keysym.sym == SDLK_PLUS)
// no longer need // PlayAudio                {
// no longer need // PlayAudio                    t_print("next\n");
// no longer need // PlayAudio                    _aPlayer.insertNewEvent(EventContainer(IMM_AUDIO_EVENT_PLAY_NEXT));
// no longer need // PlayAudio                }
// no longer need // PlayAudio
// no longer need // PlayAudio                else if (event.key.keysym.sym == SDLK_DELETE)
// no longer need // PlayAudio                {
// no longer need // PlayAudio                    t_print("Clear\n");
// no longer need // PlayAudio                    _aPlayer.insertNewEvent(EventContainer(IMM_AUDIO_EVENT_CLEAR_LIST));
// no longer need // PlayAudio                    quit = 1;
// no longer need // PlayAudio                }
// no longer need                 
// no longer need                 // RETURN or KEY 'A'
// no longer need // event.key.keysym.mod == 1 && 
// no longer need // event.key.keysym.sym == SDLK_a
// no longer need 
// no longer need                 else if (event.key.keysym.sym == SDLK_RETURN)
// no longer need                 {
// no longer need                     Audio::Entry aEntry = _aView.getPlayList()->getElement( _aView.getSelectionPos());
// no longer need                     std::string aStr = aEntry.getFilename();
// no longer need                     if (aEntry.getSystemType() == ENTRY_ISDIRECTORY)
// no longer need                     {
// no longer need                         if (event.key.keysym.mod == 0)     // MUST NOT KEY 'shift'
// no longer need                         {
// no longer need                             std::string sLastDirName;
// no longer need                             if (aStr == "..")
// no longer need                             {
// no longer need                                 sLastDirName = aEntry.getPath();
// no longer need                                 sLastDirName = DirectoryHelper::removeLastDirEntry(sLastDirName);
// no longer need                                 _aView.changeDir(sLastDirName, aEntry.getPath());
// no longer need                             }
// no longer need                             else
// no longer need                             {
// no longer need                                 aStr = FileHelper::getAbsoluteFile(aEntry);
// no longer need                                 t_print("%s\n", aStr.c_str());
// no longer need                                 _aView.changeDir(aStr, "");
// no longer need                                 _aView.up();
// no longer need                             }
// no longer need                         }
// no longer need                     }
// no longer need                     else
// no longer need                     {
// no longer need                         aBackEntry.setEntry( aEntry );
// no longer need                         quit = 1;
// no longer need                     }
// no longer need                     // jump automatically to next entry
// no longer need                     // _aView.up();
// no longer need                 }
// no longer need // PlayAudio                else if ( event.key.keysym.sym == SDLK_F12 )
// no longer need // PlayAudio                {
// no longer need // PlayAudio                    t_print("stop\n");
// no longer need // PlayAudio                    _aPlayer.insertNewEvent(EventContainer(IMM_AUDIO_EVENT_STOP));
// no longer need // PlayAudio                }
// no longer need //                 else
// no longer need //                 {
// no longer need //                     if ( _aView.dispatcher(event) == 0)
// no longer need //                     {
// no longer need //                         t_print("Key not handled : Key: %c Key: %d Mod: %d\n", event.key.keysym.sym, event.key.keysym.sym, event.key.keysym.mod);
// no longer need //                     }
// no longer need //                 }
// no longer need                 
// no longer need                 nWaitKeyEndprell++;
// no longer need                 break;
// no longer need             default:
// no longer need                 break;
// no longer need                 nWaitInNanoSec = 100000000;
// no longer need                 nWaitKeyEndprell = 0;
// no longer need             }
// no longer need             _aView.paintValues();
// no longer need             _aView.update();
// no longer need 
// no longer need //            t_print(".");
// no longer need //            fflush(stdout);
// no longer need 
// no longer need //            _aPlayer.handleAnEvent();
// no longer need 
// no longer need             // t_print("%d\n", nWaitKeyEndprell);
// no longer need 
// no longer need // zum entprellen der Tastatur
// no longer need //             if (nWaitKeyEndprell == 1)
// no longer need //             {
// no longer need //                 t_print("KeyEndprell\n");
// no longer need //                 int nWait = 6;
// no longer need //                 while (nWait--)
// no longer need //                 {
// no longer need //                     // t_print("Entprell...\n");
// no longer need //                     struct timespec delay;
// no longer need //                     delay.tv_sec = 0;
// no longer need //                     delay.tv_nsec = nWaitInNanoSec;
// no longer need //                     
// no longer need //                     nanosleep(&delay, 0); // tenth second
// no longer need //                     SDL_PollEvent( &event );
// no longer need //                     if (event.type == SDL_KEYUP)
// no longer need //                     {
// no longer need //                         // t_print("SDL_KEYUP get.\n");
// no longer need //                         nWaitKeyEndprell = 0;
// no longer need //                         break;
// no longer need //                     }
// no longer need //                 }
// no longer need //             }
// no longer need             if (quit == 1) break;
// no longer need         }
// no longer need         while( SDL_WaitEvent( &event ) );
// no longer need         
// no longer need         // _aPlayer.handleAnEvent();
// no longer need     }
// no longer need     return aBackEntry;
// no longer need }

// -----------------------------------------------------------------------------

class mySDL : public interface_Dispatch
{
    SDL_Surface *m_pImage;
    SDL_Surface *m_pDisplay;

    TTFFont *m_pFont12;
    TTFFont *m_pFont14;
    TTFFont *m_pFont18;
    TTFFont *m_pFont22;
private:
    
    mySDL(mySDL const&);
    mySDL& operator=(mySDL const&);
public:
    mySDL()
            :m_pImage(NULL),
             m_pDisplay(NULL),
             m_pFont12(NULL),
             m_pFont14(NULL),
             m_pFont18(NULL),
             m_pFont22(NULL)
        {
        }

    virtual ~mySDL()
        {}
    
    SDL_Surface* getDisplay() {return m_pDisplay;}
    TTFFont* getFont12() {return m_pFont12;}
    TTFFont* getFont14() {return m_pFont14;}
    TTFFont* getFont18() {return m_pFont18;}
    TTFFont* getFont22() {return m_pFont22;}
    
    void initialize();
    void destroy();

    void screenUpdate();

    // interface Dispatch
    virtual bool dispatch(SDL_Event const& _aEvent);
};
// -----------------------------------------------------------------------------
void mySDL::initialize()
{
    if ( SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_TIMER ) < 0 )
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

//     m_pImage = IMG_Load("/home/langha_l/Work/sdl-musicplay/mp3box.jpg");
//     
//     if (m_pImage == NULL)
//     {
//         t_print(T_DEBUG, "Das Bild konnte nicht geladen werden:%s\n",
//                 SDL_GetError());
//         exit(-1);
//     }

    

    m_pDisplay = SDL_SetVideoMode( 800, 600, 32, SDL_HWSURFACE | SDL_RLEACCEL | SDL_HWACCEL);


                                   /* SDL_HWSURFACE | SDL_RLEACCEL | SDL_HWACCEL */
                                   /* SDL_DOUBLEBUF | SDL_HWSURFACE | SDL_FULLSCREEN | SDL_RLEACCEL | SDL_HWPALETTE */
    
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
//    FontHeightTable aFontHeight(m_pDisplay);
    std::string aCurrentDir(get_current_dir_name());
    std::string sFontsDir("/home/langha_l/Work/sdl-musicplay/fonts/comic.ttf");
    m_pFont12 = new TTFFont(12, true, sFontsDir);
    m_pFont14 = new TTFFont(14, true, sFontsDir);
    m_pFont18 = new TTFFont(22, true, sFontsDir);
    m_pFont22 = new TTFFont(26, true, sFontsDir);

    boxColor(m_pDisplay, 0, 0, 799, 599, ColorToUint32(Color::black) );

    // kopiere das Bild-Surface auf das m_pDisplay-surface
    // SDL_BlitSurface(m_pImage, NULL, m_pDisplay, NULL);
    
    // srect.x = 0;
    // srect.y = 0;
    // srect.w = (m_pImage->w)/2;  // das gesamte Bild
    // srect.h = m_pImage->h;   // das gesamte Bild


// den veraenderten Bereich des m_pDisplay-surface auffrischen
    SDL_Flip(m_pDisplay);
    SDL_ShowCursor(0);

//     SDL_Delay(3000);
    

    // SDL_Rect srect = {0,0,100,100};

    // SDL_Event event;
    // SDL_EventFilter aFilter = SDL_GetEventFilter();
    
    // test the gfxPrimitives

    // rectangleColor(m_pDisplay, 0,0,20,30,ColorToUint32(Color::white));
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
    
    delete m_pFont22;
    delete m_pFont18;
    delete m_pFont14;
    delete m_pFont12;

    TTF_Quit();
    //## Sound_Quit();

}

void mySDL::screenUpdate()
{
    SDL_Rect drect;
    clipRect(0,0,800,600, drect);
    mySDL_UpdateRects(m_pDisplay, 1, &drect);
}

bool mySDL::dispatch(SDL_Event const& _aEvent)
{
    t_print(T_FLOWCONTROL,"mySDL::dispatch\n");
    bool bReturn = false;
    
    switch( _aEvent.type )
    {
    case SDL_USEREVENT:
        Global::documentKey(SDLK_ESCAPE, 0, "Leave the program.");
        break;

    case SDL_KEYDOWN:
        // t_print("Key:%d mod:%d \n", event.key.keysym.sym, event.key.keysym.mod);
        // HANDLE QUIT
        if (_aEvent.key.keysym.sym == SDLK_ESCAPE)
/* ||
            _aEvent.key.keysym.sym == SDLK_q || 
            _aEvent.key.keysym.sym == 'Q')
*/
        {
            // prepareToQuit();
            output( "QUIT\n" );
            SDL_Event* pQuitEvent = static_cast<SDL_Event*>(malloc(sizeof (SDL_QuitEvent)));
            if (pQuitEvent)
            {
                t_print(T_FLOWCONTROL, "KEY: quit\n");
                pQuitEvent->type = SDL_QUIT;
                SDL_PushEvent(pQuitEvent);
            }
            else
            {
                t_print("can't get memory for Quit Event\n");
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

// -----------------------------------------------------------------------------
class SimpleController : public interface_Dispatch, public ControllerHelper
{
    std::string m_sLastToggleDirectory;

    ViewDirectory *m_pViewDirectory;
    Audio::PlayAudio *m_pAudioPlayer;
    Audio::PlayList *m_pPlayList;

    int m_nLastPositionInDirectory;
    int m_nLastPositionInSimpleList;

    void handleKey_Return(SDLMod shift);
    void handleKey_TAB();
    // void handleKey_F1();
    void handleKey_F10();
    void handleKey_F11();
    void handleKey_F12();

    void createQuitEvent();
private:
    SimpleController(SimpleController const&);
    SimpleController& operator=(SimpleController const&);
public:
    SimpleController()
            :m_sLastToggleDirectory(),
             m_pViewDirectory(NULL),
             m_pAudioPlayer(NULL),
             m_pPlayList(NULL),
             m_nLastPositionInDirectory(0),
             m_nLastPositionInSimpleList(0)
        {}

    virtual ~SimpleController() {}
    
    void setViewDirectory(ViewDirectory* _pViewDirectory)
        {
            m_pViewDirectory = _pViewDirectory;
        }
    void setAudioPlayer(Audio::PlayAudio* _pPlayer)
        {
            m_pAudioPlayer = _pPlayer;
        }
    void setAudioPlayList(Audio::PlayList* _pPlayList)
        {
            m_pPlayList = _pPlayList;
        }
    
    // handle Key Events
    virtual bool dispatch(SDL_Event const& _aEvent);
};

void SimpleController::createQuitEvent()
{
    SDL_Event* pQuitEvent = static_cast<SDL_Event*>(malloc(sizeof (SDL_QuitEvent)));
    if (pQuitEvent)
    {
        t_print(T_DEBUG, "Created quit event.\n");
        pQuitEvent->type = SDL_QUIT;
        SDL_PushEvent(pQuitEvent);
    }
    else
    {
        t_print(T_DEBUG, "can't get memory for Quit Event\n");
        // exit(1);
    }
}

/*
 * Function is called if return is pressed.
 */
void SimpleController::handleKey_Return(SDLMod _aModifier)
{
    Audio::Entry aEntry = m_pViewDirectory->getCurrentSelectionValue(); //
    std::string sPath = aEntry.getPath();
    std::string sSoundFile = FileHelper::getAbsoluteFile(aEntry);
    
    if (aEntry.getSystemType() == ENTRY_ISDIRECTORY)
    {
        if (aEntry.getFilename() == "..")
        {
            std::string sLastDirName = sPath;
            sLastDirName = DirectoryHelper::removeLastDirEntry(sLastDirName);
            m_pViewDirectory->changeDir(sLastDirName, aEntry.getPath());
        }
        else
        {
            std::string aStr = sSoundFile;
            t_print(T_INFO, "chdir to %s\n", aStr.c_str());
            m_pViewDirectory->changeDir(aStr, "");
            // we don't want to stay on '..'
            m_pViewDirectory->up();
        }
    }
    else
    {
        // Entry is hopefully a file
        if (sSoundFile.size() > 0)
        {
            const char *pDirName = sSoundFile.c_str();
            if (FileHelper::isVideo(sSoundFile))
            {
                // is a video file
                if (m_pViewDirectory->getType() == VIEW_TYPE_DIRECTORY)
                {
                    output("LAST_DIR %s\n", sPath.c_str());
                    output("LAST_CURSOR_POS %d\n", m_pViewDirectory->getCurrentSelectionPos());
                }

                // play video, this works only with the new starter.pl tool
                // due to the fact, I can't start mplayer in a separate thread
                // because I don't know how. And starting mplayer with system()
                // gives me no change to handle it's key words right.
                // so I use a simple perl script ;-)
            
                // handle shift key
                int nShiftModifier = _aModifier & (KMOD_LSHIFT | KMOD_RSHIFT);
                if (nShiftModifier != 0)
                {
                    output("CONT_VIDEO %s\n", pDirName);
                    createQuitEvent();
                }
                else
                {
                    output("VIDEO %s\n", pDirName);
                    createQuitEvent();
                }
                     
                // Video
                // aPlayer.destroy();
                // sleep(1);
                // start_mplayer(pDirName);
                // handleMPlayer();
//              break;
                // t_print("ERROR: video is not supported at the moment.");
                // aPlayer.initialize();
            }
            else
            {            
                // it is a audio file
                t_print(T_DEBUG, "play as next file: %s\n", sSoundFile.c_str());
                if (m_pViewDirectory->getType() == VIEW_TYPE_DIRECTORY)
                {
                    m_pPlayList->append(aEntry);
                    m_pPlayList->showListOnConsole();
                    //? start play, if not already play
                    m_pAudioPlayer->insertNewEvent(EventContainer(IMM_AUDIO_EVENT_PLAY));
                }
                else
                {
                    int nNewPos = m_pViewDirectory->getCurrentSelectionPos();
                    t_print(T_DEBUG, "Set new cursor position to play %d\n", nNewPos);
//                    m_pPlayList->setCursorPos( nNewPos );
                    m_pAudioPlayer->insertNewEvent(EventContainer(IMM_AUDIO_EVENT_PLAY_NEXT, nNewPos));
                }
                m_pViewDirectory->up();
            }
             
            // replace the current list by all files in the current directory
            // if (nCurrent == REPLACE_ALL_WITH_CURRENT_DIR)
            // {
            //     StringList aPlayFiles;
            //     Audio::PlayList * pFiles = aDir.getList();
            //     
            //     aPlayList.clear();
            //     // bool bInsertNow = false;
            //     int n = pFiles->getSize();
            //     for (int i=0;i<n;i++)
            //     {
            //         Audio::Entry aEntry = pFiles->getElement(i);
            //         if (aEntry.getSystemType() == ENTRY_ISFILE)
            //         {
            //             // std::string sCurrentFile;
            //             // sCurrentFile = aDir.getCurrentPath() + "/" + aEntry.getFilename();
            //             
            //             // if (sCurrentFile == sSoundFile)
            //             // {
            //             //     bInsertNow = true;
            //             // }
            //             // if (bInsertNow == true)
            //             // {
            //             // aPlayFiles.push_back(sSoundFile);
            //             aPlayList.append(aEntry);
            //         }
            //             // aPlayer.insertNewEvent(AUDIO_EVENT_NEXT_FILE);
            //         // }
            //     }
            //     aPlayList.showList();
            //     aPlayer.insertNewEvent(EventContainer(IMM_AUDIO_EVENT_STOP));
            //     aPlayer.insertNewEvent(EventContainer(IMM_AUDIO_EVENT_PLAY));
            //     
            //     // Audio::Stop();
            //     // Audio::Play(sSoundFile);
            //     // Audio::Play(aPlayFiles);
            // }
        }    
    }
}

// /*
//  * Function is called if F1 is pressed
//  */
// void SimpleController::handleKey_F1()
// {
// }

/*
 * Function is called if F10 is pressed
 */
void SimpleController::handleKey_F10()
{
    Audio::Entry aEntry = m_pViewDirectory->getCurrentSelectionValue(); //
    std::string sPath = aEntry.getPath();
    
    if (sPath.size() > 0)
    {
        t_print(T_DEBUG, "replace play list by current directory entries: %s\n", sPath.c_str());
        
        // replace the current list by all files in the current directory
        if (m_pViewDirectory->getType() == VIEW_TYPE_DIRECTORY)
        {
            StringHelper::StringList aPlayFiles;
            t_print(T_INFO, "create recursive directory tree, %s\n", sPath.c_str());
            Audio::PlayList * pFiles = DirectoryHelper::fillArrayRecursive(sPath);
            
            Audio::PlayListIterator aIterator(pFiles, 0, "");
            m_pPlayList->clear();

            int n = pFiles->getSize();
            for (int i=0;i<n;i++)
            {
                m_pPlayList->lock();
                // Audio::Entry aEntry = pFiles->getElement(i);
                Audio::Entry aEntry = aIterator.getCurrentElement();
                if (aEntry.getSystemType() == ENTRY_ISFILE)
                {
                    m_pPlayList->append(aEntry);
                }
                m_pPlayList->unlock();
                aIterator.next();
            }
            m_pPlayList->showListOnConsole();

            m_pAudioPlayer->LockInsertEvent();
            m_pAudioPlayer->insertNewEventNoLock(EventContainer(IMM_AUDIO_EVENT_STOP));
            m_pAudioPlayer->insertNewEventNoLock(EventContainer(IMM_AUDIO_EVENT_PLAY_FROM_BEGINNING_OF_LIST));
            m_pAudioPlayer->UnLockInsertEvent();
        }
    }
}

/*
 * Function is called if F11 is pressed
 */
void SimpleController::handleKey_F11()
{
    Audio::Entry aEntry = m_pViewDirectory->getCurrentSelectionValue(); //
    std::string sPath = aEntry.getPath();
    
    if (sPath.size() > 0)
    {
        t_print(T_DEBUG, "replace play list by current directory entries: %s\n", sPath.c_str());
        
        // replace the current list by all files in the current directory
        if (m_pViewDirectory->getType() == VIEW_TYPE_DIRECTORY)
        {
            StringHelper::StringList aPlayFiles;
            Audio::PlayList * pFiles = m_pViewDirectory->getPlayList();
            
            m_pPlayList->clear();
            int n = pFiles->getSize();
            for (int i=0;i<n;i++)
            {
                m_pPlayList->lock();
                Audio::Entry aEntry = pFiles->getElement(i);
                if (aEntry.getSystemType() == ENTRY_ISFILE)
                {
                    m_pPlayList->append(aEntry);
                }
                m_pPlayList->unlock();
            }
            m_pPlayList->showListOnConsole();
            m_pAudioPlayer->LockInsertEvent();
            m_pAudioPlayer->insertNewEventNoLock(EventContainer(IMM_AUDIO_EVENT_STOP));
            m_pAudioPlayer->insertNewEventNoLock(EventContainer(IMM_AUDIO_EVENT_PLAY));                  
            m_pAudioPlayer->UnLockInsertEvent();
        }
    }
}

void SimpleController::handleKey_F12()
{
    Audio::Entry aEntry = m_pViewDirectory->getCurrentSelectionValue();
    std::string sPath = aEntry.getPath();
    std::string sSoundFile = FileHelper::getAbsoluteFile(aEntry);
 
    if (sSoundFile.size() > 0)
    {
        t_print(T_DEBUG, "add to play list: %s\n", sSoundFile.c_str());
        if (m_pViewDirectory->getType() == VIEW_TYPE_DIRECTORY)
        {
            // we can only append entries, if the directory structure is shown.
            m_pPlayList->append(aEntry);
            m_pPlayList->showListOnConsole();
            m_pViewDirectory->up();
        }
    }
}

void SimpleController::handleKey_TAB()
{
    if (m_pViewDirectory->getType() == VIEW_TYPE_DIRECTORY)
    {
        // switch to simple list
        Audio::Entry aEntry = m_pViewDirectory->getCurrentSelectionValue(); //
        std::string sPath = aEntry.getPath();

        // remember last directory and last cursor position
        m_sLastToggleDirectory = sPath;
        m_nLastPositionInDirectory = m_pViewDirectory->getCurrentSelectionPos();
                 
        m_pViewDirectory->setType(VIEW_TYPE_SIMPLELIST);
        m_pViewDirectory->setList(m_pPlayList, false);

        t_print(T_DEBUG, "set cursor to: %d\n", m_nLastPositionInSimpleList);
        m_pViewDirectory->setCursorPos(m_nLastPositionInSimpleList);
        if (hasAController())
        {
            getController()->setPropertyValue(P_INFO_NEXTPLAY, new String("    single play list"));
        }
    }
    else
    {
        // switch to directory view
        m_nLastPositionInSimpleList = m_pViewDirectory->getCurrentSelectionPos();
        m_pViewDirectory->setType(VIEW_TYPE_DIRECTORY);
        m_pViewDirectory->changeDir(m_sLastToggleDirectory, "");
        t_print(T_DEBUG, "set cursor to: %d\n", m_nLastPositionInDirectory);
        m_pViewDirectory->setCursorPos(m_nLastPositionInDirectory);
    }
}

bool SimpleController::dispatch(SDL_Event const& _aEvent)
{
    t_print(T_FLOWCONTROL, "SimpleController::dispatch");
    bool bReturn = false;
    
    switch( _aEvent.type )
    {
    case SDL_USEREVENT:
        Global::documentKey(SDLK_TAB,0 , "Toggle playlist and directory list.");
        // Global::documentKey(SDLK_F2, "");
        // Global::documentKey(SDLK_F3, "");
        // Global::documentKey(SDLK_F12, "");
        // Global::documentKey(SDLK_F11, "");
        Global::documentKey(SDLK_F10, 0, "Insert from current directory recursivly the playlist.");
        Global::documentKey(SDLK_RETURN, 0, "Insert a new file into the playlist or choose a file from the playlist.");
        break;

    case SDL_KEYDOWN:
        // t_print("Key:%d mod:%d \n", event.key.keysym.sym, event.key.keysym.mod);
        if (_aEvent.key.keysym.sym == SDLK_TAB)
        {
            t_print(T_FLOWCONTROL, "KEY: tab\n");
            t_print(T_INFO, "Toggle playlist and directory list.\n");
            handleKey_TAB();
            bReturn = true;
        }
        // else if (_aEvent.key.keysym.sym == SDLK_F1)
        // {
        //     t_print(T_FLOWCONTROL, "KEY: F1\n");
        //     t_print(T_INFO, "Print internal help.\n");
        //     handleKey_F1();
        //     bReturn = true;
        // }
        else if (_aEvent.key.keysym.sym == SDLK_F2)
        {
            t_print(T_FLOWCONTROL, "KEY: F2\n");
//             chdir(aCurrentDir.c_str());
//             // store
//             t_print(T_DEBUG, "Store playlist to file .myplaylist.\n");
//             std::ofstream oss(".myplaylist", std::ios::out);
//             aPlayList.write(oss);
            bReturn = true;
        }
        else if (_aEvent.key.keysym.sym == SDLK_F3)
        {
            t_print(T_FLOWCONTROL, "KEY: F3\n");
            t_print(T_INFO, "Load playlist to file .myplaylist.\n");
//             chdir(aCurrentDir.c_str());
//             // load
//             std::ifstream iss(".myplaylist", std::ios::in);
//             if (iss)
//             {
//                 aPlayList.lock();
//                 aPlayList.clear();
//                 aPlayList.read(iss);
//                 aPlayList.unlock();
//             }
            bReturn = true;
        }
        
        else if (_aEvent.key.keysym.sym == SDLK_F12)
        {
            t_print(T_FLOWCONTROL, "KEY: F12\n");
            handleKey_F12();
            bReturn = true;
        }
        else if (_aEvent.key.keysym.sym == SDLK_F11)
        {
            t_print(T_FLOWCONTROL, "KEY: F11\n");
            handleKey_F11();
            bReturn = true;
        }
        else if (_aEvent.key.keysym.sym == SDLK_F10)
        {
            t_print(T_FLOWCONTROL, "KEY: F10\n");
            handleKey_F10();
            bReturn = true;
        }
        
        else if (_aEvent.key.keysym.sym == SDLK_RETURN)
        {
            t_print(T_FLOWCONTROL, "KEY: return\n");
            handleKey_Return( _aEvent.key.keysym.mod);
//             // howto handle shift key
//             if (_aEvent.key.keysym.mod == 1)
//             {
//                 // KEY S-return add the current file to list
//                 nCurrent = ADD_TO_LIST;
//             }
//             else
//             {
//                 nCurrent = PLAY_NEXT;
//             }
            bReturn = true;
        }
        
        break;
    }
    return bReturn;
}

// void testDirectory()
// {
//     std::string sPath("/space/local/MpegAudio");
//     Audio::PlayList * pFiles = DirectoryHelper::fillArrayRecursive(sPath);
//     t_print("Size of Directory %d\n", pFiles->getSize());
//     exit(1);
// }

// -----------------------------------------------------------------------------
int main(int argc, char* argv[], char* env[])
{
    // testDirectory();
    // testProfile();

    char cDirName[PATH_MAX];
    char* getcwdresult = getcwd (cDirName, sizeof (cDirName));
    // t_print(T_DEBUG, "Current Directory: %s\n", cDirName);
    std::string sCurrentPath = cDirName;

    // store the environment variables
    environment = env;
    bool bSaveToStorage = false;
    std::string sPath;
    int i = 1;
    int nCursorPos = 0;
    while (i < argc)
    {
        // path variable
        if (strcmp(argv[i], "-p") == 0)
        {
            sPath = argv[i + 1];
            ++i;
        }
        // cursor position
        else if (strcmp(argv[i], "-c") == 0)
        {
            std::string sCursorPos = argv[i + 1];
            nCursorPos = strtol(sCursorPos.c_str(), NULL, 0);
            ++i;
        }
        // use storage
        else if (strcmp(argv[i], "-s") == 0)
        {
            bSaveToStorage = true;
        }
        else if (strcmp(argv[i], "-d") == 0)
        {
            aGlobalStructure.setBit(Bit_Flag_DEBUG);
            t_print(T_DEBUG, "debug mode.\n");
        }
        else if (strcmp(argv[i], "-r") == 0)
        {
            aGlobalStructure.setBit(Bit_Flag_RECTANGLE);
            t_print(T_DEBUG, "rectangle around symbols.\n");
        }
        else if (strcmp(argv[i], "-v") == 0)
        {
            aGlobalStructure.setBit(Bit_Flag_VERBOSE);
            t_print(T_VERBOSE, "verbose mode.\n");
        }
        else if (strcmp(argv[i], "-q") == 0)
        {
            // halts maul, no output all the time
            aGlobalStructure.setBit(Bit_Flag_QUIET);
        }
        else
        {
            t_print("UNKNOWN Parameter: %s\n", argv[i]);
        }
        
        ++i;
    }
    t_print(T_DEBUG, "test.\n");
    
    // if we are root, we want to run in real time
    int nUID = getuid();
    if (nUID == 0)
    {
        int errorcode = nice(-20);
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
    
    // man 3 get_current_dir_name
    std::string aCurrentDir(get_current_dir_name());
    
    if (1)
    {    
        std::string aStrDir = "/space/local/MpegAudio";
        std::string aStorage = aCurrentDir;
        aStorage += "/.storage.dat";

        // set path via parameter
        if (sPath.length() > 0)
        {
            aStrDir = sPath;
        }
        
        // DirectoryHelper aDir(  );

        Information   aInformation( aSDL.getFont18(), aSDL.getFont14() );
        // test design
        aInformation.setInfo("Welcome to my Video&Audio Player");
        aInformation.setArtist("");
        aInformation.setTitle("");
        aInformation.setAlbum("");
        aInformation.setYear("");
        aInformation.setCurrentDirectory("");
        
        Audio::PlayAudio aAudioPlayer;
        aAudioPlayer.initialize();
        
        Storage aStorageHelper(aStorage);

        DirectoryHelper aDirHelper;
        // aDirHelper.getListener()->addListener(&aInformation);

        Audio::PlayList aPlayList;
        aAudioPlayer.setPlayList(&aPlayList);

        ViewDirectory aViewDirectory( aSDL.getFont18(), &aDirHelper);
        // aView.setList(aDir.getList());
        aViewDirectory.setType(VIEW_TYPE_DIRECTORY);
        aViewDirectory.changeDir(aStrDir.c_str(), "");
        aViewDirectory.setCursorPos(nCursorPos);

        Controller aController;                               // used for property changes
        aAudioPlayer.setController(&aController);
        aViewDirectory.setController(&aController);
        
        // aAudioPlayer.setStorage(aStorageHelper, bSaveToStorage);
        // aAudioPlayer.tryToLoadFromStorage();

        SimpleController aSimpleController;
        aSimpleController.setViewDirectory(&aViewDirectory);
        aSimpleController.setAudioPlayer(&aAudioPlayer);
        aSimpleController.setAudioPlayList(&aPlayList);
        aSimpleController.setController(&aController);

        ListFilter aFilter;
        aFilter.setController(&aController);

        MVC_Dispatcher mvc;                                   // Model View Controll Helper
        mvc.setDisplay( aSDL.getDisplay() );
        mvc.setController(&aController);

        std::string aQualityProfile = aCurrentDir;
        aQualityProfile += "/.quality.ini";
        Quality aQuality(aQualityProfile);
        aQuality.setController(&aController);

        // here are the property resolver
        mvc.addToHandleController(&aInformation);
        mvc.addToHandleController(&aPlayList);
        mvc.addToHandleController(&aViewDirectory);
        mvc.addToHandleController(&aQuality);

        aViewDirectory.setQuality(&aQuality);

        // all functions which get paint() events
        mvc.addToView(&aInformation);                         // all paint() routines are registered here
        mvc.addToView(&aViewDirectory);
        
        // all event dispatcher
        mvc.addToDispatch(&aSDL);                             // all dispatch(Event) routines are registered here
        mvc.addToDispatch(&aFilter);
        mvc.addToDispatch(&aAudioPlayer);
        mvc.addToDispatch(&aViewDirectory);
        mvc.addToDispatch(&aSimpleController);
        mvc.addToDispatch(&aQuality);

        // to see something, call all registered paint() methods
        mvc.paintAllViews(aSDL.getDisplay());                 // paint all

        mvc.testLoop();

        // endless loop
        mvc.loop();
        
        // Audio::Stop();
        aAudioPlayer.destroy();
    }
    
    int errorcode = chdir(sCurrentPath.c_str());
    aSDL.destroy();
    atexit(SDL_Quit);
}
