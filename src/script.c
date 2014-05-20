/*!
 * @file script.c
 *
 * @brief �ű��Ľ����������ļ�
 * @author hikari <anderain.develop@gmail.com>
 * @date 2014-5-12
 */
#include "kuro.h"
#include "script.h"
#include "media.h"
#include "error.h"

/*
 * �ű�����
 */
char *        script = NULL;
const char *  pscript = NULL;
DList      *  listLabel = NULL;
/*
 * �����ļ�
 */

Configure config;

char    token[TOKEN_MAX];

/*!
 * @brief ��ȡ�ű�ִ�й����ֽ���
 */
unsigned long Script_GetScriptOffset () {
    return pscript - script;
}
/*!
 * @brief ���ýű�ִ�й����ֽ���
 */
void Script_SetScriptOffset (unsigned long offset) {
    pscript = script + offset;
}
/*!
 * @brief �ӽű��л�ȡһ���﷨Ԫ��
 * @note ��ȡ����ڱ���token��
 */
void Script_GetToken () {
    char * t = token;
    
    if (pscript == NULL) {
        *t++ = FILE_END_FLAG;
        *t = 0;
        return;
    }
    
    /* �����հ� */
    while (isspace(*pscript))
        pscript++;
    /* ��������ע�� */
    if (*pscript == '#') {
        while (*pscript != '\n' && *pscript != FILE_END_FLAG) {
            pscript++;
        }
        *t = '\0';
    }
    /* �ļ����� */
    else if (*pscript == FILE_END_FLAG) {
        *t++ = *pscript++;
        *t = '\0';
    }
    /* ��ȡ���� */
    else if (*pscript == '[') {
        pscript++;
        while (*pscript != ']' && *pscript != FILE_END_FLAG) {
            *t++ = *pscript++;
        }
        *t = '\0';
        pscript++;
    }
    else {
        while (!isspace(*pscript) && *pscript != FILE_END_FLAG) {
            *t++ = *pscript++;
        }
        *t = '\0';
    }
}
/*!
 * @brief ������Ҫ�����Ľű�
 * @param source �ű��ı�
 */
void Script_Set (const char * source) {
    if (script != NULL)
        free(script);
    pscript = script = source;
    if (listLabel != NULL)
        DList_Clear(listLabel,free);
    if (source != NULL)
        listLabel = DList_New();
}
/*!
 * @brief ���һ������ǩ
 * @param label ��ǩ����
 * @param offset ƫ����
 */
void Script_AddLabel (const char * name,unsigned long offset) {
    Label * l = (Label*)malloc(sizeof(Label));
    strcpy(l->name,name);
    l->offset = offset;
    DList_Push(listLabel,l);
}
/*!
 * @brief ��ת��ĳ����ǩ
 * @param label ��ǩ����
 * @param offset ƫ����
 */
void Script_GotoLabel (const char * name) {
    DNode * n;
    /* �����б�ǩ��Ѱ�� */
    for (n = listLabel->head; n != NULL ; n = n->next) {
        Label * l = (Label *)n->data;
        if (strEq(l->name,name)) {
            pscript = script + l->offset;
            return;
        }
    }
    /* δ�ҵ������Ѱ�� */

    while (TRUE) {
        Script_GetToken();
        /* �ļ��Ҳ������ */
        if (SCRIPT_END()) {
            Error_Exit("undefined label:%s",name);
        }
        /* �ҵ�����ǩ */
        else if (token[0] == ':') {
            unsigned long offset = Script_GetScriptOffset();
            Script_AddLabel(token+1,offset);
            /* �ҵ���ͬ������ǩ */
            if (strEq(token+1,name)) {
                pscript = script + offset;
                return;
            }
        }

    }
}
/*!
 * @biref ��ȡ�ļ�����
 * @param fileName �ļ���
 */
static char * Script_GetFile (const char * fileName) {
    FILE * f = fopen(fileName,"rb");
    char * text;
    size_t size;

    if (f == NULL) {
        Error_Exit("can not open script:%s",fileName);
    }
    
    fseek(f,0,SEEK_END);
    size = ftell(f);
    fseek(f,0,SEEK_SET);
    text = (char*)malloc(size);
    fread(text,size,1,f);
    fclose(f);

    return text;
}

/*!
 * @brief ��ȡȫ�������ļ�
 */
void Script_LoadGlobalConfig () {
    char * configFile = Script_GetFile (CONFIG_FILE);

    Script_Set(configFile);
    
    config.debug = FALSE;

    while (TRUE) {
        Script_GetToken();

        if (SCRIPT_END()) {
            break;
        }
        else if (TOKEN_IS("debug")) {
            config.debug = TRUE;
        }

    }

    Script_Set(NULL);
}
/*!
 * @brief ��ȡ��Ϸ�����ļ�
 */
void Script_LoadGameConfig () {
    char * configFile = Media_LoadText (GAME_CONFIG_FILE);
    Script_Set(configFile);
    
    config.textX = 0;
    config.textY = 0;
    config.lineMax = 20;

    while (TRUE) {
        Script_GetToken();

        if (SCRIPT_END()) {
            break;
        }
        else if (TOKEN_IS("textX")) {
            Script_GetToken();
            config.textX = atol(token);
        }
        else if (TOKEN_IS("textY")) {
            Script_GetToken();
            config.textY = atol(token);
        }
        else if (TOKEN_IS("nameX")) {
            Script_GetToken();
            config.nameX = atol(token);
        }
        else if (TOKEN_IS("lineMax")) {
            Script_GetToken();
            config.lineMax = atol(token);
        }
    }

    Script_Set(NULL);
}
/*!
 * @brief �ͷ����������ļ�
 */
void Script_FreeConfig () {

}