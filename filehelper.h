/*-*-c++-*-
 *
 *    $RCSfile: filehelper.h,v $
 *     Project:
 * Description:
 *
 *   Author(s): Lars Langhans
 *       $Date: 2005/11/21 20:13:48 $
 *   Copyright: (c)1996 by Lars Langhans
 *   $Revision: 1.5 $
 */

/* ==========================================================================
 *
 *               HISTORY
 *               -------
 *       $Log: filehelper.h,v $
 *       Revision 1.5  2005/11/21 20:13:48  langha_l
 *       store quality file all the time changes was created
 *
 *       Revision 1.4  2005/11/20 13:57:12  langha_l
 *       right handling of quality
 *
 *       Revision 1.3  2005/04/11 16:57:11  langha_l
 *       add writeQuality
 *
 *       Revision 1.2  2005/02/18 15:54:37  langha_l
 *       cleanups for FileHelper:: and StringHelper::
 *
 *       Revision 1.1  2004/09/15 18:28:58  langha_l
 *       remove some bugs, add icons
 *
 */

#ifndef _FILEHELPER_H
#define _FILEHELPER_H

#include <string>
#include "playlist.h"

namespace FileHelper
{
    
    bool isVideo(std::string const& _aFilename);
    bool isAudio(std::string const& _aFilename);
    std::string removeSingleFileExt(std::string const& _sStr, std::string const& _sExt);
    std::string removeFileExt(std::string const& _sStr);
    std::string getAbsoluteFile(Audio::Entry const& _aEntry);
    std::string getLastPath(std::string const& _sPath);
    
} // namespace FileHelper

#endif
