#ifndef _FONT_H_
#define _FONT_H_

#define isChs(c)    ((c) & 0x80)

#define FONT_WIDTH      12
#define FONT_HEIGHT     12

void Font_DrawString    (SDL_Surface * surface,int x,int y,const char * str,Uint32 color);
void Font_DrawChar      (SDL_Surface * surface,int x,int y,unsigned char c1,unsigned char c2,Uint32 color);
void Font_Init          ();
void Font_Close			(void);
int  Font_GetDrawWidth  (const char *);

#endif
