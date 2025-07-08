#pragma once

enum TextColor1 {
    COLOR_BLACK = 0,
    COLOR_BLUE = 1,
    COLOR_GREEN = 2,
    COLOR_CYAN = 3,
    COLOR_RED = 4,
    COLOR_MAGENTA = 5,
    COLOR_BROWN = 6,
    COLOR_WHITE = 7,
    COLOR_GRAY = 8
};

enum TextOptions {
    TEXT_BLINK = 1, // 1 bit for blink: 0 (STATIC), 1 (BLINK)
    TEXT_STATIC = 0
};

enum TextColor {
    FG_COLOR_BLACK = 0x00,
    FG_COLOR_BLUE = 0x01,
    FG_COLOR_GREEN = 0x02,
    FG_COLOR_CYAN = 0x03,
    FG_COLOR_RED = 0x04,
    FG_COLOR_MAGENTA = 0x05,
    FG_COLOR_BROWN = 0x06,
    FG_COLOR_WHITE = 0x07,
    FG_COLOR_GRAY = 0x08,
    FG_COLOR_LIGHT_BLUE_ON_BLACK = 0x09,
    FG_COLOR_LIGHT_GREEN = 0x0a,
    FG_COLOR_LIGHT_CYAN = 0x0b,
    FG_COLOR_LIGHT_RED = 0x0c,
    FG_COLOR_LIGHT_MAGENTA = 0x0d,
    FG_COLOR_YELLOW = 0x0d,
    FG_COLOR_LIGHT_WHITE = 0x0f
};

// Macro to combine foreground (F), background (B), and blink (L) into one value
#define COLOR(F, B, L) (TextColor)((F) | ((B) << 4) | ((L) << 7))
