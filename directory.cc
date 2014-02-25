/*-*-c++-*-
 *
 *    $RCSfile: directory.cc,v $
 *     Project:
 * Description:
 *
 *   Author(s): Lars Langhans
 *       $Date: 2005/11/20 13:57:12 $
 *   Copyright: (c)1996 by Lars Langhans
 *   $Revision: 1.18 $
 */

// damit kann RCS im executeable noch die Version feststellen
// static char rcsid[] = "$Id: directory.cc,v 1.18 2005/11/20 13:57:12 langha_l Exp $";

// -----------------------------------------------------------------------------
#include <string>
#include <locale>

#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>

#include <dirent.h>
#include "directory.h"
#include "t_print.h"
#include "filehelper.h"
#include "quality.h"

// -----------------------------------------------------------------------------
/* static */
std::string DirectoryHelper::removeLastDirEntry(std::string const& _sString)
{
    // we want to go one directory back.
    std::string sNew;
    int nPos = _sString.rfind("/");
    // if (nPos > 0 && (nPos + 1 == _sString.size()))
    // {
    //     nPos = _sString.rfind("/", nPos - 1);
    // }
    
    if (nPos == 0)
    {
        sNew = "/";
    }
    else
    {
        sNew = _sString.substr(0, nPos);
    }
    // t_print("%s\n", m_sCurrentPath.c_str());
    return sNew;
}

// Listener* DirectoryHelper::getListener()
// {
//     return m_pChangeDirectoryListener;
// }

void DirectoryHelper::changeDir(std::string const& _sNewDir)
{
    // if (_sNewDir == NULL) return;
    if (_sNewDir.length() < 1) return;

    std::string sNewDirCandidate;

    if (_sNewDir[0] == '/')
    {
        // new directory starts with '/' it's absolut
        sNewDirCandidate = _sNewDir;
    }
    else
    {
        if (_sNewDir ==  "..")
        {
            sNewDirCandidate = removeLastDirEntry( m_sCurrentPath );
        }
        else
        {
            sNewDirCandidate += "/";
            sNewDirCandidate += _sNewDir;
        }
    }

    // IMPORTANT: make sure, that the directory of m_sCurrentPath exist
    int nAccess = access(sNewDirCandidate.c_str(), R_OK | X_OK);
    if (nAccess == -1)
    {
        // FALLBACK: path doesn't exist, use current directory
        char cDirName[PATH_MAX];
        const char* getcwsresult = getcwd (cDirName, sizeof (cDirName));
        // t_print(T_DEBUG, "Current Directory: %s\n", cDirName);
        m_sCurrentPath = cDirName;
    }
    else
    {
        m_sCurrentPath = sNewDirCandidate;
    }
    
    fillArray();
    
}



// -----------------------------------------------------------------------------
bool DirectoryHelper::isDirectory(std::string const& _aName) const
{
    return m_pFileList->isDirectory(_aName);
    // bool bFound = false;
    
//     StringList const& aDirs  = getDirs();
// 
//     for(StringList::const_iterator it = aDirs.begin();
//         it != aDirs.end();
//         ++it)
//     {
//         std::string sName = *it;
//         if (_aName == sName)
//         {
//             bFound = true;
//             break;
//         }
//     }
    // return bFound;
}
// -----------------------------------------------------------------------------

DirectoryHelper::DirectoryHelper()
        : m_pFileList(NULL)
/*          m_pChangeDirectoryListener( new ChangeDirectoryListener()) */
{}

void DirectoryHelper::setDirectory(std::string const& _sCurrentPath)
{
    changeDir(_sCurrentPath);
}

DirectoryHelper::~DirectoryHelper()
{
    delete m_pFileList;
}

#define MAX_LEN 300

int
sortme (const void *first, const void *second)
{
    return strcmp (*(char **)first, *(char **)second);
}

