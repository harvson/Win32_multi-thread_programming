/*
* TaskQueM.c
*
* Sample code for "Multithreading Applications in Win32"
* This is from Chapter 3, Listing 3-3
*
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
    DWORD nWaitCount;
    HANDLE hWaitArray[4];
    BOOL quit;
    int exitCode;

    while (!quit) {
        MSG msg;
        int rc;

        rc = MsgWaitForMultipleObjects(
                nWaitCount,
                hWaitArray,
                FALSE,
                INFINITE,
                QS_ALLINPUT);

        if (rc == WAIT_OBJECT_0 + nWaitCount) {
            while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) { // Get Next message in queue
                if (msg.message == WM_QUIT) {
                    quit = TRUE;
                    exitCode = msg.wParam;
                    break;
                } // end if
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            } // end while
        } else if (rc >= WAIT_OBJECT_0 && rc < WAIT_OBJECT_0 + nWaitCount) {
            int nIndex = rc - WAIT_OBJECT_0;
            // 我们现在知道，在数组位置nindex的句柄被发出信号。我们必须跟踪这些句柄的含义，以决定下一步该做什么。
        } else if (rc == WAIT_TIMEOUT) {
            // Timeout expired
        } else if (rc >= WAIT_ABANDONED_0 && rc < WAIT_ABANDONED_0 + nWaitCount) {
            int nIndex = rc - WAIT_ABANDONED_0;
            // 拥有互斥锁的线程死亡，详见第4章
        } else {
            //出了什么问题
        }
    }
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
