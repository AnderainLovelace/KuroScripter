/*!
 * @file scene_talk.c
 *
 * @brief 对话场景的显示、对话脚本的解析
 * @author hikari <anderain.develop@gmail.com>
 * @date 2014-5-12
 */
#include "kuro.h"
#include "graph.h"
#include "sprite.h"
#include "timer.h"
#include "font.h"
#include "input.h"
#include "error.h"
#include "media.h"
#include "script.h"
#include "data.h"
#include "scene.h"
#include "utils.h"

BOOL quit;
/*
 * 文本文字间距
 */
#define CHAR_WIDTH      (FONT_WIDTH + 2)
#define CHAR_HEIGHT     (FONT_HEIGHT + 2)

/*
 * 文字颜色
 */
static SDL_Color textColor[] = {
	{255, 255, 255}, // 0
	{128, 128, 255}, // 1
	{255, 128, 128}, // 2
	{128, 255, 128}, // 3
	{128, 255, 255}, // 4
	{255, 128, 255}, // 5
	{255, 255, 128}, // 6
	{192, 192, 192}, // 7
	{  0,   0,   0}, // 8
};
static int textColorSize = sizeof(textColor)/sizeof(SDL_Color);
/* 
 * 图像
 */
static Sprite *        sprRole[ROLE_MAX];
static Sprite *        sprBack;
static Sprite *        sprBox;
static Sprite *        sprName;
/*
 * 绘制文本时使用，pend是
 * 逐个绘制文字的效果的指
 * 向尾部的指针
 */
const char *    text;
const char *    pend;
/*
 * 标志是否在在文字逐渐绘制中
 */
BOOL            appearing;
/*
 * 是否显示姓名
 */
BOOL            dispName;
int             nameDrawLeft; // 名字的坐标
int             nameDrawTop;
/*
 * 隐藏对话框
 */
BOOL            hideMessage;
/*!
 * @briref 重绘文字
 * @note 当appearing为TRUE时，pend决定了绘制文字的数量
 *       可实现文字逐个出现的效果，文字内容为text变量
 */
static void redrawText () {
    int             lineCharCount = 0;
    int             x = config.textX;
    int             y = config.textY;
    Uint32          color = CRGB(255,255,255);
    const char *    p;

    for (p = text;appearing ? p < pend : *p;) {
        if (isChs(*p)) {
            Font_DrawChar(screen,x,y,*p,*(p+1),color);
            x += CHAR_WIDTH;
            p++,p++;
            lineCharCount++;
        }
        else {
            /* 处理转义字符 */
            if (*p=='\\') {
                p++;
                /* 转义字符\n：手动换行 */
                if (*p == 'n') {
                    p++;
                    x = config.textX;
                    y += CHAR_HEIGHT;
                    lineCharCount = 0;
                }
                /* 转义字符\c：切换字符颜色 */
                else if (*p == 'c') {
                    p++;
                    if (isdigit(*p)) {
                        int i = *p - '0';
                        if (i >= 0 && i < textColorSize) {
                            color = CRGB(textColor[i].r,textColor[i].g,textColor[i].b);
                        }
                    }
                }
            }
            else {
                p++;
            }
        }
        if (lineCharCount >= config.lineMax) {
            x = config.textX;
            y += CHAR_HEIGHT;
            lineCharCount = 0;
        }
    }
}
/*!
 * @brief 重屏幕内容
 */
static void redraw (void) {
    RFPS_Start();
    Graph_Clear();
    {
        int i;
        /* 绘制背景 */
        //printf("0x%x\n",sprBack);
        if (sprBack != NULL)
            Sprite_Apply(sprBack);
        /* 绘制所有人物 */
        for (i = 0; i < ROLE_MAX; ++i) {
            if (sprRole[i] != NULL) {
                //printf("%d %d \n",sprRole[i]->x,sprRole[i]->y);
                Sprite_Apply(sprRole[i]);
            }
        }
        /* 绘制文本框 */
        if (!hideMessage) {
            Sprite_Apply(sprBox);
            /* 绘制文字 */
            redrawText();
            /* 绘制人物姓名 */
            if (dispName) {
                Sprite_Apply(sprName);
                Font_DrawString(screen,sprName->x + nameDrawLeft,sprName->y + nameDrawTop,data.talkerName,COLOR_WHITE);
            }
        }
    }
    Graph_Flip();    
    RFPS_End ();
}
/*!
 * @brief 文字逐个出现的绘制过程
 */
