#include <windows.h>
#include <tchar.h>
#include <iostream>

// 全局变量，用于存储事件句柄
HANDLE g_hEvent = NULL;

// 等待事件的线程函数
DWORD WINAPI
WaitForEventThread(LPVOID lpParam) {
    int threadId = reinterpret_cast<int>(lpParam);
    _tprintf(_T("Thread %d waiting for event...\n"), threadId);

    // 等待事件被触发
    WaitForSingleObject(g_hEvent, INFINITE);

    _tprintf(_T("Thread %d: Event has been triggered!\n"), threadId);

    return 0;
}

int _tmain() {
    // 创建手动重置事件对象
    g_hEvent = CreateEvent(NULL, TRUE, FALSE, _T("MyManualResetEvent"));
    if (g_hEvent == NULL) {
        _tprintf(_T("Failed to create event (error %d)\n"), GetLastError());
        return 1;
    }

    // 创建等待事件的线程1
    HANDLE hThread1 = CreateThread(NULL, 0, WaitForEventThread, reinterpret_cast<LPVOID>(1), 0, NULL);
    if (hThread1 == NULL) {
        _tprintf(_T("Failed to create thread 1 (error %d)\n"), GetLastError());
        CloseHandle(g_hEvent);
        return 1;
    }

    // 创建等待事件的线程2
    HANDLE hThread2 = CreateThread(NULL, 0, WaitForEventThread, reinterpret_cast<LPVOID>(2), 0, NULL);
    if (hThread2 == NULL) {
        _tprintf(_T("Failed to create thread 2 (error %d)\n"), GetLastError());
        CloseHandle(hThread1);
        CloseHandle(g_hEvent);
        return 1;
    }

    // 做一些工作...
    _tprintf(_T("Main Thread: Doing some work...\n"));
    Sleep(3000);

    // 模拟事件的触发
    _tprintf(_T("Main Thread: Triggering the event...\n"));
    SetEvent(g_hEvent);

    // 等待线程结束
    WaitForSingleObject(hThread1, INFINITE);
    WaitForSingleObject(hThread2, INFINITE);

    // 关闭线程和事件句柄
    CloseHandle(hThread1);
    CloseHandle(hThread2);
    CloseHandle(g_hEvent);

    return 0;
}
