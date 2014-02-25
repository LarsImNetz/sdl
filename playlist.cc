#include <string>
#include <vector>
#include <locale>
#include<algorithm>

#include "ioextend.h"
#include "playlist.h"
#include "t_print.h"
#include "mutexguard.h"
#include "property.h"
#include "controller.h"
#include "filehelper.h"

namespace Audio
{    
    std::ostream& Entry::write(std::ostream &os) const
    {
        writeTyp(os, "Entry");
        writeObject(os, m_sPath);
        writeObject(os, m_sFilename);
        writeObject(os, m_sFileType);
        writeObject(os, m_nSystemType);
        return os;
    }
    
    std::istream& Entry::read(std::istream &is)
    {
        std::string sTyp = readTyp(is);
        if (sTyp == "Entry")
        {
            m_sPath = readString(is);
            m_sFilename = readString(is);
//  TODO: Der FileType sollte kein String sondern eine Nummer sein (Optimize!) 
            m_sFileType = readString(is);
            m_nSystemType = readInt(is);
        }
        
        return is;
    }
    
// -----------------------------------------------------------------------------

    // void PlayList::update()
    // {
    //? Hmmm... seems to be not usable any longer
    // if (m_pList)
    // {
    //     delete []m_pList;
    // }
    // m_pList = new const char*[ m_aCurrentList.size() + 1];
    // int nIndex = 0;
    // for (std::vector<std::string>::const_iterator it=m_aCurrentList.begin();
    //      it != m_aCurrentList.end();
    //      it++)
    // {
    //     const char* pStr = it->c_str();
    //     // t_print( "%s\n", pStr);
    //     m_pList[nIndex++] = pStr;
    // }
    // // break condition
    // m_pList[nIndex] = NULL;
    // }
    PlayList::PlayList() 
            :/* m_pList(NULL), */
            /* m_nCursorPos(0),*/
             m_pLock(SDL_CreateMutex())
             /* m_bUpToDate(false) */
    {
        t_print(T_FLOWCONTROL, "PlayList::PlayList()\n");
    }

    PlayList::~PlayList()
    {
        SDL_DestroyMutex(m_pLock);
    }
    
    void PlayList::setNewList(PlayList const& _aPlayList)
    {
        t_print(T_FLOWCONTROL, "PlayList::setNewList()\n");
        MutexGuard a(m_pLock);

        //? deep copy?
        std::vector<Entry> const& aEntryList = _aPlayList.getList();
        // m_nCursorPos = 0;

        for(std::vector< Entry >::const_iterator it = aEntryList.begin();
            it != aEntryList.end();
            ++it)
        {
            Entry const& aEntry = *it;
            m_aEntryList.push_back(aEntry);
            // t_print(T_INFO, "* %s\n", aEntry.getFilename().c_str());
        }
    }
        

        
    void PlayList::append(Entry const& _sValue)
    {
        t_print(T_FLOWCONTROL, "PlayList::append()\n");
        MutexGuard a(m_pLock);

        m_aEntryList.push_back(_sValue);
        // update();
    }
        
    // void PlayList::begin()
    // {
    //     m_nCurrentIndex = 0;
    // }
    
    CursorPos PlayList::getSize() const 
    {
        t_print(T_FLOWCONTROL, "PlayList::getSize()\n");
        return m_aEntryList.size();
    }
    
    // bool PlayList::hasMoreElements() const
    // {
    //     if (m_nCurrentIndex < m_aEntryList.size()) // if (m_pList[m_nCurrentIndex] != NULL)
    //     {
    //         return true;
    //     }
    //     return false;
    // }
    
    // Entry PlayList::getCurrentElement(int _nOffset) const
    // {
    //     // assert(m_pList != NULL);
    //     
    //     if (m_nCurrentIndex + _nOffset < m_aEntryList.size())
    //     {
    //         return Entry(m_aEntryList[m_nCurrentIndex + _nOffset]);
    //     }
    //     return Entry();
    // }
    
