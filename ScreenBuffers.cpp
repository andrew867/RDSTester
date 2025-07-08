#include "Network.h"
#include "ScreenBuffers.h"

TerminalScreenBuffers g_TerminalBuffers[MAX_TERMINALS] = { 0 };

int StartScreenOutput(char terminalId, int sockfd) {
    TerminalScreenBuffers* term = &g_TerminalBuffers[(unsigned char)terminalId];

    for (int i = 0; i < MAX_SCREEN_BUFFERS; ++i) {
        if (InterlockedCompareExchange(&term->buffers[i].locked, 1, 0) == 0) {
            term->activeIndex = i;
            term->buffers[i].dataLen = 0;
            return i;
        }
    }

    return -1;
}

void SendToScreen(char terminalId, const void* data, size_t len, int sockfd) {
    TerminalScreenBuffers* term = &g_TerminalBuffers[(unsigned char)terminalId];
    int idx = term->activeIndex;

    if (idx < 0 || idx >= MAX_SCREEN_BUFFERS) return;

    ScreenBuffer* buf = &term->buffers[idx];

    if (InterlockedCompareExchange(&buf->locked, 1, 1) == 0) return;

    if (buf->dataLen + len >= SCREEN_PACKET_SIZE) {
        if (buf->dataLen > 0) {
            SendRawToTerminal(sockfd, terminalId, buf->data, buf->dataLen);
            buf->dataLen = 0;
        }
    }

    memcpy(buf->data + buf->dataLen, data, len);
    buf->dataLen += len;
}

void EndScreenOutput(char terminalId, int sockfd) {
    TerminalScreenBuffers* term = &g_TerminalBuffers[(unsigned char)terminalId];
    int idx = term->activeIndex;

    if (idx < 0 || idx >= MAX_SCREEN_BUFFERS) return;

    ScreenBuffer* buf = &term->buffers[idx];

    if (InterlockedCompareExchange(&buf->locked, 1, 1) == 0) return;

    if (buf->dataLen > 0) {
        SendRawToTerminal(sockfd, terminalId, buf->data, buf->dataLen);
    }

    InterlockedExchange(&buf->locked, 0);
    buf->dataLen = 0;
}

void SendRawToTerminal(int sockfd, char terminalId, char* data, size_t len) {
    // Send packet directly to terminal 1-32
    SendOne(sockfd, data, len, deviceTable[terminalId].ipAddr, PORT_CMD);
}