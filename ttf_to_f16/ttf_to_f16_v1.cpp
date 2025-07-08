#include "Common.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ft2build.h>
#include FT_FREETYPE_H


#define WIDTH 8
#define HEIGHT 16
#define TOTAL_CHARS 256
#define FOOTER_SIZE 512
#define OUTPUT_SIZE 4096

int main(int argc, char** argv) {
    if (argc < 3) {
        printf("Usage: %s <font.ttf> <output.f16> [footer_template.f16]\n", argv[0]);
        return 1;
    }

    FT_Library ft;
    FT_Face face;

    if (FT_Init_FreeType(&ft)) {
        printf("Could not init FreeType\n");
        return 1;
    }

    if (FT_New_Face(ft, argv[1], 0, &face)) {
        printf("Could not open font %s\n", argv[1]);
        return 1;
    }

    FT_Set_Pixel_Sizes(face, 0, HEIGHT);

    unsigned char buffer[OUTPUT_SIZE] = { 0 };

    for (int c = 0; c < TOTAL_CHARS; c++) {
        if (FT_Load_Char(face, c, FT_LOAD_RENDER)) {
            continue;
        }

        FT_Bitmap* bmp = &face->glyph->bitmap;
        for (int y = 0; y < HEIGHT; y++) {
            unsigned char row = 0;
            for (unsigned int x = 0; x < WIDTH; x++) {
                unsigned int px = x - face->glyph->bitmap_left;
                unsigned int py = y - (HEIGHT - face->glyph->bitmap_top);
                if (px >= 0 && px < bmp->width && py >= 0 && py < bmp->rows) {
                    if (bmp->buffer[py * bmp->pitch + px] > 128)
                        row |= (1 << (7 - x));
                }
            }
            buffer[c * HEIGHT + y] = row;
        }
    }

    // Optional: append footer from template
    if (argc >= 4) {
        FILE* footer = fopen(argv[3], "rb");
        if (footer) {
            fseek(footer, -FOOTER_SIZE, SEEK_END);
            fread(buffer + OUTPUT_SIZE - FOOTER_SIZE, 1, FOOTER_SIZE, footer);
            fclose(footer);
        }
    }

    FILE* out = fopen(argv[2], "wb");
    if (!out) {
        perror("fopen");
        return 1;
    }

    fwrite(buffer, 1, OUTPUT_SIZE, out);
    fclose(out);

    FT_Done_Face(face);
    FT_Done_FreeType(ft);

    printf("F16 font written to %s\n", argv[2]);
    return 0;
}
