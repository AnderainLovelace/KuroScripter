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
#include "script.h"

Sprite * sprTitleBack;
static int selectItemIndex ;

static void redrawTitle (void) {
    int i;
    static char text[][16] = {"开始游戏","读取存档","退出游戏"};

    RFPS_Start();
    Graph_Clear();
    {
        Sprite_Apply(sprTitleBack);
        for (i = 0;i < 3;++i)
            Font_DrawString(screen,
                            config.titleTextX + config.titleTextSpW * i,config.titleTextY + config.titleTextSpH * i,
                            text[i],selectItemIndex == i ? COLOR_WHITE : COLOR_BLACK);
    }
    Graph_Flip();    
    RFPS_End ();
}

int Scene_Title (int t) {
    selectItemIndex = t;

    sprTitleBack = Sprite_LoadImage("title.jpg");

	redrawTitle ();
    while (TRUE) {
        redrawTitle ();
        Input_Update();
        if (Input_Trigger(GKEY_A)) {
            break;
        }
        if (Input_Trigger(GKEY_UP)) {
            selectItemIndex --;
            if (selectItemIndex < 0)
                selectItemIndex = 2;
			
        }
        if (Input_Trigger(GKEY_DOWN)) {
            selectItemIndex ++;
            if (selectItemIndex >= 3)
                selectItemIndex = 0;
        }
	}
    Sprite_Free(sprTitleBack,TRUE);
    sprTitleBack = NULL;

    return selectItemIndex;
}