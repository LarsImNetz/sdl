/*-*-c++-*-
 *
 *    $RCSfile: checkmp3.cc,v $
 *     Project:
 * Description:
 *
 *   Author(s): Lars Langhans
 *       $Date: 2005/11/20 13:56:43 $
 *   Copyright: (c)1996 by Lars Langhans
 *   $Revision: 1.8 $
 */

// damit kann RCS im executeable noch die Version feststellen
// static char rcsid[] = "$Id: checkmp3.cc,v 1.8 2005/11/20 13:56:43 langha_l Exp $";

/* ==========================================================================
 *
 *               HISTORY
 *               -------
 *       $Log: checkmp3.cc,v $
 *       Revision 1.8  2005/11/20 13:56:43  langha_l
 *       add new repair, but not ready yet
 *
 *       Revision 1.7  2005/07/03 18:39:25  langha_l
 *       change cast to static_cast
 *
 *       Revision 1.6  2005/02/18 15:54:37  langha_l
 *       cleanups for FileHelper:: and StringHelper::
 *
 *       Revision 1.5  2005/02/17 19:50:22  langha_l
 *       cleanups
 *
 *       Revision 1.4  2005/01/19 12:13:57  langha_l
 *       much changes for version 2, more modulisation
 *
 *       Revision 1.3  2003/05/13 07:32:32  langha_l
 *       new parameter handling
 *
 *       Revision 1.2  2003/05/12 16:25:57  langha_l
 *       lot of changes
 *
 *       Revision 1.1.1.1  2003/04/28 16:24:23  langha_l
 *       SDL MusicPlay
 *
 */

#include <string>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>

#include <SDL.h>
#include <SDL_sound.h>

#include "t_print.h"
#include "profile.h"
#include "filehelper.h"

#define BUFFSIZE 256 * 1024
#define SAMPLESIZE 512

int nLeftLow = 0;
int nLeftHigh = 0;
int nRightLow = 0;
int nRightHigh = 0;

void usage(const char* sFilename)
{
    t_print("usage:\n %s [OPTIONS] AUDIOFILE\n", sFilename);
    t_print("OPTIONS:\n");
    t_print(" -f  force execution\n");
    t_print(" -r  try to repair a not playable music file. (*.mp3 only)\n");
    t_print(" -n  don't create info file.\n");
    t_print(" -h  this help.\n");
    exit(0);
}

// -----------------------------------------------------------------------------

int nCheckSoundVolume(char* pBuffer, int nBytes, Uint16 _nFormat)
{
    if (_nFormat == AUDIO_S16LSB)
    {
        int nLen = nBytes; // (16 Bit, Stereo)

        for (int i=0;i<nLen;i += 4)
        {
            char ll = pBuffer[i];
            char lh = pBuffer[i+1];

            char rl = pBuffer[i+2];
            char rh = pBuffer[i+3];

            if (ll != 0 || lh != 0 || rl != 0 || rh != 0)
            {
                // volatile int dummy = 0;
            
                int nLeft  = lh << 8 | ll;
                int nRight = rh << 8 | rl;

                if (nLeft < nLeftLow)  nLeftLow  = nLeft;
                if (nLeft > nLeftHigh) nLeftHigh = nLeft;
                
                if (nRight < nRightLow)  nRightLow  = nRight;
                if (nRight > nRightHigh) nRightHigh = nRight;
            }
        }
        
    }
    return 0;
}

// -----------------------------------------------------------------------------
// SDL Callback function
void outputaudio(void *unused, Uint8 *stream, int len)
{}

