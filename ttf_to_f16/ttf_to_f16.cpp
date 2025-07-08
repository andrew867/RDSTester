#include "Common.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <ft2build.h>
#include FT_FREETYPE_H



int case_insensitive_cmp(const char* a, const char* b) {
    while (*a && *b) {
        if (tolower((unsigned char)*a) != tolower((unsigned char)*b)) {
            return tolower((unsigned char)*a) - tolower((unsigned char)*b);
        }
        a++;
        b++;
    }
    return *a - *b;
}

int is_fon_file(const char* filename) {
    const char* ext = strrchr(filename, '.');
    if (!ext) return 0;
    return case_insensitive_cmp(ext, ".fon") == 0;
}

int load_fon_font(const char* filename, unsigned char* buffer) {
    FILE* file;
    if (fopen_s(&file, filename, "rb") != 0 || !file) {
        perror("fopen_s");
        return 1;
    }

    unsigned char header[64];
    if (fread(header, 1, sizeof(header), file) != sizeof(header)) {
        fclose(file);
        return 1;
    }

    if (header[0] != 'M' || header[1] != 'Z') {
        printf("Not a valid NE executable (FON file)\n");
        fclose(file);
        return 1;
    }

    long ne_offset = header[0x3C] | (header[0x3D] << 8) | (header[0x3E] << 16) | (header[0x3F] << 24);
    fseek(file, ne_offset, SEEK_SET);
    unsigned char ne_header[64];
    if (fread(ne_header, 1, sizeof(ne_header), file) != sizeof(ne_header)) {
        fclose(file);
        return 1;
    }

    if (ne_header[0] != 'N' || ne_header[1] != 'E') {
        printf("Not a valid NE header\n");
        fclose(file);
        return 1;
    }

    // For simplicity, assume the font data is located at a fixed offset
    // and is 4096 bytes long. A full NE parser would be more complex.
    fseek(file, -OUTPUT_SIZE, SEEK_END);
    fread(buffer, 1, OUTPUT_SIZE, file);
    fclose(file);
    return 0;
}

int main(int argc, char** argv) {
    if (argc < 3) {
        printf("Usage: %s <font.ttf|font.fon> <output.f16> [footer_template.f16]\n", argv[0]);
        return 1;
    }

    unsigned char buffer[OUTPUT_SIZE] = { 0 };

    if (is_fon_file(argv[1])) {
        if (load_fon_font(argv[1], buffer)) {
            return 1;
        }
    }
    else {
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

        for (int c = 0; c < TOTAL_CHARS; c++) {
            if (FT_Load_Char(face, c, FT_LOAD_RENDER)) {
                continue;
            }

            FT_Bitmap* bmp = &face->glyph->bitmap;
            for (int y = 0; y < HEIGHT; y++) {
                unsigned char row = 0;
                for (int x = 0; x < WIDTH; x++) {
                    int px = x - face->glyph->bitmap_left;
                    int py = y - (HEIGHT - face->glyph->bitmap_top);
                    if (px >= 0 && px < (int)bmp->width && py >= 0 && py < (int)bmp->rows) {
                        if (bmp->buffer[py * bmp->pitch + px] > 128)
                            row |= (1 << (7 - x));
                    }
                }
                buffer[c * HEIGHT + y] = row;
            }
        }

        FT_Done_Face(face);
        FT_Done_FreeType(ft);
    }


    if (argc >= 4) {
        FILE* footer;
        if (fopen_s(&footer, argv[3], "rb") == 0 && footer) {
            fseek(footer, -FOOTER_SIZE, SEEK_END);
            fread(buffer + OUTPUT_SIZE - FOOTER_SIZE, 1, FOOTER_SIZE, footer);
            fclose(footer);
        }
    }
    else {
        memcpy(buffer + OUTPUT_SIZE - FOOTER_SIZE, default_footer, FOOTER_SIZE);
    }


    FILE* out;
    if (fopen_s(&out, argv[2], "wb") != 0 || !out) {
        perror("fopen_s");
        return 1;
    }

    fwrite(buffer, 1, OUTPUT_SIZE, out);
    fclose(out);

    printf("F16 font written to %s\n", argv[2]);
    return 0;
}
