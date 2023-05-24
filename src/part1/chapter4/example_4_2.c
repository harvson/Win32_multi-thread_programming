#include <windows.h>

struct Node {
    struct Node *next;
    int data;
};

struct List {
    struct Node *head;
    HANDLE hMutex;
};

struct List *CreateList() {
    struct List *list = (struct List *) malloc(sizeof(struct List));
    list->head = NULL;
    list->hMutex = CreateMutex(NULL, FALSE, NULL);
    return list;
}

void DeleteList(struct List *list) {
    CloseHandle(list->hMutex);
    free(list);
}

void SwapLists(struct List *list1, struct List *list2) {
    struct Node *tmp_node;
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

int main() {
    return 0;
}