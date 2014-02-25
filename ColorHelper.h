#ifndef SDL_HELPER
#define SDL_HELPER

#include <SDL.h>

Uint32 ColorToUint32(SDL_Color const& _aColor, int _nAlpha=0xff);
Uint32 toUint32(SDL_Color const& _aColor);
Uint32 toUint32(short _nRed, short _nGreen, short _nBlue, short _nAlpha=0xff);

#endif
