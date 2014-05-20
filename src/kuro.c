/*!
 * @file kuro.c
 *
 * @brief 应用程序入口
 * @author hikari <anderain.develop@gmail.com>
 * @date 2014-5-14
 */
#include "kuro.h"
#include "media.h"
#include "graph.h"
#include "input.h"
#include "font.h"
#include "oimg_jpeg.h"
#include "script.h"
#include "sprite.h"
#include "data.h"
#include "scene.h"
#include "error.h"

void cleanUp (void) {
    Script_FreeConfig();
	Media_Close();
	Font_Close();
    Graph_Quit();	
}

int main (void) {
    int index = 0;
    
    Script_LoadGlobalConfig();

	Graph_Init();
	Input_Init();
    
    OIMG_InitJPG();
    
    Media_Init(MEDIA_FILE);

    Script_LoadGameConfig();

    Font_Init();
    
    // show logo
    /*if (!config.debug) {
        Sprite * spr = Sprite_LoadImage("title.jpg");
        Graph_Clear();
        Sprite_Apply(spr);
        Graph_Flip();
        SDL_Delay(1000);
        Sprite_Free(spr,TRUE);
    }*/
    
    atexit(cleanUp);


    while (TRUE) {
        index = Scene_Title(index);
        if (index == 0) {
            Data_Init();
            Scene_Talk();
        }
        else if (index == 1) {
            const char * fileName;
            fileName = Scene_Data(SCENE_LOAD);
            
            if (!strEq(fileName,"")) {
                Data_Load(fileName);
                Scene_Talk();
            }
        }
        else {
            break;
        }

    }
    
    return 0;
}