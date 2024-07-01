#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <time.h>
#include "../../include/MtVerify.h"

int ReadSomething() {
    BOOL rc;
    HANDLE hFile;
    DWORD numread;
    OVERLAPPED overlap;
    char buf[512];

    // 打开文件进行重叠读取
    hFile = CreateFile("C:\\WINDOWS\\WINFILE.EXE",
                       GENERIC_READ,
                       FILE_SHARE_READ | FILE_SHARE_WRITE,
                       NULL,
                       OPEN_EXISTING,
                       FILE_FLAG_OVERLAPPED,
                       NULL
    );
    if (hFile == INVALID_HANDLE_VALUE)
        return -1;

    // 初始化 OVERLAPPED 结构
    memset(&overlap, 0, sizeof(overlap));
    overlap.Offset = 1500;

    // 请求数据
    rc = ReadFile(
            hFile,
            buf,
            300,
            &numread,
            &overlap
    );

    if (rc) {
        // 数据已成功读取
    } else {
        // 操作是否排队？
        if (GetLastError() == ERROR_IO_PENDING) {
            // 我们可以在这里做一会儿别的事情......

            WaitForSingleObject(hFile, INFINITE);
            rc = GetOverlappedResult(
                    hFile,
                    &overlap,
                    &numread,
                    FALSE
            );
        } else {
            // Something went wrong
        }
    }

    CloseHandle(hFile);

    return TRUE;
}