#ifndef _INPUT_H_
#define _INPUT_H_
/*
 * 游戏需要的按键为：
 * 上、下、左、右、L，R，A，B
 */
#define GKEY_UP			0
#define GKEY_DOWN		1
#define GKEY_LEFT		2
#define GKEY_RIGHT		3
#define GKEY_A			4
#define GKEY_B			5
#define GKEY_L			6
#define GKEY_R			7
#define GKEY_ESCAPE     8

void    Input_Init      ();
void    Input_Update    ();
BOOL    Input_Trigger   (int gkey);
BOOL    Input_Repeat    (int gkey);

#endif