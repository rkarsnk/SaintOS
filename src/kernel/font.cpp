/**
 * @file font.cpp
 *
 * フォント描画のプログラムを集めたファイル.
 */
#include <font.hpp>

extern const uint8_t _binary_hankaku_bin_start;
extern const uint8_t _binary_hankaku_bin_end;
extern const uint8_t _binary_hankaku_bin_size;

const uint8_t* GetFont(char c) {
  auto index = 16 * static_cast<unsigned int>(c);
  if (index >= reinterpret_cast<uintptr_t>(&_binary_hankaku_bin_size)) {
    return nullptr;
  }
  return &_binary_hankaku_bin_start + index;
}

/**
 * WriteAscii関数
 */
void WriteAscii(PixelWriter& writer, int x, int y, char c,
                const PixelColor& color) {
  const uint8_t* font = GetFont(c);
  if (font == nullptr) {
    return;
  }
  for (int cy = 0; cy < 16; ++cy) {
    for (int cx = 0; cx < 8; ++cx) {
      if ((font[cy] << cx) & 0x80u) {
        writer.Write(x + cx, y + cy, color);
      }
    }
  }
}

/**
 * WriteString関数
 */
void WriteString(PixelWriter& writer, int x, int y, const char* string,
                 const PixelColor& color) {
  for (int i = 0; string[i] != '\0'; ++i) {
    WriteAscii(writer, x + 8 * i, y, string[i], color);
  }
}