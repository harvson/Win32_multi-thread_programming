#include <windows.h>
#include <stdio.h>

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

DWORD WINAPI ThreadFunction(LPVOID lpParam) {
    List *list = (List *) lpParam;

    for (int i = 1; i <= 5; i++) {
        Node *newNode = (Node *) malloc(sizeof(Node));
        if (newNode == NULL) {
            // 处理内存分配失败的情况
            fprintf(stderr, "Memory allocation failed.\n");
            return 1;
        }
        newNode->data = i;
        newNode->next = NULL;

        // 向链表添加新节点
        AddHead(list, newNode);

        // 模拟其他操作
        Sleep(100);
    }

    return 0;
}


int main() {
    List *list1 = CreateList();
    List *list2 = CreateList();

    // 创建两个线程来操作链表
    HANDLE thread1 = CreateThread(NULL, 0, ThreadFunction, list1, 0, NULL);
    HANDLE thread2 = CreateThread(NULL, 0, ThreadFunction, list2, 0, NULL);

    // 等待线程完成
    WaitForSingleObject(thread1, INFINITE);
    WaitForSingleObject(thread2, INFINITE);

    // 打印链表内容
    Node *currentNode = list1->head;
    printf("List 1: ");
    while (currentNode != NULL) {
        printf("%d ", currentNode->data);
        currentNode = Next(list1, currentNode);
    }
    printf("\n");

    currentNode = list2->head;
    printf("List 2: ");
    while (currentNode != NULL) {
        printf("%d ", currentNode->data);
        currentNode = Next(list2, currentNode);
    }
    printf("\n");

    // 清理资源
    DeleteList(list1);
    DeleteList(list2);

    CloseHandle(thread1);
    CloseHandle(thread2);

    return 0;
}