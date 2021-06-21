#pragma once

#include <cstring>
#include <new>

#include <font.hpp>
#include <framebuffer.hpp>
#include <graphics.hpp>
#include <logger.hpp>
#include <printk.hpp>

class Console {
 public:
  /* ttyRows行 * ttyColumns列 */
  static const int ttyRows = RES_VERT / 16, ttyColumns = RES_HORZ / 8;

  Console(PixelWriter& writer, const PixelColor& fg_color,
          const PixelColor& bg_color);
  void PutString(const char* s);

 private:
  void NewLine();

  PixelWriter& writer_;
  const PixelColor fg_color_, bg_color_;
  char buffer_[ttyRows][ttyColumns + 1];
  int cursor_row_, cursor_column_;
};

extern char console_buf[sizeof(Console)];
extern Console* console;

void console_init();