#include <windows.h>

typedef struct _Node {
    struct _Node *next;
    int data;
} Node;

typedef struct _List {
    Node *head;
    CRITICAL_SECTION critical_sec; // 临界区
} List;

List *CreateList() {
    List *pList = (List *) malloc(sizeof(List));
    pList->head = NULL;
    InitializeCriticalSection(&pList->critical_sec);
    return pList;
}

void DeleteList(List *pList) {
    DeleteCriticalSection(&pList->critical_sec);
    free(pList);
}

void AddHead(List *pList, Node *node) {
    EnterCriticalSection(&pList->critical_sec);
    node->next = pList->head;
    pList->head = node;
    LeaveCriticalSection(&pList->critical_sec);
}

void Insert(List *pList, Node *afterNode, Node *newNode) {
    EnterCriticalSection(&pList->critical_sec);
    if (afterNode == NULL) {
        AddHead(pList, newNode);
    } else {
        newNode->next = afterNode->next;
        afterNode->next = newNode;
    }
    LeaveCriticalSection(&pList->critical_sec);
}

Node *Next(List *pList, Node *node) {
    Node *next;
    EnterCriticalSection(&pList->critical_sec);
    next = node->next;
    LeaveCriticalSection(&pList->critical_sec);
    return next;
}

void SwapLists(List *list1, List *list2) {
    Node *temp_node;
    EnterCriticalSection(&list1->critical_sec);
    EnterCriticalSection(&list2->critical_sec);
    temp_node = list1->head;
    list1->head = list2->head;
    list2->head = temp_node;
    LeaveCriticalSection(&list1->critical_sec);
    LeaveCriticalSection(&list2->critical_sec);
}

int main() {
    return 0;
}