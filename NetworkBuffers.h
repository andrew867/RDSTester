#pragma once

#include <windows.h>
#include <stdbool.h>

#define MAX_TERMINALS 32
#define MAX_NETWORK_BUFFERS 8
#define NETWORK_PACKET_SIZE 500

typedef struct {
    volatile LONG locked;
    size_t dataLen;
    char data[NETWORK_PACKET_SIZE];
} NetworkBuffer;

typedef struct {
    NetworkBuffer buffers[MAX_NETWORK_BUFFERS];
    int activeIndex;
} TerminalNetworkBuffers;

extern TerminalNetworkBuffers g_NetworkBuffers[MAX_TERMINALS];

extern int StartNetworkOutput(char terminalId, int sockfd, int port);
extern void SendToNetwork(char terminalId, const void* data, size_t len, int sockfd, int port);
extern void EndNetworkOutput(char terminalId, int sockfd, int port);
extern void SendRawToNetwork(char terminalId, int sockfd, char* data, size_t len, int port);
