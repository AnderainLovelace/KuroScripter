#ifndef _KURO_H_
#define _KURO_H_

#ifndef WIN32
#define ___NDLESS___
#endif


#ifdef ___NDLESS___
#include "os.h"
#endif

#include "SDL/SDL_config.h"
#include "SDL/SDL.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "ctype.h"
#include "setjmp.h"

#define TRUE  1
#define FALSE 0 
#define BOOL  int
#define real  double

#define strEq(s1,s2) (strcmp(s1,s2)==0)

#ifndef ___NDLESS___

#define FONT_FILE           "krs/hzk12.hzk"
#define MEDIA_FILE          "krs/game.krp"
#define CONFIG_FILE         "krs/config.krs"
#define SAVE_FILE_PATH      "krs/save/"
#define SAVE_FILE_EXT       ".krd"
#define MEDIA_PATH          "krs/media/"
#define ESCAPE_IMAGE        "krs/escape.jpg"

#else

#define MEDIA_FILE          "/documents/krs/game.krp.tns"
#define FONT_FILE           "/documents/krs/hzk12.hzk.tns"
#define CONFIG_FILE         "/documents/krs/config.krs.tns"
#define SAVE_FILE_PATH      "/documents/krs/save/"
#define ESCAPE_IMAGE        "/documents/krs/escape.jpg.tns"
#define MEDIA_PATH          "/documents/krs/media/"

#define SAVE_FILE_EXT       ".krd.tns"
#endif

#define GAME_CONFIG_FILE     "config.krs"

#endif