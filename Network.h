#pragma once

#include "Common.h"
#include "DeviceTable.h"
#include <windows.h>    // For DWORD, Sleep
#include <stdio.h>      // For fprintf, snprintf
#include <stdlib.h>     // For malloc, free
#include <string.h>     // For memset, strcpy
#include <stdarg.h>     // For va_list, va_start, va_end
#include <winsock2.h>   // For sockaddr_in, sendto
#include <ws2tcpip.h>   // For inet_pton

#ifdef __cplusplus
extern "C" {
#endif

	static WSAData wsaData;

	extern void PrintWSAError(void);

	//extern void SendBroadcast(int sockfd, char* payload, int payloadLen, int port);
	extern void SendOne(int sockfd, char* payload, int payloadLen, const char* addr, int port);

	extern void SendTextLine(int sockfd, int deviceId, int row, int col, TextColor color, const char* str);
	extern void SendScreenBuffer(int sockfd, int deviceId, char screen_buffer[25][81]);

	extern void SendDrawBox(int sockfd, int deviceId, int topLeft, int botLeft, int botRight, int topRight, char boxType1, char boxType2);
	extern void SendClearVideoArea(int sockfd, int deviceId, int rowStart, int rowEnd, int colStart, int colEnd, TextColor color);
	extern void SendSetVideoMode(int sockfd, int deviceId, int videoMode);
	extern void SendClearScreen(int sockfd, int deviceId, TextColor color);
	extern void SendGetInfo(int sockfd, int deviceId);
	extern void SendResetVideo(int sockfd, int deviceId);
	extern void SendKeypadBeep(int sockfd, int deviceId);
	extern void SendTestScreen(int sockfd, char terminalId);

#ifdef __cplusplus
}
#endif
