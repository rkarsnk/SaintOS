/**
 * @file main.cpp
 *
 * カーネル本体のプログラムを書いたソースファイル.
 */
// C++ header
#include <console.hpp>      //Consoleクラス
#include <cpufunc.hpp>      //アセンブラ
#include <font.hpp>         //WriteAscii
#include <framebuffer.hpp>  //フレームバッファ初期化
#include <global.hpp>       //グローバル変数
#include <graphics.hpp>     //PixelWriterクラス
#include <logger.hpp>       //log関数
#include <mouse.hpp>        //Mouseクラス
#include <operator.hpp>     //配置new
#include <pci.hpp>          //pci初期化・探索
#include <printk.hpp>       //printk関数
#include <xhc.hpp>          //xhc探索
// C header
#include <frame_buffer_config.h>

extern "C" void KernelMain(const FrameBufferConfig& frame_buffer_config) {
  SetLogLevel(kInfo);
  /*フレームバッファを初期化*/
  framebuffer_init(frame_buffer_config, {0x00, 0x00, 0x00});

  console_init();

  mouse_cursor_init();

  pci_init();

  xhc_init();

  halt();
}
