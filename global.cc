/*-*-c++-*-
 *
 *    $RCSfile: global.cc,v $
 *     Project:
 * Description:
 *
 *   Author(s): Lars Langhans
 *       $Date: 2005/07/20 07:03:14 $
 *   Copyright: (c)1996 by Lars Langhans
 *   $Revision: 1.5 $
 */

#include <string>
#include <SDL.h>
#include <stdlib.h>
#include "t_print.h"

namespace Global
{

    bool shiftKey(SDL_Event const& _aEvent)
    {
        if (_aEvent.key.keysym.mod & KMOD_SHIFT != 0)
        {
            return true;
        }
        return false;
    }
    
//  TODO: more than one event should not occur in a short time frame, pack it together 

void createUserEvent(std::string const& _sStr)
{
    SDL_Event* pUserEvent = static_cast<SDL_Event*>(malloc(sizeof (SDL_UserEvent)));
    if (pUserEvent)
    {
        // t_print(T_DEBUG, "User event created. (%s)\n", _sStr.c_str());
        pUserEvent->type = SDL_USEREVENT;
        SDL_PushEvent(pUserEvent);
    }
    else
    {
        t_print(T_INFO, "Can't get memory for User Event\n");
        // exit(1);
    }
}

    std::string showKey(int _nKey)
    {
        std::string sKeyName;
        switch(_nKey)
        {
        case SDLK_BACKSPACE:
            sKeyName = "BACKSPACE";
            break;
        case SDLK_TAB:
            sKeyName = "TAB";
            break;
        case SDLK_CLEAR:
            sKeyName = "CLEAR";
            break;
        case SDLK_RETURN:
            sKeyName = "RETURN";
            break;
        case SDLK_PAUSE:
            sKeyName = "PAUSE";
            break;
        case SDLK_ESCAPE:
            sKeyName = "ESCAPE";
            break;
        case SDLK_SPACE:
            sKeyName = "SPACE";
            break;
        case SDLK_EXCLAIM:
            sKeyName = "EXCLAIM";
            break;
        case SDLK_QUOTEDBL:
            sKeyName = "QUOTEDBL";
            break;
        case SDLK_HASH:
            sKeyName = "HASH";
            break;
        case SDLK_DOLLAR:
            sKeyName = "DOLLAR";
            break;
        case SDLK_AMPERSAND:
            sKeyName = "AMPERSAND";
            break;
        case SDLK_QUOTE:
            sKeyName = "QUOTE";
            break;
        case SDLK_LEFTPAREN:
            sKeyName = "LEFTPAREN";
            break;
        case SDLK_RIGHTPAREN:
            sKeyName = "RIGHTPAREN";
            break;
        case SDLK_ASTERISK:
            sKeyName = "ASTERISK";
            break;
        case SDLK_PLUS:
            sKeyName = "PLUS";
            break;
        case SDLK_COMMA:
            sKeyName = "COMMA";
            break;
        case SDLK_MINUS:
            sKeyName = "MINUS";
            break;
        case SDLK_PERIOD:
            sKeyName = "PERIOD";
            break;
        case SDLK_SLASH:
            sKeyName = "SLASH";
            break;
        case SDLK_0:
            sKeyName = "0";
            break;
        case SDLK_1:
            sKeyName = "1";
            break;
        case SDLK_2:
            sKeyName = "2";
            break;
        case SDLK_3:
            sKeyName = "3";
            break;
        case SDLK_4:
            sKeyName = "4";
            break;
        case SDLK_5:
            sKeyName = "5";
            break;
        case SDLK_6:
            sKeyName = "6";
            break;
        case SDLK_7:
            sKeyName = "7";
            break;
        case SDLK_8:
            sKeyName = "8";
            break;
        case SDLK_9:
            sKeyName = "9";
            break;
        case SDLK_COLON:
            sKeyName = "COLON";
            break;
        case SDLK_SEMICOLON:
            sKeyName = "SEMICOLON";
            break;
        case SDLK_LESS:
            sKeyName = "LESS";
            break;
        case SDLK_EQUALS:
            sKeyName = "EQUALS";
            break;
        case SDLK_GREATER:
            sKeyName = "GREATER";
            break;
        case SDLK_QUESTION:
            sKeyName = "QUESTION";
            break;
        case SDLK_AT:
            sKeyName = "AT";
            break;
        case SDLK_LEFTBRACKET:
            sKeyName = "LEFTBRACKET";
            break;
        case SDLK_BACKSLASH:
            sKeyName = "BACKSLASH";
            break;
        case SDLK_RIGHTBRACKET:
            sKeyName = "RIGHTBRACKET";
            break;
        case SDLK_CARET:
            sKeyName = "CARET";
            break;
        case SDLK_UNDERSCORE:
            sKeyName = "UNDERSCORE";
            break;
        case SDLK_BACKQUOTE:
            sKeyName = "BACKQUOTE";
            break;
        case SDLK_a:
            sKeyName = "a";
            break;
        case SDLK_b:
            sKeyName = "b";
            break;
        case SDLK_c:
            sKeyName = "c";
            break;
        case SDLK_d:
            sKeyName = "d";
            break;
        case SDLK_e:
            sKeyName = "e";
            break;
        case SDLK_f:
            sKeyName = "f";
            break;
        case SDLK_g:
            sKeyName = "g";
            break;
        case SDLK_h:
            sKeyName = "h";
            break;
        case SDLK_i:
            sKeyName = "i";
            break;
        case SDLK_j:
            sKeyName = "j";
            break;
        case SDLK_k:
            sKeyName = "k";
            break;
        case SDLK_l:
            sKeyName = "l";
            break;
        case SDLK_m:
            sKeyName = "m";
            break;
        case SDLK_n:
            sKeyName = "n";
            break;
        case SDLK_o:
            sKeyName = "o";
            break;
        case SDLK_p:
            sKeyName = "p";
            break;
        case SDLK_q:
            sKeyName = "q";
            break;
        case SDLK_r:
            sKeyName = "r";
            break;
        case SDLK_s:
            sKeyName = "s";
            break;
        case SDLK_t:
            sKeyName = "t";
            break;
        case SDLK_u:
            sKeyName = "u";
            break;
        case SDLK_v:
            sKeyName = "v";
            break;
        case SDLK_w:
            sKeyName = "w";
            break;
        case SDLK_x:
            sKeyName = "x";
            break;
        case SDLK_y:
            sKeyName = "y";
            break;
        case SDLK_z:
            sKeyName = "z";
            break;
        case SDLK_DELETE:
            sKeyName = "DELETE";
            break;
        case SDLK_KP0:
            sKeyName = "KP0";
            break;
        case SDLK_KP1:
            sKeyName = "KP1";
            break;
        case SDLK_KP2:
            sKeyName = "KP2";
            break;
        case SDLK_KP3:
            sKeyName = "KP3";
            break;
        case SDLK_KP4:
            sKeyName = "KP4";
            break;
        case SDLK_KP5:
            sKeyName = "KP5";
            break;
        case SDLK_KP6:
            sKeyName = "KP6";
            break;
        case SDLK_KP7:
            sKeyName = "KP7";
            break;
        case SDLK_KP8:
            sKeyName = "KP8";
            break;
        case SDLK_KP9:
            sKeyName = "KP9";
            break;
        case SDLK_KP_PERIOD:
            sKeyName = "KP_PERIOD";
            break;
        case SDLK_KP_DIVIDE:
            sKeyName = "KP_DIVIDE";
            break;
        case SDLK_KP_MULTIPLY:
            sKeyName = "KP_MULTIPLY";
            break;
        case SDLK_KP_MINUS:
            sKeyName = "KP_MINUS";
            break;
        case SDLK_KP_PLUS:
            sKeyName = "KP_PLUS";
            break;
        case SDLK_KP_ENTER:
            sKeyName = "KP_ENTER";
            break;
        case SDLK_KP_EQUALS:
            sKeyName = "KP_EQUALS";
            break;
        case SDLK_UP:
            sKeyName = "UP";
            break;
        case SDLK_DOWN:
            sKeyName = "DOWN";
            break;
        case SDLK_RIGHT:
            sKeyName = "RIGHT";
            break;
        case SDLK_LEFT:
            sKeyName = "LEFT";
            break;
        case SDLK_INSERT:
            sKeyName = "INSERT";
            break;
        case SDLK_HOME:
            sKeyName = "HOME";
            break;
        case SDLK_END:
            sKeyName = "END";
            break;
        case SDLK_PAGEUP:
            sKeyName = "PAGEUP";
            break;
        case SDLK_PAGEDOWN:
            sKeyName = "PAGEDOWN";
            break;
        case SDLK_F1:
            sKeyName = "F1";
            break;
        case SDLK_F2:
            sKeyName = "F2";
            break;
        case SDLK_F3:
            sKeyName = "F3";
            break;
        case SDLK_F4:
            sKeyName = "F4";
            break;
        case SDLK_F5:
            sKeyName = "F5";
            break;
        case SDLK_F6:
            sKeyName = "F6";
            break;
        case SDLK_F7:
            sKeyName = "F7";
            break;
        case SDLK_F8:
            sKeyName = "F8";
            break;
        case SDLK_F9:
            sKeyName = "F9";
            break;
        case SDLK_F10:
            sKeyName = "F10";
            break;
        case SDLK_F11:
            sKeyName = "F11";
            break;
        case SDLK_F12:
            sKeyName = "F12";
            break;
        case SDLK_F13:
            sKeyName = "F13";
            break;
        case SDLK_F14:
            sKeyName = "F14";
            break;
        case SDLK_F15:
            sKeyName = "F15";
            break;
        case SDLK_NUMLOCK:
            sKeyName = "NUMLOCK";
            break;
        case SDLK_CAPSLOCK:
            sKeyName = "CAPSLOCK";
            break;
        case SDLK_SCROLLOCK:
            sKeyName = "SCROLLOCK";
            break;
        case SDLK_RSHIFT:
            sKeyName = "RSHIFT";
            break;
        case SDLK_LSHIFT:
            sKeyName = "LSHIFT";
            break;
        case SDLK_RCTRL:
            sKeyName = "RCTRL";
            break;
        case SDLK_LCTRL:
            sKeyName = "LCTRL";
            break;
        case SDLK_RALT:
            sKeyName = "RALT";
            break;
        case SDLK_LALT:
            sKeyName = "LALT";
            break;
        case SDLK_RMETA:
            sKeyName = "RMETA";
            break;
        case SDLK_LMETA:
            sKeyName = "LMETA";
            break;
        case SDLK_LSUPER:
            sKeyName = "LSUPER";
            break;
        case SDLK_RSUPER:
            sKeyName = "RSUPER";
            break;
        case SDLK_MODE:
            sKeyName = "MODE";
            break;
        case SDLK_COMPOSE:
            sKeyName = "COMPOSE";
            break;
        case SDLK_HELP:
            sKeyName = "HELP";
            break;
        case SDLK_PRINT:
            sKeyName = "PRINT";
            break;
        case SDLK_SYSREQ:
            sKeyName = "SYSREQ";
            break;
        case SDLK_BREAK:
            sKeyName = "BREAK";
            break;
        case SDLK_MENU:
            sKeyName = "MENU";
            break;
        case SDLK_POWER:
            sKeyName = "POWER";
            break;
        case SDLK_EURO:
            sKeyName = "EURO";
            break;
        case SDLK_UNDO:
            sKeyName = "UNDO";
            break;

        default:
            sKeyName = "Trottel, pfleg mal die DB.";
        }
        
        return sKeyName;
    }