// -----------------------------------------------------------------------------
// could be changed some day, but not today ;-), because, works for me.
void DirectoryHelper::fillArray()
{
    const int totalfile = 200;
    struct dirent *pDirEntry;
    char *pDirName;
    char *aFileList[totalfile];
    char *aDirList[totalfile];
    int nDirCount = 0;
    int nFileCount = 0;
    struct stat aFileStatus;

    delete m_pFileList;
    m_pFileList = new Audio::PlayList();

    int errorcode = chdir(m_sCurrentPath.c_str());

    DIR *pCurrentDir = opendir(m_sCurrentPath.c_str());
    
    for (nFileCount = 0; nFileCount < totalfile;)
    {
        pDirEntry = readdir (pCurrentDir);
        if (pDirEntry)
        {
            pDirName = (pDirEntry->d_name);
            // t_print("- %d, %s\n", strlen(pDirName), pDirName);
            stat (pDirName, &aFileStatus);
            // int nValue = aFileStatus.st_mode;
            // t_print("st_mode: %8o - %s\n", nValue, pDirName);
            
            if ((S_IFREG & aFileStatus.st_mode) == S_IFREG)	//it is a file
            {
                if (FileHelper::isAudio(pDirName) ||
                    FileHelper::isVideo(pDirName)
                    ) 
                {
                    int n = access(pDirName, R_OK);
                    if (n == 0)
                    {
                        char *temp;
                        temp = static_cast<char *>( malloc (sizeof (char) * MAX_LEN));
                        // memcheck (temp);
                        memset (temp, 0, MAX_LEN);
                        strncpy (temp, pDirName, MAX_LEN - 1);
                        aFileList[nFileCount++] = temp;
                    }
                    else
                    {
                        t_print(T_INFO, "Not readable: %s\n", pDirName);
                    }
                }
            }
            else
            {
                if ((S_IFDIR & aFileStatus.st_mode) == S_IFDIR)	//it is a dir
                {
                    if (strcmp(pDirName,".") == 0) continue;
                    // if (strcmp(pDirName,"..") == 0) continue;

                    int n = access(pDirName, R_OK|X_OK);
                    if (n == 0)
                    {
                        char *temp;
                        temp = static_cast<char *>( malloc (sizeof (char) * MAX_LEN));
                        // memcheck (temp);
                        memset (temp, 0, MAX_LEN);
                        strncpy (temp, pDirName, MAX_LEN - 1);
                        aDirList[nDirCount++] = temp;
                    }
                }
            }
        }
        else
        {
            // directory end arrived, leave for loop
            break;
        }
    }
    
    closedir (pCurrentDir);
    qsort (aFileList, nFileCount, sizeof (char *), sortme);
    qsort (aDirList, nDirCount, sizeof (char *), sortme);

    // t_print(T_INFO, "List current directory.\n");
    for (int i = 0; i < nDirCount ; i++)
    {
        // m_aDirList.push_back(aDirList[i]);

        m_pFileList->append(Audio::Entry(m_sCurrentPath, aDirList[i],"",ENTRY_ISDIRECTORY ));
        // append_to_list (aFileList[nCount], ISFILE);
        // t_print(T_INFO, "D %s\n", aDirList[i]);
        free (aDirList[i]);
        aDirList[i] = NULL;
    }

    for (int i = 0; i < nFileCount ; i++)
    {
        // m_aFileList.push_back(aFileList[i]);
        Audio::Entry aEntry(m_sCurrentPath, aFileList[i],"",ENTRY_ISFILE);
        m_pFileList->append(aEntry);
        // append_to_list (aFileList[nCount], ISFILE);
        // t_print("- %s\n", aFileList[i]);
        free (aFileList[i]);
        aFileList[i] = NULL;
    }
}


Audio::PlayList* DirectoryHelper::fillArrayRecursive(std::string const& _sCurrentPath)
{
    Audio::PlayList *pFileList = new Audio::PlayList();
    fillDirectory(_sCurrentPath, pFileList);
    return pFileList;
}

void DirectoryHelper::fillDirectory(std::string const& _sCurrentDirectory, Audio::PlayList *_pFileList)
{
    const int totalfile = 500;
    struct dirent *pDirEntry;
    char *pDirName;
    int nFileCount = 0;
    struct stat aFileStatus;

//    chdir(_sCurrentDirectory.c_str());

    DIR *pCurrentDir = opendir(_sCurrentDirectory.c_str());
    
    for (nFileCount = 0; nFileCount < totalfile;)
    {
        pDirEntry = readdir (pCurrentDir);
        if (pDirEntry)
        {
            pDirName = (pDirEntry->d_name);
            // std::string sAbsoluteName = _sCurrentDirectory + "/" + pDirName;
            std::string sAbsoluteName = _sCurrentDirectory;
            sAbsoluteName += "/";
            sAbsoluteName += pDirName;

            // t_print("- %d, %s\n", strlen(pDirName), pDirName);
            // t_print("  %s\n", sAbsoluteName.c_str());
            stat (sAbsoluteName.c_str(), &aFileStatus);
            // int nValue = aFileStatus.st_mode;
            // t_print("st_mode: %8o - %s\n", nValue, pDirName);
            
            if ((S_IFREG & aFileStatus.st_mode) == S_IFREG)	//it is a file
            {
                if (FileHelper::isAudio(pDirName) ||
                    FileHelper::isVideo(pDirName)
                    ) 
                {
                    int n = access(sAbsoluteName.c_str(), R_OK);
                    if (n == 0)
                    {
                        std::string sFilename(pDirName);
                        Audio::Entry aEntry(_sCurrentDirectory, sFilename, "", ENTRY_ISFILE);
                        _pFileList->append(aEntry);
                    }
                    else
                    {
                        t_print(T_INFO, "Not readable: %s\n", pDirName);
                    }
                }
            }
            else
            {
                if ((S_IFDIR & aFileStatus.st_mode) == S_IFDIR)	//it is a dir
                {
                    if (strcmp(pDirName,".") == 0) continue;
                    if (strcmp(pDirName,"..") == 0) continue;

                    int n = access(sAbsoluteName.c_str(), R_OK|X_OK);
                    if (n == 0)
                    {
                        // std::string sNewDir = _sCurrentDirectory;
                        // sNewDir += "/";
                        // sNewDir += pDirName;
                        fillDirectory(sAbsoluteName, _pFileList);
                    }
                }
                else
                {
                    t_print("Unknown file type for: '%s'. \n", sAbsoluteName.c_str());
                }
            }
        }
        else
        {
            // directory end arrived, leave for loop
            break;
        }
    }
    
    closedir (pCurrentDir);
}

/* static */
/*
Audio::PlayList DirectoryHelper::createDirList(std::string const& _sDir)
{
    DirectoryHelper aDir(_sDir.c_str());
    return aDir.getPlayList();
}
*/
