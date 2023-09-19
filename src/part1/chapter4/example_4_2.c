#include <windows.h>
#include <stdio.h>

typedef struct _Node {
    struct _Node *next;
    int data;
} Node;

typedef struct _List {
    Node *head;
    HANDLE hMutex;
} List;

List *CreateList() {
    List *list = (List *) malloc(sizeof(List));
    list->head = NULL;
    list->hMutex = CreateMutex(NULL, FALSE, NULL);
    return list;
}

void DeleteList(List *list) {
    CloseHandle(list->hMutex);
    free(list);
}

void AddHead(List *pList, Node *node) {
    WaitForSingleObject(pList->hMutex, INFINITE);
    node->next = pList->head;
    pList->head = node;
    ReleaseMutex(pList->hMutex);
}

void Insert(List *pList, Node *afterNode, Node *newNode) {
    WaitForSingleObject(pList->hMutex, INFINITE);
    if (afterNode == NULL) {
        AddHead(pList, newNode);
    } else {
        newNode->next = afterNode->next;
        afterNode->next = newNode;
    }
    ReleaseMutex(pList->hMutex);
}

Node *Next(List *pList, Node *node) {
    Node *next;
    WaitForSingleObject(pList->hMutex, INFINITE);
    next = node->next;
    ReleaseMutex(pList->hMutex);
    return next;
}

void SwapLists(List *list1, List *list2) {
    Node *tmp_node;
    HANDLE arrhandles[2];

    arrhandles[0] = list1->hMutex;
    arrhandles[1] = list2->hMutex;
    WaitForMultipleObjects(2, arrhandles, TRUE, INFINITE);
    tmp_node = list1->head;
    list1->head = list2->head;
    list2->head = tmp_node;
    ReleaseMutex(arrhandles[0]);
    ReleaseMutex(arrhandles[1]);
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
