/**
 * @file main.cpp
 *
 * カーネル本体のプログラムを書いたソースファイル.
 */
// C++ header
#include <console.hpp>      //Consoleクラス
#include <font.hpp>         //WriteAscii
#include <framebuffer.hpp>  //フレームバッファ初期化
#include <global.hpp>       //グローバル変数
#include <graphics.hpp>     //PixelWriterクラス
#include <mouse.hpp>        //マウスカーソル表示
#include <operator.hpp>     //配置new
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

void halt() {
  while (1)
    __asm__("hlt");
}

extern "C" void KernelMain(const FrameBufferConfig& frame_buffer_config) {
  const int kFrameWidth = frame_buffer_config.horizontal_resolution;
  const int kFrameHeight = frame_buffer_config.vertical_resolution;
  /*フレームバッファを初期化*/
  framebuffer_init(frame_buffer_config);
  /*
  DrawRectangle(*pixel_writer, {0, 21}, {kFrameWidth, 1}, {0x00, 0x00, 0x00});
  FillRectangle(*pixel_writer, {0, 0}, {kFrameWidth, 20}, {0xBB, 0xBB, 0xBB});
  */
  FillCircle(*pixel_writer, {200, 300}, 80, {0xFF, 0x00, 0x00});
  FillCircle(*pixel_writer, {600, 300}, 80, {0x00, 0x00, 0xFF});

  console = new (console_buf)
      Console(*pixel_writer, {0x00, 0x00, 0x00}, {0xE6, 0xE6, 0xE6});

  printk("Hello. SaintOS World.\n");

  /* なんちゃってマウスカーソルを描画 */
  draw_mouse_cursor();

  halt();
}