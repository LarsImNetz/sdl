#include <stdlib.h>
#include <iostream.h>
#include "SDL.h"
#include "SDL_image.h"

int main()
{
  // Die zwei bereits bekannten Surfaces deklarieren
  SDL_Surface *display;
  SDL_Surface *image;
  SDL_Surface *hintergrund;
  
  SDL_Event event;
  int quit = 0;

  SDL_Rect drect;
  SDL_Rect srect;
  
  if ( SDL_Init( SDL_INIT_VIDEO) < 0 )
  {
     fprintf(stderr, "SDL konnte nicht initialisiert werden:  %s\n", SDL_GetError());
     exit(1);
   }

   atexit(SDL_Quit);
   
   display = SDL_SetVideoMode( 800, 600, 16, SDL_SWSURFACE);
   if ( display == NULL )
   {
     fprintf(stderr, "Konnte kein Fenster: Auflösung 800x600 oeffnen werden: %s\n", SDL_GetError());
     exit(-1);
   }

  // Bild laden
  image = IMG_Load("tux-transparent.gif");
  if (image == NULL)
  {
     fprintf(stderr, "Das Bild konnte nicht geladen werden: %s\n", SDL_GetError());
     exit(-1);
  }

  // Hintergrundbild laden
  hintergrund = IMG_Load("dawn.jpg");
  if (hintergrund == NULL)
  {
     fprintf(stderr, "Das Bild konnte nicht geladen werden: %s\n", SDL_GetError());
     exit(-1);
  }

  // Mauszeiger verstecken
  SDL_ShowCursor(0);
  
  // Kompletten Hintergrund aufbauen
 SDL_BlitSurface( hintergrund, 0, display, 0 );
 SDL_Flip( display );
  
  // Event-Handling  
        while( quit == 0)
        {
            while( SDL_PollEvent( &event ) )
            {
                switch( event.type )
                {
                    case SDL_MOUSEMOTION:
                    printf( "Mausposition: x: %i und y: %i\n", event.motion.x, event.motion.x);

                   // alte stelle wieder fuellen
                   SDL_BlitSurface(hintergrund, &drect, display, &drect);
                   SDL_UpdateRects(display,1,&drect);

                   // Setzen des Quellbereichs
                   srect.x = 0;
                   srect.y = 0;
                   srect.w = image->w;  // das gesamte Bild
                   srect.h = image->h;   // das gesamte Bild

                    // Setzen des Zielbereichs
                   drect.x = event.motion.x;
                   drect.y = event.motion.y;
                   drect.w = image->w;
                   drect.h = image->h;

                   // kopiere surface auf display-surface
                   SDL_BlitSurface(image, &srect, display, &drect);

                   // den veränderten Bildschirm-Bereich auffrischen
                   SDL_UpdateRects(display,1,&drect);
                   
                    break;

                    case SDL_MOUSEBUTTONDOWN:     // Quit beim druecken des Mousbuttons
                        quit = 1;
                        break;

                    default:
                        break;
                }
            }
        }
  
  // Das Bitmap-Surface löschen
  SDL_FreeSurface(image);

  exit(0);  
}
