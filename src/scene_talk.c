/*!
 * @file scene_talk.c
 *
 * @brief �Ի���������ʾ���Ի��ű��Ľ���
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
 * �ı����ּ��
 */
#define CHAR_WIDTH      (FONT_WIDTH + 2)
#define CHAR_HEIGHT     (FONT_HEIGHT + 2)

/*
 * ������ɫ
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
 * ͼ��
 */
static Sprite *        sprRole[ROLE_MAX];
static Sprite *        sprBack;
static Sprite *        sprBox;
static Sprite *        sprName;
/*
 * �����ı�ʱʹ�ã�pend��
 * ����������ֵ�Ч����ָ
 * ��β����ָ��
 */
const char *    text;
const char *    pend;
/*
 * ��־�Ƿ����������𽥻�����
 */
BOOL            appearing;
/*
 * �Ƿ���ʾ����
 */
BOOL            dispName;
int             nameDrawLeft; // ���ֵ�����
int             nameDrawTop;
/*
 * ���ضԻ���
 */
BOOL            hideMessage;
/*!
 * @briref �ػ�����
 * @note ��appearingΪTRUEʱ��pend�����˻������ֵ�����
 *       ��ʵ������������ֵ�Ч������������Ϊtext����
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
            /* ����ת���ַ� */
            if (*p=='\\') {
                p++;
                /* ת���ַ�\n���ֶ����� */
                if (*p == 'n') {
                    p++;
                    x = config.textX;
                    y += CHAR_HEIGHT;
                    lineCharCount = 0;
                }
                /* ת���ַ�\c���л��ַ���ɫ */
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
 * @brief ����Ļ����
 */
static void redraw (void) {
    RFPS_Start();
    Graph_Clear();
    {
        int i;
        /* ���Ʊ��� */
        //printf("0x%x\n",sprBack);
        if (sprBack != NULL)
            Sprite_Apply(sprBack);
        /* ������������ */
        for (i = 0; i < ROLE_MAX; ++i) {
            if (sprRole[i] != NULL) {
                //printf("%d %d \n",sprRole[i]->x,sprRole[i]->y);
                Sprite_Apply(sprRole[i]);
            }
        }
        /* �����ı��� */
        if (!hideMessage) {
            Sprite_Apply(sprBox);
            /* �������� */
            redrawText();
            /* ������������ */
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
 * @brief ����������ֵĻ��ƹ���
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
 * @brief ���ý�ɫ����
 * @param i ��ɫ���
 * @param fileName ͼƬ�ļ�
 * @param save tureʱ���浽data
 */
static void setRole(int i,const char * fileName,BOOL save) {
	if (i < 0  ||  i > ROLE_MAX) {
		return;
	}
	/* �ͷ��������� */
	if (fileName == NULL || strEq(fileName,"") || strEq(fileName,NIL_FLAG)) {
		if (sprRole[i] != NULL) {
			Sprite_Free(sprRole[i],TRUE);
		}
		sprRole[i] = NULL;
		/* ���浽���� */
		if (save)
			strcpy(data.talkRole[i],"");
	}
	/* �ı��������� */
	else {
		if (sprRole[i] != NULL) {
			SDL_FreeSurface(sprRole[i]->surface);
			sprRole[i]->surface = Media_LoadImage(fileName);
		}
		else {
			sprRole[i] = Sprite_LoadImage(fileName);
		}
		/* ���浽���� */
		if (save)
			strcpy(data.talkRole[i],fileName);
        setRoleY(i,SCR_HEIGHT - sprRole[i]->surface->h,save);
	}
}
/*!
 * @brief ���ý�ɫX����
 * @param i ��ɫ���
 * @param x x����
 * @param fileName ͼƬ�ļ�
 * @param save tureʱ���浽data
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
 * @brief ���ý�ɫY����
 * @param i ��ɫ���
 * @param y y����
 * @param fileName ͼƬ�ļ�
 * @param save tureʱ���浽data
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
 * @brief ���ý�ɫX���꣨���ݰٷֱȣ�
 * @param i ��ɫ���
 * @param r ��ɫ����λ������Ļ�еİٷֱ�
 * @param fileName ͼƬ�ļ�
 * @param save tureʱ���浽data
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
 * @brief ���ñ���
 * @param fileName ͼƬ�ļ�
 * @param save tureʱ���浽data
 */
static void setBackground(const char * fileName,BOOL save) {
    if (sprBack != NULL)
        Sprite_Free(sprBack,TRUE);
	/* �ͷ����� */
	if (fileName == NULL || strEq(fileName,"") || strEq(fileName,NIL_FLAG)) {
        sprBack = NULL;
        if (save)
            strcpy(data.talkBack,"");
	}
	/* �ı����� */
	else {
        sprBack = Sprite_LoadImage(fileName);
        if (save)
            strcpy(data.talkBack,fileName);
	}
}
/*!
 * @brief �ͷ�������Դ
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
 * @brief ����data�������¶�ȡ����
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
 * @brief ��ʾ��Ϣ���ȴ��������
 */
static void messageWait () {
    hideMessage = FALSE;
    redrawTextApper();
    redraw();
    while (!quit) {
        Input_Update();
        /* ������Ļ���� */
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
            data.selectItem[0] = mystrdup("����浵");
            data.selectItem[1] = mystrdup("��ȡ�浵");
            data.selectItem[2] = mystrdup("�˳���Ϸ");
            data.selectItem[3] = mystrdup("ȡ��");
            data.var[0] = -1;
            Scene_Select(TRUE);
            /* �浵 */
            if (data.var[0] == 0) {
                const char * fileName;
                fileName = Scene_Data(SCENE_SAVE);
                
                if (!strEq(fileName,"")) {
                    Data_Save(fileName);
                }
            }
            /* ���� */
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
            /* �˳� */
            else if (data.var[0] == 2){
                quit = TRUE;
                break;
            }
            redraw();
        }
    }
}
/*!
 * @brief ���жԻ�����
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
        /* ��¼λ�� */
        data.scriptOffset = Script_GetScriptOffset();
        /* �ű����� */
        Script_GetToken();
        /* �ű����� */
        if (SCRIPT_END()) {
            break;
        }
        /* ���������� */
        else if (TOKEN_EMPTY()) {
            continue;
        }
        /* ��ʾ�Ի� */
        else if (isChs(token[0])) {
            text = token;
            messageWait();
        }
        /* ����˵�������� */
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
        /* �ҵ�����ǩ */
        else if (token[0] == ':') {
            Script_AddLabel(token+1,Script_GetScriptOffset());
        }
        /* ���ñ��� */
        else if (TOKEN_IS("bg")) {
            Script_GetToken();
            setBackground(token,TRUE);
        }
        /* �ƶ����� */
        else if (TOKEN_IS("mr")) {
            int     i;
            real    pos;
            
            Script_GetToken();
            i = atol(token);
            Script_GetToken();
            pos = myatof(token);
            setRolePosRel(i,pos,TRUE);
        }
        /* �ƶ�����x */
        else if (TOKEN_IS("mx")) {
            int     i;
            int     v;
            
            Script_GetToken();
            i = atol(token);
            Script_GetToken();
            v = atol(token);
            setRoleX(i,v,TRUE);
        }
        /* �ƶ�����y */
        else if (TOKEN_IS("my")) {
            int     i;
            int     v;
            
            Script_GetToken();
            i = atol(token);
            Script_GetToken();
            v = atol(token);
            setRoleY(i,v,TRUE);
        }
        /* �л��������� */
        else if (TOKEN_IS("cr")) {
            int i;
            Script_GetToken();
            i = atol(token);
            Script_GetToken();
            setRole(i,token,TRUE);
        }
        /* �ͷ��������� */
        else if (TOKEN_IS("dar")) {
            for (i = 0; i < ROLE_MAX; ++i) {
                setRole(i,NULL,TRUE);
            }
        }
        /* ���ѡ�� */
        else if (TOKEN_IS("si")) {
            Script_GetToken();
            data.selectItem[data.selectSize++] = mystrdup(token);
        }
        /* ִ��ѡ�� */
        else if (TOKEN_IS("sl")) {
            hideMessage = TRUE;
            redraw();
            //freeSprite();
            Scene_Select(FALSE);
            //reloadSprite();
        }
		/* �������� */
		else if (TOKEN_IS("var")) {
			int ol,opr,or;
			/* ��ȡ������1 */
			Script_GetToken();
			if (token[0] != '$') {
				Error_Exit("%s is not a valid var in [var] command", token);
			}
			else {
				ol = atol(token + 1);
			}
			/* ��ȡ����� */
			Script_GetToken();
			if (TOKEN_IS("="))
				opr = 0;
			else if (TOKEN_IS("+"))
				opr = 1;
			else if (TOKEN_IS("-"))
				opr = 2;
			else
				Error_Exit("unkown operator in [var] command:%s", token);
			/* ��ȡ������2 */
			Script_GetToken();
			if (token[0] != '$') {
				or = atol(token);
			}
			else {
				int v = atol(token + 1);
				or = data.var[v];
			}
			/* ���� */
			if (opr == 0) data.var[ol] = or ;
			else if (opr == 1) data.var[ol] += or ;
			else if (opr == 2) data.var[ol] -= or ;
		}
        /* ������ת */
        else if (TOKEN_IS("if")) {
            int     ol,or,opr;
            BOOL    con = FALSE;
            /* ��ȡ������1 */
            Script_GetToken();
            if (token[0] != '$') {
                ol = atol(token);
            }
            else {
                int v = atol(token + 1);
                ol = data.var[v];
            }
            /* ��ȡ����� */
            Script_GetToken();
            if (TOKEN_IS("eq"))
                opr = 0;
            else if (TOKEN_IS("lt"))
                opr = 1;
            else if (TOKEN_IS("le"))
                opr = 2;
            else
                Error_Exit("unkown operator in [if] command:%s",token);
            /* ��ȡ������2 */
            Script_GetToken();
            if (token[0] != '$') {
                or = atol(token);
            }
            else {
                int v = atol(token + 1);
                or = data.var[v];
            }
            /* ���бȽ� */
            if (opr == 0) {
                con = (ol == or);
            }
            else if (opr == 1) {
                con = (ol < or);
            }
            else if (opr == 2) {
                con = (ol <= or);
            }
            /* ��ȡ����� */
            Script_GetToken();
            /* ����棬��ִ����ת */
            if (con) {
                char label[LABEL_MAX];
                strcpy(label,token);
                Script_GotoLabel(label);
            }
        } 
        /* ��ת */
        else if (TOKEN_IS("go")) {
            char label[LABEL_MAX];
            Script_GetToken();
            strcpy(label,token);
            Script_GotoLabel(label);
        } 
        /* ��ת */
        else if (TOKEN_IS("switch")) {
            Script_GetToken();
            strcpy(data.scriptName,token);
            Script_Set(NULL);
            Script_Set(Media_LoadText(data.scriptName));
        } 
        /* δ֪���� */
        else  {
            Error_Exit("unknow command:%s",token);
        }
    }
    /* �ͷ�������Դ */
    Script_Set(NULL);
    freeSprite();
}
