#ifndef _DATA_H_
#define _DATA_H_

#define VAR_SIZE    255
#define ITEM_SIZE   20
/*
 * 角色最大数量
 */
#define ROLE_MAX    9

typedef struct {
    /* 变量 */
    int         var[VAR_SIZE];
    /* 当前正在执行的脚本 */
    char        scriptName[128];
    /* 脚本已经执行的字节 */
    unsigned long scriptOffset;
    /* 选择画面的条目数量 */
    int         selectSize;
    /* 选择画面的条目 */ 
    char *      selectItem[ITEM_SIZE];
    /* 当前的背景 */
    char        talkBack[128];
    /* 当前人物 */
    char        talkRole[ROLE_MAX][128];
    /* 当前人物坐标 */
    struct {
        int x;
        int y;
    }talkRolePos[ROLE_MAX];
    /* 当前说话者 */
    char        talkerName[32];
}
GameData;

extern GameData data;

void Data_Init();
void Data_Save(const char * fileName);
void Data_Load(const char * fileName);

#endif