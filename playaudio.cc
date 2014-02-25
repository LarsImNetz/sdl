#include <string>
#include <fstream>

#include <assert.h>

#include <SDL_sound.h>
#include <SDL_mutex.h>
#include <SDL_mixer.h>

#include "playaudio.h"
#include "storage.hxx"
#include "t_print.h"
#include "directory.h"
#include "property.h"
#include "global.h"

namespace Audio
{
// -----------------------------------------------------------------------------
    
    bool bGlobal_CurrentPlayAudio = false;
    int nGlobal_Volumn = 64; /* SDL_MIX_MAXVOLUME is 128 */
    
    int nBytesPerSecond = 0; // 44100 * 2 * 2 

    Mix_Music *pMusic = NULL;

    int    m_nCount = 0;
    // double m_nCurrentPlayPosition = 0.0;

    PlayAudio *m_pAudioPlayer = NULL;
    SDL_mutex *m_pCountLock = NULL;

    void initCount()
    {
        m_pCountLock = SDL_CreateMutex();
    }
    void destroyCount()
    {
        SDL_DestroyMutex(m_pCountLock);
    }
    
    // increase the current play position, this is safe via SDL_mutex
    void addCountPosition(int _nAddValue)
    {
        assert(m_pCountLock != 0);
        SDL_mutexP(m_pCountLock);
        m_nCount += _nAddValue;
        if (m_nCount < 0)
        {
            m_nCount = 0;
        }
        SDL_mutexV(m_pCountLock);
    }
    // set the current play position, this is safe via SDL_mutex
    void setCountPosition(int _nValue)
    {
        assert(m_pCountLock != 0);
        SDL_mutexP(m_pCountLock);
        if (_nValue < 0)
        {
            _nValue = 0;
        }
        m_nCount = _nValue;
        SDL_mutexV(m_pCountLock);
    }
    
    // get the current play position, this is safe via SDL_mutex
    int getCountPosition()
    {
        int nValue;
        assert(m_pCountLock != 0);
        SDL_mutexP(m_pCountLock);
        nValue = m_nCount;
        SDL_mutexV(m_pCountLock);
        return nValue;
    }

    double getCurrentPlayPosition()
    {
        return double(getCountPosition()) / nBytesPerSecond /* (44100 * 2 * 2) */;
    }
    
    void MixCallback(void *udata, Uint8 *stream, int len)
    {
        addCountPosition(len);
        // t_print("%d\n", len);
        
        // double nCurrentPlayPosition = getCurrentPlayPosition();

        // t_print("%.1f\n", nCurrentPlayPosition);
        // fflush(stdout);
    }

    // this callback is called after music play is done, so play next music file
    void EndCallback()
    {
        t_print(T_AUDIOPLAYCONTROL, "PlayAudio::EndCallback()\n");
        if (m_pAudioPlayer)
        {
#warning read the current comments
            // here we should get the next file, which we wann'a play
            // and give the name to the event
            m_pAudioPlayer->insertNewEvent(EventContainer(IMM_AUDIO_EVENT_PLAY_NEXT_FAST));
            Mix_SetPostMix(NULL, NULL);       // callback at playtime
        }
    }


    // ------------------------------------------------------------
    PlayAudio::PlayAudio()
            : m_pPlayList(NULL),
              m_pPlayListIterator(NULL),
              m_pTimerHelper(NULL),
              m_pLockTimerHelper(NULL),
              m_sCurrentFilename(),
              m_sCurrentPlayListName(),
              m_aStorage(),
              m_bSaveToStorage(false),
              m_bDestroyed(false),
              /* m_bPlay(false), */
              m_bTogglePause(false)
    {
        // we need a global pointer out of the class to ourself for the callback functions
        m_pAudioPlayer = this;
    }

