#include "fixfont.hxx"
#include <SDL_image.h>
// #include <SDL_gfxPrimitives.h>

/*
  read all known alphas, store and hold the chars in a map
*/

#include "A_16x16.xpm"
#include "B_16x16.xpm"
#include "C_16x16.xpm"
#include "D_16x16.xpm"
#include "E_16x16.xpm"
#include "F_16x16.xpm"
#include "G_16x16.xpm"
#include "H_16x16.xpm"
#include "I_16x16.xpm"
#include "J_16x16.xpm"
#include "K_16x16.xpm"
#include "L_16x16.xpm"
#include "M_16x16.xpm"
#include "N_16x16.xpm"
#include "O_16x16.xpm"
#include "P_16x16.xpm"
#include "Q_16x16.xpm"
#include "R_16x16.xpm"
#include "S_16x16.xpm"
#include "T_16x16.xpm"
#include "U_16x16.xpm"
#include "V_16x16.xpm"
#include "W_16x16.xpm"
#include "X_16x16.xpm"
#include "Y_16x16.xpm"
#include "Z_16x16.xpm"
#include "digits_16x16.xpm"
#include "interpunction_16x16.xpm"
#include "box_16x16.xpm"

void FixFont::fillArray()
{
    m_aFontMap["A"] = IMG_ReadXPMFromArray((char **)A_16x16_xpm);
    m_aFontMap["B"] = IMG_ReadXPMFromArray((char **)B_16x16_xpm);
    m_aFontMap["C"] = IMG_ReadXPMFromArray((char **)C_16x16_xpm);
    m_aFontMap["D"] = IMG_ReadXPMFromArray((char **)D_16x16_xpm);
    m_aFontMap["E"] = IMG_ReadXPMFromArray((char **)E_16x16_xpm);
    m_aFontMap["F"] = IMG_ReadXPMFromArray((char **)F_16x16_xpm);
    m_aFontMap["G"] = IMG_ReadXPMFromArray((char **)G_16x16_xpm);
    m_aFontMap["H"] = IMG_ReadXPMFromArray((char **)H_16x16_xpm);
    m_aFontMap["I"] = IMG_ReadXPMFromArray((char **)I_16x16_xpm);
    m_aFontMap["J"] = IMG_ReadXPMFromArray((char **)J_16x16_xpm);
    m_aFontMap["K"] = IMG_ReadXPMFromArray((char **)K_16x16_xpm);
    m_aFontMap["L"] = IMG_ReadXPMFromArray((char **)L_16x16_xpm);
    m_aFontMap["M"] = IMG_ReadXPMFromArray((char **)M_16x16_xpm);
    m_aFontMap["N"] = IMG_ReadXPMFromArray((char **)N_16x16_xpm);
    m_aFontMap["O"] = IMG_ReadXPMFromArray((char **)O_16x16_xpm);
    m_aFontMap["P"] = IMG_ReadXPMFromArray((char **)P_16x16_xpm);
    m_aFontMap["Q"] = IMG_ReadXPMFromArray((char **)Q_16x16_xpm);
    m_aFontMap["R"] = IMG_ReadXPMFromArray((char **)R_16x16_xpm);
    m_aFontMap["S"] = IMG_ReadXPMFromArray((char **)S_16x16_xpm);
    m_aFontMap["T"] = IMG_ReadXPMFromArray((char **)T_16x16_xpm);
    m_aFontMap["U"] = IMG_ReadXPMFromArray((char **)U_16x16_xpm);
    m_aFontMap["V"] = IMG_ReadXPMFromArray((char **)V_16x16_xpm);
    m_aFontMap["W"] = IMG_ReadXPMFromArray((char **)W_16x16_xpm);
    m_aFontMap["X"] = IMG_ReadXPMFromArray((char **)X_16x16_xpm);
    m_aFontMap["Y"] = IMG_ReadXPMFromArray((char **)Y_16x16_xpm);
    m_aFontMap["Z"] = IMG_ReadXPMFromArray((char **)Z_16x16_xpm);
// digits
    m_aFontMap["0"] = IMG_ReadXPMFromArray((char **)Zero_16x16_xpm);
    m_aFontMap["1"] = IMG_ReadXPMFromArray((char **)One_16x16_xpm);
    m_aFontMap["2"] = IMG_ReadXPMFromArray((char **)Two_16x16_xpm);
    m_aFontMap["3"] = IMG_ReadXPMFromArray((char **)Three_16x16_xpm);
    m_aFontMap["4"] = IMG_ReadXPMFromArray((char **)Four_16x16_xpm);
    m_aFontMap["5"] = IMG_ReadXPMFromArray((char **)Five_16x16_xpm);
    m_aFontMap["6"] = IMG_ReadXPMFromArray((char **)Six_16x16_xpm);
    m_aFontMap["7"] = IMG_ReadXPMFromArray((char **)Seven_16x16_xpm);
    m_aFontMap["8"] = IMG_ReadXPMFromArray((char **)Eight_16x16_xpm);
    m_aFontMap["9"] = IMG_ReadXPMFromArray((char **)Nine_16x16_xpm);
// interpunction
    m_aFontMap["."] = IMG_ReadXPMFromArray((char **)Point_16x16_xpm);
    m_aFontMap[":"] = IMG_ReadXPMFromArray((char **)DoublePoint_16x16_xpm);
    m_aFontMap[";"] = IMG_ReadXPMFromArray((char **)Semicolon_16x16_xpm);
    m_aFontMap[","] = IMG_ReadXPMFromArray((char **)Comma_16x16_xpm);
    m_aFontMap["_"] = IMG_ReadXPMFromArray((char **)Underscore_16x16_xpm);
// calculation
//     m_aFontMap["+"] = IMG_ReadXPMFromArray(Plus_16x16_xpm);
//     m_aFontMap["-"] = IMG_ReadXPMFromArray(Minus_16x16_xpm);
//     m_aFontMap["*"] = IMG_ReadXPMFromArray(Mul_16x16_xpm);
//     m_aFontMap["/"] = IMG_ReadXPMFromArray(Div_16x16_xpm);
//     m_aFontMap["="] = IMG_ReadXPMFromArray(Equal_16x16_xpm);
//     m_aFontMap["("] = IMG_ReadXPMFromArray(OpenBrace_16x16_xpm);
//     m_aFontMap[")"] = IMG_ReadXPMFromArray(CloseBrace_16x16_xpm);
// 
//     m_aFontMap["!"] = IMG_ReadXPMFromArray(Attention_16x16_xpm);
//     m_aFontMap["?"] = IMG_ReadXPMFromArray(Question_16x16_xpm);
    // TODO:

    m_pEmptyBox = IMG_ReadXPMFromArray((char **)box_16x16_xpm);
}

