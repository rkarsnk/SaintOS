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
#include <mouse.hpp>        //マウスカーソル表示
#include <operator.hpp>     //配置new
#include <pci.hpp>          //pci初期化・探索
#include <printk.hpp>       //printk関数
//#include <xhc.hpp>          //xhc探索
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

  printk("Hello. SaintOS World.\n");

  printk("Console Format:%d rows * %d columns.\n", console->ttyRows,
         console->ttyColumns);

  /* テストコード 
  pci::pci_cfg_test();
  */
  pci_init();

  SetLogLevel(kInfo);

  pci::Device* xhc_dev = nullptr;
  for (int i = 0; i < pci::num_device; ++i) {
    if (pci::devices[i].class_code.Match(0x0Cu, 0x03u, 0x30u)) {
      xhc_dev = &pci::devices[i];

      // Intel製のコントローラを優先 */
      // なおQEMUではnec製のコントローラがエミュレートされる．
      if (PCI_VENDOR_INTEL == pci::ReadVendorId(*xhc_dev)) {
        break;
      }
    }
  }

  if (xhc_dev) {
    Log(kInfo, "xHC has been found: %02d:%02d:%d ", xhc_dev->bus,
        xhc_dev->device, xhc_dev->function);
    Log(kInfo, "vendor:%04x, device:%04x\n", xhc_dev->vendor_id,
        xhc_dev->device_id);
  }

  //  find_xhc();
  /* なんちゃってマウスカーソルを描画 */
  //draw_mouse_cursor();

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