    std::string showModifier(int _nModifier)
    {
        std::string sModifier;
        switch(_nModifier)
        {
        case KMOD_LSHIFT:
            sModifier = "LShift";
            break;
        case KMOD_RSHIFT:
            sModifier = "RShift";
            break;
        case KMOD_LCTRL:
            sModifier = "LCtrl";
            break;
        case KMOD_RCTRL:
            sModifier = "RCtrl";
            break;
        case KMOD_LALT:
            sModifier = "LAlt";
            break;
        case KMOD_RALT:
            sModifier = "RAlt";
            break;
        case KMOD_LMETA:
            sModifier = "LMeta";
            break;
        case KMOD_RMETA:
            sModifier = "RMeta";
            break;
        case KMOD_NUM:
            sModifier = "Num";
            break;
        case KMOD_CAPS:
            sModifier = "Caps";
            break;
        case KMOD_MODE:
            sModifier = "Mode";
            break;

        case KMOD_SHIFT:
            sModifier = "Shift";
            break;                
        case KMOD_CTRL:
            sModifier = "Ctrl";
            break;                
        case KMOD_ALT:
            sModifier = "Alt";
            break;                
        case KMOD_META:
            sModifier = "Meta";
            break;
        default:
            sModifier = "UNKNOWN MODIFIER";
        }
        return sModifier;
    }
    
    void documentKey(int _nKey, int _nModifier, std::string const& _sDocumentationForKey, int _nToKey=0)
    {
        std::string sKeys;
        if (_nModifier != 0)
        {
            sKeys = showModifier(_nModifier);
            sKeys += "-";
        }        
        sKeys += showKey(_nKey);
        if (_nToKey != 0)
        {
            sKeys += "-";
            sKeys += showKey(_nToKey);
        }
        t_print("Key:%s ", sKeys.c_str());
        int nTabSize = 10 + 6 - sKeys.size();
        for (int i =0;i<nTabSize;i++)
        {
            t_print(" ");
        }
        t_print("%s\n", _sDocumentationForKey.c_str());
    }

} // namespace Global
