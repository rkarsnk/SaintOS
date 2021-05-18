/* -------------------------------------------------------------
#pragma コンパイラに特定の情報を渡すために使用するコンパイラ指令
  - once: 定義の重複を防ぐ。インクルードガードと同様
-------------------------------------------------------------- */
#pragma once

#include <stdint.h>

// SVGA
#define RES_HORZ 800
#define RES_VERT 600

enum PixelFormat {
  kPixelRGBResv8BitPerColor,
  kPixelBGRResv8BitPerColor,
};

struct FrameBufferConfig {
  uint8_t* frame_buffer;
  uint32_t pixels_per_scan_line;
  uint32_t horizontal_resolution;
  uint32_t vertical_resolution;
  enum PixelFormat pixel_format;
};
