/*!
 * @file data.c
 *
 * @brief ��Ϸ����ʱ���ݵĹ���
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
 * @brief ��ʼ����Ϸ���ݣ���ʼ����Ϸ
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
 * @brief ���ļ���ȡ����
 * @param fileName �ļ���
 */
void Data_Load(const char * fileName) {
    FILE *  fp;
    int     i,s;

    fp = fopen(fileName,"rb");
    
    if (fp == NULL) 
        Error_Exit("can not load to %s",fileName);

    /* ��ȡ�������ű��ļ������ű�ƫ���� */

    fread(& data,sizeof(GameData),1,fp);
    Script_Set(Media_LoadText(data.scriptName));
    Script_SetScriptOffset(data.scriptOffset);

    /* ��ȡ����� */
    fread(&s,sizeof(s),1,fp);
    for (i = 0;i < s;++i) {
        Label * l = (Label*)malloc(sizeof(Label));
        fread(l->name,LABEL_MAX,1,fp);
        fread(&l->offset,sizeof(l->offset),1,fp);
    }
    fclose(fp);
}
/*!
 * @brief �����ݱ��浽�ļ�
 * @param fileName �ļ���
 */
void Data_Save(const char * fileName) {
    FILE *  fp;
    DNode   *n;

    fp = fopen(fileName,"wb");
    
    if (fp == NULL) 
        Error_Exit("can not save to %s",fileName);

    /* ����������ű��ļ������ű�ƫ���� */
    fwrite(& data,sizeof(GameData),1,fp);

    /* ��������� */
    fwrite(& listLabel->size,sizeof(listLabel->size),1,fp);
    for (n = listLabel->head; n != NULL; n = n->next) {
        Label * l = (Label*)n->data;
        fwrite(l->name,LABEL_MAX,1,fp);
        fwrite(&l->offset,sizeof(l->offset),1,fp);
    }
    fclose(fp);
}