    // Entry PlayList::getNextElement()
    // {
    //     Entry sNext;
    //     sNext = getCurrentElement();
    //     if (sNext.getSystemType() != -1)
    //     {
    //         m_nCurrentIndex++;
    //     }
    //     return sNext;
    // }
    
    Entry PlayList::getElement(CursorPos _nOffset)
    {
        static Entry aEmptyEntry = Entry();
        
        t_print(T_FLOWCONTROL, "PlayList::getElement(%d)\n", _nOffset);
        MutexGuard a(m_pLock);

        // t_print(T_DEBUG, "   Offset: %d\n", _nOffset);
        // t_print(T_DEBUG, "Real size: %d\n\n", (CursorPos)m_aEntryList.size());
        if (_nOffset >= static_cast<CursorPos>(m_aEntryList.size()) || _nOffset < 0)
        {
            return aEmptyEntry;
        }
        return m_aEntryList[_nOffset];
    }
    
    // Interface to write down the data into a stream
    std::ostream& PlayList::write(std::ostream &os)
    {
        // writeVector(os, m_aEntryList);
        /* PRE  : */                                                                 
        /* POST : */                                                                 
        
        /* LLA?: how can I be save it's a vector<X>???  */
        
        MutexGuard a(m_pLock);

        int nItems = m_aEntryList.size();
        writeNumber(os, nItems);                        /* Anzahl der ArrayItems */ 
        
        for(std::vector< Entry >::const_iterator it = m_aEntryList.begin();
            it != m_aEntryList.end();
            ++it)
        {
            Entry const& aEntry = *it;
            aEntry.write(os);
        }
        return os;
    }

    // Interface to read data from a given stream
    std::istream& PlayList::read(std::istream &is)
    {
        // readVector(is, m_aEntryList);
        MutexGuard a(m_pLock);

        int nNum = readNumber(is);
        if (nNum > 0)
        {
            for (int n=0;n<nNum;n++)
            {
                Entry aEntry;
                aEntry.read(is);
                m_aEntryList.push_back(aEntry);
            }
        }
        
        // update();
        return is;
    }

    void PlayList::clear()
    {
        t_print(T_FLOWCONTROL, "PlayList::clear()\n");
        MutexGuard a(m_pLock);

        // m_pList = NULL;
        // m_nCursorPos = 0;
        m_aEntryList.clear();
        // m_sCurrentFilter = std::string();
    }
    
    bool PlayList::isDirectory(std::string const& _sName) const
    {
        t_print(T_FLOWCONTROL, "PlayList::isDirectory()\n");
        bool bFound = false;
    
        for(std::vector< Entry >::const_iterator it = m_aEntryList.begin();
            it != m_aEntryList.end();
            ++it)
        {
            Entry const& aEntry = *it;

            std::string sName = aEntry.getFilename();
            if (_sName == sName && aEntry.getSystemType() == ENTRY_ISDIRECTORY)
            {
                bFound = true;
                break;
            }
        }
        return bFound;
    }

    void PlayList::lock()
    {
//  TODO: mechanism to lock the current list 
        SDL_mutexP(m_pLock);
    }
    
    void PlayList::unlock()
    {
//  TODO: mechanism to unlock the current list 
        SDL_mutexV(m_pLock);
    }
    
