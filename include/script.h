#ifndef _SCRIPT_H_
#define _SCRIPT_H_

#include "dlist.h"

#define LABEL_MAX 32

typedef struct {
    char name[LABEL_MAX];
    unsigned long offset;
}
Label;

extern DList * listLabel;

#define TOKEN_MAX 512

void Script_Set                 (const char * source);
void Script_GetToken            ();
void Script_LoadGlobalConfig    ();
void Script_LoadGameConfig      ();
void Script_FreeConfig          ();
void Script_AddLabel            (const char * name,unsigned long offset);
void Script_GotoLabel           (const char * name);

unsigned long Script_GetScriptOffset();
void Script_SetScriptOffset (unsigned long offset);

#define FILE_END_FLAG           '!'
#define NIL_FLAG				"~"
#define SCRIPT_END()            (token[0] == FILE_END_FLAG)
#define TOKEN_EMPTY()           (token[0] == '\0')
#define TOKEN_IS(s)             (strEq(token,s))

extern char token[];

typedef struct {
    BOOL    debug;
    int     textX;
    int     textY;
    int     nameX;
    int     lineMax;
    int     fps;
    int     titleTextX;
    int     titleTextY;
    int     titleTextSpW;
    int     titleTextSpH;
}
Configure;

extern Configure config;

#endif