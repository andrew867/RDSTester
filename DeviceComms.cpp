#include "Network.h"
#include "ScreenBuffers.h"
#include "NetworkBuffers.h"
#include "DeviceComms.h"

#pragma warning(disable: 4244) //conversion warnings for int to char in below screen buffers
#pragma warning(disable: 4838)
#pragma warning(disable: 4309)

// TODO: can't return a local array like that, you would need to dynamically allocate it
static char* DevicePong1(int deviceId, int *dataLen) {
    char bytes[] = { 0xff, 0x3, 0x13, 0xc0, 0xa8, 0xb, 0x59, 0x0, 0x83, 0xff, 0xff, 0xff, 0x0, 0x2 };
    return bytes;
}

static char* DevicePong2(int deviceId, int* dataLen) {
    char bytes[] = { 0xff, 0x0, 0x1, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0xbc, 0xfd, 0x6b, 0x7, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x52, 0x44, 0x53, 0x43, 0x6f, 0x6e, 0x73, 0x6f, 0x6c, 0x65, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x1, 0x0, 0x0, 0x0, 0xff, 0xff, 0x34, 0x12 };
    return bytes;
}

static char* SetSerial(int deviceId, int* dataLen) {
    char bytes[] = { 0x1, 0x3, 0x12, 0x44, 0x45, 0x34, 0x2d, 0x31, 0x38, 0x34, 0x38, 0x34, 0x35, 0x0, 0x2 };
    return bytes;
}

void SendDevicePong1(int sockfd, int deviceId) {
    //TODO: complete this
    //needs to be a broadcast message and not to one device
    int dataLen;
    //fprintf(stderr, "send pong 1\n");

    const char* data = DevicePong1(deviceId, &dataLen);
    StartNetworkOutput(deviceId, sockfd, PORT_CMD);
    SendToNetwork(deviceId, data, dataLen, sockfd, PORT_CMD);
    EndNetworkOutput(deviceId, sockfd, PORT_CMD);
    //free(data);
}

void SendDevicePong2(int sockfd, int deviceId) {
    //TODO: complete this
    //needs to be a broadcast message and not to one device
    int dataLen;
    //fprintf(stderr, "send pong 2\n");

    const char* data = DevicePong2(deviceId, &dataLen);
    StartNetworkOutput(deviceId, sockfd, PORT_CONTROL);
    SendToNetwork(deviceId, data, dataLen, sockfd, PORT_CONTROL);
    EndNetworkOutput(deviceId, sockfd, PORT_CONTROL);
    //free(data);
}

char* TextLine(int deviceId, int row, int col, TextColor color, const char* str, int *dataLen) {
    int bufSize;
    char* buf;

    bufSize = strlen(str) + 7;
    buf = (char*)malloc(bufSize);

    buf[0] = (char)deviceId;
    buf[1] = VIDEO_CMD;
    buf[2] = OP3_DISPLAY_TEXT;
    buf[3] = (char)(row - 1);
    buf[4] = (char)(col - 1);
    buf[5] = (char)color;

    strcpy(buf + 6, str);

    *dataLen = bufSize;
    return buf;
}

void SendTextLine(int sockfd, int deviceId, int row, int col, TextColor color, const char* str) {
    int bufSize;
    char* buf;

    bufSize = strlen(str) + 7;
    buf = TextLine(deviceId, row, col, color, str, &bufSize);

    SendOne(
        sockfd, buf, bufSize,
        deviceTable[deviceId].ipAddr,
        PORT_CMD
    );
    free(buf);
}

void SendScreenBuffer(int sockfd, int deviceId, char screen_buffer[25][81]) {
    StartScreenOutput(deviceId, sockfd);
    for (int row = 0; row < 25; ++row) {
        int dataLen;
        char* data = TextLine(
            deviceId,
            row + 1,
            1,
            COLOR(FG_COLOR_LIGHT_GREEN, COLOR_BLACK, TEXT_STATIC),
            (char*)screen_buffer[row],
            &dataLen
        );
        SendToScreen(deviceId, data, dataLen, sockfd);

        free(data);
    }
    EndScreenOutput(deviceId, sockfd);
}

