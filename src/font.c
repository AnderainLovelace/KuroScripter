/*!
 * @file font.c
 *
 * @brief �����ֿ�Ĵ���
 * @author hikari <anderain.develop@gmail.com>
 * @date 2014-5-12
 */
#include "kuro.h"
#include "font.h"
#include "graph.h"
#include "error.h"

#define FONT_12_SIZE    (16*12/8)

FILE * fileFont = NULL;

/*!
 * @biref ����һ������ 
 * @param surface Ŀ��surface
 * @param x
 * @param y
 * @param c1 ���ֵĵ�һ���ֽ�
 * @param c2 ���ֵĵڶ����ֽ�
 * @param color ������ɫ
 */
void Font_DrawChar (SDL_Surface * surface,int x,int y,unsigned char c1,unsigned char c2,Uint32 color) {
    
    unsigned char mat[FONT_12_SIZE];

    fseek(fileFont,(94*(c1-0xa1)+(c2-0xa1))*FONT_12_SIZE,SEEK_SET);
    fread(mat,FONT_12_SIZE,1,fileFont);

    //Graph_WriteGraph(surface,x+1,y,12,12,mat,COLOR_BLACK);
    //Graph_WriteGraph(surface,x-1,y,12,12,mat,COLOR_BLACK);
    //Graph_WriteGraph(surface,x,y+1,12,12,mat,COLOR_BLACK);
    //Graph_WriteGraph(surface,x,y-1,12,12,mat,COLOR_BLACK);
    //Graph_WriteGraph(surface,x+1,y+1,12,12,mat,COLOR_BLACK);
    Graph_WriteGraph(surface,x,y,12,12,mat,color);
}
/*!
 * @biref ��ȡ�ַ������ƿ��
 * @param str �����ı�
 * @return int �ַ������ƿ��
 */
int Font_GetDrawWidth (const char * str) {
    int width = 0;
    while (*str) {
        if (isChs(*str)) {
            str++,str++;
            width += FONT_WIDTH + 1;
        }
        else {
            str++;
            width += FONT_WIDTH;
            continue;
        }
    }
    return width;
}
/*!
 * @biref ����һ���ַ���
 * @param surface Ŀ��surface
 * @param x
 * @param y
 * @param str �����ı�
 */
void Font_DrawString (SDL_Surface * surface,int x,int y,const char * str,Uint32 color) {
    while (*str) {
        if (isChs(*str)) {
            Font_DrawChar(surface,x,y,*str,*(str+1),color);
            str++,str++;
            x += FONT_WIDTH + 1;
        }
        else {
            unsigned char c1,c2;
            c1 = 0xA3;
            c2 = 0x80 + *str;
            Font_DrawChar(surface,x,y,c1,c2,color);
            x += FONT_WIDTH;
            str++;
        }
    }
}
/*!
 * @biref �ر��ֿ�
 */
void Font_Close () {
    if (fileFont != NULL) {
        fclose(fileFont);
    }
}
/*!
 * @biref ���ֿ�
 */
void Font_Init () {
    fileFont = fopen(FONT_FILE,"rb");
    if (fileFont == NULL) {
        Error_Exit("can not open font:%s",FONT_FILE);
    }
}