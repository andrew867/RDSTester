#include "TestLoop.h"
#include "Network.h"
#include "DeviceComms.h"
#include <stdlib.h>

HANDLE hThreadTestLoop;
DWORD threadIdTestLoop;

extern volatile bool testRunning = false;

static DWORD WINAPI TestLoopThread(LPVOID lpParam) {
    //If you don't care about Rx of keypad or device events you can refresh the screen
    //and/or send packets a whole lot faster than below.
    //The device needs a minimum of 40ms to poll the keypad and report the data
    //if you send data too fast, it can't poll the keyboard hardware, and outbound network
    //packets stop
    TestLoopParams* params = (TestLoopParams*)lpParam;
    if (!params || !params->isRunning) {
        return 1;
    }

    int sockfd = params->sockfd;
    volatile bool* isRunning = params->isRunning;
    char terminalId = 2;
    
    //for (int i = COLOR_BLACK; i <= COLOR_GRAY; i++) {
    //    if (!*isRunning) return 0;
    //    SendClearScreen(sockfd, terminalId, COLOR(FG_COLOR_BLACK, (TextColor)i, TEXT_STATIC));
    //    Sleep(40);
    //}

    while (*isRunning) {
        //SendClearScreenTest(sockfd, terminalId);
        for (int i = COLOR_BLACK; i <= COLOR_GRAY; i++) {
            if (!*isRunning) {
                break;
            }

            SendClearScreen(sockfd, terminalId, COLOR(FG_COLOR_BLACK, (TextColor)i, TEXT_STATIC));
            Sleep(40);
        }

        if (!*isRunning) {
            break;
        }

        SendScreenBuffer(sockfd, terminalId, screen_buffer_test);
        Sleep(40);

        if (!*isRunning) {
            break;
        }

        for (int i = COLOR_BLACK; i <= COLOR_GRAY; i++) {
            if (!*isRunning) {
                break;
            }

            SendClearScreen(sockfd, terminalId, COLOR(FG_COLOR_BLACK, (TextColor)i, TEXT_STATIC));
            Sleep(40);
        }
        //SendClearScreenTest(sockfd, terminalId);
        //Sleep(80);

        if (!*isRunning) {
            break;
        }

        SendTestScreen(sockfd, terminalId);
        Sleep(40);
    }

    free(params);
    return 0;
}


void StartTestLoop(int sockfd, volatile bool* isRunning) {
    TestLoopParams* params = (TestLoopParams*)malloc(sizeof(TestLoopParams));
    if (!params) {
        fprintf(stderr, "Failed to allocate memory for thread params\n");
        return;
    }

    params->sockfd = sockfd;
    params->isRunning = isRunning;

    hThreadTestLoop = CreateThread(NULL, 0, TestLoopThread, params, 0, &threadIdTestLoop);
    printf("TestLoop: Started...\n");
}

void StartTestLoopEx(int sockfd, volatile bool* isRunning, char terminalId) {
    //start a test for the terminalId and store the 
    // thread id, terminalId, running flag
    return;

    TestLoopParams* params = (TestLoopParams*)malloc(sizeof(TestLoopParams));
    if (!params) {
        fprintf(stderr, "Failed to allocate memory for thread params\n");
        return;
    }

    params->sockfd = sockfd;
    params->isRunning = isRunning;
    params->terminalId = terminalId;

    hThreadTestLoop = CreateThread(NULL, 0, TestLoopThread, params, 0, &threadIdTestLoop);
    printf("TestLoop: Started...\n");
}

void StopTestLoop(volatile bool* isRunning) {
    DWORD waitResult;
    testRunning = false;
    waitResult = WaitForSingleObject(hThreadTestLoop, 1000);
    if (waitResult != WAIT_OBJECT_0) {
        fprintf(stderr, "Test loop thread did not exit in time. Forcing termination.\n");
        TerminateThread(hThreadTestLoop, 0);
    }
    printf("TestLoop: TERMINATED!\n");

}

void StopTestLoopEx(volatile bool* isRunning, char terminalId) {
    //find process id of that terminal's test if it's running
    //terminate that process only
    return;

    DWORD waitResult;
    testRunning = false;
    waitResult = WaitForSingleObject(hThreadTestLoop, 1000);
    if (waitResult != WAIT_OBJECT_0) {
        fprintf(stderr, "Test loop thread did not exit in time. Forcing termination.\n");
        TerminateThread(hThreadTestLoop, 0);
    }
    printf("TestLoop: TERMINATED!\n");

}
