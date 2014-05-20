/*!
 * @file sprite.c
 *
 * @brief 负责实现图像显示
 * @author hikari <anderain.develop@gmail.com>
 * @date 2014-5-11
 */
#include "kuro.h"
#include "sprite.h"
#include "error.h"
#include "graph.h"
#include "media.h"
#include "rotozoom.h"
/*! 
 * @brief 创建一个空白精灵
 *
 * @return 一个精灵指针
 */
Sprite * Sprite_Create () {
    Sprite * sprite;

    sprite = (Sprite*)malloc(sizeof(Sprite));
    sprite->x       = 0;
    sprite->y       = 0;
    sprite->zoomX   = 1.0;
    sprite->zoomY   = 1.0;
    sprite->angle   = 0;
    sprite->visible = TRUE;
    sprite->surface = NULL;

	return sprite;  
}
/*! 
 * @brief 读入一张png图像并生成一个精灵
 *
 * @param fileName png文件名
 * @return 一个精灵指针
 */
Sprite * Sprite_LoadImage (const char * fileName) {
    Sprite * sprite = Sprite_Create();
    sprite->surface = Media_LoadImage(fileName);
	return sprite;  
}
/*! 
 * @brief 释放一个精灵
 *
 * @param sprite 精灵的指针
 * @param disposeSurface 是否释放Surface
 */
void Sprite_Free (Sprite * sprite,BOOL disposeSurface) {
    if (sprite == NULL)
        return;
    if (disposeSurface && sprite->surface != NULL)
        SDL_FreeSurface(sprite->surface);
    free(sprite);
}
/*! 
 * @brief 将精灵绘制到屏幕上
 *
 * @param sprite 精灵的指针
 */
void Sprite_Apply (const Sprite * sprite) {
    SDL_Surface * tempSurface = NULL;
    if (sprite == NULL)
        return;
    if (!sprite->visible)
        return;
    if (sprite->surface == NULL)
        return;
    if (sprite->angle == 0.0 && sprite->zoomX == 1.0 && sprite->zoomY == 1.0) {
        Graph_ApplySurfaceTo(sprite->x,sprite->y,sprite->surface,screen);
    }
    else {
        int ow = sprite->surface->w,oh = sprite->surface->h,w,h;
        rotozoomSurfaceSizeXY(ow,oh,sprite->angle,sprite->zoomX,sprite->zoomY,&w,&h);
        tempSurface = rotozoomSurfaceXY(sprite->surface,sprite->angle,
                                        sprite->zoomX,sprite->zoomY,TRUE);
        Graph_ApplySurfaceTo(sprite->x - (w - ow) / 2,sprite->y - (h - oh) / 2,tempSurface,screen);
        SDL_FreeSurface(tempSurface);
    }
}