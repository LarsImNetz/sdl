#include "ColorHelper.h"

Uint32 ColorToUint32(SDL_Color const& _aColor, int _nAlpha /* =0xff */ )
{
    Uint32 rgba;
    rgba = (_aColor.r << 24);
    rgba |= (_aColor.g << 16);
    rgba |= (_aColor.b << 8);
    rgba |= /* _aColor.a */ _nAlpha;
    return rgba;
}

Uint32 toUint32(SDL_Color const& _aColor)
{
    return ColorToUint32(_aColor);
}

// Uint32 toUint32(short _nRed, short _nGreen, short _nBlue)
// {
//     Uint32 rgba;
//     rgba = ((_nRed & 0xff) << 24);
//     rgba |= ((_nGreen & 0xff) << 16);
//     rgba |= ((_nBlue & 0xff) << 8);
//     rgba |= /* _aColor.a */ 255;
//     return rgba;
// }


Uint32 toUint32(short _nRed, short _nGreen, short _nBlue, short _nAlpha)
{
    Uint32 rgba;
    rgba = ((_nRed & 0xff) << 24);
    rgba |= ((_nGreen & 0xff) << 16);
    rgba |= ((_nBlue & 0xff) << 8);
    rgba |= /* _aColor.a */ (_nAlpha & 0xff);
    return rgba;
}
