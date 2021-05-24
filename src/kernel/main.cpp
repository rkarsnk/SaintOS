/**
 * @file main.cpp
 *
 * カーネル本体のプログラムを書いたソースファイル.
 */
// C++ header
#include <console.hpp>  //Consoleクラス
#include <error.hpp>
#include <font.hpp>         //WriteAscii
#include <framebuffer.hpp>  //フレームバッファ初期化
#include <global.hpp>       //グローバル変数
#include <graphics.hpp>     //PixelWriterクラス
#include <logger.hpp>       //log関数
#include <mouse.hpp>        //マウス表示
#include <operator.hpp>     //配置new
#include <pci.hpp>          //pci初期化・探索
#include <printk.hpp>       //printk関数
#include <xhc.hpp>          //xhc探索
// C header
#include <frame_buffer_config.h>

void halt() {
  while (1)
    __asm__("hlt");
}

extern "C" void KernelMain(const FrameBufferConfig& frame_buffer_config) {
  /*フレームバッファを初期化*/
  framebuffer_init(frame_buffer_config, {0x00, 0x00, 0x00});

  /*
  Console(PixelWriter& writer,
          const PixelColor& fg_color,
          const PixelColor& bg_color)
  */
  console = new (console_buf)
      Console(*pixel_writer, {0xFF, 0xFF, 0xFF}, {0x00, 0x00, 0x00});

  mouse_cursor = new (mouse_cursor_buf)
      MouseCursor{pixel_writer, {0xFF, 0xFF, 0xFF}, {500, 400}};

  printk("Hello. SaintOS World.\n");
  SetLogLevel(kInfo);

  printk("Console Format:%d rows * %d columns.\n", console->ttyRows,
         console->ttyColumns);

  /* テストコード 
  pci::pci_cfg_test();
  */
  pci_init();

  xhc_init();

  halt();
}
/*----------------------------------------------------------------------
  const int kFrameWidth = frame_buffer_config.horizontal_resolution;
  const int kFrameHeight = frame_buffer_config.vertical_resolution;

  //四角形を出力
  DrawRectangle(*pixel_writer, {0, 21}, {kFrameWidth, 1}, {0x00, 0x00,
  0x00});
  //塗りつぶし四角形を出力
  FillRectangle(*pixel_writer, {0, 0}, {kFrameWidth, 20}, {0xBB, 0xBB,
  0xBB});

  //円を出力
  DrawCircle(*pixel_writer, {200, 300}, 80, {0xFF, 0x00, 0x00});
  //塗りつぶし円を出力
  FillCircle(*pixel_writer, {600, 300}, 80, {0x00, 0x00, 0xFF});
  ----------------------------------------------------------------------*/