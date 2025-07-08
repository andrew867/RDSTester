#pragma once
#pragma warning(disable: 4244) // conversion warnings for int to char in below screen buffers
#pragma warning(disable: 4838)
#pragma warning(disable: 4309)
#include "Common.h"
#include <windows.h>
#include <stdlib.h>


#ifdef __cplusplus
extern "C" {
#endif

    extern HANDLE hThreadTestLoop;
    extern DWORD threadIdTestLoop;

    static char screen_buffer_test[25][81] = {
        "********************************************************************************",
        "*                                                                              *",
        "*                                                                              *",
        "*                                                                              *",
        "*                                                                              *",
        "*                                                                              *",
        "*                                                                              *",
        "*                                                                              *",
        "*                                                                              *",
        "*                                                                              *",
        "*                                                                              *",
        "*                                                                              *",
        "*                                HELLO WORLD                                   *",
        "*                                                                              *",
        "*                                                                              *",
        "*                                                                              *",
        "*                                                                              *",
        "*                                                                              *",
        "*                                                                              *",
        "*                                                                              *",
        "*                                                                              *",
        "*                                                                              *",
        "*                                                                              *",
        "*                                                                              *",
        "********************************************************************************"
    };

    typedef struct {
        int sockfd;
        volatile bool* isRunning;
        char terminalId;
    } TestLoopParams;

    //typedef struct {
    //    char terminalId;
    //    DWORD threadId;
    //    bool isRunning;
    //} TerminalTestParams;

    //extern TerminalTestParams* TerminalTests;

    extern volatile bool testRunning;

    void StartTestLoop(int sockfd, volatile bool* isRunning);
    void StartTestLoopEx(int sockfd, volatile bool* isRunning, char terminalId);
    void StopTestLoop(volatile bool* isRunning);
    void StopTestLoopEx(volatile bool* isRunning, char terminalId);

#ifdef __cplusplus
}
#endif
