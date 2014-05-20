#ifndef _SPRITE_H_
#define _SPRITE_H_

typedef struct {
    int             x;
    int             y;
    real            zoomX;
    real            zoomY;
    int             angle;
    BOOL            visible;
    SDL_Surface*    surface;
}
Sprite;

Sprite *    Sprite_Create           ();
Sprite *    Sprite_LoadImage        (const char * fileName);
Sprite *    Sprite_CreateBySurface  (SDL_Surface * surface);
void        Sprite_Free             (Sprite * sprite,BOOL disposeSurface);
void        Sprite_Apply            (const Sprite * sprite);

#endif