#ifndef _SCENE_H_
#define _SCENE_H_

void Scene_Select (BOOL);
int  Scene_Title (int);
void Scene_Talk ();
const char * Scene_Data (int type);

#define SCENE_SAVE 0
#define SCENE_LOAD 1

#endif