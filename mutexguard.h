/*-*-c++-*-
 *
 *    $RCSfile: mutexguard.h,v $
 *     Project:
 * Description:
 *
 *   Author(s): Lars Langhans
 *       $Date: 2005/06/23 07:38:48 $
 *   Copyright: (c)1996 by Lars Langhans
 *   $Revision: 1.2 $
 */


#ifndef MUTEX_GUARD_H
#define MUTEX_GUARD_H

/**
 * Helper class for mutex handling, only MutexGuard x(m_pLock) is need,
 * the free function is build by the compiler
 */

#include <SDL_mutex.h>

class MutexGuard
{
    SDL_mutex *m_pLock;

private:
    MutexGuard(MutexGuard const&);
    MutexGuard& operator=(MutexGuard const&);

public:
    MutexGuard(SDL_mutex* _pLock)
            :m_pLock(_pLock)
        {
            // m_pLock = _pLock;
            SDL_mutexP(m_pLock);
        }
    ~MutexGuard()
        {
            SDL_mutexV(m_pLock);
        }
};

#endif
