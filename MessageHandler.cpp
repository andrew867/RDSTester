#include "MessageHandler.h"
#include "Network.h"
#include "DeviceComms.h"
#include "TestLoop.h"
#include "Common.h"


void HandleKeypadPress(int sockfd, char terminalId, char button) {
    int row = button + 1;
    DWORD exitCode;

    switch (button) {
    case 0x0a:
        SendTestScreen(sockfd, terminalId);
        break;
    case 0x0b:
        GetExitCodeThread(hThreadTestLoop, &exitCode);
        if (exitCode != STILL_ACTIVE) {
            testRunning = true;
            StartTestLoop(sockfd, &testRunning);
        } else {
            testRunning = false;
            StopTestLoop(&testRunning);
        }
        break;
    case 0x0c:
        SendKeypadBeep(sockfd, terminalId);
        break;
    case 0x0d:
        SendScreenBuffer(sockfd, terminalId, screen_buffer1);
        break;
    case 0x0e:
        SendScreenBuffer(sockfd, terminalId, screen_buffer);
        break;
    //case 0x0f:
    //    //SendScreenBuffer(sockfd, terminalId, screen_buffer3);
    //    break;
    case 0x00:
        SendClearScreen(sockfd, terminalId, COLOR(FG_COLOR_WHITE, COLOR_BLACK, TEXT_STATIC));
        break;
    case 0x01:
        SendResetVideo(sockfd, terminalId);
        break;
    default:
        TextColor1 colorBG = (TextColor1)((button % 8));
        TextColor colorFG = (TextColor)(((button + 1) % 8) + 1);
        SendClearVideoArea(sockfd, 2, row, row, 1, 40, COLOR(colorFG, colorBG, TEXT_STATIC));
        SendTextLine(sockfd, 2, row, row, COLOR(colorFG, colorBG, TEXT_BLINK), "look at that button");
        printf("OP1_KEYPAD_PRESS: terminal: %d, key: %d\n", terminalId, button);
        break;
    }

}

void HandleDevicePing1(int sockfd, char terminalId, char data) {
    //TODO: Update list of online devices
    //SendDevicePong1(sockfd, terminalId);
    return;
}

void HandleDevicePing2(int sockfd, char terminalId, char data) {
    //TODO: Update list of online keypads
    //SendDevicePong2(sockfd, terminalId);
    return;
}

void HandleServerPong(int sockfd, char terminalId, char data) {
    //Print/update currently online RDS servers
    return;
}

void HandleOpZero(int sockfd, char* buf, int len) {
    if (len < 3) {
        fprintf(stderr, "got invalid message\n");
        return;
    }

    char terminalId = buf[0];
    char opOne = buf[1];
    char opTwo = buf[2];
    char data = buf[3];

    switch (opTwo) {
    case OP0_SERVER_PONG:
        HandleServerPong(sockfd, terminalId, data);
        //printf("OP0_SERVER_PONG: terminal: %d, data: %d\n", terminalId, data);
        break;
    default:
        printf("SERVER_CMD: terminal: %d, opOne: %d, opTwo: %d\n", terminalId, opOne, opTwo);
        break;
    }

    return;
}

void HandleOpOne(int sockfd, char* buf, int len) {
    if (len < 3) {
        fprintf(stderr, "got invalid message\n");
        return;
    }

    char terminalId = buf[0];
    char opOne = buf[1];
    char opTwo = buf[2];
    char data = buf[3];

    switch (opTwo) {
    case OP1_KEYPAD_PRESS:
        HandleKeypadPress(sockfd, terminalId, data);
        //printf("OP1_KEYPAD_PRESS: terminal: %d, key: %d\n", terminalId, data);
        break;
    }

    return;
}

void HandleOpTwo(int sockfd, char* buf, int len) {
    if (len < 3) {
        fprintf(stderr, "got invalid message\n");
        return;
    }

    char terminalId = buf[0];
    char opOne = buf[1];
    char opTwo = buf[2];
    char data = buf[3];

    switch (opTwo) {
    case OP2_DEVICE_ALIVE:
        HandleDevicePing1(sockfd, terminalId, data);
        //printf("OP2_DEVICE_ALIVE: terminal: %d\n", terminalId);
        break;
    case OP2_KEYPAD_ALIVE:
        HandleDevicePing2(sockfd, terminalId, data);
        //printf("OP2_KEYPAD_ALIVE: terminal: %d\n", terminalId);
        break;
    case OP2_DEVICE_BUSY:
        //printf("OP2_DEVICE_BUSY: terminal: %d data: %d\n", terminalId, data);
        break;
    }

    return;
}

void ParseMessage(int sockfd, char* buf, int len) {
    if (len < 2) {
        fprintf(stderr, "got invalid message\n");
        return;
    }

    char terminalId = buf[0];
    char opOne = buf[1];
    char opTwo = buf[2];

    switch (opOne) {
    case SERVER_CMD:
        HandleOpZero(sockfd, buf, len);
        //printf("SERVER_CMD: terminal: %d, opOne: %d, opTwo: %d\n", terminalId, opOne, opTwo);
        break;
    case SYSTEM_CMD:
        HandleOpOne(sockfd, buf, len);
        //printf("SYSTEM_CMD: terminal: %d, opOne: %d, opTwo: %d\n", terminalId, opOne, opTwo);
        break;
    case UNIT_CMD:
        HandleOpTwo(sockfd, buf, len);
        //printf("UNIT_CMD: terminal: %d, opOne: %d, opTwo: %d\n", terminalId, opOne, opTwo);
        break;
    case VIDEO_CMD:
        printf("VIDEO_CMD: terminal: %d, opOne: %d, opTwo: %d\n", terminalId, opOne, opTwo);
        break;
    case TOUCH_CMD:
        printf("TOUCH_CMD: terminal: %d, opOne: %d, opTwo: %d\n", terminalId, opOne, opTwo);
        break;
    case FIRMWARE_CMD:
        printf("FIRMWARE_CMD: terminal: %d, opOne: %d, opTwo: %d\n", terminalId, opOne, opTwo);
        break;
    default:
        printf("UNKNOWN_CMD: terminal: %d, opOne: %d, opTwo: %d\n", terminalId, opOne, opTwo);
        break;
    }
}
