/*-*-c++-*-
 *
 *    $RCSfile: scrollviewer.h,v $
 *     Project:
 * Description:
 *
 *   Author(s): Lars Langhans
 *       $Date: 2005/06/23 07:38:48 $
 *   Copyright: (c)1996 by Lars Langhans
 *   $Revision: 1.2 $
 */

/* ==========================================================================
 *
 *               HISTORY
 *               -------
 *       $Log: scrollviewer.h,v $
 *       Revision 1.2  2005/06/23 07:38:48  langha_l
 *       start with -Weffc++ cleanups
 *
 *       Revision 1.1  2003/05/12 16:25:58  langha_l
 *       lot of changes
 *
 */

#ifndef _scrollviewer_h
#define _scrollviewer_h

// -----------------------------------------------------------------------------
// Diese Klasse hilft uns beim scrollen
// sie erlaubt in ein ander geschachtelte Scroller
// So ist der Erste Sroller immer(!) mit dem gesamten zu scrollenden Bereich zu initialisieren
// der naechste dann nur noch mit dem Ausschnitt

class ScrollViewer
{
    // friend class ScrollViewer;
    ScrollViewer *m_pParent;
    int m_nPos;
    int m_nMinPos;
    int m_nMaxPos;

private:
    ScrollViewer(ScrollViewer const&);
    ScrollViewer& operator=(ScrollViewer const&);
public:
    ScrollViewer(ScrollViewer* _pParent, int min, int max, int _nCurrent = -1)
            :m_pParent(_pParent),
             m_nPos(min),
             m_nMinPos(min),
             m_nMaxPos(max)
        {
            if (_nCurrent != -1)
                m_nPos = _nCurrent;
        }
    void up();
    void down();
    int getSelectionPos();
    int getMinPos() {return m_nMinPos;}
    int getMaxPos() {return m_nMaxPos;}
    void checkOnMax(int nChildPos);
    // int getPos() {return m_nPos;}
};



#endif
