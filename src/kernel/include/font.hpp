#pragma once

#include <cstdint>
#include <graphics.hpp>

#define FONT_X_LEN 8
#define FONT_Y_LEN 16

void WriteAscii(PixelWriter& writer, int x, int y, char c,
                const PixelColor& color);
void WriteString(PixelWriter& writer, int x, int y, const char* string,
                 const PixelColor& color);