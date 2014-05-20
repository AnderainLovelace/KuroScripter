#ifndef _GRAPH_H_
#define _GRAPH_H_

extern SDL_Surface * screen;

#define DEF_TRANS 60

#define SCR_WIDTH   320
#define SCR_HEIGHT  240

extern Uint32 COLOR_MASK,COLOR_BLACK,COLOR_WHITE;

#define CRGB(r,g,b)     (SDL_MapRGB(screen->format,r,g,b))

#ifdef ___NDLESS___
#define SCR_BPP         (has_colors ? 16 : 8)
#else
#define SCR_BPP         (16)
#endif

void Graph_Init             ();
void Graph_Quit             (void);
void Graph_Clear            ();
void Graph_Flip             ();
void Graph_ApplySurfaceTo   (int x, int y, SDL_Surface * src,SDL_Surface * dest); 
void Graph_SetPixel         (SDL_Surface *surface, int x, int y, Uint32 pixel);
void Graph_WriteGraph       (SDL_Surface * surface,int x,int y,int w,int h,unsigned char * pimage,Uint32 color);

void Graph_Freeze           ();
void Graph_Shock            (int frames);
void Graph_Transition       (int frames,void (*redraw)(void));

void Graph_ScreenSave       ();
void Graph_ScreenRestore    ();

#endif