static void redrawTextApper () {
    pend = text;
    appearing = TRUE;
    for (;;) {
        redraw();
        Input_Update();
        if (Input_Repeat  (GKEY_B) || 
            Input_Trigger (GKEY_A) || 
            Input_Repeat  (GKEY_ESCAPE)) {
            break;
        }
        while (*pend && !isChs(*pend)) {
            pend++;
        }
        if (*pend == '\0')
            break;
        else {
            pend++;
            pend++;
        }
    }
    pend = NULL;
    appearing = FALSE;
}
static void setRoleY(int i,int y,BOOL save);
/*!
 * @brief 设置角色形象
 * @param i 角色标号
 * @param fileName 图片文件
 * @param save ture时保存到data
 */
static void setRole(int i,const char * fileName,BOOL save) {
	if (i < 0  ||  i > ROLE_MAX) {
		return;
	}
	/* 释放人物形象 */
	if (fileName == NULL || strEq(fileName,"") || strEq(fileName,NIL_FLAG)) {
		if (sprRole[i] != NULL) {
			Sprite_Free(sprRole[i],TRUE);
		}
		sprRole[i] = NULL;
		/* 保存到数据 */
		if (save)
			strcpy(data.talkRole[i],"");
	}
	/* 改变人物形象 */
	else {
		if (sprRole[i] != NULL) {
			SDL_FreeSurface(sprRole[i]->surface);
			sprRole[i]->surface = Media_LoadImage(fileName);
		}
		else {
			sprRole[i] = Sprite_LoadImage(fileName);
		}
		/* 保存到数据 */
		if (save)
			strcpy(data.talkRole[i],fileName);
        setRoleY(i,SCR_HEIGHT - sprRole[i]->surface->h,save);
	}
}
/*!
 * @brief 设置角色X坐标
 * @param i 角色标号
 * @param x x坐标
 * @param fileName 图片文件
 * @param save ture时保存到data
 */
static void setRoleX(int i,int x,BOOL save) {
	if (i < 0  ||  i > ROLE_MAX) {
		return;
	}
    if (sprRole[i] == NULL)
        return;
    sprRole[i]->x = x;
    if (save)
        data.talkRolePos[i].x = x;
}
/*!
 * @brief 设置角色Y坐标
 * @param i 角色标号
 * @param y y坐标
 * @param fileName 图片文件
 * @param save ture时保存到data
 */
static void setRoleY(int i,int y,BOOL save) {
	if (i < 0  ||  i > ROLE_MAX) {
		return;
	}
    if (sprRole[i] == NULL)
        return;
    sprRole[i]->y = y;
    if (save)
        data.talkRolePos[i].y = y;
}
/*!
 * @brief 设置角色X坐标（根据百分比）
 * @param i 角色标号
 * @param r 角色中心位置在屏幕中的百分比
 * @param fileName 图片文件
 * @param save ture时保存到data
 */
static void setRolePosRel(int i,real r,BOOL save) {
	if (i < 0  ||  i > ROLE_MAX) {
		return;
	}
    if (sprRole[i] == NULL)
        return;
    sprRole[i]->x = (int)(SCR_WIDTH * r) - sprRole[i]->surface->w / 2;
    if (save)
        data.talkRolePos[i].x = sprRole[i]->x;
}
/*!
 * @brief 设置背景
 * @param fileName 图片文件
 * @param save ture时保存到data
 */
static void setBackground(const char * fileName,BOOL save) {
    if (sprBack != NULL)
        Sprite_Free(sprBack,TRUE);
	/* 释放形象 */
	if (fileName == NULL || strEq(fileName,"") || strEq(fileName,NIL_FLAG)) {
        sprBack = NULL;
        if (save)
            strcpy(data.talkBack,"");
	}
	/* 改变形象 */
	else {
        sprBack = Sprite_LoadImage(fileName);
        if (save)
            strcpy(data.talkBack,fileName);
	}
}
/*!
 * @brief 释放所有资源
 */
static void freeSprite() {
    int i;
    Sprite_Free(sprBack,TRUE);
    sprBack = NULL;
    for (i=0;i<ROLE_MAX;++i) {
        Sprite_Free(sprRole[i],TRUE);
        sprRole[i] = NULL;
    }
    Sprite_Free(sprBox,TRUE);   sprBox  = NULL;
    Sprite_Free(sprName,TRUE);  sprName = NULL;
}
/*!
 * @brief 根据data数据重新读取数据
 */