    void PlayAudio::initialize()
    {
        t_print(T_AUDIOPLAYCONTROL, "PlayAudio::initialize()\n");
        initCount();

        // --------------------------------- Audio init ---------------------------------

        if (Mix_OpenAudio(44100, AUDIO_S16, 2, 4096) < 0)
        {
            t_print(T_DEBUG, "Mix: Unable to open audio: %s\n",
                    SDL_GetError());
            return;
        }
        else
        {
            int audio_rate;
            Uint16 audio_format;
            int audio_channels;

            Mix_QuerySpec(&audio_rate, &audio_format, &audio_channels);
            t_print( T_INFO, "Opened audio at %d Hz %d bit %s", audio_rate,
                   (audio_format&0xFF),
                   (audio_channels > 1) ? "stereo" : "mono");
            t_print(T_INFO, "\n");

            // m_aInfo.format =   audio_format;
            // m_aInfo.channels = audio_channels;
            // m_aInfo.rate =     audio_rate;

            t_print( T_INFO, "format   ");
            int nFormatBytes = 0;
            switch (audio_format)
            {
            case AUDIO_U8:
            case AUDIO_S8:
                t_print( T_INFO, "AUDIO_x8");
                nFormatBytes = 1;
                break;
            case AUDIO_U16LSB:
            case AUDIO_S16LSB:
            case AUDIO_U16MSB:
            case AUDIO_S16MSB:
                t_print( T_INFO, "AUDIO_x16");
                nFormatBytes = 2;
                break;
            default:
                nFormatBytes = 0;
                t_print( T_INFO, "Problems with Format\n");
            }
            t_print( T_INFO, "\n");
            
            nBytesPerSecond = nFormatBytes * audio_channels * audio_rate;
            t_print( T_INFO, "channels %d\n", audio_channels);
            t_print( T_INFO, "rate     %d\n", audio_rate);
            t_print( T_INFO, "Bytes per second %d\n", nBytesPerSecond);
        }
            
        Mix_VolumeMusic(nGlobal_Volumn); /* SDL_MIX_MAXVOLUME */
        initTimer();
    }

    // ----------------------------------------
    void PlayAudio::initFile(std::string const& _sSoundFile)
    {
        t_print(T_AUDIOPLAYCONTROL, "PlayAudio::initFile()\n");
        m_sCurrentFilename = _sSoundFile;

        Mix_SetPostMix(MixCallback, NULL);       // callback at playtime
        Mix_HookMusicFinished(EndCallback);      // callback at playtime end
        
        t_print( T_DEBUG, "Set Position.\n");
        setCountPosition(0);
        pMusic = Mix_LoadMUS(_sSoundFile.c_str());
        t_print( T_DEBUG, "Load done.\n");
        // setVolumn(0);

		Mix_FadeInMusic(pMusic, 0, 100);
        t_print( T_DEBUG, "Fade done.\n");
    }

    std::string PlayAudio::getCurrentFile()
    {
        t_print(T_AUDIOPLAYCONTROL, "PlayAudio::getCurrentFile()\n");
        std::string sFile;
        Audio::Entry aEntry = m_pPlayListIterator->getCurrentElement();
        // m_pPlayListIterator->next();

        sFile = FileHelper::getAbsoluteFile(aEntry);
        if (sFile == "/")
        {
            sFile = "";
        }
        return sFile;
    }
    
