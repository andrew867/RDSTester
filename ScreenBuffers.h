#pragma once

#include <windows.h>
#include <stdbool.h>

#define MAX_TERMINALS 32
#define MAX_SCREEN_BUFFERS 8
#define SCREEN_PACKET_SIZE 500 //device crashes if you send paquets that are too big - also managed to corrupt the firmware on one of them

typedef struct {
    volatile LONG locked;
    size_t dataLen;
    char data[SCREEN_PACKET_SIZE];
} ScreenBuffer;

typedef struct {
    ScreenBuffer buffers[MAX_SCREEN_BUFFERS];
    int activeIndex;
} TerminalScreenBuffers;

extern TerminalScreenBuffers g_TerminalBuffers[MAX_TERMINALS];

extern int StartScreenOutput(char terminalId, int sockfd);
extern void SendToScreen(char terminalId, const void* data, size_t len, int sockfd);
extern void EndScreenOutput(char terminalId, int sockfd);
extern void SendRawToTerminal(int sockfd, char terminalId, char* data, size_t len);