    void PlayList::showListOnConsole()
    {
        /* PRE  : */                                                                 
        /* POST : */
        
        t_print(T_INFO, "Show List\n");
        for(std::vector< Entry >::const_iterator it = m_aEntryList.begin();
            it != m_aEntryList.end();
            ++it)
        {
            Entry const& aEntry = *it;
            t_print(T_INFO, "%s\n", aEntry.getFilename().c_str());
        }
    }

//     void PlayList::setFilter(std::string const& _sCurrentFilterName)
//     {
//         m_sCurrentFilter = _sCurrentFilterName;
//         // refreshes are necessary, so caches have to clean up.
//         // m_bUpToDate = false;
//     }
    

// -----------------------------------------------------------------------------
// // Functor, Stringvergleich ohne Beruecksichtigung der
// // Gross-/Kleinschreibung
// struct ignorecase
// {
//     bool operator()(const string& s1, const string& s2) const
//     {
//         string::const_iterator iter1 = s1.begin();
//         string::const_iterator iter2 = s2.begin();
//         while ((iter1!=s1.end()) && (iter2!=s2.end()))
//         {
//             // tolower aus locale. Beruecksichtigt die
//             // landesspezifischen Umwandlungen
//             char cs1 = std::tolower(*iter1);
//             char cs2 = std::tolower(*iter2);
//             if (cs1<cs2)
//                 return true;
//             if (cs1>cs2)
//                 return false;
//             ++iter1;
//             ++iter2;
//         }
//         return false;
//     }
// };
// -----------------------------------------------------------------------------
    PlayListIterator::PlayListIterator(PlayList *_pPlayList, CursorPos _nStart, std::string const& _sFilter)
            :m_pPlayList(_pPlayList),
             m_nStartPosition(_nStart),
             m_sCurrentFilter(_sFilter),
             m_nRealPosition(_nStart)
    {}
    
    void PlayListIterator::resetPosition(CursorPos _nStart)
    {
        t_print(T_FLOWCONTROL, "PlayListIterator::resetPosition()\n");
        if (_nStart < 0)
        {
            t_print(T_DEBUG, "PlayListIterator::resetPosition() ERROR: start value must not smaller 0.\n");
            _nStart = 0;
        }
        
        m_nStartPosition = _nStart;
        m_nRealPosition = _nStart;
        
        m_pPlayList->lock();
        if (m_sCurrentFilter.length() != 0)
        {
            while (m_nRealPosition < m_pPlayList->getSize())
            {
                Entry aEntry = m_pPlayList->getElement(m_nRealPosition); // get next element
                std::string sFilename = aEntry.getFilename();
                if (isFilterAppear(sFilename, m_sCurrentFilter))
                {
                    break;
                }
                else
                {
                    ++ m_nRealPosition;
                }
            }
        }
        m_pPlayList->unlock();
    }
    
    // return true as long as the _sFilter occur in the _sFilename
    // sample true == isFilterAppear("test", "es");
    // sample false == isFilterAppear("test", "x");

    bool PlayListIterator::isFilterAppear(std::string const& _sFilename, std::string const& _sFilter)
    {
        t_print(T_FLOWCONTROL, "PlayListIterator::isFilterAppear()\n");
        std::string sLowerFilename = _sFilename;
        std::transform(sLowerFilename.begin(), sLowerFilename.end(),
                       sLowerFilename.begin(),
                       static_cast<int(*)(int)>(std::tolower));
        // t_print(T_DEBUG, "tolower: %s\n", sLowerFilename.c_str());
        // t_print(T_DEBUG, "       : %s\n", _sFilter.c_str());

        if (sLowerFilename.find(_sFilter) != static_cast<unsigned int>(-1))
        {
            // t_print(T_DEBUG, "       : Filter occur\n\n" );
            return true;
        }
        return false;
    }
    
    // increment,
    // if filter is set, until we arrive an entry with isFilterAppear() is true
    void PlayListIterator::next()
    {
        t_print(T_FLOWCONTROL, "PlayListIterator::next()\n");
        m_pPlayList->lock();
        if (m_sCurrentFilter.length() != 0)
        {
            while (m_nRealPosition < m_pPlayList->getSize())
            {
                Entry aEntry = m_pPlayList->getElement(++ m_nRealPosition); // get next element
                std::string sFilename = aEntry.getFilename();
                if (isFilterAppear(sFilename, m_sCurrentFilter))
                {
                    break;
                }
            }
        }
        else
        {
            ++ m_nRealPosition;
        }
        m_pPlayList->unlock();
    }
    
