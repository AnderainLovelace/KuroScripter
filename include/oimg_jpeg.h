#ifndef _OIMG_JPEG_
#define _OIMG_JPEG_

int     OIMG_InitJPG();
void    OIMG_QuitJPG();

#include "kuro.h"

#ifdef ___NDLESS___

#include "SDL/SDL.h"
#include "SDL/SDL_version.h"
#include "SDL/begin_code.h"

extern DECLSPEC int SDLCALL OIMG_isJPG(SDL_RWops *src);
extern DECLSPEC SDL_Surface * SDLCALL OIMG_LoadJPG_RW(SDL_RWops *src);

#define OIMG_SetError	SDL_SetError
#define OIMG_GetError	SDL_GetError

#include "SDL/close_code.h"

#else


#include "SDL_image.h"
#define OIMG_LoadJPG_RW     IMG_LoadJPG_RW
#define OIMG_isJPG          IMG_isJPG

#endif /* __NDLESS__ */

#endif /* _OIMG_JPEG_ */
