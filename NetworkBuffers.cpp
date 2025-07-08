#include "Network.h"
#include "NetworkBuffers.h"

TerminalNetworkBuffers g_TerminalBuffers[MAX_TERMINALS] = { 0 };

int StartNetworkOutput(char terminalId, int sockfd, int port) {
    TerminalNetworkBuffers* term = &g_TerminalBuffers[(unsigned char)terminalId];

    for (int i = 0; i < MAX_NETWORK_BUFFERS; ++i) {
        if (InterlockedCompareExchange(&term->buffers[i].locked, 1, 0) == 0) {
            term->activeIndex = i;
            term->buffers[i].dataLen = 0;
            return i;
        }
    }

    return -1;
}

void SendToNetwork(char terminalId, const void* data, size_t len, int sockfd, int port) {
    TerminalNetworkBuffers* term = &g_TerminalBuffers[(unsigned char)terminalId];
    int idx = term->activeIndex;

    if (idx < 0 || idx >= MAX_NETWORK_BUFFERS) return;

    NetworkBuffer* buf = &term->buffers[idx];

    if (InterlockedCompareExchange(&buf->locked, 1, 1) == 0) {
        return;
    }

    if (buf->dataLen + len >= NETWORK_PACKET_SIZE) {
        if (buf->dataLen > 0) {
            SendRawToNetwork(sockfd, terminalId, buf->data, buf->dataLen, port);
            buf->dataLen = 0;
        }
    }

    memcpy(buf->data + buf->dataLen, data, len);
    buf->dataLen += len;
}

void EndNetworkOutput(char terminalId, int sockfd, int port) {
    TerminalNetworkBuffers* term = &g_TerminalBuffers[(unsigned char)terminalId];
    int idx = term->activeIndex;

    if (idx < 0 || idx >= MAX_NETWORK_BUFFERS) return;

    NetworkBuffer* buf = &term->buffers[idx];

    if (InterlockedCompareExchange(&buf->locked, 1, 1) == 0) {
        return;
    }

    if (buf->dataLen > 0) {
        SendRawToNetwork(sockfd, terminalId, buf->data, buf->dataLen, port);
    }

    InterlockedExchange(&buf->locked, 0);
    buf->dataLen = 0;
}

void SendRawToNetwork(char terminalId, int sockfd, char* data, size_t len, int port) {
    // Send packet to net broadcast
    SendOne(sockfd, data, len, "255.255.255.255", port);
}