static void reloadSprite() {
    int i;
    
    sprBox = Sprite_LoadImage("message.bmp");
    sprName = Sprite_LoadImage("message_name.bmp");
	
    sprBox->x = (SCR_WIDTH - sprBox->surface->w) / 2;
    sprBox->y = SCR_HEIGHT - sprBox->surface->h;

    sprName->x = config.nameX;
    sprName->y = sprBox->y - sprName->surface->h;
    
    setBackground(data.talkBack,FALSE);
    
    for (i = 0; i < ROLE_MAX ; ++i) {
        setRole(i,data.talkRole[i],FALSE);
        setRoleX(i,data.talkRolePos[i].x,FALSE);
        setRoleY(i,data.talkRolePos[i].y,FALSE);
    }
    
    dispName = !strEq(data.talkerName,"");
    
    nameDrawLeft = (sprName->surface->w - Font_GetDrawWidth(data.talkerName)) / 2;
    appearing = FALSE;
}
/*!
 * @brief 显示消息并等待玩家输入
 */
static void messageWait () {
    hideMessage = FALSE;
    redrawTextApper();
    redraw();
    while (!quit) {
        Input_Update();
        /* 隐藏屏幕内容 */
        if (Input_Trigger(GKEY_ESCAPE)) {
            Sprite * spr = Sprite_Create();
            spr->surface = Media_LoadImageAbs(ESCAPE_IMAGE);
            Graph_Clear();
            Sprite_Apply(spr);
            Graph_Flip();
            while (!quit) {
                Input_Update();
                if (Input_Trigger(GKEY_A) ||
                    Input_Trigger(GKEY_B) ||
                    Input_Trigger(GKEY_ESCAPE))
                    break;
            }
            Sprite_Free(spr,TRUE);
            redraw();
        }
        if (Input_Trigger(GKEY_A)) {
            break;
        }
        if (Input_Repeat(GKEY_B)) {
            break;
        }
        if (Input_Trigger(GKEY_L)) {
            hideMessage = !hideMessage;
            redraw();
        }
        if (Input_Trigger(GKEY_R)) {
            data.selectSize = 4;
            data.selectItem[0] = mystrdup("保存存档");
            data.selectItem[1] = mystrdup("读取存档");
            data.selectItem[2] = mystrdup("退出游戏");
            data.selectItem[3] = mystrdup("取消");
            data.var[0] = -1;
            Scene_Select(TRUE);
            /* 存档 */
            if (data.var[0] == 0) {
                const char * fileName;
                fileName = Scene_Data(SCENE_SAVE);
                
                if (!strEq(fileName,"")) {
                    Data_Save(fileName);
                }
            }
            /* 读档 */
            else if (data.var[0] == 1){
                const char * fileName;
                fileName = Scene_Data(SCENE_LOAD);
                
                if (!strEq(fileName,"")) {
                    Data_Load(fileName);
                    freeSprite();
                    reloadSprite();
                    break;
                }
            }
            /* 退出 */
            else if (data.var[0] == 2){
                quit = TRUE;
                break;
            }
            redraw();
        }
    }
}
/*!
 * @brief 进行对话界面
 */
