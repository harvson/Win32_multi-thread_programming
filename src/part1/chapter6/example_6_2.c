#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <time.h>
#include "../../include/MtVerify.h"

// 需要将事件保留在它们自己的数组中，以便我们可以等待它们。
HANDLE ghEvents[MAX_REQUESTS];
// 跟踪每个单独的 I/O 操作
OVERLAPPED gOverlapped[MAX_REQUESTS];
// 处理相关文件。
HANDLE ghFile;
// 需要一个地方来放置所有这些数据
char gBuffers[MAX_REQUESTS][READ_SIZE];

int QueueRequest(int nIndex, DWORD dwLocation, DWORD dwAmount) {
    int i;
    BOOL rc;
    DWORD dwNumread;
    DWORD err;

    MTVERIFY(
            ghEvents[nIndex] = CreateEvent(
                    NULL, // No security
                    TRUE, // 手动复位 - 极其重要！
                    FALSE, // 最初将事件设置为非信号状态
                    NULL // No name
            )
    );
    gOverlapped[nIndex].hEvent = ghEvents[nIndex];
    gOverlapped[nIndex].Offset = dwLocation;

    for (i = 0; i < MAX_TRY_COUNT; i++) {
        rc = ReadFile(
                ghFile,
                gBuffers[nIndex],
                dwAmount,
                &dwNumread,
                &gOverlapped[nIndex]
        );

        // Handle success
        if (rc) {
            printf("Read #%d completed immediately.\n", nIndex);
            return TRUE;
        }

        err = GetLastError();

        // Handle the error that isn't an error. rc is zero here.
        if (err == ERROR_IO_PENDING) {
            // 异步 I/O 仍在进行中
            printf("Read #%d queued for overlapped I/O.\n", nIndex);
            return TRUE;
        }

        // 处理可恢复的错误
        if (err == ERROR_INVALID_USER_BUFFER ||
            err == ERROR_NOT_ENOUGH_QUOTA ||
            err == ERROR_NOT_ENOUGH_MEMORY) {
            Sleep(50); // 请稍等片刻，稍后再试
            continue;
        }

        // Give up on fatal error.
        break;
    }

    printf("ReadFile failed.\n");
    return -1;
}