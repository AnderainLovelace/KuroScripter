#ifndef _MEDIA_H_
#define _MEDIA_H_

#include "kuro.h"
#include "graph.h"
#include "media.h"
#include "tar_reader.h"
#include "error.h"


void            Media_Init          (const char * fileName);
SDL_Surface *   Media_LoadImage     (const char * fileName);
char *          Media_LoadText      (const char * fileName);
void 			Media_Close 	    (void);
SDL_Surface *   Media_LoadImageAbs  (const char * filePath);

#endif