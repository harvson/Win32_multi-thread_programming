/*
* TaskQueM.c
*
* Sample code for "Multithreading Applications in Win32"
* This is from Chapter 3, Listing 3-3
*
* 调用ThreadFunc NUM_TASKS次数，使用不超过THREAD_POOL_SIZE的线程。这个版本使用WaitForMultipleObjects来提供更优的解决方案。.
* Build command： cl /MD TaskQueM.c
*/

#define WIN32_LEAN_AND_MEAN

#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
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
    DWORD rc;

    for (i = 1; i <= NUM_TASKS; i++) {
        /* 在使用池中的所有线程之前，不需要等待一个线程退出 */
        if (i > THREAD_POOL_SIZE) {
            /* Wait for one thread to terminate */
            rc = WaitForMultipleObjects(
                    THREAD_POOL_SIZE,
                    hThrds,
                    FALSE,
                    INFINITE);
            slot = rc - WAIT_OBJECT_0; // 线程池第slot个线程结束
            MTVERIFY(slot >= 0 && slot < THREAD_POOL_SIZE);
            printf("Slot %d terminated\n", slot);
            MTVERIFY(CloseHandle(hThrds[slot]));
        }
        /* Create a new thread in the given available slot */
        MTVERIFY(hThrds[slot++] = CreateThread(NULL,
                                               0,
                                               ThreadFunc,
                                               (LPVOID) slot,
                                               0,
                                               &threadId));
        printf("Launched thread #%d (slot %d)\n", i - 1, slot);
    }

    /* 现在等待所有线程终止 */
    rc = WaitForMultipleObjects(
            THREAD_POOL_SIZE,
            hThrds,
            TRUE,
            INFINITE);

    MTVERIFY(rc >= WAIT_OBJECT_0 && rc < WAIT_OBJECT_0 + THREAD_POOL_SIZE);
    for (slot = 0; slot < THREAD_POOL_SIZE; slot++)
        MTVERIFY(CloseHandle(hThrds[slot]));
    printf("All slots terminated\n");

    return EXIT_SUCCESS;
}

/*
* 这个函数只是随机调用Sleep一段时间，从而模拟一些需要时间的任务
*
* 参数“n”是句柄数组的索引，保留它是为了提供信息
*/
DWORD WINAPI ThreadFunc(LPVOID n) {
    srand(GetTickCount());

    Sleep((rand() % 10) * 800 + 500);
    printf("Slot %d idle\n", n);
    return ((DWORD) n);
}