// -----------------------------------------------------------------------------
int main(int argc, char** argv)
{
    const char* sOptString = "frnh";
    int nOpt = 0;
    bool bForceExec = false;
    bool bCreateInfo = true;
    bool bRepair = false;

    while ((nOpt = getopt(argc, argv, sOptString)) != -1)
    {
        switch (nOpt)
        {
        case 'f':
            bForceExec = true;
            break;
        case 'r':
            bRepair = true;
            break;
        case 'n':
            bCreateInfo = false;
            break;
        // case 't':
        //     t_print("option t with %s\n", optarg);
        //     break;
        case 'h':
            usage(argv[0]);
            break;
        case '?':
            break;
        default:
            t_print ("?? getopt returned character code 0%o ??\n", nOpt);
        }
    }
    std::string sSoundFile;
    if (optind < argc)
    {
        t_print ("Audiofile: ");
        while (optind < argc)
        {
            sSoundFile = argv[optind++];
            t_print ("%s ", sSoundFile.c_str());
            fflush(stdout);
        }
        t_print ("\n");
    }

    if (sSoundFile.size() == 0)
    {
        usage(argv[0]);
    }
    
    t_print(T_DEBUG, "Consistence checker for mp3\n");

    std::string sInfoSoundFile = FileHelper::removeFileExt(sSoundFile);
    sInfoSoundFile += ".info";

    int nAccess = 0;
    if ((nAccess = access(sSoundFile.c_str(), R_OK)) != 0)
    {
        t_print(T_DEBUG, "no access (no: %d) to file %s\n", nAccess, sSoundFile.c_str());
        exit(1);
    }
    FILE* IN;
    IN = fopen(sSoundFile.c_str(),"r");
    if (IN != NULL)
    {
        int buf[4];
        buf[0] = fgetc(IN);
        buf[1] = fgetc(IN);
        buf[2] = fgetc(IN);
        buf[3] = fgetc(IN);
        if (buf[0] == 0xff &&
            buf[1] == 0xfb &&
            buf[2] == 0xb0 &&
            buf[3] == 0x04)
        {
            printf("looks good.\n");
        }
        else
        {
            printf("unknown\n");
        }
        
        fclose(IN);
    }
    else
    {
        t_print(T_DEBUG, "can't open file %s for read.\n", sSoundFile.c_str());
    }
    
    if ((nAccess = access(sInfoSoundFile.c_str(), F_OK)) == 0)
    {
        t_print(T_DEBUG, "infofile '%s' already exist\n", sInfoSoundFile.c_str());
        if (bForceExec != true)
        {
            t_print(T_DEBUG, "Cancel execution, parameter '-f' could help here, to force execution.\n");
            exit(-1);
        }
    }

    // Uint32 nAlreadyInit = SDL_WasInit(0);
    // if ( SDL_InitSubSystem(SDL_INIT_AUDIO) < 0)
    // {
    //     t_print(T_DEBUG, "SDL konnte nicht initialisiert werden:  %s\n",
    //             SDL_GetError());
    //     exit(1);
    // }
    

    if ( SDL_Init( SDL_INIT_AUDIO ) < 0 )
    {
        t_print(T_DEBUG, "SDL konnte nicht initialisiert werden:  %s\n",
                SDL_GetError());
        exit(1);
    }
    
    if ( Sound_Init() == 0)
    {
        t_print(T_DEBUG, "no sound:  %s\n", Sound_GetError());
        exit(1);
    }

    
    // --------------------------------- Audio init ---------------------------------

    /* SDL Audio Specifications */
    SDL_AudioSpec aspec, obtained;
    aspec.format = AUDIO_S16LSB;
    /* The desired audio frequency in samples-per-second. */
    aspec.freq     = 44100;
    
    /* Number of channels (mono/stereo) */
    aspec.channels = 2;
    
    /* The desired size of the audio buffer in samples. This number should be a
       power of two, and may be adjusted by the audio driver to a value more
       suitable for the hardware. Good values seem to range between 512
       and 8192 inclusive, depending on the application and CPU speed. 
       Smaller values yield faster response time, but can lead to underflow
       if the application is doing heavy processing and cannot fill the audio
       buffer in time. A stereo sample consists of both right and left channels
       in LR ordering. Note that the number of samples is directly related to 
       time by the following formula: ms = (samples*1000)/freq */ 
    aspec.samples  = SAMPLESIZE;
    
    /* This should be set to a function that will be called when the audio device
       is ready for more data. It is passed a pointer to the audio buffer, and
       the length in bytes of the audio buffer. This function usually runs in a
       separate thread, and so you should protect data structures that it
       accesses by calling SDL_LockAudio and SDL_UnlockAudio in your code.
       
       The callback prototype is: 
       void callback(void *userdata, Uint8 *stream, int len); 
       userdata is the pointer stored in userdata field of the SDL_AudioSpec.
       stream is a pointer to the audio buffer you want to fill with information and 
       len is the length of the audio buffer in bytes. */
    aspec.callback = outputaudio;
    
    /* This pointer is passed as the first parameter to the callback function. */
    aspec.userdata = NULL;
    
    // pSample = NULL;
    
    /* Open the audio device and start playing sound! */
    if(SDL_OpenAudio(&aspec, &obtained) < 0) 
    {
        t_print(T_DEBUG, "SDL: Unable to open audio: %s\n",
                SDL_GetError());
        exit(-1);
    } 
    SDL_PauseAudio(1);
    
    
    Sound_AudioInfo aInfo;
    aInfo.format =   AUDIO_S16LSB;
    aInfo.channels = 2;
    aInfo.rate =     44100;

    t_print(T_DEBUG, "reading: %s\n", sSoundFile.c_str());
#if 0
    const Sound_DecoderInfo **i = Sound_AvailableDecoders();
    
    while (*i)
    {
        const Sound_DecoderInfo *pInfo = *i;
        std::string aStrs = showStrings(pInfo->extensions);
        
        t_print("Supported sound format: [%s], which is [%s].\n", aStrs.c_str(), pInfo->description);
        *i++;
        // ...and other fields...
    }
#endif

    Uint32 nBuffer = BUFFSIZE;

    Sound_Sample* pSample = Sound_NewSampleFromFile(sSoundFile.c_str(),
                                                    &aInfo,
                                                    nBuffer);
    if (pSample == NULL)
    {
        t_print(T_DEBUG, "SDL: Sound_NewSampleFromFile failed: %s\n", Sound_GetError());
        exit(1);
    }
    Uint32 nCount = 0;

    int nWaveLength = 0;
    int nError = 0;
    
    if (pSample && !bRepair)
    {
        while (pSample)
        {
            nCount = Sound_Decode(pSample);

            nCheckSoundVolume(static_cast<char*>(pSample->buffer), nCount, aInfo.format);
            nWaveLength += nCount;
            t_print(T_DEBUG, "len in bytes: %d\r", nWaveLength);
            
            if (nCount != nBuffer)
            {
                // lt. Anleitung, check flag, EOF, error, anything else
                break;
            }
        }
        
        t_print(T_DEBUG, "\n");

        int nFlag = pSample->flags;
        // {
        // case SOUND_SAMPLEFLAG_NONE:
        //     t_print(T_DEBUG, "got SOUND_SAMPLEFLAG_NONE\n");
        //     nError = 2;
        //     break;

#if SOUND_VER_MAJOR == 1
        if (nFlag & SOUND_SAMPLEFLAG_CANSEEK)
        {
            t_print(T_DEBUG, "got SOUND_SAMPLEFLAG_CANSEEK\n");
            // nError |= 0;
        }
#else
        if (nFlag & SOUND_SAMPLEFLAG_NEEDSEEK)
        {
            t_print(T_DEBUG, "got SOUND_SAMPLEFLAG_NEEDSEEK\n");
            nError |= 3;
        }
#endif
        if (nFlag & SOUND_SAMPLEFLAG_EOF)
        {
            t_print(T_DEBUG, "got SOUND_SAMPLEFLAG_EOF\n");
            if (nWaveLength == 0)
            {
                // can't read file, maybe a problem.
                nError |= 6;
            }
            // nError = 0;
        }
        if (nFlag & SOUND_SAMPLEFLAG_ERROR)
        {        
            t_print(T_DEBUG, "got SOUND_SAMPLEFLAG_ERROR\n");
            nError |= 4;
        }
        if (nFlag & SOUND_SAMPLEFLAG_EAGAIN)
        {
            t_print(T_DEBUG, "got SOUND_SAMPLEFLAG_EAGAIN\n");
            nError |= 5;
        }
        
        // t_print(T_DEBUG, "SDL-Error: %s\n", Sound_GetError());
        Sound_FreeSample(pSample);
    }
    if (nError == 0 && !bRepair)
    {
        t_print(T_DEBUG, "ok. Soundfile read/decodeable.\n");

        t_print(T_DEBUG, " Left min: %6d max %6d .\n", nLeftLow, nLeftHigh);
        t_print(T_DEBUG, "Right min: %6d max %6d .\n", nRightLow, nRightHigh);

        int nMax = std::max(abs(nLeftLow), nLeftHigh);
        nMax = std::max(nMax, nRightHigh);
        nMax = std::max(nMax, abs(nRightLow));

        double fMaximum = double(nMax) * 100 / 32768;
        t_print(T_DEBUG, "Maximum Value %f.\n", fMaximum );
        
        // std::string sInfoSoundFile = sSoundFile;
        // sInfoSoundFile += ".info";

        if (bCreateInfo == true)
        {
            Profile aProfile(sInfoSoundFile.c_str());
            aProfile.insertValue("name", "name"   , sSoundFile);
            aProfile.insertValue("wave info", "left_low"   , nLeftLow);
            aProfile.insertValue("wave info", "left_high"  , nLeftHigh);
            aProfile.insertValue("wave info", "right_low"  , nRightLow);
            aProfile.insertValue("wave info", "right_high" , nRightHigh);
            aProfile.insertValue("wave info", "abs_maximum", fMaximum);
            aProfile.insertValue("wave info", "wave_length", nWaveLength);
            
            // FILE *out = fopen(sInfoSoundFile.c_str(), "w");
            // if (out)
            // {
            //     fprintf(out, "# Autogenerated loudness information file for\n");
            //     fprintf(out, "# file: %s\n", sSoundFile.c_str());
            //     fprintf(out, "left_low   = %d\n", nLeftLow);
            //     fprintf(out, "left_high  = %d\n", nLeftHigh);
            //     fprintf(out, "right_low  = %d\n", nRightLow);
            //     fprintf(out, "right_high = %d\n", nRightHigh);
            //     fprintf(out, "abs_maximum = %f\n", fMaximum);
            //     fprintf(out, "wave_length = %d\n", nWaveLength);
            //     fclose(out);
            // }
            // else
            // {
            //     t_print(T_DEBUG, "error: can't create info file.\n");
            // }
        }
        else
        {
            t_print(T_DEBUG, "Creation of info file suppressed, this handles parameter '-n'\n");
        }
    }
    else
    {
        if (bRepair == true)
        {
            
            t_print(T_DEBUG, "Search '0xfffbb004' magic code in file. ");
            FILE *in = fopen(sSoundFile.c_str(), "r");
            if (in)
            {
                int buf[4];
                buf[0] = 0;
                buf[1] = 0;
                buf[2] = 0;
                buf[3] = 0;
                
                while(!feof(in))
                {
                    int c = fgetc(in);
                    buf[0] = buf[1];
                    buf[1] = buf[2];
                    buf[2] = buf[3];
                    buf[3] = c;
                    if (buf[0] == 0xff &&
                        buf[1] == 0xfb &&
                        buf[2] == 0xb0 &&
                        buf[3] == 0x04)
                    {
                        break;
                    }                    
                }
                if (!feof(in))
                {
                    t_print(T_DEBUG, "Found.\n");
                    
                    std::string sNewSoundFile;
                    sNewSoundFile = sSoundFile + ".fix";
                    FILE *out = fopen(sNewSoundFile.c_str(), "w");
                    if (out)
                    {
                        t_print(T_DEBUG, "write new file %s.\n", sNewSoundFile.c_str());
                        
                        fputc(buf[0], out);
                        fputc(buf[1], out);
                        fputc(buf[2], out);
                        fputc(buf[3], out);
                        int nRealGot = 0;
                        while(!feof(in))
                        {
                            const int MAXBUFSIZE = 32768;
                            char buff[MAXBUFSIZE];
                            
                            nRealGot = fread(buff, sizeof(char), MAXBUFSIZE, in);
                            if (nRealGot > 0)
                            {
                                size_t egal = fwrite(buff, sizeof(char), nRealGot, out);
                            }
                        }
                        fclose(out);
                    }
                    else
                    {
                        t_print(T_DEBUG, "can't create file: %s.\n", sNewSoundFile.c_str());
                    }                
                    fclose(in);
                }
                else
                {
                    t_print(T_DEBUG, "not found.\n");
                }
            }
            else
            {
                t_print(T_DEBUG, "can't open file '%s' for read.\n", sSoundFile.c_str());
            }
        }
        else
        {
            t_print(T_DEBUG, "Do not repair, parameter '-r' could help here.\n");
        }   
    }
    
    SDL_CloseAudio();
    Sound_Quit();
    atexit(SDL_Quit);
    
    return nError;
}
