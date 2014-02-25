/*-*-c++-*-
 *
 *    $RCSfile: filehelper.cc,v $
 *     Project:
 * Description:
 *
 *   Author(s): Lars Langhans
 *       $Date: 2005/11/21 20:13:48 $
 *   Copyright: (c)1996 by Lars Langhans
 *   $Revision: 1.5 $
 */


#include <fnmatch.h>
// #include <libgen.h>

#include "filehelper.h"
#include "stringhelper.h"
#include "profile.h"

namespace FileHelper
{
    
bool isVideo(std::string const& _aFilename)
{
    const char* pBasename = basename(StringHelper::toLowerCase(_aFilename).c_str());
    
    // int n = fnmatch ("*.avi", pBasename, FNM_PATHNAME | FNM_CASEFOLD);
    if (
        (fnmatch ("*.avi",  pBasename, FNM_PATHNAME | FNM_CASEFOLD) == 0) ||
        (fnmatch ("*.mpg",  pBasename, FNM_PATHNAME | FNM_CASEFOLD) == 0) ||
        (fnmatch ("*.dv",  pBasename, FNM_PATHNAME | FNM_CASEFOLD) == 0) ||
        (fnmatch ("*.mpeg", pBasename, FNM_PATHNAME | FNM_CASEFOLD) == 0) ||
        (fnmatch ("*.vob", pBasename, FNM_PATHNAME | FNM_CASEFOLD) == 0) ||
        (fnmatch ("*.bin", pBasename, FNM_PATHNAME | FNM_CASEFOLD) == 0)
        )
    {
        return true;
    }
    return false;
}

bool isAudio(std::string const& _aFilename)
{
    const char* pBasename = basename(_aFilename.c_str());
    
    // int n = fnmatch ("*.avi", pBasename, FNM_PATHNAME | FNM_CASEFOLD);
    if (
        (fnmatch ("*.mp2",  pBasename, FNM_PATHNAME | FNM_CASEFOLD) == 0) ||
        (fnmatch ("*.mp3",  pBasename, FNM_PATHNAME | FNM_CASEFOLD) == 0) ||
        (fnmatch ("*.wav",  pBasename, FNM_PATHNAME | FNM_CASEFOLD) == 0)
        )
    {
        return true;
    }
    return false;
}

// -----------------------------------------------------------------------------

std::string removeSingleFileExt(std::string const& _sStr, std::string const& _sExt)
{
    int nExt = _sStr.rfind(_sExt);
    if (nExt > 0)
    {
        std::string aStr = _sStr.substr(0, nExt);
        return aStr;
    }
    return _sStr;
}

std::string removeFileExt(std::string const& _sStr)
{
    std::string aStr = _sStr;
    if (aStr.rfind(".mp3") > 0) aStr = removeSingleFileExt(aStr, ".mp3");
    if (aStr.rfind(".mp2") > 0) aStr = removeSingleFileExt(aStr, ".mp2");
    if (aStr.rfind(".wav") > 0) aStr = removeSingleFileExt(aStr, ".wav");
    return aStr;
}


// -----------------------------------------------------------------------------
/* static */
std::string getAbsoluteFile(Audio::Entry const& _aEntry)
{
    std::string sFilename;
    std::string sPath = _aEntry.getPath();

    if (sPath == "/")                            // this may be a disign failure, but I don't see where.
    {
        sFilename = sPath + _aEntry.getFilename();
    }
    else
    {
        sFilename = sPath + "/" + _aEntry.getFilename();
    }    
    return sFilename;
}

std::string getFilename(std::string const& _sStr)
{
    std::string sStr = basename(_sStr.c_str());
    return sStr;
}

std::string getLastPath(std::string const& _sPath)
{
    std::string sPath;
    int nPos = _sPath.rfind("/");
    if (nPos > 0)
    {
        sPath = _sPath.substr(nPos + 1);
    }
    return sPath;
}

} // namespace FileHelper

