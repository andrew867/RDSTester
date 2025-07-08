#include "Common.h"
#include "Network.h"
#include "MessageHandler.h"
#include "TestLoop.h"
#include "RDSTester.h"


static DWORD WINAPI ListenerThread(LPVOID lpParam) {
    const char* listenIpStr = (const char*)lpParam;

    int sockfd;
    if ((sockfd = socket(PF_INET, SOCK_DGRAM, 0)) < 0) {
        fprintf(stderr, "socket() failed\n");
        PrintWSAError();
        return 1;
    }

    g_Sockfd = sockfd;

    int optval = 1;
    if (setsockopt(sockfd, SOL_SOCKET, SO_BROADCAST, (char*)&optval, sizeof(optval)) < 0) {
        fprintf(stderr, "setsockopt() SO_BROADCAST failed\n");
        PrintWSAError();
        return 1;
    }

    if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, (char*)&optval, sizeof(optval)) < 0) {
        fprintf(stderr, "setsockopt() SO_REUSEADDR failed\n");
        PrintWSAError();
        return 1;
    }

    struct sockaddr_in listenAddr;
    memset(&listenAddr, 0, sizeof(listenAddr));
    listenAddr.sin_family = AF_INET;
    listenAddr.sin_port = htons(PORT_CONTROL);

    if (listenIpStr && strlen(listenIpStr) > 0) {
        if (inet_pton(AF_INET, listenIpStr, &(listenAddr.sin_addr)) != 1) {
            fprintf(stderr, "Invalid listen IP address: %s\n", listenIpStr);
            return 1;
        }
        printf("Binding to IP: %s\n", listenIpStr);
    }
    else {
        listenAddr.sin_addr.s_addr = htonl(INADDR_ANY);
        printf("Binding to IP: INADDR_ANY\n");
    }

    if (bind(sockfd, (struct sockaddr*)&listenAddr, sizeof(listenAddr)) == -1) {
        fprintf(stderr, "bind() failed\n");
        PrintWSAError();
        return 1;
    }

    printf("Listener started on port %d\n", PORT_CONTROL);

    while (true) {
        char msg[512];
        struct sockaddr_in from;
        int fromLen = sizeof(from);
        
        memset(&from, 0, sizeof(from));
        
        int len = recvfrom(sockfd, msg, sizeof(msg), 0, (struct sockaddr*)&from, &fromLen);
        if (len == -1) {
            fprintf(stderr, "recvfrom() failed\n");
            PrintWSAError();
            break;
        }

        char addrStr[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &(from.sin_addr), addrStr, INET_ADDRSTRLEN);
        //printf("Received %d bytes from %s:%d\n", len, addrStr, ntohs(from.sin_port));

        if (len > 0) {
            ParseMessage(g_Sockfd, msg, len);
        }
        else {
            printf("Received empty message, exiting loop\n");
            break;
        }
    }

    closesocket(sockfd);
    return 0;
}

void DoExit() {
    TerminateThread(hThread, 0);

    if (testRunning) {
        testRunning = false;
        DWORD waitResult = WaitForSingleObject(hThreadTestLoop, 3000);
        if (waitResult != WAIT_OBJECT_0) {
            fprintf(stderr, "Test loop thread did not exit in time. Forcing termination.\n");
            TerminateThread(hThreadTestLoop, 0);
        }
    }

    WSACleanup();
    exit(0);
}

int main() {
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        fprintf(stderr, "WSAStartup() failed.\n");
        return 1;
    }

    hThread = CreateThread(NULL, 0, ListenerThread, (LPVOID)"0.0.0.0", 0, &threadId);
    printf("Started thread!\n");
    
    char key;
    char terminalId = 2;
    DWORD exitCode;

    while (key = getchar()) {
        switch (key) {
        case 'l':
            GetExitCodeThread(hThreadTestLoop, &exitCode);
            if (exitCode != STILL_ACTIVE) {
                testRunning = true;
                StartTestLoop(g_Sockfd, &testRunning);
            }
            else {
                testRunning = false;
                StopTestLoop(&testRunning);
            }
            break;
        case 's':
            SendScreenBuffer(g_Sockfd, terminalId, screen_buffer1);
            break;
        case 'c':
            SendClearScreen(g_Sockfd, terminalId, COLOR(FG_COLOR_BLACK, COLOR_BLACK, TEXT_STATIC));
            break;
        case 'r':
            SendResetVideo(g_Sockfd, terminalId);
            break;
        case 't':
            SendTestScreen(g_Sockfd, terminalId);
            break;
        case 'b':
            SendKeypadBeep(g_Sockfd, terminalId);
            break;
        case 'q':
            DoExit();
            break;
        default:
            //printf("another key\n");
            break;
        }
    }


    return 0;   
}
