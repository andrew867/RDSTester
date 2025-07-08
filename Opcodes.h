#pragma once

enum OpCodeCmd {
    SERVER_CMD = 0,
    SYSTEM_CMD = 1,
    UNIT_CMD = 2,
    VIDEO_CMD = 3,
    TOUCH_CMD = 4,
    FIRMWARE_CMD = 5
};

enum OpCode0 {
    OP0_SERVER_PONG = 1 //responds to unit?
};

enum OpCode1 {
    OP1_UNKNOWN_0,
    OP1_KEYPAD_PRESS = 1,
    OP1_KEYPAD_BEEP = 2, //int deviceId
    OP1_UNKNOWN_3,
    OP1_GET_INFO = 4, //no params sent after OP3_GET_INFO
    OP1_UNKNOWN_5,
    OP1_SET_ID = 6, //int deviceId, char *Str, int deviceIdNew Destination[11] = deviceIdNew;
    OP1_UNKNOWN_7,
    OP1_SET_SERIAL_NO = 8, //int deviceId, char *text
    OP1_SET_PROD_DATE = 9, //int deviceId, char *text
    OP1_UNKNOWN_10,
    OP1_UNKNOWN_11,
    OP1_UNKNOWN_12,
    OP1_FIRMWARE_START, //int deviceId
    OP1_UNKNOWN_14,
    OP1_UNKNOWN_15
};

enum OpCode2 {
    OP2_ALIVE = 0x02,
    OP2_DEVICE_ALIVE = 0x05,
    OP2_KEYPAD_ALIVE = 0x06,
    OP2_DEVICE_BUSY = 0x0a,
    OP2_DOWNLOAD_START = 0x0d, //13
    OP2_DOWNLOAD_END = 0x0e //14
};

enum OpCode3 {
    OP3_UNKNOWN = 0,
    OP3_VIDEO_MODE_SET = 1, //int deviceId, int videoMode
    OP3_RESET_DEVICE = 2, //no params
    OP3_VIDEO_AREA_CLEAR = 3, //int deviceId, int xStart, int yStart, int xEnd, int yEnd, char color 
    OP3_VIDEO_AREA_SAVE = 4, //int deviceId, int rowStart, int colStart, int rowEnd, int colEnd, int regionNumber
    OP3_VIDEO_AREA_RESTORE = 5, //int deviceId, int row, int col, int regionNumber
    OP3_VIDEO_AREA_FREE = 6, //int deviceId, int region
    OP3_DISPLAY_TEXT = 7, //int deviceId, int row, int col, char color, char *Str
    OP3_DRAW_BOX = 8, //int deviceId, int rowStart, int colStart, int rowEnd, int colEnd, char boxType1, char boxType2
    OP3_COPY_VIDEO_AREA = 9, //int deviceId, int rowStart, int colStart, int rowEnd, int colEnd, int dstRow, int dstCol
    OP3_CURSOR_ENABLE = 10, //int deviceId, unsigned int blinking
    OP3_CURSOR_DISABLE = 11, //no params
    OP3_CURSOR_SET_LOCATION = 12, //int deviceId, int col, int row
    OP3_CLOCK_ENABLE = 13, //8 params
    OP3_CLOCK_DISABLE = 14, //int deviceId, int clockId (max 99)
    OP3_CLOCK_DISABLE_ALL = 15, //no params
    OP3_GET_INFO = 16, //no params
    OP3_SET_ID = 18, //int deviceId, char *text, int deviceIdNew
    OP3_SET_PORT = 19, //need more data
    OP3_SET_SERIAL_NO = 20, //int deviceId, char *text
    OP3_SET_PROD_DATE_EX = 21, //int deviceId, char *text
    OP3_SS_ENABLE = 22, //4 params
    OP3_VIDEO_REGION_UPDATE = 23, //int deviceId, unsigned int regionNumber, char a3, int rowStart, int colStart, int rowEnd ,int colEnd
    OP3_CLOCK_ENABLE_EX = 25, //int deviceId, int clockId, unsigned int timeHours, unsigned int timeMinutes, unsigned int timeSeconds, int row, int col, char a8, int clockType, int clockUpDown
    OP3_TICKER_DISABLE = 27, //int deviceId, char tickerId
    OP3_PRINTER_SET = 28, //int deviceId, int printerType 0,3,4,5
    OP3_TEXT_PRINT = 29, //int deviceId, char *text
    OP3_DISPLAY_TEXT_W = 30, //int deviceId, int row, int col, char color, wchar_t *String
    OP3_TEXT_PRINT_W = 31, //int deviceId, wchar_t *text
    OP3_VIDEO_REGION_UPDATE_EX = 32, //same as 23
    OP3_PRINTER_SET_EX = 34, //int deviceId, int printerType 0,3,4,5
    OP3_WEB_VIEW_URL = 35, //string
    OP3_TAB_PREV = 36,
    OP3_TAB_NEXT = 37,
    OP3_TAB_CLEAR_VIEWS = 38,
    OP3_TAB_ADD = 39
};

enum OpCode4 {
    OP4_SET_TOUCH_MODE = 4, //int touchMode
    OP4_TOUCH_CAL = 5
};

enum OpCode5 { //codes for E2 and E25
    OP5_FIRMWARE_START = 2, //
    OP5_FIRMWARE_DATA = 3, //
    OP5_FIRMWARE_END = 4, //
    OP5_FONT_START = 6, //
    OP5_FONT_DATA = 7, //
    OP5_FONT_END = 8, //
    OP5_FIRMWARE_E2_START = 17, //E2 start
    OP5_FIRMWARE_E2_DATA = 18, //E2 data
    OP5_FIRMWARE_E2_END = 19, //E2 end
    OP5_FIRMWARE_E25_START = 33, //E25 start
    OP5_FIRMWARE_E25_DATA = 34, //E25 data
    OP5_FIRMWARE_E25_END = 35, //E25 end

};
