#ifndef __LIST_H__
#define __LIST_H__
typedef struct listNode {
    struct listNode* prev;
    struct listNode* next;
    void* value;
} listNode;

typedef struct list {
    listNode* head;
    listNode* tail;
    unsigned long len;
    unsigned long capacity;
}list;

list *listCreate(int n);
list *listAddNodeHead(list *list, void *value);
list *listAddNodeTail(list *list, void *value);



#endif
