#pragma once

#include <cstdint>
#include <graphics.hpp>

void WriteAscii(PixelWriter& writer, int x, int y, char c, const PixelColor& color);
void WriteString(PixelWriter& writer, int x, int y, const char *string, const PixelColor& color);