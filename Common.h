#pragma once
#define WIN32_LEAN_AND_MEAN  // Say this...
#define _CRT_SECURE_NO_WARNINGS
#pragma comment(lib, "ws2_32.lib")

#include <windows.h>         // And now we can include that.
#include <winsock2.h>        // And this.
#include <WS2tcpip.h>
#include <iostream>
#include "DeviceTable.h"
#include "Color.h"
#include "Opcodes.h"

CONST int PORT_CONTROL = 33536;
CONST int PORT_CMD = 33537;

