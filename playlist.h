#ifndef PLAYLIST_H
#define PLAYLIST_H

#include <iostream>
#include <string>
#include <vector>

#include <SDL_mutex.h>

#include "i_handlecontroller.h"
#include "t_print.h"

#define MAX_CURSORPOS 32767

#define ENTRY_ISFILE 1
#define ENTRY_ISDIRECTORY 2

namespace Audio
{
    class Entry
    {
        std::string m_sPath;
        std::string m_sFilename;
        std::string m_sFileType;                 // mp3, avi, ogg, ...
        int m_nSystemType;                         // Directory, Regular file, link

    public:
        
        Entry()
                :m_sPath(),
                 m_sFilename(),
                 m_sFileType(),
                 m_nSystemType(-1)
            {
                // t_print(T_DEBUG, "Create Empty Entry.\n");
            }
        
        Entry(std::string const& _sPath, std::string const& _sFilename, std::string const& _sFileType, int _nFileSystemType)
                :m_sPath(_sPath),
                 m_sFilename(_sFilename),
                 m_sFileType(_sFileType),
                 m_nSystemType(_nFileSystemType)
            {
                // path must not end with '/'
                int nPathSize = m_sPath.size();
                if ((nPathSize > 0 && (m_sPath[nPathSize - 1]) == '/'))
                {
                    m_sPath = m_sPath.substr(0, nPathSize - 1);
                }
            }
        std::string getPath() const
            {
                return m_sPath;
            }
        std::string getFilename() const {return m_sFilename;}
        std::string getFileType() const {return m_sFileType;}
        int getSystemType() const {return m_nSystemType;}

        std::ostream& write(std::ostream & _os) const;
        std::istream& read(std::istream & _os);

    };

    // -----------------------------------------------------------------------------


    typedef int CursorPos;
    class PlayListIterator;

    class PlayList :
        public interface_handleController
    {
        std::vector<Entry> m_aEntryList;          // List of files we want to hear
        // std::vector<std::string> m_aPlayedList;     // List of files, we have heard
        //        const char **m_pList;

        // std::string m_sCurrentFilter;

        // unsigned int m_nCurrentIndex;
        // CursorPos m_nCursorPos;
        SDL_mutex *m_pLock;
        // bool m_bUpToDate;
        
        friend class PlayListIterator;

        void update();
        std::vector<Entry> const& getList() const {return m_aEntryList;}
        // CursorPos getCursorPos() const;
        // Cursor movement
//        CursorPos prev();
//        CursorPos next();
        
    private:
        PlayList(PlayList const&);
        PlayList& operator=(PlayList const&);

    public:
        PlayList();
        virtual ~PlayList();

        void setNewList(PlayList const& _aCurrentList);
        void append(Entry const& _sValue);
        
        Entry getElement(CursorPos _nOffset);
        CursorPos getSize() const;
        int findPosition(std::string const& _sName);

        
        // Interface to write down the data into a stream
        std::ostream& write(std::ostream &os);
        
        // Interface to read data from a given stream
        std::istream& read(std::istream &is);
        
        // Clear list, set Cursor to pos 0
        void clear();

        bool isDirectory(std::string const& _sName) const;
        void showListOnConsole();

        void lock();
        void unlock();

        // This function manipulates only the back values
        // it change nothing on the list itself.
        // getSize(),
        // getElement(nth), 
        // prev()
        // next()

//    public:
        // void setUpToDate()
        //     {
        //         m_bUpToDate = true;
        //     }
        // 
        // bool isUpToDate()
        //     {
        //         return m_bUpToDate;
        //     }
        
        virtual bool handleController(Controller *_pController);
    };

// -----------------------------------------------------------------------------
    class PlayListIterator
    {
        PlayList *m_pPlayList;
        CursorPos m_nStartPosition;
        std::string m_sCurrentFilter;
        CursorPos m_nRealPosition;

        bool isFilterAppear(std::string const& _sFilename, std::string const& _sFilter);
    private:
        PlayListIterator(PlayListIterator const&);
        PlayListIterator& operator=(PlayListIterator const&);
    public:
        PlayListIterator(PlayList *_pPlayList, CursorPos _nStart, std::string const& _sFilter);
        void resetPosition(CursorPos _nStart);

        void next();
        void prev();
        Entry getCurrentElement();
        CursorPos getSize();
        CursorPos getCursorPosition();
        void setCursorPosition(CursorPos _nPos);

        int findFilteredPosition(std::string const& _sFilename);
    };
    
} // namespace Audio

#endif
