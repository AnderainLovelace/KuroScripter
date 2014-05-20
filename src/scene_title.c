/*!
 * @file scene_title.c
 *
 * @brief 标题画面显示
 * @author hikari <anderain.develop@gmail.com>
 * @date 2014-5-14
 */
#include "kuro.h"
#include "graph.h"
#include "sprite.h"
#include "timer.h"
#include "font.h"
#include "input.h"
#include "data.h"
#include "scene.h"

Sprite * sprTitleBack;
static int index ;

static void redrawTitle (void) {
    RFPS_Start();
    Graph_Clear();
    {
        Sprite_Apply(sprTitleBack);
        Font_DrawString(screen,225,136,"开始游戏",index == 0 ? COLOR_WHITE : COLOR_BLACK);
        Font_DrawString(screen,230,150,"读取存档",index == 1 ? COLOR_WHITE : COLOR_BLACK);
        Font_DrawString(screen,235,164,"退出游戏",index == 2 ? COLOR_WHITE : COLOR_BLACK);
    }
    Graph_Flip();    
    RFPS_End ();
}

int Scene_Title (int t) {
    index = t;

    sprTitleBack = Sprite_LoadImage("title.jpg");

	redrawTitle ();
    while (TRUE) {
        redrawTitle ();
        Input_Update();
        if (Input_Trigger(GKEY_A)) {
            break;
        }
        if (Input_Trigger(GKEY_UP)) {
            index --;
            if (index < 0)
                index = 2;
			
        }
        if (Input_Trigger(GKEY_DOWN)) {
            index ++;
            if (index >= 3)
                index = 0;
        }
	}
    Sprite_Free(sprTitleBack,TRUE);
    sprTitleBack = NULL;

    return index;
}