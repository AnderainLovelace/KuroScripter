#ifndef _DATA_H_
#define _DATA_H_

#define VAR_SIZE    255
#define ITEM_SIZE   20
/*
 * ��ɫ�������
 */
#define ROLE_MAX    9

typedef struct {
    /* ���� */
    int         var[VAR_SIZE];
    /* ��ǰ����ִ�еĽű� */
    char        scriptName[128];
    /* �ű��Ѿ�ִ�е��ֽ� */
    unsigned long scriptOffset;
    /* ѡ�������Ŀ���� */
    int         selectSize;
    /* ѡ�������Ŀ */ 
    char *      selectItem[ITEM_SIZE];
    /* ��ǰ�ı��� */
    char        talkBack[128];
    /* ��ǰ���� */
    char        talkRole[ROLE_MAX][128];
    /* ��ǰ�������� */
    struct {
        int x;
        int y;
    }talkRolePos[ROLE_MAX];
    /* ��ǰ˵���� */
    char        talkerName[32];
}
GameData;

extern GameData data;

void Data_Init();
void Data_Save(const char * fileName);
void Data_Load(const char * fileName);

#endif