/*!
 * @file sprite.c
 *
 * @brief ����ʵ��ͼ����ʾ
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
 * @brief ����һ���հ׾���
 *
 * @return һ������ָ��
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
 * @brief ����һ��pngͼ������һ������
 *
 * @param fileName png�ļ���
 * @return һ������ָ��
 */
Sprite * Sprite_LoadImage (const char * fileName) {
    Sprite * sprite = Sprite_Create();
    sprite->surface = Media_LoadImage(fileName);
	return sprite;  
}
/*! 
 * @brief �ͷ�һ������
 *
 * @param sprite �����ָ��
 * @param disposeSurface �Ƿ��ͷ�Surface
 */
void Sprite_Free (Sprite * sprite,BOOL disposeSurface) {
    if (sprite == NULL)
        return;
    if (disposeSurface && sprite->surface != NULL)
        SDL_FreeSurface(sprite->surface);
    free(sprite);
}
/*! 
 * @brief ��������Ƶ���Ļ��
 *
 * @param sprite �����ָ��
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