    std::string PlayAudio::peekNextFile()
    {
        t_print(T_AUDIOPLAYCONTROL, "PlayAudio::peekNextFile()\n");
        std::string sFile;
        Audio::Entry aEntry = m_pPlayListIterator->getCurrentElement();
        sFile = aEntry.getFilename();
        return sFile;
    }
    // ----------------------------------------
    int PlayAudio::setVolumn(int _nVolumn)
    {
        t_print(T_AUDIOPLAYCONTROL, "PlayAudio::setVolumn(%d)\n", _nVolumn);
        if (_nVolumn < 0)
        {
            _nVolumn = 0;
        }
        
        if (_nVolumn > SDL_MIX_MAXVOLUME )
        {
            _nVolumn = SDL_MIX_MAXVOLUME;
        }
        Mix_VolumeMusic(_nVolumn);

        int nPercent = _nVolumn * 100 / SDL_MIX_MAXVOLUME;
//        m_aInformation.drawProgressBar(nPercent);
        if (hasAController())
        {
            getController()->setPropertyValue(P_INFO_PROGRESSBAR, new Integer(nPercent));
        }
        
        return _nVolumn;
    }

/*
    void PlayAudio::handleSoundFade()
    {
        t_print(T_AUDIOPLAYCONTROL, "PlayAudio::handleSoundFade()\n");
        static int nVolumnCount = 0;
        static int nVolumn = 0;
        static bool bVolumnArrived = false;

        nVolumnCount ++;

        if (nVolumnCount > 5 && bVolumnArrived == false)
        {
            if (nVolumn < nGlobal_Volumn)
            {
                t_print(T_DEBUG, "increase volumn\n");
                nVolumn++;
                // setVolumn(nVolumn);
            }
            if (nVolumn > nGlobal_Volumn)
            {
                nVolumn--;
            }
            if (nVolumn == nGlobal_Volumn)
            {
                bVolumnArrived = true;
            }
        }
    }
*/
// -----------------------------------------------------------------------------
    Uint32 TimerFkt(Uint32 _nInterval, void* param)
    {
        // t_print("playaudio.c::TimerFkt()\n");
        // t_print(T_AUDIOPLAYCONTROL, "playaudio.c::TimerFkt()\n");
        if (param)
        {
            TimerHelper* pHelper = static_cast<TimerHelper*>(param);
            if (pHelper->m_pPlayAudio)
            {
                MutexGuard m(pHelper->m_pPlayAudio->m_pLockTimerHelper);
                if (pHelper->m_bStopTimer == false)
                {
                    // printf(".");
                    // fflush(stdout);
                    pHelper->m_pPlayAudio->handleAnEvent();
                }
                // else
                // {
                //     if (pHelper->m_nID != 0)
                //     {
                //         t_print(T_DEBUG, "Timer stopped and removed.\n");
                //         SDL_RemoveTimer(pHelper->m_nID);
                //         pHelper->m_nID = 0;
                //         _nInterval = 0;
                //     }
                //     else
                //     {
                //         t_print(T_DEBUG, "This should not happen, Timer is already removed!\n");
                //     }
                // }
            }
        }
        return _nInterval;
    }

#warning the PlayAudio::handleAnEvent need an update
/*
  LLA: 2005.05.20 Ich habe so das Gefühl, das der Code zu handleAnEvent nicht der beste ist.
                  Er verschluckt sich manchmal
*/
    void PlayAudio::showEvent(int _nCurrentEvent)
    {
        switch(_nCurrentEvent)
        {
        case IMM_AUDIO_EVENT_LOUDNESS_LOWER:
            t_print( T_AUDIOPLAYCONTROL, "IMM_AUDIO_EVENT_LOUDNESS_LOWER\n");
            break;

        case IMM_AUDIO_EVENT_LOUDNESS_HIGHER:
            t_print( T_AUDIOPLAYCONTROL, "IMM_AUDIO_EVENT_LOUDNESS_LOWER\n");
            break;

        case IMM_AUDIO_EVENT_TOGGLE_PAUSE:
            t_print( T_AUDIOPLAYCONTROL, "IMM_AUDIO_EVENT_TOGGLE_PAUSE\n");
            break;

        case IMM_AUDIO_EVENT_PLAY_FROM_BEGINNING_OF_LIST:
            t_print( T_AUDIOPLAYCONTROL, "IMM_AUDIO_EVENT_PLAY_FROM_BEGINNING_OF_LIST\n");
            break;

        case IMM_AUDIO_EVENT_PLAY:
            t_print( T_AUDIOPLAYCONTROL, "IMM_AUDIO_EVENT_PLAY\n");
            break;

        case IMM_AUDIO_EVENT_STOP_FAST:
            t_print( T_AUDIOPLAYCONTROL, "IMM_AUDIO_EVENT_STOP_FAST ");
            break;

        case IMM_AUDIO_EVENT_STOP:
            t_print( T_AUDIOPLAYCONTROL, "IMM_AUDIO_EVENT_STOP\n");
            break;

        case IMM_AUDIO_EVENT_PLAY_NEXT:
            t_print( T_AUDIOPLAYCONTROL, "IMM_AUDIO_EVENT_PLAY_NEXT\n");
            break;

        case IMM_AUDIO_EVENT_PLAY_PREV:
            t_print( T_AUDIOPLAYCONTROL, "IMM_AUDIO_EVENT_PLAY_PREV\n");
            break;

        case IMM_AUDIO_EVENT_PLAY_NEXT_FAST:
            // t_print("Dispatch: Audio event play next\n");
            t_print( T_AUDIOPLAYCONTROL, "IMM_AUDIO_EVENT_PLAY_NEXT_FAST\n");
            break;

        case IMM_AUDIO_EVENT_CLEAR_LIST:
            // t_print("Dispatch: Audio event play next\n");
            t_print( T_AUDIOPLAYCONTROL, "IMM_AUDIO_EVENT_CLEAR_LIST\n");
            break;

        case IMM_AUDIO_EVENT_JUMP_FORWARD:
            t_print( T_AUDIOPLAYCONTROL, "IMM_AUDIO_EVENT_JUMP_FORWARD\n");
            break;

        case IMM_AUDIO_EVENT_JUMP_BACKWARD:
            t_print( T_AUDIOPLAYCONTROL, "IMM_AUDIO_EVENT_JUMP_BACKWARD\n");
            break;

        default:
            t_print( T_AUDIOPLAYCONTROL, "unhandled event\n");
            break;
        }
    }
    
