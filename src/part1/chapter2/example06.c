
// 节录自 BACKPRNT 的 PrintText( )函数内容
// Asks user which printer to use, then creates
// background printing thread.

#include <windows.h>

HANDLE PrintText(HWND hwndParent, char *pszText) {
    ThreadPrintInfo *pInfo;
    HANDLE hThread;
    DWORD dwThreadId;
    int result;
    DOCINFO docInfo;

    PRINTDLG dlgPrint;

    // Put up Common Dialog for Printing and get hDC.
    memset(&dlgPrint, 0, sizeof(PRINTDLG));
    dlgPrint.lStructSize = sizeof(PRINTDLG);
    dlgPrint.hwndOwner = hwndParent;
    dlgPrint.Flags = PD_ALLPAGES | PD_USEDEVMODECOPIES
                     | PD_NOPAGENUMS | PD_NOSELECTION | PD_RETURNDC;
    dlgPrint.hInstance = hInst;
    if (!PrintDlg(&dlgPrint))
        return;

    // Initialize Printer device
    docInfo.cbSize = sizeof(DOCINFO);
    docInfo.lpszDocName = "Background Printing Example";
    docInfo.lpszOutput = NULL;
    docInfo.lpszDatatype = NULL;
    docInfo.fwType = 0;
    result = StartDoc(dlgPrint.hDC, &docInfo);
    result = StartPage(dlgPrint.hDC);

    pInfo = HeapAlloc(GetProcessHeap(),
                      HEAP_ZERO_MEMORY,
                      sizeof(ThreadPrintInfo));
    pInfo->hDlg = hwndParent;
    pInfo->hWndParent = hwndParent;
    pInfo->hDc = dlgPrint.hDC;
    pInfo->bPrint = TRUE;
    strcpy(pInfo->szText, pszText);

    MTVERIFY(hThread = CreateThread(NULL, 0,
                                    BackgroundPrintThread, (LPVOID) pInfo,
                                    0, &dwThreadId));

    // keep track of all background printing threads
    gPrintJobs[gNumPrinting++] = hThread;

    return hThread;
}
