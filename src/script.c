/*!
 * @file script.c
 *
 * @brief 脚本的解析与配置文件
 * @author hikari <anderain.develop@gmail.com>
 * @date 2014-5-12
 */
#include "kuro.h"
#include "script.h"
#include "media.h"
#include "error.h"

/*
 * 脚本解析
 */
char *        script = NULL;
const char *  pscript = NULL;
DList      *  listLabel = NULL;
/*
 * 配置文件
 */

Configure config;

char    token[TOKEN_MAX];

/*!
 * @brief 获取脚本执行过的字节数
 */
unsigned long Script_GetScriptOffset () {
    return pscript - script;
}
/*!
 * @brief 设置脚本执行过的字节数
 */
void Script_SetScriptOffset (unsigned long offset) {
    pscript = script + offset;
}
/*!
 * @brief 从脚本中获取一个语法元素
 * @note 获取结果在变量token中
 */
void Script_GetToken () {
    char * t = token;
    
    if (pscript == NULL) {
        *t++ = FILE_END_FLAG;
        *t = 0;
        return;
    }
    
    /* 跳过空白 */
    while (isspace(*pscript))
        pscript++;
    /* 跳过单行注释 */
    if (*pscript == '#') {
        while (*pscript != '\n' && *pscript != FILE_END_FLAG) {
            pscript++;
        }
        *t = '\0';
    }
    /* 文件结束 */
    else if (*pscript == FILE_END_FLAG) {
        *t++ = *pscript++;
        *t = '\0';
    }
    /* 获取内容 */
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
 * @brief 设置需要解析的脚本
 * @param source 脚本文本
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
 * @brief 添加一个语句标签
 * @param label 标签名字
 * @param offset 偏移量
 */
void Script_AddLabel (const char * name,unsigned long offset) {
    Label * l = (Label*)malloc(sizeof(Label));
    strcpy(l->name,name);
    l->offset = offset;
    DList_Push(listLabel,l);
}
/*!
 * @brief 跳转到某个标签
 * @param label 标签名字
 * @param offset 偏移量
 */
void Script_GotoLabel (const char * name) {
    DNode * n;
    /* 在已有标签中寻找 */
    for (n = listLabel->head; n != NULL ; n = n->next) {
        Label * l = (Label *)n->data;
        if (strEq(l->name,name)) {
            pscript = script + l->offset;
            return;
        }
    }
    /* 未找到，向后寻找 */

    while (TRUE) {
        Script_GetToken();
        /* 文件找不到标号 */
        if (SCRIPT_END()) {
            Error_Exit("undefined label:%s",name);
        }
        /* 找到语句标签 */
        else if (token[0] == ':') {
            unsigned long offset = Script_GetScriptOffset();
            Script_AddLabel(token+1,offset);
            /* 找到相同的语句标签 */
            if (strEq(token+1,name)) {
                pscript = script + offset;
                return;
            }
        }

    }
}
/*!
 * @biref 获取文件内容
 * @param fileName 文件名
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
 * @brief 读取全局配置文件
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
 * @brief 读取游戏配置文件
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
 * @brief 释放所有配置文件
 */
void Script_FreeConfig () {

}