    void PlayAudio::handleAnEvent()
    {
        // t_print( T_DEBUG, "start: handleAnEvent\n");

        // set right volumn after some run throughs

        
        // Fan
        // printf("*");
        // fflush(stdout);

        // show content of the dispatcher
//        print();

        // we must have 2 lists
        // immediately
        // long term

        if (hasANewEvent())
        {    
            showEvents();

            EventContainer aCurrentEvent = peekEvent();
            int nEvent = aCurrentEvent.getEvent();
            bool bFastMode = false;

            t_print(T_AUDIOPLAYCONTROL, "PlayAudio::handleAnEvent() has a event (%d)\n", nEvent);

            showEvent(nEvent);
            switch(nEvent)
            {

            case IMM_AUDIO_EVENT_LOUDNESS_LOWER:
                popEvent();
                nGlobal_Volumn -= 1;
                nGlobal_Volumn = setVolumn(nGlobal_Volumn);

                // t_print("current loudness: %d\n", nGlobal_Volumn);
                break;
            case IMM_AUDIO_EVENT_LOUDNESS_HIGHER:
                popEvent();
                nGlobal_Volumn += 1;
                nGlobal_Volumn = setVolumn(nGlobal_Volumn);
            
                // t_print("current loudness: %d\n", nGlobal_Volumn);
                break;

            case IMM_AUDIO_EVENT_TOGGLE_PAUSE:
                popEvent();
                toggle_pause();

                break;

            case IMM_AUDIO_EVENT_PLAY_FROM_BEGINNING_OF_LIST:
                // m_bPlayFromBeginning = true;
                m_pPlayListIterator->setCursorPosition(0);

            case IMM_AUDIO_EVENT_PLAY:
                popEvent();
                // m_bPlay = true;
                m_bTogglePause = false;
                // if we not currently playing something, play current file
                if (bGlobal_CurrentPlayAudio == false)
                {
                    std::string sCurrentFile = getCurrentFile();
                    play(sCurrentFile);
                }
                break;

            case IMM_AUDIO_EVENT_STOP_FAST:
                bFastMode = true;
                // no break!

            case IMM_AUDIO_EVENT_STOP:
                popEvent();
                stop(bFastMode);
                // bGlobal_CurrentPlayAudio = false;
                // m_bPlay = false;
            
//!            m_aInformation.setArtist("");
                break;

            case IMM_AUDIO_EVENT_PLAY_NEXT:
                popEvent();
                if (true)
                {
                    double nPlayNext = aCurrentEvent.getValue();
                    if (aCurrentEvent.isValueValid())
                    {
                        m_pPlayListIterator->setCursorPosition(static_cast<int>(nPlayNext));
                    }
                    else
                    {
                        m_pPlayListIterator->next();
                    }
                }
                // t_print( T_AUDIOPLAYCONTROL, m_bPlay ? "m_bPlay==true\n" : "m_bPlay==false\n");

                // Due to the fact, we read always the first value out (queue)
                // we have to insert values in reverse order!!!!!
//                insertEventAtFront(EventContainer(IMM_AUDIO_EVENT_PLAY));
                // Nice feature, if we play something, fade it out, 
                // if we play nothing, we need no fade out.
                LockInsertEvent();
                if (bGlobal_CurrentPlayAudio == true)
                {
//                    insertEventAtFront(EventContainer(IMM_AUDIO_EVENT_STOP));
                    insertNewEventNoLock(EventContainer(IMM_AUDIO_EVENT_STOP));
                }
                else
                {
//                    insertEventAtFront(EventContainer(IMM_AUDIO_EVENT_STOP_FAST));
                    insertNewEventNoLock(EventContainer(IMM_AUDIO_EVENT_STOP_FAST));
                }
                insertNewEvent(EventContainer(IMM_AUDIO_EVENT_PLAY));
                UnLockInsertEvent();
                break;

            case IMM_AUDIO_EVENT_PLAY_PREV:
                popEvent();
                // m_pPlayListIterator->prev();
                m_pPlayListIterator->prev();

                // Due to the fact, we read always the first value out (queue)
                // we have to insert values in reverse order!!!!!
//                insertEventAtFront(EventContainer(IMM_AUDIO_EVENT_PLAY));
                // Nice feature, if we play something, fade it out, 
                // if we play nothing, we need no fade out.
                LockInsertEvent();
                if (bGlobal_CurrentPlayAudio == true)
                {
//                    insertEventAtFront(EventContainer(IMM_AUDIO_EVENT_STOP));
                    insertNewEventNoLock(EventContainer(IMM_AUDIO_EVENT_STOP));
                }
                else
                {
//                    insertEventAtFront(EventContainer(IMM_AUDIO_EVENT_STOP_FAST));
                    insertNewEventNoLock(EventContainer(IMM_AUDIO_EVENT_STOP_FAST));
                }
                insertNewEvent(EventContainer(IMM_AUDIO_EVENT_PLAY));
                UnLockInsertEvent();
                break;
            case IMM_AUDIO_EVENT_PLAY_NEXT_FAST:
                popEvent();
                m_pPlayListIterator->next();
                // Due to the fact, we read always the first value out (queue)
                // we have to insert values in reverse order!!!!!
//                insertEventAtFront(EventContainer(IMM_AUDIO_EVENT_PLAY));
//                insertEventAtFront(EventContainer(IMM_AUDIO_EVENT_STOP_FAST));
                LockInsertEvent();
                insertNewEventNoLock(EventContainer(IMM_AUDIO_EVENT_STOP_FAST));
                insertNewEventNoLock(EventContainer(IMM_AUDIO_EVENT_PLAY));
                UnLockInsertEvent();            
                break;

            case IMM_AUDIO_EVENT_CLEAR_LIST:
                popEvent();
                // insertNewEvent(IMM_AUDIO_EVENT_CLEAR_LIST);
                m_pPlayList->clear();
                //  TODO: clear list, show list (not supported) 

                // getPlayList().showList();
                break;

            case IMM_AUDIO_EVENT_JUMP_FORWARD:
            {
                popEvent();
                double nJumpTo = aCurrentEvent.getValue();
                t_print( T_AUDIOPLAYCONTROL, "JumpTo: %f\n", nJumpTo);
                if (nJumpTo == AUDIO_END)
                {
//                    insertEventAtFront(EventContainer(IMM_AUDIO_EVENT_PLAY_NEXT));
                    insertNewEvent(EventContainer(IMM_AUDIO_EVENT_PLAY_NEXT));
                }
                else
                {
                    seekForward(nJumpTo);
                }
            }
        
            break;

            case IMM_AUDIO_EVENT_JUMP_BACKWARD:
                popEvent();
                seekBackward(aCurrentEvent.getValue());
                break;

            default:
                break;
            }
        }
        
        // t_print( T_AUDIOPLAYCONTROL, "stop: handleAnEvent\n");
    }

