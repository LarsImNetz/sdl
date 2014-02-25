#ifndef FIXFONT_HXX
#define FIXFONT_HXX

#include <map>
#include <string>
#include <SDL.h>

struct Point
{
	Sint16 x, y;
};
struct Size
{
	Sint16 w, h;
};

class FixFont
{
    typedef std::map<std::string, SDL_Surface *> MapStringSDL_Surface;
    MapStringSDL_Surface m_aFontMap;
    SDL_Surface *m_pEmptyBox;
    SDL_Surface *m_pDisplay;

    void fillArray();

    // NoCopy
    FixFont(FixFont const&);
    FixFont& operator=(FixFont const&);

    SDL_Surface* getSurfaceFromChar(std::string const& _aChar);
public:
    
    FixFont(SDL_Surface *_pDisplay);
    ~FixFont();
    
    Size setChar(std::string const& _aCharName, Point const& _aRect);
    Size setText(std::string const& _aAlpha, Point const& _aRect);
};

#endif