void SendDrawBox(int sockfd, int deviceId, int topLeft, int botLeft, int botRight, int topRight, char boxType1, char boxType2) {
    if (topLeft > botLeft
        || topLeft < 1
        || topLeft > 25
        || botLeft < 1
        || botLeft > 25) {
        return;
    }
    if (botRight > topRight
        || botRight < 1
        || botRight > 80
        || topRight < 1
        || topRight > 80) {
        return;
    }
    char pkt[] = {
        (char)deviceId,
        VIDEO_CMD,
        OP3_DRAW_BOX,
        (char)(topLeft - 1),
        (char)(botRight - 1),
        (char)(botLeft - 1),
        (char)(topRight - 1),
        boxType1,
        boxType2
    };

    SendOne(sockfd, pkt, sizeof(pkt),
        deviceTable[deviceId].ipAddr, PORT_CMD);
}


//Max screen we have is 25x80?
void SendClearVideoArea(int sockfd, int deviceId, int rowStart, int rowEnd, int colStart, int colEnd, TextColor color) {
    if (rowStart > rowEnd || rowStart < 1 || rowEnd < 1 || rowStart > 25 || rowEnd > 25) {
        return;
    }

    if (colStart > colEnd || colStart < 1 || colEnd < 1 || colStart > 80 || colEnd > 80) {
        return;
    }

    char buf[8];

    buf[0] = deviceId;
    buf[1] = VIDEO_CMD;
    buf[2] = OP3_VIDEO_AREA_CLEAR;

    buf[7] = (char)color;
    buf[4] = (char)(colStart - 1);
    buf[6] = (char)(colEnd - 1);
    buf[3] = (char)(rowStart - 1);
    buf[5] = (char)(rowEnd - 1);

    SendOne(
        sockfd, buf, sizeof(buf),
        deviceTable[deviceId].ipAddr,
        PORT_CMD
    );
}

char* ClearVideoArea(int deviceId, int rowStart, int rowEnd, int colStart, int colEnd, TextColor color, int* dataLen) {
    int bufSize;
    char* buf;

    if (rowStart > rowEnd || rowStart < 1 || rowEnd < 1 || rowStart > 25 || rowEnd > 25) {
        return NULL;
    }

    if (colStart > colEnd || colStart < 1 || colEnd < 1 || colStart > 80 || colEnd > 80) {
        return NULL;
    }

    bufSize = 8;
    buf = (char*)malloc(bufSize);

    buf[0] = deviceId;
    buf[1] = VIDEO_CMD;
    buf[2] = OP3_VIDEO_AREA_CLEAR;

    buf[7] = (char)color;
    buf[4] = (char)(colStart - 1);
    buf[6] = (char)(colEnd - 1);
    buf[3] = (char)(rowStart - 1);
    buf[5] = (char)(rowEnd - 1);

    *dataLen = bufSize;
    return buf;
}

void SendSetVideoMode(int sockfd, int deviceId, int videoMode) {
    if (videoMode != 3 && videoMode != 1 && videoMode != 4 && videoMode != 5) {
        return;
    }

    char buf[] = {
        (char)deviceId,
        VIDEO_CMD,
        OP3_VIDEO_MODE_SET,
        (char)videoMode
    }
    ;
    SendOne(sockfd, buf, sizeof(buf), deviceTable[deviceId].ipAddr, PORT_CMD);
}

char* ClearScreen( int deviceId, TextColor color, int* dataLen) {
    int bufSize;
    char* buf;

    bufSize = 8;
    buf = (char*)malloc(bufSize);

    buf[0] = (char)deviceId;
    buf[1] = VIDEO_CMD;
    buf[2] = OP3_VIDEO_AREA_CLEAR;
    buf[3] = 0x00;
    buf[4] = 0x00;
    buf[5] = 24;
    buf[6] = 79;
    buf[7] = (char)color;

    *dataLen = bufSize;
    return buf;
}