    // ----------------------------------------
    void PlayAudio::toggle_pause()
    {
        t_print(T_AUDIOPLAYCONTROL, "PlayAudio::togglePause()\n");
        if (m_bTogglePause == true)
        {
            Mix_PauseMusic();
            // Mix_ResumeMusic();
            Mix_FadeInMusic(pMusic, 0, 1500);
            m_bTogglePause = false;
        }
        else
        {
            Mix_FadeOutMusic(1500);
            Mix_PauseMusic();
            m_bTogglePause = true;
        }
    }
    
    // ----------------------------------------
    void PlayAudio::play(std::string const& _sFile)
    {
        // t_print( T_AUDIOPLAYCONTROL, "bGlobal_CurrentPlayAudio is false\n");
        if (_sFile.length() > 0)
        {
            t_print( T_INFO, "Start play file%s\n", _sFile.c_str());
            initFile(_sFile);
            start();
            // popEvent();
            
            // show information
            const char* cFile = basename( _sFile.c_str() );
//                    m_aInformation.setArtist( cFile );
            if (hasAController())
            {
                getController()->setPropertyValue(P_INFO_TITLE, new String("    now playing"), false);
                getController()->setPropertyValue(P_INFO_MUSIC_ARTIST, new String(cFile), false);
                std::string sNextPlayFile = peekNextFile();
                if (sNextPlayFile.length() > 0)
                {
                    getController()->setPropertyValue(P_INFO_NEXTPLAY, new String(std::string("    next play is ") + peekNextFile()), false);
                }
                getController()->setPropertyValue(P_PLAY_CURRENT_FILENAME, new String(_sFile), false);
                getController()->setPropertyValue(P_VIEW_REPAINT_DIRECTORY, new Boolean(true), true);
            }
            // m_aInformation.setNextPlay(std::string("    next play is ") + peekNextFile());

            // m_bPlay = true;
        }
    }
    
    // ----------------------------------------
    void PlayAudio::pause()
    {
        t_print(T_AUDIOPLAYCONTROL, "PlayAudio::pause()\n");
		Mix_PauseMusic();
    }
    
    // ----------------------------------------
    void PlayAudio::start()
    {
        t_print(T_AUDIOPLAYCONTROL, "PlayAudio::start()\n");
        bGlobal_CurrentPlayAudio = true;
		Mix_ResumeMusic();
		Mix_FadeInMusic(pMusic, 0, 1500);
    }
    
    // ----------------------------------------
    void PlayAudio::stop(bool _bFast)
    {
        t_print(T_AUDIOPLAYCONTROL, "PlayAudio::stop()\n");
        if (bGlobal_CurrentPlayAudio)
        {
            if (_bFast == false)
            {
                Mix_FadeOutMusic(1500);
                SDL_Delay(1500);
            }
            bGlobal_CurrentPlayAudio = false;
            Mix_HaltMusic();
            Mix_SetPostMix(NULL, NULL);       // callback at playtime
        }
    }
    