void Scene_Talk () {
    int i;

    quit = FALSE;
    
    for (i=0;i<ROLE_MAX;++i)
        sprRole[i] = NULL;
    sprBack = NULL;
    reloadSprite();
    appearing = FALSE;
    dispName = FALSE;

    nameDrawTop = (sprName->surface->h - FONT_HEIGHT) / 2;
    hideMessage = FALSE;
	
    while (!quit) {
        /* 记录位置 */
        data.scriptOffset = Script_GetScriptOffset();
        /* 脚本解析 */
        Script_GetToken();
        /* 脚本结束 */
        if (SCRIPT_END()) {
            break;
        }
        /* 跳过空命令 */
        else if (TOKEN_EMPTY()) {
            continue;
        }
        /* 显示对话 */
        else if (isChs(token[0])) {
            text = token;
            messageWait();
        }
        /* 设置说话人姓名 */
        else if (TOKEN_IS("n")) {
            Script_GetToken();
            if (TOKEN_IS(NIL_FLAG)) {
                strcpy(data.talkerName,"");
                dispName = FALSE;
            }
            else {
                dispName = TRUE;
                strcpy(data.talkerName,token);
                nameDrawLeft = (sprName->surface->w - Font_GetDrawWidth(data.talkerName)) / 2;
            }
        }
        /* 找到语句标签 */
        else if (token[0] == ':') {
            Script_AddLabel(token+1,Script_GetScriptOffset());
        }
        /* 设置背景 */
        else if (TOKEN_IS("bg")) {
            Script_GetToken();
            setBackground(token,TRUE);
        }
        /* 移动人物 */
        else if (TOKEN_IS("mr")) {
            int     i;
            real    pos;
            
            Script_GetToken();
            i = atol(token);
            Script_GetToken();
            pos = myatof(token);
            setRolePosRel(i,pos,TRUE);
        }
        /* 移动人物x */
        else if (TOKEN_IS("mx")) {
            int     i;
            int     v;
            
            Script_GetToken();
            i = atol(token);
            Script_GetToken();
            v = atol(token);
            setRoleX(i,v,TRUE);
        }
        /* 移动人物y */
        else if (TOKEN_IS("my")) {
            int     i;
            int     v;
            
            Script_GetToken();
            i = atol(token);
            Script_GetToken();
            v = atol(token);
            setRoleY(i,v,TRUE);
        }
        /* 切换人物形象 */
        else if (TOKEN_IS("cr")) {
            int i;
            Script_GetToken();
            i = atol(token);
            Script_GetToken();
            setRole(i,token,TRUE);
        }
        /* 释放所有人物 */
        else if (TOKEN_IS("dar")) {
            for (i = 0; i < ROLE_MAX; ++i) {
                setRole(i,NULL,TRUE);
            }
        }
        /* 添加选项 */
        else if (TOKEN_IS("si")) {
            Script_GetToken();
            data.selectItem[data.selectSize++] = mystrdup(token);
        }
        /* 执行选择 */
        else if (TOKEN_IS("sl")) {
            hideMessage = TRUE;
            redraw();
            //freeSprite();
            Scene_Select(FALSE);
            //reloadSprite();
        }
		/* 变量操作 */
		else if (TOKEN_IS("var")) {
			int ol,opr,or;
			/* 获取操作数1 */
			Script_GetToken();
			if (token[0] != '$') {
				Error_Exit("%s is not a valid var in [var] command", token);
			}
			else {
				ol = atol(token + 1);
			}
			/* 获取运算符 */
			Script_GetToken();
			if (TOKEN_IS("="))
				opr = 0;
			else if (TOKEN_IS("+"))
				opr = 1;
			else if (TOKEN_IS("-"))
				opr = 2;
			else
				Error_Exit("unkown operator in [var] command:%s", token);
			/* 获取操作数2 */
			Script_GetToken();
			if (token[0] != '$') {
				or = atol(token);
			}
			else {
				int v = atol(token + 1);
				or = data.var[v];
			}
			/* 运算 */
			if (opr == 0) data.var[ol] = or ;
			else if (opr == 1) data.var[ol] += or ;
			else if (opr == 2) data.var[ol] -= or ;
		}
        /* 条件跳转 */
        else if (TOKEN_IS("if")) {
            int     ol,or,opr;
            BOOL    con = FALSE;
            /* 获取操作数1 */
            Script_GetToken();
            if (token[0] != '$') {
                ol = atol(token);
            }
            else {
                int v = atol(token + 1);
                ol = data.var[v];
            }
            /* 获取运算符 */
            Script_GetToken();
            if (TOKEN_IS("eq"))
                opr = 0;
            else if (TOKEN_IS("lt"))
                opr = 1;
            else if (TOKEN_IS("le"))
                opr = 2;
            else
                Error_Exit("unkown operator in [if] command:%s",token);
            /* 获取操作数2 */
            Script_GetToken();
            if (token[0] != '$') {
                or = atol(token);
            }
            else {
                int v = atol(token + 1);
                or = data.var[v];
            }
            /* 进行比较 */
            if (opr == 0) {
                con = (ol == or);
            }
            else if (opr == 1) {
                con = (ol < or);
            }
            else if (opr == 2) {
                con = (ol <= or);
            }
            /* 获取语句标号 */
            Script_GetToken();
            /* 如果真，则执行跳转 */
            if (con) {
                char label[LABEL_MAX];
                strcpy(label,token);
                Script_GotoLabel(label);
            }
        } 
        /* 跳转 */
        else if (TOKEN_IS("go")) {
            char label[LABEL_MAX];
            Script_GetToken();
            strcpy(label,token);
            Script_GotoLabel(label);
        } 
        /* 跳转 */
        else if (TOKEN_IS("switch")) {
            Script_GetToken();
            strcpy(data.scriptName,token);
            Script_Set(NULL);
            Script_Set(Media_LoadText(data.scriptName));
        } 
        /* 未知命令 */
        else  {
            Error_Exit("unknow command:%s",token);
        }
    }
    /* 释放所有资源 */
    Script_Set(NULL);
    freeSprite();
}
