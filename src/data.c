/*!
 * @file data.c
 *
 * @brief 游戏运行时数据的管理
 * @author hikari <anderain.develop@gmail.com>
 * @date 2014-5-13
 */

#include "kuro.h"
#include "error.h"
#include "data.h"
#include "script.h"
#include "media.h"

GameData data;

/*!
 * @brief 初始化游戏数据，开始新游戏
 */
void Data_Init () {
    int i;
    for (i = 0; i < VAR_SIZE; ++i) {
        data.var[i] = 0;
    }
    strcpy(data.scriptName,"main.krs");

    data.selectSize = 0;
    data.scriptOffset = 0;

    strcpy(data.talkBack,"");
    strcpy(data.talkerName,"");

    for (i = 0; i < ROLE_MAX ; ++i) {
        strcpy(data.talkRole[i],"");
    }

    Script_Set(Media_LoadText(data.scriptName));
    Script_SetScriptOffset(data.scriptOffset);
}
/*!
 * @brief 从文件读取数据
 * @param fileName 文件名
 */
void Data_Load(const char * fileName) {
    FILE *  fp;
    int     i,s;

    fp = fopen(fileName,"rb");
    
    if (fp == NULL) 
        Error_Exit("can not load to %s",fileName);

    /* 读取变量、脚本文件名、脚本偏移量 */

    fread(& data,sizeof(GameData),1,fp);
    Script_Set(Media_LoadText(data.scriptName));
    Script_SetScriptOffset(data.scriptOffset);

    /* 读取语句标号 */
    fread(&s,sizeof(s),1,fp);
    for (i = 0;i < s;++i) {
        Label * l = (Label*)malloc(sizeof(Label));
        fread(l->name,LABEL_MAX,1,fp);
        fread(&l->offset,sizeof(l->offset),1,fp);
    }
    fclose(fp);
}
/*!
 * @brief 将数据保存到文件
 * @param fileName 文件名
 */
void Data_Save(const char * fileName) {
    FILE *  fp;
    DNode   *n;

    fp = fopen(fileName,"wb");
    
    if (fp == NULL) 
        Error_Exit("can not save to %s",fileName);

    /* 保存变量、脚本文件名、脚本偏移量 */
    fwrite(& data,sizeof(GameData),1,fp);

    /* 保存语句标号 */
    fwrite(& listLabel->size,sizeof(listLabel->size),1,fp);
    for (n = listLabel->head; n != NULL; n = n->next) {
        Label * l = (Label*)n->data;
        fwrite(l->name,LABEL_MAX,1,fp);
        fwrite(&l->offset,sizeof(l->offset),1,fp);
    }
    fclose(fp);
}
