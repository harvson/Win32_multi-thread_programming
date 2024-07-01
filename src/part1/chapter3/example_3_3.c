/*
* TaskQueS.c
*
* Sample code for "Multithreading Applications in Win32"
* This is from Chapter 3, Listing 3-2
 * 6个任务，3个线程
*
* 调用 ThreadFunc NUM_TASKS 次，使用不超过 THREAD_POOL_SIZE 线程。此版本使用 WaitForSingleObject，它提供了一个非常次优的解决方案。
*
* Build command： cl /MD TaskQueS.c
*/

#define WIN32_LEAN_AND_MEAN

#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <time.h>
#include "../../include/MtVerify.h"

DWORD WINAPI ThreadFunc(LPVOID);

#define THREAD_POOL_SIZE 3
#define MAX_THREAD_INDEX (THREAD_POOL_SIZE-1)
#define NUM_TASKS 6

int main() {
    HANDLE hThrds[THREAD_POOL_SIZE];
    int slot = 0;
    DWORD threadId;
    int i;
    DWORD exitCode;

    for (i = 1; i <= NUM_TASKS; i++) {
        if (i > THREAD_POOL_SIZE) {
            WaitForSingleObject(hThrds[slot], INFINITE);
            MTVERIFY(GetExitCodeThread(hThrds[slot], &exitCode));
            printf("Slot %d terminated\n", exitCode);
            MTVERIFY(CloseHandle(hThrds[slot]));
        }
        MTVERIFY(hThrds[slot] = CreateThread(NULL,
                                             0,
                                             ThreadFunc,
                                             (LPVOID) slot,
                                             0,
                                             &threadId));
        printf("Launched thread #%d (slot %d)\n", i, slot);
        if (++slot > MAX_THREAD_INDEX)
            slot = 0;
    }

    for (slot = 0; slot < THREAD_POOL_SIZE; slot++) {
        WaitForSingleObject(hThrds[slot], INFINITE);
        MTVERIFY(CloseHandle(hThrds[slot]));
    }
    printf("All slots terminated\n");

    return EXIT_SUCCESS;
}

/*
* 这个函数只是随机调用Sleep一段时间，从而模拟一些需要时间的任务。
* 参数“n”是句柄数组的索引，保留它是为了提供信息。
*/
DWORD WINAPI ThreadFunc(LPVOID n) {
    srand(GetTickCount());
    int random_sleep_time = (rand() % 8) * 500 + 500;
//    printf("sleep for %d ms\n", random_sleep_time);
    Sleep(random_sleep_time);
//    printf("Slot %d idle\n", n);
    return ((DWORD) n);
}