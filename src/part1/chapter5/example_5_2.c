#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <time.h>
#include "../../include/MtVerify.h"

DWORD WINAPI ThreadFunc(LPVOID);

int main() {
    DWORD dwThreadId;
    HANDLE hThread;
    hThread = CreateThread(NULL,
                           0,
                           ThreadFunc,
                           NULL,
                           CREATE_SUSPENDED,
                           &dwThreadId);
    ResumeThread(hThread);
    CloseHandle(hThread);

    getchar();
    return EXIT_SUCCESS;
}

DWORD WINAPI ThreadFunc(LPVOID) {
    printf("Start\n");
    Sleep(3000);
    printf("Over\n");
}
