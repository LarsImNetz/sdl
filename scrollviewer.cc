/*-*-c++-*-
 *
 *    $RCSfile: scrollviewer.cc,v $
 *     Project:
 * Description:
 *
 *   Author(s): Lars Langhans
 *       $Date: 2003/05/12 16:25:58 $
 *   Copyright: (c)1996 by Lars Langhans
 *   $Revision: 1.1 $
 */

// damit kann RCS im executeable noch die Version feststellen
// static char rcsid[] = "$Id: scrollviewer.cc,v 1.1 2003/05/12 16:25:58 langha_l Exp $";

#include "scrollviewer.h"

void ScrollViewer::down()
{
    -- m_nPos;
    if (m_nPos < m_nMinPos)
    {
        m_nPos = m_nMinPos;
        if (m_pParent) m_pParent->down();
    }
}

void ScrollViewer::up()
{
    ++ m_nPos;
    if (m_nPos > m_nMaxPos)
    {
        m_nPos = m_nMaxPos;
        if (m_pParent) m_pParent->up();
    }
    if (m_pParent) m_pParent->checkOnMax(m_nPos);
}

void ScrollViewer::checkOnMax(int _nPos = 0)
{
    if ((m_nPos + _nPos) > m_nMaxPos)
    {
        m_nPos = m_nMaxPos - _nPos;
    }
}

int ScrollViewer::getSelectionPos()
{
    int nValue = m_nPos;
    if (m_pParent)
    {
        nValue += m_pParent->getSelectionPos();
    }
    return nValue;
}