    // decrement,
    // if filter is set, until we arrive an entry with isFilterAppear() is true
    void PlayListIterator::prev()
    {
        t_print(T_FLOWCONTROL, "PlayListIterator::prev()\n");
        m_pPlayList->lock();
        if (m_sCurrentFilter.length() != 0)
        {
            while (m_nRealPosition > 0)
            {
                Entry aEntry = m_pPlayList->getElement(-- m_nRealPosition); // get prev element
                std::string sFilename = aEntry.getFilename();
                if (isFilterAppear(sFilename, m_sCurrentFilter))
                {
                    break;
                }
            }
        }
        else
        {
            -- m_nRealPosition;
        }
        m_pPlayList->unlock();
    }
    
    Entry PlayListIterator::getCurrentElement()
    {
        t_print(T_FLOWCONTROL, "PlayListIterator::getCurrentElement()\n");
        m_pPlayList->lock();
        // LLA: the next and prev methods are the only posibilties to move the cursor
        //      forward / backward, so here we only have to give back the currentElement
        Entry aEntry = m_pPlayList->getElement(m_nRealPosition);
        m_pPlayList->unlock();
        return aEntry;
    }

    // return the size of the internal list, as long as the filter isn't set, it's easy
    //     is the filter set, we must run through the list and only if isFilterAppear() returns true
    //     we can increase the size.
    CursorPos PlayListIterator::getSize()
    {
        t_print(T_FLOWCONTROL, "PlayListIterator::getSize()\n");
        m_pPlayList->lock();
        CursorPos nSize = 0;
        if (m_sCurrentFilter.length() != 0)
        {
            // t_print(T_DEBUG, "Content in PlayListIterator::getSize()\n");
            for (std::vector<Entry>::const_iterator it=m_pPlayList->m_aEntryList.begin();
                 it != m_pPlayList->m_aEntryList.end();
                 it++)
            {
                Entry aEntry = *it;
                std::string sFilename = aEntry.getFilename();
                if (isFilterAppear(sFilename, m_sCurrentFilter))
                {
                    // t_print(T_DEBUG, " : %s\n", sFilename.c_str());
                    ++nSize;
                }
            }
            t_print(T_DEBUG, "PlayListIterator::getSize():=%d\n\n", nSize);
        }
        else
        {
            nSize = m_pPlayList->getSize();
        }
        m_pPlayList->unlock();
        return nSize;
    }

    CursorPos PlayListIterator::getCursorPosition()
    {
        t_print(T_FLOWCONTROL, "PlayListIterator::getCursorPosition()\n");
        return m_nRealPosition;
    }

    void PlayListIterator::setCursorPosition(CursorPos _nPos)
    {
        t_print(T_FLOWCONTROL, "PlayListIterator::()\n");
        m_nRealPosition = _nPos;
    }

    // 
    int PlayListIterator::findFilteredPosition(std::string const& _sFilename)
    {
        int nPosition = 0;
        t_print(T_FLOWCONTROL, "PlayListIterator::findFilteredPosition()\n");
        for (std::vector<Entry>::const_iterator it=m_pPlayList->m_aEntryList.begin();
             it != m_pPlayList->m_aEntryList.end();
                 it++)
        {
            Entry aEntry = *it;
            std::string sFilename = aEntry.getFilename();
            if (isFilterAppear(sFilename, m_sCurrentFilter))
            {
                if (sFilename == _sFilename)
                {
                    return nPosition;
                }
                ++nPosition;
            }
        }
        return -1;
    }
    
    int PlayList::findPosition(std::string const& _sName)
    {
        int nPosition = 0;
        t_print(T_FLOWCONTROL, "PlayList::findPosition()\n");
        for (std::vector<Entry>::const_iterator it=m_aEntryList.begin();
             it != m_aEntryList.end();
                 it++)
        {
            Entry aEntry = *it;
            std::string sFilename = aEntry.getFilename();
            if (sFilename == _sName)
            {
                return nPosition;
            }
            ++nPosition;
        }
        return -1;
    }
    
// interface handleController
bool PlayList::handleController(Controller *_pController)
{
    bool bReturn = false;
    if (_pController != NULL)
    {
    }
    
    return bReturn;
}

} // namespace Audio
