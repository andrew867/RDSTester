#include "Common.h"
#include "DeviceTable.h"
#include "Network.h"

void PrintWSAError() {
    DWORD err = WSAGetLastError();

    fprintf(stderr, "WSA error: %d\n", err);
}

//void SendBroadcast2(int sockfd, char* payload, int payloadLen, int port) {
//    //TODO:Fix this function so it functions and sends broadcast paquets
//    struct sockaddr_in broadcastAddr;
//    int broadcastEnable = 1;
//
//    //if (setsockopt(sockfd, SOL_SOCKET, SO_BROADCAST, (char*)&broadcastEnable, sizeof(broadcastEnable)) < 0) {
//    //    fprintf(stderr, "setsockopt(SO_BROADCAST) failed: ");
//    //    PrintWSAError();
//    //    return;
//    //}
//
//    memset(&broadcastAddr, 0, sizeof(broadcastAddr));
//    broadcastAddr.sin_addr.s_addr = htonl(INADDR_BROADCAST);
//    broadcastAddr.sin_family = AF_INET;
//    broadcastAddr.sin_port = htons(port);
//
//    
//
//    //if (inet_pton(AF_INET, "255.255.255.255", &broadcastAddr.sin_addr) <= 0) {
//    //    fprintf(stderr, "inet_pton() failed for broadcast address\n");
//    //    return;
//    //}
//
//    int rc = sendto(
//        sockfd, payload, payloadLen, 0,
//        (struct sockaddr*)&broadcastAddr, sizeof(broadcastAddr)
//    );
//
//    if (rc != payloadLen) {
//        if (rc == -1) {
//            fprintf(stderr, "sendto(broadcast) error: ");
//            PrintWSAError();
//        }
//        else {
//            fprintf(stderr, "sendto(broadcast) error: short send (%d of %d bytes)\n", rc, payloadLen);
//        }
//    }
//    //fprintf(stderr, "broadcast send done\n");
//
//}
//
//void SendBroadcast(int sockfd, char* payload, int payloadLen, const char* addr, int port = PORT_CMD) {
//    struct sockaddr_in dst;
//
//    memset(&dst, 0, sizeof(dst));
//
//    inet_pton(AF_INET, addr, &(dst.sin_addr));
//
//    dst.sin_family = AF_INET;
//    dst.sin_port = htons(port);
//
//    int rc = sendto(
//        sockfd, payload, payloadLen, 0,
//        (struct sockaddr*)&dst, sizeof(dst));
//
//
//    if (rc != payloadLen) {
//        if (rc == -1) {
//            fprintf(stderr, "sendto(%s) error: ", addr);
//            PrintWSAError();
//            return;
//        }
//
//    }
//}


void SendOne(int sockfd, char* payload, int payloadLen, const char* addr, int port = PORT_CMD) {
    struct sockaddr_in dst;

    memset(&dst, 0, sizeof(dst));

    inet_pton(AF_INET, addr, &(dst.sin_addr));

    dst.sin_family = AF_INET;
    dst.sin_port = htons(port);

    int rc = sendto(
        sockfd, payload, payloadLen, 0,
        (struct sockaddr*)&dst, sizeof(dst));


    if (rc != payloadLen) {
        if (rc == -1) {
            fprintf(stderr, "sendto(%s) error: ", addr);
            PrintWSAError();
            return;
        }
    }
}



