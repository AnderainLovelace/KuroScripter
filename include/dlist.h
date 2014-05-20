#ifndef _DLIST_H_
#define _DLIST_H_

typedef struct tagDNode {
    struct tagDNode * pre,*next;
    void * data;
}
DNode;

typedef struct {
    DNode * head,* tail;
    int     size;
}
DList;

DNode * DNode_New       ();
DList * DList_New       ();
void    DList_Push      (DList * l,void * data);
void    DList_Clear     (DList * l,void (*freeData)(void *));
void    DList_Remove    (DList * l,void * data,void (*freeData)(void *));
void *  DList_PopFront  (DList * l);

/* 用DList伪装为队列 */
typedef DList DQueue;

#define DQueue_New      DList_New
#define DQueue_Push     DList_Push
#define DQueue_Clear    DList_Clear
#define DQueue_Pop      DList_PopFront

#define DQueue_Top(q)       ((q)->head)
#define DQueue_Bottom(q)    ((q)->tail)

#endif