    // ----------------------------------------
    void PlayAudio::destroy()
    {
        t_print(T_AUDIOPLAYCONTROL, "PlayAudio::destroy()\n");

        // shut down the timer
        destroyTimer();

        if (!m_bDestroyed)
        {
            if (bGlobal_CurrentPlayAudio)
            {
                if (m_bSaveToStorage)
                {
                    m_aStorage.empty();
                    m_aStorage.store(STORAGE_CURRENT_FILENAME, m_sCurrentFilename);
                    m_aStorage.store(STORAGE_CURRENT_POS_AS_DOUBLE, getCurrentPlayPosition());
                    m_aStorage.store(STORAGE_CURRENT_VOLUMN, nGlobal_Volumn);

                    std::ofstream oss(m_sCurrentPlayListName.c_str(), std::ios::out);
                    // writeVector(oss, m_aCurrentList);

                    // m_aStorage.writeVector(m_aCurrentList);
                }
                stop(false);
            }
            m_bDestroyed = true;
        }
        if( Mix_PlayingMusic() )
        {
            Mix_FadeOutMusic(1500);
            // SDL_Delay(1500);
        }


        if ( pMusic )
        {
            Mix_FreeMusic(pMusic);
            pMusic = NULL;
        }
        Mix_CloseAudio();
    }
    
    // ----------------------------------------
    PlayAudio::~PlayAudio()
    {
        t_print(T_AUDIOPLAYCONTROL, "PlayAudio::~PlayAudio()\n");
        if (!m_bDestroyed) destroy();
        delete m_pPlayListIterator;

        // m_pCountLock = NULL;
    }

    // ----------------------------------------
/*
    void PlayAudio::addToList(Audio::Entry const& _aEntry)
    {
    //     m_aCurrentList.push_back(_sFile);
    //     m_aInformation.setNextPlay(peekNextFile());
        t_print( T_INFO, "Add To List %s:\n", _aEntry.getFilename().c_str());
        m_pPlayList->append(_aEntry);
    }
*/
/*
    void PlayAudio::showList()
    {
        t_print( T_INFO, "List:\n");
        m_pPlayList->showList();

//         // int nSize = m_aCurrentList.size();
//         // m_aCurrentList.end();
//         std::string sName;
//         for (std::vector<std::string>::const_iterator it=m_aCurrentList.begin();
//              it != m_aCurrentList.end();
//              it++)
//         {
//             sName = *it;
//             t_print( T_INFO, "%s\n", sName.c_str());
//         }
    }
*/    
    // ----------------------------------------
    void PlayAudio::seekForward(double _nJumpSecondsForward) 
    {
        t_print(T_AUDIOPLAYCONTROL, "PlayAudio::seekForward()\n");
        // get currentpos
        // stop playing
        // calculate new position
        // seek position
        // start playing
        // stop(true);
        Mix_FadeOutMusic(300);
        Mix_PauseMusic();
        Mix_FadeInMusicPos(pMusic, 0, 300, _nJumpSecondsForward);
        // int nOk = Mix_SetMusicPosition(_nJumpSecondsForward);
        // if (nOk == 0)
        // {
        // t_print("seek works.\n");
        // seek works
        // Mix_FadeInMusicPos(pMusic, 0, 1000, _nJumpSecondsForward);
        
        // Set new position, as fast as possible
        addCountPosition(int(double(_nJumpSecondsForward * nBytesPerSecond)) /* (44100 * 2 * 2) */ );
        // }
        // DEBUG: else
        // DEBUG: {
        // DEBUG:     t_print("seek does not work.\n");
        // DEBUG: }
        
//        Mix_RewindMusic();   
    }
    // ----------------------------------------
    void PlayAudio::seekBackward(double _nJumpSecondsBackward) 
    {
        t_print(T_AUDIOPLAYCONTROL, "PlayAudio::seekBackward()\n");
        // get currentpos
        // stop playing
        // calculate new position
        // seek position
        // start playing

//        Mix_FadeOutMusic(300);
        Mix_PauseMusic();
        if (_nJumpSecondsBackward == AUDIO_START)
        {
            setCountPosition(0);
        }
        else
        {
            addCountPosition(int(double(-_nJumpSecondsBackward * nBytesPerSecond)) /* (44100 * 2 * 2) */ );
        }       
        Mix_RewindMusic();
        double nNewPosition = getCurrentPlayPosition() - _nJumpSecondsBackward;
        if (nNewPosition < 0.0)
        {
            nNewPosition = 0;
        }
        Mix_FadeInMusicPos(pMusic, 0, 300, nNewPosition);
    }
    
        
    // ----------------------------------------
//     void PlayAudio::setInfo(Information const& _aInfo)
//     {
//         m_aInformation = _aInfo;
//     }

