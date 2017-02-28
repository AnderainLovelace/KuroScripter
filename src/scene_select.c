/*!
 * @file scene_select.c
 *
 * @brief 选择画面的显示
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

static Sprite * sprSelect;
static Sprite * sprSelectBack;

static int   selectItemIndex;

void redrawSelect (void) {
    RFPS_Start();
    Graph_Clear();
    {
        int i;
        int left = (SCR_WIDTH - sprSelect->surface->w) / 2;
        int space  = sprSelect->surface->h + 4;
        int top = (SCR_HEIGHT - data.selectSize * space) / 2;

        sprSelect->x = left;
        sprSelect->y = top;

        Graph_ScreenRestore();

        Sprite_Apply(sprSelectBack);
        
        for (i = 0; i < data.selectSize ; ++i) {
            Sprite_Apply(sprSelect);
            Font_DrawString(screen,
                sprSelect->x + (sprSelect->surface->w - Font_GetDrawWidth(data.selectItem[i])) / 2,
                sprSelect->y + (sprSelect->surface->h - FONT_HEIGHT) / 2,
                data.selectItem[i],
                selectItemIndex == i ? COLOR_WHITE : COLOR_BLACK);
            sprSelect->y += space;
        }
    }
    Graph_Flip();    
    RFPS_End ();
}

/*!
 * @biref 显示选择界面
 * @param allowCancel 是否允许取消
 * @note 选择的结果保存在data.var[0]中
 */
void Scene_Select (BOOL allowCancel) {
    int i;
    Graph_ScreenSave();
 
    selectItemIndex = 0;
    sprSelect = Sprite_LoadImage("select.bmp");
	if(*(data.talkBack))
		sprSelectBack = Sprite_LoadImage(data.talkBack);

    while (TRUE) {
        redrawSelect ();
        Input_Update();
        if (Input_Trigger(GKEY_A)) {
            break;
        }
        if (allowCancel && Input_Trigger(GKEY_B)) {
            selectItemIndex = -1;
            break;
        }
        if (Input_Trigger(GKEY_UP)) {
            selectItemIndex --;
            if (selectItemIndex < 0)
                selectItemIndex = data.selectSize - 1;
        }
        if (Input_Trigger(GKEY_DOWN)) {
            selectItemIndex ++;
            if (selectItemIndex >= data.selectSize)
                selectItemIndex = 0;
        }

	}
    for (i = 0;i < data.selectSize ; ++i) {
        free(data.selectItem[i]);
    }
    data.var[0] = selectItemIndex;
    data.selectSize = 0;
    Sprite_Free(sprSelect,TRUE);
	if (*(data.talkBack))
		Sprite_Free(sprSelectBack,TRUE);
}