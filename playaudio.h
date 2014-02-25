/*-*-c++-*-
 *
 *    $RCSfile: playaudio.h,v $
 *     Project:
 * Description:
 *
 *   Author(s): Lars Langhans
 *       $Date: 2006/01/25 17:55:18 $
 *   Copyright: (c)1996 by Lars Langhans
 *   $Revision: 1.25 $
 */


#ifndef _playaudio_h
#define _playaudio_h

#include <vector>
#include <string>
#include <SDL_sound.h>

#include "i_dispatch.h"
#include "dispatcher.h"
#include "storage.hxx"
#include "controller.h"

#include "playlist.h"

namespace Audio
{

#define SAMPLESIZE 512 /* 3*1024 */
#define BUFFSIZE   2048 /* 2048 is tested, this is the value which comes from the callback */ /* 44100*2*2/10 */ /* 1024 */

#define AUDIO_START -1
#define AUDIO_END   -2

#define AUDIO_EVENT_START               0
// #define AUDIO_EVENT_NEXT_FILE 1
#define IMM_AUDIO_EVENT_STOP            2
#define IMM_AUDIO_EVENT_PLAY_NEXT       3
#define IMM_AUDIO_EVENT_PLAY            4

#define IMM_AUDIO_EVENT_LOUDNESS_LOWER  5
#define IMM_AUDIO_EVENT_LOUDNESS_HIGHER 6

#define IMM_AUDIO_EVENT_STOP_FAST       7
#define IMM_AUDIO_EVENT_PLAY_NEXT_FAST  8

#define IMM_AUDIO_EVENT_CLEAR_LIST      9

#define IMM_AUDIO_EVENT_JUMP_FORWARD    10
#define IMM_AUDIO_EVENT_JUMP_BACKWARD   11

#define IMM_AUDIO_EVENT_TOGGLE_PAUSE    12

#define IMM_AUDIO_EVENT_PLAY_FROM_BEGINNING_OF_LIST 13
#define IMM_AUDIO_EVENT_PLAY_PREV       14

    class TimerHelper;
    // ------------------------------------------------------------
    class PlayAudio : public Dispatcher, public interface_Dispatch, public ControllerHelper
    {
        Audio::PlayList *m_pPlayList;                // NOT OWNER!!!!
        Audio::PlayListIterator *m_pPlayListIterator; // NOT OWNER!!!!

        TimerHelper *m_pTimerHelper;
    public:
        SDL_mutex *m_pLockTimerHelper;
    private:
        
        std::string             m_sCurrentFilename;
        // std::queue<std::string> m_aCurrentList;
        std::string              m_sCurrentPlayListName;

        // Sound_AudioInfo         m_aInfo;
        
        Storage m_aStorage;
        bool m_bSaveToStorage;

        bool m_bDestroyed;
        bool m_bPlay;
        bool m_bTogglePause; // I love this fucking behaviour, what is the meaning of true?
                             // true: pause

        void play(std::string const& _sFileToPlay);
        void start();
        void stop(bool);
        void pause();
        void toggle_pause();

        void seekForward(double _nJumpSecondsForward);
        void seekBackward(double _nJumpSecondsBackward);
        int setVolumn(int _nVolumn);
        
        void initFile(std::string const& _sSoundFile);
        std::string getCurrentFile();
        std::string peekNextFile();

        virtual void showEvent(int _nCurrentEvent);

        // Information m_aInformation;

        PlayAudio(PlayAudio const&);
        PlayAudio& operator=(PlayAudio const&);
    public:
        PlayAudio();
        virtual ~PlayAudio();
        void setPlayList(Audio::PlayList *_pList);
        
        void initialize();
        void destroy();

        // void dispatcher();
        // void handleSoundFade();
        virtual void handleAnEvent();

//         void addToList(Audio::Entry const& _sEntry);
//         void emptyList();
//        void setInfo(Information const& _aInfo);
//         void showList();

//        Audio::Entry getElement( int _nIndex );

        // Direct access to PlayList
        // LLA: direct access forbidden, it's no longer a real member
        // Audio::PlayList& getPlayList() {return m_aPlayList;}
        // void setStorage(Storage const& _aStorage, bool _bSaveToStorage);
        // void tryToLoadFromStorage();

        void initTimer();
        void destroyTimer();

        // interface Dispatch
        virtual bool dispatch(SDL_Event const& _aEvent);
    };

    class TimerHelper
    {
    private:
        TimerHelper(TimerHelper const&);
        TimerHelper& operator=(TimerHelper const&);
    public:
        SDL_TimerID m_nID;
        bool m_bStopTimer;
        PlayAudio *m_pPlayAudio;
        
        TimerHelper(PlayAudio *_pPlayAudio)
                :m_nID(0),
                 m_bStopTimer(false),
                 m_pPlayAudio(_pPlayAudio)
            {}
    };

}

#endif


// Local Variables:
// mode:C++
// tab-width:4
// End:
