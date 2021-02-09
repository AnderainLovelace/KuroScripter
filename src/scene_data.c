/*!
 * @file scene_data.c
 *
 * @brief 读取、保存画面的显示
 * @author hikari <anderain.develop@gmail.com>
 * @date 2014-5-13
 */
#include "kuro.h"

#include "graph.h"
#include "sprite.h"
#include "timer.h"
#include "font.h"
#include "input.h"
#include "media.h"
#include "data.h"
#include "scene.h"

#define PAGE_ITEM_MAX 5
#define PAGE_MAX 3

Sprite *    sprDataBack;
Sprite *    sprSelect;

static int         selectItemIndex;
static int         page;

static char itemText[PAGE_ITEM_MAX][64];
static BOOL itemExist[PAGE_ITEM_MAX];
static char fileName[128];

static void refreshItem () {
    FILE * fp;
    int i,ri = page * PAGE_ITEM_MAX;

    for (i = 0;i < PAGE_ITEM_MAX;++i,++ri) {
        char filePath[128];
        sprintf(filePath,SAVE_FILE_PATH "%d" SAVE_FILE_EXT,ri);
        fp = fopen(filePath,"rb");
        if (fp == NULL) {
            sprintf(itemText[i],"【%03d】无数据",ri + 1);
            itemExist[i] = FALSE;
        }
        else {
            sprintf(itemText[i],"【%03d】已存档",ri + 1);
            itemExist[i] = TRUE;
            fclose(fp);
        }
    }
}

static void redrawData (void) {
    RFPS_Start();
    Graph_Clear();
    {
        int i;
        int left = (SCR_WIDTH - sprSelect->surface->w) / 2;
        int space  = sprSelect->surface->h + 4;
        int top = (SCR_HEIGHT - PAGE_ITEM_MAX * space) / 2;

        Sprite_Apply(sprDataBack);

        sprSelect->x = left;
        sprSelect->y = top;

        for (i = 0; i < PAGE_ITEM_MAX ; ++i) {
            Sprite_Apply(sprSelect);
            Font_DrawString(screen,
                sprSelect->x + (sprSelect->surface->w - Font_GetDrawWidth(itemText[i])) / 2,
                sprSelect->y + (sprSelect->surface->h - FONT_HEIGHT) / 2,
                itemText[i],
                selectItemIndex == i ? COLOR_WHITE : COLOR_BLACK);
            sprSelect->y += space;
        }
    }
    Graph_Flip();    
    RFPS_End ();
}

/*!
 * @brief 读档或存档的界面
 * @param type 0 为存档，1为读档
 * @return 返回一个文件名
 */
const char * Scene_Data (int type) {
    sprSelect = Sprite_LoadImage("select.bmp");
    sprDataBack = Sprite_LoadImage(type==SCENE_SAVE ? "back_save.jpg" : "back_load.jpg");
    selectItemIndex = 0;
    page = 0;

    refreshItem();

    strcpy(fileName,"");

    while (TRUE) {
        redrawData ();
        Input_Update();
        if (Input_Trigger(GKEY_A)) {
            if ((type == SCENE_LOAD && itemExist[selectItemIndex]) || type == SCENE_SAVE) {
                sprintf(fileName,SAVE_FILE_PATH "%d" SAVE_FILE_EXT,selectItemIndex + page * PAGE_ITEM_MAX);
                break;
            }
        }
        if (Input_Trigger(GKEY_B)) {
            break;
        }
        if (Input_Trigger(GKEY_LEFT)) {
            page --;
            if (page < 0)
                page = PAGE_MAX - 1;
            refreshItem();
        }
        if (Input_Trigger(GKEY_RIGHT)) {
            page ++;
            if (page >= PAGE_MAX)
                page = 0;
            refreshItem();
        }
        if (Input_Trigger(GKEY_UP)) {
            selectItemIndex --;
            if (selectItemIndex < 0)
                selectItemIndex = PAGE_ITEM_MAX - 1;
        }
        if (Input_Trigger(GKEY_DOWN)) {
            selectItemIndex ++;
            if (selectItemIndex >= PAGE_ITEM_MAX)
                selectItemIndex = 0;
        }
	}
    Sprite_Free(sprDataBack,TRUE);
    Sprite_Free(sprSelect,TRUE);

    return fileName;
}