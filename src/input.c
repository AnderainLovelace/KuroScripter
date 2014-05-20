/*!
 * @file input.c
 *
 * @brief ���봦��
 * @author hikari <anderain.develop@gmail.com>
 * @date 2014-5-15
 */
#include "kuro.h"
#include "input.h"
#include "utils.h"
#if 1
/*
 * ��Ϸ������Ҫ�İ�������
 */
#define KEY_MAX 18
/*
 * ����״̬
 */
static BOOL keyState[KEY_MAX];
/*
 * ����ӳ��
 */
static struct {
	int sdlk;
	int gkey;
}
keyMap[KEY_MAX] = {
	{SDLK_w,GKEY_UP},
	{SDLK_s,GKEY_DOWN},
	{SDLK_a,GKEY_LEFT},
	{SDLK_d,GKEY_RIGHT},
	{SDLK_z,GKEY_A},
	{SDLK_x,GKEY_B},
	{SDLK_q,GKEY_L},
	{SDLK_e,GKEY_R},
	/*
	{SDLK_8,GKEY_UP},
	{SDLK_5,GKEY_DOWN},
	{SDLK_4,GKEY_LEFT},
	{SDLK_6,GKEY_RIGHT},
	{SDLK_1,GKEY_A},
	{SDLK_2,GKEY_B},
	{SDLK_7,GKEY_L},
	{SDLK_9,GKEY_R}*/
	
	{SDLK_UP          ,GKEY_UP},
	{SDLK_DOWN        ,GKEY_DOWN},
	{SDLK_LEFT        ,GKEY_LEFT},
	{SDLK_RIGHT       ,GKEY_RIGHT},
	{SDLK_PLUS        ,GKEY_A},
	{SDLK_MINUS       ,GKEY_B},
	{SDLK_ASTERISK    ,GKEY_L},
	{SDLK_SLASH       ,GKEY_R},
    
    {SDLK_TAB         ,GKEY_ESCAPE},
    {SDLK_BACKSPACE   ,GKEY_ESCAPE}
};
/*!
 * @brief ��ʲôҲ����
 */
void Input_Init () {
}
/*!
 * @brief ˢ�¡���ȡ���벢����
 */
void Input_Update () {
	SDL_Event event;
	while (SDL_PollEvent(&event) != 0) {
		if (event.type == SDL_QUIT) { 
            exit(0);
		}
		else if (event.type == SDL_KEYDOWN) {
			int i = 0,k = event.key.keysym.sym;
			if (k == SDLK_ESCAPE)
				exit(0);
#ifdef ___NDLESS___
            /* �������NSPIRE�����£���ӵ��ڶԱȶȹ��� */
            if (k == SDLK_a)
                ContrastInc();
            if (k == SDLK_b)
                ContrastDec();
#endif
			for (i=0;i<KEY_MAX;++i) {
				if (k == keyMap[i].sdlk) {
					keyState[keyMap[i].gkey] = 1;
				}
			}
		}
		else if (event.type == SDL_KEYUP) {
			int i = 0;
			for (i=0;i<KEY_MAX;++i) {
				if (event.key.keysym.sym == keyMap[i].sdlk) {
					keyState[keyMap[i].gkey] = 0;
				}
			}
		}
	}
}
/*!
 * @brief �жϰ����Ƿ���
 * @param gkey �ڲ���������
 * @return BOOL ָ���ļ�����ʱ����TRUE�����򷵻�FALSE
 *
 * @note ֻ����һ�ΰ���
 */
BOOL Input_Trigger (int gkey) {
	if (keyState[gkey]) {
		keyState[gkey] = FALSE;
		return TRUE;
	}
	return FALSE;
}
/*!
 * @brief �жϰ����Ƿ���
 * @param gkey �ڲ���������
 * @return BOOL ָ���ļ�����ʱ����TRUE�����򷵻�FALSE
 *
 * @note �ظ���ⰴ����״̬
 */
BOOL Input_Repeat (int gkey) {
	if (keyState[gkey]) {
		return TRUE;
	}
	return FALSE;
}
#else
/*
 * ��Ϸ������Ҫ�İ�������
 */
#define KEY_MAX 8
/*
 * ����״̬
 */
static BOOL keyState[KEY_MAX];
static BOOL keyRealState[KEY_MAX];
/*
 * ����ӳ��
 */
static struct {
	int sdlk;
	int gkey;
}
keyMap[KEY_MAX] = {
};
/*!
 * @brief ��ʲôҲ����
 */
void Input_Init () {
	int i;
	for (i = 0;i < KEY_MAX; ++i) {
		keyState[i] = FALSE;
		keyRealState[i] = FALSE;
	}
}
/*!
 * @brief ˢ�¡���ȡ���벢����
 */
void Input_Update () {
	SDL_Event event;
	
	int i;
	
	while (SDL_PollEvent(&event) != 0) {
		if (event.type == SDL_QUIT) { 
			exit(0);
		}
	}
	
	for (i = 0;i < KEY_MAX; ++i) {
		keyRealState[i] = FALSE;
	}
	
	if (isKeyPressed(KEY_NSPIRE_UP))        keyRealState[GKEY_UP]       = TRUE;
	if (isKeyPressed(KEY_NSPIRE_DOWN))		keyRealState[GKEY_DOWN]     = TRUE;
	if (isKeyPressed(KEY_NSPIRE_LEFT))		keyRealState[GKEY_LEFT]     = TRUE;
	if (isKeyPressed(KEY_NSPIRE_RIGHT))		keyRealState[GKEY_RIGHT]    = TRUE;
	if (isKeyPressed(KEY_NSPIRE_A))			keyRealState[GKEY_A]        = TRUE;
	if (isKeyPressed(KEY_NSPIRE_B))			keyRealState[GKEY_B]        = TRUE;
	if (isKeyPressed(KEY_NSPIRE_E))			keyRealState[GKEY_L]        = TRUE;
	if (isKeyPressed(KEY_NSPIRE_F))			keyRealState[GKEY_R]        = TRUE;

	/*
	 * keyRealState��,TURE��ʾ������
	 * keyState�У�TRUE��ʾ���Ѿ�����
	 */
	for (i = 0;i < KEY_MAX; ++i) {
		if (keyRealState[i] == FALSE) {
			keyState[i] = FALSE;
		}
	}
}
/*!
 * @brief �жϰ����Ƿ���
 * @param gkey �ڲ���������
 * @return BOOL ָ���ļ�����ʱ����TRUE�����򷵻�FALSE
 *
 * @note ֻ����һ�ΰ���
 */
BOOL Input_Trigger (int gkey) {
	if (!keyState[gkey] && keyRealState[gkey]) {
		keyState[gkey] = TRUE;
		return TRUE;
	}
	return FALSE;
}
/*!
 * @brief �жϰ����Ƿ���
 * @param gkey �ڲ���������
 * @return BOOL ָ���ļ�����ʱ����TRUE�����򷵻�FALSE
 *
 * @note �ظ���ⰴ����״̬
 */
BOOL Input_Repeat (int gkey) {
	if (keyRealState[gkey]) {
		return TRUE;
	}
	return FALSE;
}
#endif
