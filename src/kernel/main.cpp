/**
 * @file main.cpp
 *
 * カーネル本体のプログラムを書いたソースファイル.
 */

// C++ header
#include <asmfunc.h>
#include <console.hpp>      //Consoleクラス
#include <cpufunc.hpp>      //アセンブラ
#include <efi_memmap.hpp>   //UEFI MemoryMap
#include <font.hpp>         //WriteAscii
#include <framebuffer.hpp>  //フレームバッファ初期化
#include <graphics.hpp>     //PixelWriterクラス
#include <interrupt.hpp>    //割込み
#include <logger.hpp>       //log関数
#include <mmgr.hpp>         //MemoryManager
#include <mouse.hpp>        //Mouseクラス
#include <operator.hpp>     //配置new
#include <pageing.hpp>      //ページング
#include <pci.hpp>          //pci初期化・探索
#include <printk.hpp>       //printk関数
#include <segment.hpp>      //segment
#include <xhc.hpp>          //xhc探索

// C header
#include <frame_buffer_config.h>

alignas(16) uint8_t kernel_main_stack[1024 * 1024];

extern "C" void KernelMainNewStack(
    const FrameBufferConfig& frame_buffer_config_ref,
    const MemoryMap& memory_map_ref) {
  SetLogLevel(kInfo);

  FrameBufferConfig frame_buffer_config{frame_buffer_config_ref};
  MemoryMap memory_map{memory_map_ref};

  uint32_t size_of_stack = sizeof(kernel_main_stack);

  SetupSegments();
  const uint16_t kernel_cs = 1 << 3;  //kernel code segment
  const uint16_t kernel_ss = 2 << 3;  //kernel stack segment
  SetDSAll(0);
  SetCSSS(kernel_cs, kernel_ss);

  SetupIdentityPageTable();

  //メモリマネージャの初期化
  memory_manager = new (memory_manager_buf) BitmapMemoryManager;

  const auto memory_map_base = reinterpret_cast<uintptr_t>(memory_map.buffer);
  uintptr_t available_end    = 0;
  for (uintptr_t iter = memory_map_base;
       iter < memory_map_base + memory_map.map_size;
       iter += memory_map.descriptor_size) {
    auto desc = reinterpret_cast<const MemoryDescriptor*>(iter);
    if (available_end < desc->physical_start) {
      memory_manager->MarkAllocated(
          FrameID{available_end / kBytesPerFrame},
          (desc->physical_start - available_end) / kBytesPerFrame);
    }

    const auto physical_end =
        desc->physical_start + desc->number_of_pages * kUEFIPageSize;

    if (IsAvailable(static_cast<MemoryType>(desc->type))) {
      available_end = physical_end;
    } else {
      memory_manager->MarkAllocated(
          FrameID{desc->physical_start / kBytesPerFrame},
          desc->number_of_pages * kUEFIPageSize / kBytesPerFrame);
    }
  }
  memory_manager->SetMemoryRange(FrameID{1},
                                 FrameID{available_end / kBytesPerFrame});

  /*フレームバッファを初期化*/
  framebuffer_init(frame_buffer_config, {0x00, 0x00, 0x00});

  console_init();

  Log(kInfo, "[INFO] Kernel Stack begin: 0x%08x, length: %08x(%d)\n",
      &kernel_main_stack, size_of_stack, size_of_stack);

  mouse_cursor_init();

  pci_scan();

  xhc_init();

  halt();
}