    // -----------------------------------------------------------------------------

/*
    void PlayAudio::emptyList()
    {
        m_pPlayList->clear();
    }
*/    
    // // ----------------------------------------
    // void PlayAudio::setStorage(Storage const& _aStorage, bool _bSaveToStorage)
    // {
    //     m_aStorage = _aStorage;
    //     m_bSaveToStorage = _bSaveToStorage;
    // }
    // ----------------------------------------
//     void tryToLoadFromStorage()
//     {
//         std::string sName;
//         sName = m_aStorage.restore(STORAGE_CURRENT_FILENAME, std::string());
// 
//         double nCurrentPos = 0;
//         nCurrentPos = m_aStorage.restore(STORAGE_CURRENT_POS_AS_DOUBLE, double(0));
// 
//         int nVolumn = 0;
//         nVolumn = m_aStorage.restore(STORAGE_CURRENT_VOLUMN, int(0));
//         nGlobal_Volumn = setVolumn(nVolumn);
//         // setVolumn(0);
// 
//         if (sName.length() > 0)
//         {
//             t_print( T_DEBUG, "Start: load file %s\n", sName.c_str());
//             addToList(sName);
//             insertNewEvent(EventContainer(IMM_AUDIO_EVENT_PLAY_NEXT_FAST));
// 
//             // set nCurrentPos
//             t_print( T_DEBUG, "Start: Set new position to %f\n", nCurrentPos);
//             insertNewEvent(EventContainer(IMM_AUDIO_EVENT_JUMP_FORWARD, nCurrentPos));
//             // handleAnEvent();
//         }
// 
//         std::ifstream iss(m_sCurrentPlayListName.c_str(), std::ios::in);
// //        int nCount = readVector(iss, m_aCurrentList);
//     }

    const int TIMER_INTERVAL=100; // interval in ms
    void PlayAudio::initTimer()
    {
        t_print(T_AUDIOPLAYCONTROL, "PlayAudio::initTimer()\n");
        m_pLockTimerHelper = SDL_CreateMutex();
        m_pTimerHelper = new TimerHelper(this);
        m_pTimerHelper->m_nID = SDL_AddTimer(TIMER_INTERVAL, TimerFkt, m_pTimerHelper);
    }
    void PlayAudio::destroyTimer()
    {
        t_print(T_AUDIOPLAYCONTROL, "PlayAudio::destroyTimer()\n");
        if (m_pTimerHelper)
        {
            MutexGuard m(m_pLockTimerHelper); // wait until timer is ready
            m_pTimerHelper->m_bStopTimer = true;
            // usleep(3 * TIMER_INTERVAL * 1000);                         // little bit longer as the timer is!
            // t_print(T_DEBUG, "Timer stopped and removed.\n");
            SDL_RemoveTimer(m_pTimerHelper->m_nID);
            delete m_pTimerHelper;
        }
        if (m_pLockTimerHelper)
        {
            SDL_DestroyMutex(m_pLockTimerHelper);
        }
    }
    
