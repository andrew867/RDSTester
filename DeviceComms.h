#pragma once

#include "Common.h"
#include "DeviceTable.h"
#include "Network.h"
#include "ScreenBuffers.h"
#include "NetworkBuffers.h"


#ifdef __cplusplus
extern "C" {
#endif

	extern void SendDevicePong1(int sockfd, int deviceId);
	extern void SendDevicePong2(int sockfd, int deviceId);

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
	extern void SendClearScreenTest(int sockfd, int terminalId);

#ifdef __cplusplus
}
#endif