void SendClearScreen(int sockfd, int deviceId, TextColor color) {
    char buf[] = {
        (char)deviceId,
        VIDEO_CMD,
        OP3_VIDEO_AREA_CLEAR,
        0x00,
        0x00,
        24,
        79,
        (char)color
    };

    SendOne(sockfd, buf, sizeof(buf), deviceTable[deviceId].ipAddr, PORT_CMD);
}

void SendClearScreenTest(int sockfd, int deviceId) {
    StartScreenOutput(deviceId, sockfd);
    for (int i = COLOR_BLACK; i <= COLOR_GRAY; i++) {
        int dataLen;
        char* data = ClearScreen(deviceId, COLOR(FG_COLOR_LIGHT_GREEN, (TextColor)i, TEXT_STATIC), &dataLen);
        SendToScreen(deviceId, data, dataLen, sockfd);
        free(data);
    }
    EndScreenOutput(deviceId, sockfd);
}

void SendGetInfo(int sockfd, int deviceId) {
    char buf[] = {
        (char)deviceId, VIDEO_CMD, OP3_GET_INFO
    };

    SendOne(sockfd, buf, sizeof(buf), deviceTable[deviceId].ipAddr, PORT_CMD);
}

void SendResetVideo(int sockfd, int deviceId) {
    char buf[] = {
        (char)deviceId, VIDEO_CMD, OP3_RESET_DEVICE, 0x00
    };

    SendOne(sockfd, buf, sizeof(buf), deviceTable[deviceId].ipAddr, PORT_CMD);
}

void SendKeypadBeep(int sockfd, int deviceId) {
    char buf[] = {
        (char)deviceId, SYSTEM_CMD, OP1_KEYPAD_BEEP, 0x00
    };

    SendOne(sockfd, buf, sizeof(buf), deviceTable[deviceId].ipAddr, PORT_CMD);
}

void SendTestScreen(int sockfd, char terminalId) {
    int numColors = 8;
    int numRows = 25;
    int numCols = 80;
    int barHeight = 3;
    

    //SendSetVideoMode(sockfd, 2, 0x03);
    //SendGetInfo(g_Sockfd, 2); //DisplayAttribute = getDisplayAttribute(deviceId, 15, i, 0);
    //SendClearScreen(sockfd, 2, COLOR(FG_COLOR_BLACK, COLOR_BLACK, TEXT_STATIC)); //this comes from above attibute 15
    StartScreenOutput(terminalId, sockfd);
    
    for (int i = 0; i < numRows; i++) {
        TextColor1 colorBG = (TextColor1)((i % numColors));
        TextColor colorFG = (TextColor)(((i + 1) % numColors) + 1);
        int blink = (i % 2 == 0) ? TEXT_BLINK : TEXT_STATIC;
        char text[80];
        int startRow = i+1;
        int endRow = i+1;

        int dataLen;
        char* data;

        snprintf(text, sizeof(text), "This is line %d, with color %d-%d", i + 1, colorFG, colorBG);

        //SendClearVideoArea(sockfd, terminalId, startRow, endRow, 1, numCols, COLOR(colorFG, colorBG, (TextColor)blink));
        data = ClearVideoArea(terminalId, startRow, endRow, 1, numCols, COLOR(colorFG, colorBG, (TextColor)blink), &dataLen);
        SendToScreen(terminalId, data, dataLen, sockfd);
        free(data);

        data = TextLine(terminalId, startRow, 12, COLOR(colorFG, colorBG, (TextColor)blink), text, &dataLen);
        SendToScreen(terminalId, data, dataLen, sockfd);
        free(data);
        
    }
    EndScreenOutput(terminalId, sockfd);
}