FixFont::FixFont(SDL_Surface *_pDisplay)
        :m_pDisplay(_pDisplay),
         m_pEmptyBox(NULL)
{
    // as late as possible
    // fillArray();
}

FixFont::~FixFont()
{
    for (MapStringSDL_Surface::const_iterator it=m_aFontMap.begin();
         it != m_aFontMap.end();
         it++)
    {
        std::pair<std::string, SDL_Surface*> aPair = *it;
        SDL_FreeSurface(aPair.second);
    }
    if (m_pEmptyBox) 
    {
        SDL_FreeSurface(m_pEmptyBox);
    }
}

SDL_Surface* FixFont::getSurfaceFromChar(std::string const& _aChar)
{
    if (m_aFontMap.size() == 0)
    {
        fillArray();
    }
    SDL_Surface *pSurface = m_aFontMap[_aChar];
    if (pSurface == NULL)
    {
        pSurface = m_pEmptyBox;
    }    
    return pSurface;
}

Size FixFont::setChar(std::string const& _aCharName, Point const& _aPos)
{
    SDL_Surface *pSurface = getSurfaceFromChar(_aCharName);
    SDL_Rect drect = {_aPos.x, _aPos.y, 0, 0};
    Size aSize = {16, 16};
    if (pSurface)
    {
        SDL_BlitSurface(pSurface, 0, m_pDisplay, &drect);
        Size aSize = {drect.w, drect.h};
        // (void)nNewX;
        return aSize;
    }
    return aSize;
}

Size FixFont::setText(std::string const& _sStr, Point const& _aPos)
{
    const int nLength = _sStr.length();
    Size aReturnSize = {0, 0};
    Point aPos = _aPos;
    for (int i=0; i<nLength; ++i)
    {
        Size aSize = setChar(_sStr.substr(i,1), aPos);
        aPos.x += aSize.w;
        aReturnSize.w += aSize.w;
        aReturnSize.h = aSize.h;
    }
    return aReturnSize;
}
