/**
 * @file main.cpp
 *
 * カーネル本体のプログラムを書いたソースファイル.
 */
// C++ header
#include <console.hpp>
#include <font.hpp>
#include <framebuffer.hpp>
#include <global.hpp>
#include <graphics.hpp>
#include <operator.hpp>
// C header
#include <frame_buffer_config.h>

/*
 printk関数
*/
int printk(const char* format, ...) {
  va_list ap;
  int result;
  char s[1024];

  va_start(ap, format);
  result = vsprintf(s, format, ap);
  va_end(ap);

  console->PutString(s);
  return result;
}

extern "C" void KernelMain(const FrameBufferConfig& frame_buffer_config) {
  framebuffer_init(frame_buffer_config);

  console = new (console_buf)
      Console(*pixel_writer, {0x00, 0x00, 0x00}, {0xE6, 0xE6, 0xE6});

  for (int i = 0; i < 27; ++i) {
    printk("printk: %d\n", i);
  }

  while (1)
    __asm__("hlt");
}