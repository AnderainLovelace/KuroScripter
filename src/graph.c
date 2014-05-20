/*!
 * @file graph.c
 *
 * @brief 图像的显示
 * @author hikari <anderain.develop@gmail.com>
 * @date 2014-5-13
 */
#include "kuro.h"
#include "graph.h"
#include "timer.h"
#include "input.h"
#include "script.h"

SDL_Surface * screen;
//SDL_Surface * screenBuf;

Uint32 COLOR_BLACK,COLOR_MASK,COLOR_WHITE;

#define SHOCK_MOVE_STEP 2 

/*!
 * @biref 退出并清理资源
 */
void Graph_Quit (void) {
    SDL_Quit();
}
/*!
 * @biref 初始化图形
 */
void Graph_Init () {
	SDL_Init(SDL_INIT_VIDEO);
    SDL_WM_SetCaption(config.debug ? "KuroScripter [debug]" : "KuroScripter [release]", NULL);
    SDL_ShowCursor(FALSE);
    screen = SDL_SetVideoMode(SCR_WIDTH, SCR_HEIGHT,SCR_BPP, SDL_SWSURFACE);
    //screenBuf = SDL_CreateRGBSurface(SDL_SWSURFACE,SCR_WIDTH, SCR_HEIGHT,SCR_BPP,
	//		    screen->format->Rmask,screen->format->Gmask,
	//		    screen->format->Bmask,screen->format->Amask);
                
    COLOR_BLACK = CRGB(0,0,0);
    COLOR_MASK  = CRGB(255,0,255);
    COLOR_WHITE = CRGB(255,255,255);
}
/*!
 * @biref 清除屏幕内容
 */
void Graph_Clear ()
{
	SDL_FillRect(screen,NULL,COLOR_BLACK);
}
/*!
 * @biref 显示屏幕内容
 */
void Graph_Flip () {
    SDL_Flip(screen);
}
/*!
 * @biref 绘制一个surface到另一个surface
 * @param x
 * @param y
 * @param src   源surface
 * @param dest  目的surface
 */
void Graph_ApplySurfaceTo(int x, int y, SDL_Surface * src,SDL_Surface * dest)  
{  
    SDL_Rect offset;  
    offset.x = x;  
    offset.y = y;  
  
    SDL_BlitSurface(src, NULL, dest, &offset);  
}
/*!
 * @biref 画面震动
 * @param frames震动帧数
 */
 /*
void Graph_Shock (int frames) {
	int i,x = 0,y = 0;
	int state = 0;

	for (i=0;i<frames;++i) {
		RFPS_Start();
        Input_Update();
		Graph_Clear();

		if (state == 0) {
			x+=SHOCK_MOVE_STEP;
			y+=SHOCK_MOVE_STEP;
			if (x>=SHOCK_OFFSET)
				state = 1;
		}
		else {
			x-=SHOCK_MOVE_STEP;
			y-=SHOCK_MOVE_STEP;
			if (x<=-SHOCK_OFFSET)
				state = 0;
		}
		Graph_ApplySurfaceTo(x,y,screenBuf,screen);

		Graph_Flip();
		RFPS_End();
	}
}*/
/*!
 * @biref 冻结画面
 */
void Graph_Freeze () {
	//Graph_ApplySurfaceTo(0,0,screen,screenBuf);
}
/*!
 * @biref 保存当前画面内容
 */
void Graph_ScreenSave () {
	//Graph_ApplySurfaceTo(0,0,screen,screenBuf);
}
/*!
 * @biref 绘制保留的画面内容
 */
void Graph_ScreenRestore () {
	//Graph_ApplySurfaceTo(0,0,screenBuf,screen);
}
/*!
 * @biref 平滑渐变
 */
void Graph_Transition (int frames,void (*redraw)(void)) {
	/*int i;
	float opy = 16,opy_stp = opy / frames;

	for (i=0;i<=frames;++i,opy -= opy_stp) {
		RFPS_Start();
		Graph_Clear();
        Input_Update();
		if (redraw != NULL)
			redraw();
        SDL_SetAlpha(screenBuf,SDL_SRCALPHA|SDL_RLEACCEL,(Uint8)(i == frames ? 0 : opy));
		Graph_ApplySurfaceTo(0,0,screenBuf,screen);
		Graph_Flip();
		RFPS_End();
	}
	SDL_SetAlpha(screenBuf,SDL_SRCALPHA|SDL_RLEACCEL,255);*/
}
/*!
 * @biref 绘制一个像素到surface
 * @param surface 目的surface
 * @param x
 * @param y 
 * @param pixel 像素颜色 
 */
void Graph_SetPixel(SDL_Surface *surface, int x, int y, Uint32 pixel)
{
    int bpp = surface->format->BytesPerPixel;

    Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp;

    switch(bpp) {
    case 1:
        *p = (Uint8)pixel;
        break;

    case 2:
        *(Uint16 *)p = (Uint16)pixel;
        break;

    case 3:
        if(SDL_BYTEORDER == SDL_BIG_ENDIAN) {
            p[0] = (Uint8)((pixel >> 16) & 0xff);
            p[1] = (Uint8)((pixel >> 8) & 0xff);
            p[2] = (Uint8)(pixel & 0xff);
        } else {
            p[0] = (Uint8)(pixel & 0xff);
            p[1] = (Uint8)((pixel >> 8) & 0xff);
            p[2] = (Uint8)((pixel >> 16) & 0xff);
        }
        break;

    case 4:
        *(Uint32 *)p = pixel;
        break;
    }
}
/*!
 * @biref 绘制一个像素位图到surface
 * @param surface 目的surface
 * @param x
 * @param y 
 * @param w 图像宽度
 * @param h 图像高度
 * @param pimage 位图数据
 * @param pixel 像素颜色 
 */
void Graph_WriteGraph (SDL_Surface * surface,int x,int y,int w,int h,unsigned char * pimage,Uint32 color) {
    unsigned char p;
    int i,j,k,pixel,rx=0,ry=0;
    int iwidth = w/8 + (w % 8 ? 1:0);

    for (i = 0;i < h; ++i, pimage += iwidth) {
        ry = y + i;
        if (ry >= SCR_HEIGHT)
            return;
        else if (ry < 0)
            continue;
        for (j = 0;j < iwidth; ++j) {
            p = pimage[j];
            for (k = 0;k < 8; ++k) {
                rx = x + (8 - k) + (j << 3); /* j * 8 */
                if (rx < 0 || rx > SCR_WIDTH) 
                    continue;
                pixel = p & 1;
                p >>= 1;
                if (pixel) {
                    Graph_SetPixel(surface,rx,ry,color);
                }
            }
        }
    }
}
