/*-*-c++-*-
 *
 *    $RCSfile: information.h,v $
 *     Project:
 * Description:
 *
 *   Author(s): Lars Langhans
 *       $Date: 2005/07/20 07:03:14 $
 *   Copyright: (c)1996 by Lars Langhans
 *   $Revision: 1.8 $
 */

#ifndef _information_h
#define _information_h

#include <string>
#include <SDL.h>
#include "ttffont.h"
// #include "listener.h"
#include "i_view.h"
#include "i_handlecontroller.h"
#include "controller.h"

class Information : public /* implements */ /* ListenerAbfaenger */ interface_View,
                    public interface_handleController
{
    SDL_Surface *m_pDisplay;
    TTFFont     *m_pFont;
    TTFFont     *m_pKeyFont;

    std::string m_sInfo;
    std::string m_sArtist;
    std::string m_sTitle;
    std::string m_sAlbum;
    std::string m_sYear;
    std::string m_sCurrentDirectory;
    std::string m_sFilter;

    int m_nPercent;

    bool m_bInfo;
    bool m_bArtist;
    bool m_bTitle;
    bool m_bAlbum;
    bool m_bYear;
    bool m_bCurrentDirectory;
    bool m_bFilter;

    Information(Information const&);
    Information& operator=(Information const&);
public:
/*
    Information()
            :init(NULL,NULL)
        {
        }
*/    
    Information( TTFFont *_pFont, TTFFont *_pKeyFont )
            :m_pDisplay(NULL),
             m_pFont(_pFont),
             m_pKeyFont(_pKeyFont),
             m_nPercent(0),
             m_bInfo(true),
             m_bArtist(true),
             m_bTitle(true),
             m_bAlbum(true),
             m_bYear(true),
             m_bCurrentDirectory(true),
             m_bFilter(true)
        {
        }
    
    virtual ~Information() {}
    
    bool setInfo(std::string const& _sText);
    bool setArtist(std::string const &_sText);
    bool setTitle(std::string const &_sText);
    bool setAlbum(std::string const &_sText);
    bool setYear(std::string const &_sText);
    bool setCurrentDirectory(std::string const &_sText);
    bool setProgressBar(int _nPercent);
    bool setFilter(std::string const& _sText);

    // implementation of interface_View
    virtual bool paint( SDL_Surface* _pSurface );

private:
    // void clearArtist();
    void showInfo(std::string const& _sText);
    void showArtist(std::string const &_sText);
    void showTitle(std::string const &_sText);
    void showAlbum(std::string const &_sText);
    void showYear(std::string const &_sText);
    void showCurrentDirectory(std::string const &_sText);
    void showFilter(std::string const& _sText);

    int showKey(std::string const &_sText, int _nX);

    // ListenerAbfaenger
    // void handleAnEvent(int _nValue);
    // void handleAnEvent(std::string const& _sStr);

    void drawProgressBar(int _nPercent);

    virtual bool handleController(Controller *_pController);
};

#endif