    bool PlayAudio::dispatch(SDL_Event const& _aEvent)
    {
        t_print(T_AUDIOPLAYCONTROL, "PlayAudio::dispatch\n");
        bool bReturn = false;
    
        switch( _aEvent.type )
        {
        case SDL_USEREVENT:
            Global::documentKey(SDLK_DOWN, KMOD_SHIFT, "Decrease volume.");
            Global::documentKey(SDLK_UP, KMOD_SHIFT, "Increase volume.");
            Global::documentKey(SDLK_RIGHT, 0, "Jump 10sec backward.");
            Global::documentKey(SDLK_LEFT, 0, "Jump 10sec forward.");
            Global::documentKey(SDLK_F5, 0, "Toggle pause.");
            Global::documentKey(SDLK_HOME, 0, "Jump to audio file start.");
            Global::documentKey(SDLK_END, 0, "Jump to audio file end.");
            Global::documentKey(SDLK_PLUS, 0, "Play next audio file.");
            Global::documentKey(SDLK_MINUS, 0, "Play prev audio file.");
            Global::documentKey(SDLK_F6, 0, "Clear play list.");
            Global::documentKey(SDLK_F12, 0, "Stop audio play.");
            
            break;

        case SDL_KEYDOWN:
            // LOUDNESS
            if (Global::shiftKey(_aEvent) && _aEvent.key.keysym.sym == SDLK_DOWN)
            {
                // shift down
                t_print(T_AUDIOPLAYCONTROL, "KEY: shift down\n");
                t_print(T_DEBUG, "leiser\n");
                insertNewEvent(EventContainer(IMM_AUDIO_EVENT_LOUDNESS_LOWER));
                bReturn = true;
            }
            else if (Global::shiftKey(_aEvent) && _aEvent.key.keysym.sym == SDLK_UP)
            {
                // shift up
                t_print(T_AUDIOPLAYCONTROL, "KEY: shift up\n");
                t_print(T_DEBUG, "lauter\n");
                insertNewEvent(EventContainer(IMM_AUDIO_EVENT_LOUDNESS_HIGHER));
                bReturn = true;
            }

            // else if (event.key.keysym.mod == 1 && event.key.keysym.sym == SDLK_LEFT)
            else if (_aEvent.key.keysym.sym == SDLK_LEFT)
            {
                // shift left

                // Audio::Entry aEntry = m_pViewDirectory->getCurrentSelectionValue();
                // std::string sPath = aEntry.getPath();
                // std::string sSoundFile = FileHelper::getAbsoluteFile(aEntry);
                // 
                // if (aEntry.getSystemType() == ENTRY_ISDIRECTORY)
                // {
                // }
                
                t_print(T_AUDIOPLAYCONTROL, "KEY: page down\n");
                t_print(T_DEBUG, "jump 10sec backward\n");
                insertNewEvent(EventContainer(IMM_AUDIO_EVENT_JUMP_BACKWARD, 10.0));
                bReturn = true;
            }

            // else if (event.key.keysym.mod == 1 && event.key.keysym.sym == SDLK_RIGHT)
            else if (_aEvent.key.keysym.sym == SDLK_RIGHT)
            {
                // shift right
                t_print(T_AUDIOPLAYCONTROL, "KEY: page up\n");
                t_print(T_DEBUG, "jump 10sec forward\n");
                insertNewEvent(EventContainer(IMM_AUDIO_EVENT_JUMP_FORWARD, 10.0));
                bReturn = true;
            }
                
            
            else if (_aEvent.key.keysym.sym == SDLK_F5)
            {
                // shift left
                t_print(T_AUDIOPLAYCONTROL, "KEY: F5\n");
                t_print(T_DEBUG, "toggle\n");
                insertNewEvent(EventContainer(IMM_AUDIO_EVENT_TOGGLE_PAUSE));
                bReturn = true;
            }
            else if (_aEvent.key.keysym.sym == SDLK_HOME)
            {
                // shift left
                t_print(T_AUDIOPLAYCONTROL, "KEY: home\n");
                t_print(T_DEBUG, "jump to audio file start\n");
                insertNewEvent(EventContainer(IMM_AUDIO_EVENT_JUMP_BACKWARD, AUDIO_START));
                bReturn = true;
            }

            // else if (event.key.keysym.mod == 1 && event.key.keysym.sym == SDLK_RIGHT)
            else if (_aEvent.key.keysym.sym == SDLK_END)
            {
                // shift right
                t_print(T_AUDIOPLAYCONTROL, "KEY: end\n");
                t_print(T_DEBUG, "jump to audio file end\n");
                insertNewEvent(EventContainer(IMM_AUDIO_EVENT_JUMP_FORWARD, AUDIO_END));
                bReturn = true;
            }

            // 
            else if (_aEvent.key.keysym.sym == SDLK_PLUS)
            {
                t_print(T_AUDIOPLAYCONTROL, "KEY: plus\n");
                t_print(T_DEBUG, "play next audio file\n");
                insertNewEvent(EventContainer(IMM_AUDIO_EVENT_PLAY_NEXT));
                bReturn = true;
            }
            else if (_aEvent.key.keysym.sym == SDLK_MINUS)
            {
                t_print(T_AUDIOPLAYCONTROL, "KEY: minus\n");
                t_print(T_DEBUG, "play prev audio file\n");
                insertNewEvent(EventContainer(IMM_AUDIO_EVENT_PLAY_PREV));
                bReturn = true;
            }

            else if (_aEvent.key.keysym.sym == SDLK_F6)
            {
                t_print(T_AUDIOPLAYCONTROL, "KEY: F6\n");
                t_print(T_DEBUG, "Clear play list\n");
                insertNewEvent(EventContainer(IMM_AUDIO_EVENT_CLEAR_LIST));
                bReturn = true;
            }
            else if ( _aEvent.key.keysym.sym == SDLK_F12 )
            {
                t_print(T_AUDIOPLAYCONTROL, "KEY: F12\n");
                t_print(T_DEBUG, "stop audio play\n");
                insertNewEvent(EventContainer(IMM_AUDIO_EVENT_STOP));
            }
            
            break;
            // default:
            // bReturn = false;
        }
        return bReturn;
    }
    void PlayAudio::setPlayList(PlayList *_pList)
    {
        m_pPlayList = _pList;
        m_pPlayListIterator = new PlayListIterator(_pList, 0, "");
    }
} // namespace Audio

