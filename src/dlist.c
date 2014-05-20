#include "kuro.h"
#include "dlist.h"
#include "error.h"
/*!
 * @brief 生成一个空的双向链表节点
 * @return Dlist* 生成的节点对象
 */
DNode * DNode_New () {
    DNode * n;
    n = (DNode*)malloc(sizeof(DNode));
    n->data = NULL;
    n->pre  = NULL;
    n->next = NULL;
    return n;
}
/*!
 * @brief 生成一个空的双向链表
 * @return Dlist* 生成的链表对象
 */
DList * DList_New () {
    DList * l;
    l = (DList*)malloc(sizeof(DList));
    l->head = NULL;
    l->tail = NULL;
    l->size = 0;
    return l;
}
/*!
 * @brief 将一个元素插入链表尾部
 * @param l 链表
 * @param data 需要插入的对象
 */
void DList_Push(DList * l,void * data) {
    if (l->tail == NULL) {
        l->head = l->tail = DNode_New();
    }
    else {
        DNode * pre = l->tail;
        l->tail->next = DNode_New();
        l->tail = l->tail->next;
        l->tail->pre = pre;
    }
    l->tail->data = data;
    l->size++;
}
/*!
 * @brief 清除链表内容
 * @param l 链表
 * @param freeData 释放data的函数
 *
 * @note freeData为NULL时不释放
 */
void DList_Clear(DList * l,void (*freeData)(void *)) {
    DNode * n1,* n2;
    for (n1 = l->head;n1 != NULL; n1 = n2) {
        n2 = n1->next;
        if (freeData != NULL)
            freeData(n1->data);
        free(n1);
    }
    l->head = NULL;
    l->tail = NULL;
    l->size = 0;
}
/*!
 * @brief 删除链表中的一个元素
 * @param l 链表
 * @param data 想要释放的元素
 * @param freeData 释放data的函数
 *
 * @note freeData为NULL时不释放
 */
void DList_Remove(DList * l,void * data,void (*freeData)(void *)) {
    DNode * n;
    for (n=l->head;n != NULL;n = n->next) {
        if (n->data == data)
            break;
    }
    if (n != NULL) {
        /* head */
        if (n->pre == NULL) {
            l->head = n->next;
            if (n->next != NULL)
                n->next->pre = n->pre;
            if (freeData != NULL)
                freeData(n->data);
            free(n);
        }
        /* tail */
        else if (n->next == NULL) {
            l->tail = n->pre;
            l->tail->next = NULL;
            if (freeData != NULL && n->data != NULL)
                freeData(n->data);
            free(n);
        }
        else {
            n->pre->next = n->next;
            n->next->pre = n->pre;
            if (freeData != NULL && n->data != NULL)
                freeData(n->data);
            free(n);
        }
        l->size--;
    }
    /*
    for (n=l->tail;n != NULL;n = n->pre) {
        if (data == n->data) {
            DNode * pre,* next;
            pre  = n->pre;
            next = n->next;
            if (pre != NULL)
                pre->next = next;
            if (next != NULL)
                next->pre = pre;
            if (freeData != NULL) {
                freeData(n->data);
            }
            free(n);
            l->size--;
            break;
        }
    }*/
    if (l->size <= 0) {
        l->head = NULL;
        l->tail = NULL;
    }
}
/*!
 * @brief 移除链表收个元素并返回
 * @param l 链表
 */
void * DList_PopFront  (DList * l) {
    DNode * node;
    void * pData;

    if (l->size <= 0) {
        Error_Exit("DList_PopFront:list empty!");
    }
    node = l->head;
    l->head = node->next;
    
    pData = node->data;

    free(node);
    l->size--;

    if (l->size == 0)
        l->tail = NULL;
    else
        l->head->pre = NULL;

    return pData;
}