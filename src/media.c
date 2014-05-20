/*!
 * @file media.c
 *
 * @brief ý����Ĵ���
 * @author hikari <anderain.develop@gmail.com>
 * @date 2014-5-11
 */
#include "kuro.h"
#include "graph.h"
#include "media.h"
#include "tar_reader.h"
#include "error.h"
#include "oimg_jpeg.h"
#include "script.h"

TAR_FILE * mediaPack = NULL;
/*!
 * @brief ����Ϸý���
 * @param fileName �ļ���
 */
void Media_Init (const char * fileName) {
    if (config.debug) {
        // ʲôҲ����
    }
    else {
        mediaPack = Tar_Open(fileName);
        if (mediaPack == NULL) {
            Error_Exit("can not open package:%s",fileName);
        }
    }
}
/*!
 * @brief �ر���Ϸý���
 */
void Media_Close (void) {
    if (config.debug) {
    }
    else {
        if (mediaPack != NULL)
	        fclose(mediaPack);
    }
}
/*!
 * @brief �Ӿ���·���ж�ȡͼƬ
 * @param filePath �ļ���
 * @return SDL_Surface
 */
SDL_Surface * Media_LoadImageAbs (const char * filePath) {
    FILE * fp;
    SDL_Surface *   surface = NULL;
    SDL_Surface *   optSurface = NULL;
    SDL_RWops *     rw;
    
    fp = fopen(filePath,"rb");

    if (fp == NULL) {
        Error_Exit("can not open image:(path):%s",filePath);
    }

    rw = SDL_RWFromFP(fp,1);

    if (OIMG_isJPG(rw)) { 
        surface = OIMG_LoadJPG_RW(rw);
        SDL_FreeRW(rw);
    }
    else {
        surface = SDL_LoadBMP_RW(rw,TRUE);
    }

    fclose(fp);

    if (surface == NULL) {
        Error_Exit("can not load image:(path):%s",filePath);
    }

    optSurface = SDL_DisplayFormat(surface);
    SDL_FreeSurface(surface);

    SDL_SetColorKey(optSurface,SDL_SRCCOLORKEY,COLOR_MASK);

    return optSurface;
}

/*!
 * @brief ��ý����ж�ȡͼƬ
 * @param fileName �ļ���
 * @return SDL_Surface
 */
SDL_Surface * Media_LoadImage (const char * fileName) {
    SDL_Surface *   surface = NULL;
    SDL_Surface *   optSurface = NULL;
    SDL_RWops *     rw;
    
    if (config.debug) {
        char filePath[512];

        sprintf(filePath,MEDIA_PATH "%s",fileName);

        return Media_LoadImageAbs(filePath);
    }
    else {
        void *          data = NULL;
        int             result;
        size_t          fileSize;
		
        if (mediaPack == NULL) {
            Error_Exit("media package NULL");
        }
        data = Tar_GetInTarFile(mediaPack,fileName,&fileSize,&result);
        if (result != TAR_SUCCESS) {
            Error_Exit("can not open image in package:%s",fileName);
        }
		
        rw = SDL_RWFromMem(data,fileSize);
		
        if (OIMG_isJPG(rw)) { 
            surface = OIMG_LoadJPG_RW(rw);
            SDL_FreeRW(rw);
        }
        else {
            surface = SDL_LoadBMP_RW(rw,TRUE);
        }
    
        free(data);

        if (surface == NULL) {
            Error_Exit("can not load image:%s",fileName);
        }

        optSurface = SDL_DisplayFormat(surface);
        SDL_FreeSurface(surface);

        SDL_SetColorKey(optSurface,SDL_SRCCOLORKEY,COLOR_MASK);

        return optSurface;
    }

}
/*!
 * @brief ��ý����ж�ȡ�ı�
 * @param fileName �ļ���
 * @param char*
 */
char * Media_LoadText (const char * fileName) {
    if (config.debug) {
        FILE * f;
        char * text;
        size_t size;

        char filePath[512];

        sprintf(filePath,MEDIA_PATH "%s",fileName);
        f = fopen(filePath,"rb");

        if (f == NULL) {
            Error_Exit("can not open text file:%s",fileName);
        }

        fseek(f,0,SEEK_END);
        size = ftell(f);
        fseek(f,0,SEEK_SET);
        text = (char*)malloc(size);
        fread(text,size,1,f);
        fclose(f);

        return text;
    }
    else {
        void *  data;
        int     result;

        if (mediaPack == NULL) {
            Error_Exit("media package NULL");
        }
    
        data = Tar_GetInTarFile(mediaPack,fileName,NULL,&result);

        if (result != TAR_SUCCESS) {
            Error_Exit("can not open text file in package:%s",fileName);
        }

        return (char*)data;
    }
}