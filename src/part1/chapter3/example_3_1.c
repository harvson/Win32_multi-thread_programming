/*
* ExitThread.c
*
* Sample code for "Multithreading Applications in Win32"
* This is from Chapter 2, Listing 2-3
*
* Demonstrate ExitThread
*/

#define WIN32_LEAN_AND_MEAN

#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

DWORD WINAPI ThreadFunc(LPVOID);

void AnotherFunc();

int main() {
    HANDLE hThrd;
    DWORD threadId;

    hThrd = CreateThread(NULL,
                         0,
                         ThreadFunc,
                         (LPVOID) 1,
                         0,
                         &threadId);
    if (hThrd)
        printf("Thread launched\n");

    while (1) {
        DWORD state = WaitForSingleObject(hThrd, 500); // 每500毫秒 time-out 一次
        if (state == WAIT_TIMEOUT) {
            puts("Time out!");
        } else if (state == WAIT_FAILED) {
            printf("err code = %lu\n", GetLastError());
        } else if (state == WAIT_OBJECT_0) {
            printf("state changed~ state = %lu\n", state);
            break;
        }
    }

    CloseHandle(hThrd);

    return EXIT_SUCCESS;
}


DWORD WINAPI ThreadFunc(LPVOID n) {
    printf("Thread running\n");
    AnotherFunc();
    return 0;
}

void AnotherFunc() {
    printf("About to exit thread\n");
    Sleep(3000);
}