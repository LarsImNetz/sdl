/*-*-c++-*-
 *
 *    $RCSfile: quality.cc,v $
 *     Project:
 * Description:
 *
 *   Author(s): Lars Langhans
 *       $Date: 2005/11/21 20:13:48 $
 *   Copyright: (c)1996 by Lars Langhans
 *   $Revision: 1.6 $
 */

#include <string>
#include <SDL.h>
#include "quality.h"
#include "t_print.h"
#include "property.h"
#include "global.h"
#include "profile.h"

Quality::Quality(std::string const& _sPropertyFile)
        :m_sPropertyFile(_sPropertyFile),
         m_pProfile(NULL)
{
    int nAccess = 0;
    if ((nAccess = access(m_sPropertyFile.c_str(), R_OK)) != 0)
    {
        // can't access the property file
        t_print(T_DEBUG, "Can't access %s property file.\n", m_sPropertyFile.c_str());
    }
}

Quality::~Quality()
{
    delete m_pProfile;
}


Profile* Quality::getProfile()
{
    if (m_pProfile == NULL)
    {
        m_pProfile = new Profile(m_sPropertyFile);
    }
    return m_pProfile;
}

bool Quality::dispatch(SDL_Event const& _aEvent)
{
    t_print(T_FLOWCONTROL, "Quality::dispatch");
    bool bReturn = false;
    int nQuality = -1;

    switch( _aEvent.type )
    {
    case SDL_USEREVENT:
        Global::documentKey(SDLK_0, 0, "Handle 0-9 as a quality identifier.", SDLK_9);
        break;

    case SDL_KEYDOWN:
        if ( _aEvent.key.keysym.sym >= SDLK_0 && _aEvent.key.keysym.sym <= SDLK_9 )
        {
            t_print(T_FLOWCONTROL, "KEY: digit\n");
            nQuality = static_cast<char>(_aEvent.key.keysym.sym) - '0';
            bReturn = true;
        }
        break;
    }

    if (bReturn == true)
    {
        if (hasAController())
        {
            getController()->setPropertyValue(P_QUALITY_VALUE, new Integer(nQuality));
            getController()->setPropertyValue(P_VIEW_REPAINT_DIRECTORY, new Boolean(true));
        }
    }
    return bReturn;
}

int Quality::getQuality(std::string const& _sFilename)
{
    // std::string sFilename = _aEntry.getPath() + _aEntry.getFilename();    
    std::string sValue = getProfile()->getValue(_sFilename, "quality");
    int nQuality = 0; // default
    if (sValue.size() > 0)
    {
        nQuality = atoi(sValue.c_str());
    }
    return nQuality;
}

void Quality::setQuality(std::string const& _sFilename, int _nQuality)
{
    Profile* pProfile = getProfile();
    if (pProfile)
    {
        pProfile->insertValue(_sFilename, "quality", _nQuality);
        pProfile->store();
    }
}

bool Quality::handleController(Controller *_pController)
{
    bool bReturn = false;
    if (_pController != NULL)
    {
        if (_pController->hasPropertyValue(P_QUALITY_VALUE))
        {
            int nQuality = _pController->getPropertyAsIntValue(P_QUALITY_VALUE);
            std::string sCurrentFilename = _pController->getPropertyAsStringValue(P_VIEW_CURRENT_FILENAME);
            
            // int nCursorPos = findPosition(sCurrentFilename);
            // if (nCursorPos >= 0 && nCursorPos < m_aEntryList.size())
            // {
            //  doesn't work here!   std::string sFullFilename = m_aEntryList[nCursorPos];
            // }
            setQuality(sCurrentFilename, nQuality);
            // we have to remove the property or every time this value will writen to other
            // entries too.
            _pController->removePropertyValue(P_QUALITY_VALUE);
            
            // FileHelper::writeQuality(sCurrentFilename, nQuality);
        
            // we need a cursor
            bReturn = true;
        }
    }
    
    return bReturn;
}

//         std::string sPropertyFile;
//         sPropertyFile = m_sCurrentPath + "/" + aFileList[i] + ".ini";
//         aEntry.setQuality(Quality::getQuality(sPropertyFile));

// void writeQuality(std::string const& _sName, int _nQuality)
// {
//     std::string sFilenameNoSuffix = removeFileExt(_sName) + ".ini";
//     std::string sProfileName = getFilename(sFilenameNoSuffix);
//     Profile aProfile(sProfileName);
//     aProfile.insertValue("LLA Taste", "quality", _nQuality);
// }
