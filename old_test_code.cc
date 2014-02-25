// -----------------------------------------------------------------------------
// int nSlash = sSoundFile.rfind('/');
// if (nSlash > 0)
// {
//     aStrDir = sSoundFile.substr(0, nSlash);
// }            

// LLA: IMHO is char* basename(char*) a better candidate

// -----------------------------------------------------------------------------
/*
// make_file_array(20);
printf("Directories:\n");
showList(aDir.getDirs());

printf("Files:\n");
showList(aDir.getFiles());

aDir.changeDir("current");
// aDir.changeDir("..");
// aDir.changeDir("..");
// aDir.changeDir("..");

printf("Files:\n");
showList(aDir.getFiles());

exit(0);
*/

//     aFont.settext(display, Color::white, 10,10, aStr.c_str());
//     int quit = 0;
//     // Event-Handling
//     while( quit == 0)
//     {
//        while( SDL_PollEvent( &event ) )
//        {     
//           switch( event.type )
//           {
//           case SDL_MOUSEMOTION:
//              {
//                 // printf( "Mausposition: x: %i und y: %i\n", event.motion.x, event.motion.y);
//              
//                 SDL_Rect drect = {event.motion.x, event.motion.y,0,0};
//              
//                 SDL_BlitSurface(image, &srect, display, &drect);
//                 // SDL_BlitSurface(pText,0,display,&drect);
//                 SDL_UpdateRects(display,1,&drect);
//                 aFont.setOutlineText(display, Color::white, event.motion.x,event.motion.y, "Ein anderer Text-Test.");
//              }
//              
// //#              // alte stelle wieder fuellen
// //#              SDL_BlitSurface(hintergrund, &drect, display, &drect);
// //#              SDL_UpdateRects(display,1,&drect);
// //#              
// //#              // Setzen des Quellbereichs
// //#              srect.x = 0;
// //#              srect.y = 0;
// //#              srect.w = image->w;  // das gesamte Bild
// //#              srect.h = image->h;   // das gesamte Bild
// //#              
// //#              // Setzen des Zielbereichs
// //#              drect.x = event.motion.x;
// //#              drect.y = event.motion.y;
// //#              drect.w = image->w;
// //#              drect.h = image->h;
// //#              
// //#              // kopiere surface auf display-surface
// //#              SDL_BlitSurface(image, &srect, display, &drect);
// //#              
// //#              // den veränderten Bildschirm-Bereich auffrischen
// //#              SDL_UpdateRects(display,1,&drect);
// //#              
//              break;
//              
//           case SDL_MOUSEBUTTONDOWN:     // Quit beim druecken des Mousbuttons
//              quit = 1;
//              break;
//              
//           case SDL_QUIT:
//              quit = 1;
//              break;
//           case SDL_KEYDOWN:
//              if (event.key.keysym.sym == SDLK_ESCAPE || 
//                  event.key.keysym.sym == SDLK_q || 
//                  event.key.keysym.sym == 'Q')
//              {
//                 quit = 1;
//              }
//              else
//              {
//                 quit = 0;
//              }
//              break;
//           default:
//               break;
//           }
//        }
//     }


class SelectedText
{
    SDL_Surface *m_pDisplay;
    int m_x;
    int m_y;
    int m_w;
    int m_h;
    std::string m_sSelectedText;
public:
    SelectedText()
            :m_pDisplay(NULL), m_x(0),m_y(0),m_w(0),m_h(0) {}
    
    SelectedText(SDL_Surface *_pDisplay, int x, int y, int w, int h, std::string const& _sSelection)
            :m_pDisplay(_pDisplay),
             m_x(x),
             m_y(y),
             m_w(w),
             m_h(h),
             m_sSelectedText(_sSelection)
        {}

    void paintRectAngle()
        {
            if (m_pDisplay)
            {
                rectangleColor(m_pDisplay, m_x, m_y, m_x + m_w - 1, m_y + m_h - 1, ColorToUint32(Color::white));

                // SDL_Rect drect        = {m_x, m_y, m_w, m_h};
                // SDL_UpdateRects(m_pDisplay, 1, &drect);
            }
        }
};
