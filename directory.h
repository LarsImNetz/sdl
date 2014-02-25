/*-*-c++-*-
 *
 *    $RCSfile: directory.h,v $
 *     Project:
 * Description:
 *
 *   Author(s): Lars Langhans
 *       $Date: 2005/02/18 15:54:37 $
 *   Copyright: (c)1996 by Lars Langhans
 *   $Revision: 1.12 $
 */

#ifndef _directory_h
#define _directory_h

#include <string>
// #include "stringhelper.h"
#include "playlist.h"
// #include "listener.h"
// #include "listenerabfaenger.h"

#include "filehelper.h"
// #include "controller.h"

// bool isVideo(std::string const&);
// -----------------------------------------------------------------------------
// class FileHelper
// {
//     FileHelper();
// public:
//     
// };

// -----------------------------------------------------------------------------
class DirectoryHelper 
//: public ControllerHelper
{
    std::string m_sCurrentPath;
    Audio::PlayList *m_pFileList;

    void fillArray();
    static void fillDirectory(std::string const& _sCurrentDir, Audio::PlayList *_pList);

    DirectoryHelper(const DirectoryHelper&);
    DirectoryHelper& operator=(const DirectoryHelper&);
public:
    DirectoryHelper();
    ~DirectoryHelper();

    void setDirectory(std::string const& _sNewDir);
    void changeDir(std::string const& _sNewDir /*, const char* _sLastDirName */ );

    static std::string removeLastDirEntry(std::string const& _sString);

    std::string getCurrentPath() const {return m_sCurrentPath;}
    bool isDirectory(std::string const& _aName) const;
    Audio::PlayList* getList() {return m_pFileList;}

    // static Audio::PlayList createDirList(std::string const& _sDir);
    static Audio::PlayList* fillArrayRecursive(std::string const& _sCurrentPath);
};


#endif
