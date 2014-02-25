#ifndef VIEWDIRECTORY_H
#define VIEWDIRECTORY_H

#include <SDL.h>
#include "scrollviewer.h"
#include "ttffont.h"
#include "playlist.h"
#include "i_dispatch.h"
#include "i_view.h"
#include "controller.h"
#include "i_handlecontroller.h"

enum ViewType {
    VIEW_TYPE_NOTHING=0,
    VIEW_TYPE_DIRECTORY=1,
    VIEW_TYPE_SIMPLELIST=2
};

class DoubleScroller
{
    ScrollViewer *m_pMainFieldScroller;
    ScrollViewer *m_pScroller;

private:
    DoubleScroller(DoubleScroller const&);
    DoubleScroller& operator=(DoubleScroller const&);
public:
    DoubleScroller()
            :m_pMainFieldScroller(NULL),
             m_pScroller(NULL)
        {}
    ~DoubleScroller();
    // Navigation
    void up();
    void down();

    int getSelectionPos();
    void setSelectionPos(int _n);
    void refreshScroller(int _nSize, int _nGlobalPosition, int _nMaxSize);

    // return the position in the MainField
    int getGlobalPos();
    
    // return the min pos in the last scroller
    int getMinPos();
    
    // return the max pos in the last scroller
    int getMaxPos();

};
// -----------------------------------------------------------------------------
class Quality;

class ViewDirectory 
    : public interface_Dispatch,
      public interface_View,
      public ControllerHelper,
      public interface_handleController
{
    int m_nLines;
    int m_x;
    int m_y;
    int m_w;
    int m_nOffset;

    int m_yUpdate;
    SDL_Surface *m_pDisplay;
    SDL_Surface **m_pBackgrounds;
    SDL_Surface **m_pTextSurfaces;
    TTFFont     *m_pFont;

    Audio::PlayList *m_pPlayList;
    Audio::PlayListIterator *m_pPlayListIterator;

    DirectoryHelper *m_pDirectoryHelper;
    SDL_mutex *m_pLock;

    ViewType m_eType;

    DoubleScroller m_aScroller;
    short m_nUpdateIsNeed;
    
    std::string m_sCurrentFilter;
    std::string m_sCurrentFilename;

    std::string m_sSpecialFilename; // this filename will be shown in other Color

    int m_nCurrentRealSelectionPos;
    int m_nNextScrollerPosition;

    Quality* m_pQuality;            // Quality DB

    void setUpdateIsNeed(bool _bValue);
    bool isUpdateNeed();

    void paintValues();

    void setFilter(std::string const& _sCurrentFilter);
    void setSpecialFilename(std::string const& _sCurrentFilter);

    void clearSurface(SDL_Surface* _pSurface);
    void copyBackground();
    void initBackground();
private:
    ViewDirectory(ViewDirectory const&);
    ViewDirectory& operator=(ViewDirectory const&);
public:

    ViewDirectory( TTFFont *_pFont, DirectoryHelper *_pDirHelper );
    virtual ~ViewDirectory();

    void update();
    
    // bool isDirectory() const {return m_bIsDirectory;}
    ViewType getType() const {return m_eType;}
    void setType(ViewType _eType) {m_eType = _eType;}
    
    // void setDirectory(bool _bIsDirectory) {m_bIsDirectory = _bIsDirectory;}
    void setList(Audio::PlayList * _pPlayList, bool _bIsFilterChange);

    // bool isDirectory(std::string const& _sName);
    void changeDir(std::string const& _sName, std::string const& _sLastDirName);
    Audio::PlayList* getPlayList() {return m_pPlayList;}

    int getCurrentSelectionPos();
    Audio::Entry getCurrentSelectionValue();
    void setCursorPos(int _nCursorPos);

    // Navigation
    void up();
    void down();

    // handle keys (Navigation) for ViewDirectory
    // interface Dispatch
    virtual bool dispatch(SDL_Event const& _aEvent);

    // interface View
    virtual bool paint(SDL_Surface* _pSurface);

    virtual bool handleController(Controller *_pController);

    void setQuality(Quality* _pQuality)
        {
            m_pQuality = _pQuality;
        }
    
};
#endif
