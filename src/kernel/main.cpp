/**
 * @file main.cpp
 *
 * カーネル本体のプログラムを書いたソースファイル.
 */

#include <numeric>
#include <vector>
// C++ header
#include <asmfunc.h>        //アセンブラ
#include <console.hpp>      //Consoleクラス
#include <cpufunc.hpp>      //アセンブラ
#include <efi_memmap.hpp>   //UEFI MemoryMap
#include <font.hpp>         //WriteAscii
#include <framebuffer.hpp>  //フレームバッファ初期化
#include <graphics.hpp>     //PixelWriterクラス
#include <interrupt.hpp>    //割込み
#include <layer.hpp>        //layer管理
#include <logger.hpp>       //log関数
#include <mmgr.hpp>         //MemoryManager
#include <mouse.hpp>        //Mouseクラス
#include <operator.hpp>     //配置new
#include <pageing.hpp>      //ページング
#include <pci.hpp>          //pci初期化・探索
#include <segment.hpp>      //segment
#include <window.hpp>       //window管理
#include <xhc.hpp>          //xhc探索

// C header
#include <frame_buffer_config.h>

char pixel_writer_buf[sizeof(RGBResv8BitPerColorPixelWriter)];
PixelWriter* pixel_writer;

char console_buf[sizeof(Console)];
Console* console;

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

alignas(16) uint8_t kernel_main_stack[1024 * 1024];

unsigned int mouse_layer_id;

void MouseObserver(int8_t displacement_x, int8_t displacement_y) {
  layer_manager->MoveRelative(mouse_layer_id, { displacement_x, displacement_y });
  layer_manager->Draw();
}

/**
 * XHCI用割り込みハンドラ 
 */
struct Message {
  enum Type {
    kInterruptXHCI,
  } type;
};

ArrayQueue<Message>* main_queue;

usb::xhci::Controller* xhc;

__attribute__((interrupt)) //割込みハンドラ
void IntHandlerXHCI(IntrFrame* frame) {
  main_queue->Push(Message{ Message::kInterruptXHCI });
  NotifyEndOfInterrupt();
}

/**
 * KernelMainNewStack() 
 * カーネル用スタック領域を持つカーネルメイン関数
 *  - frame_buffer_config_ref
 *  - memory_map_ref
 */
extern "C" void KernelMainNewStack(const FrameBufferConfig& frame_buffer_config_ref,
                                   const MemoryMap& memory_map_ref) {
  SetLogLevel(kInfo);

  FrameBufferConfig frame_buffer_config{ frame_buffer_config_ref };
  MemoryMap memory_map{ memory_map_ref };

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
  for (uintptr_t iter = memory_map_base; iter < memory_map_base + memory_map.map_size;
       iter += memory_map.descriptor_size) {
    auto desc = reinterpret_cast<const MemoryDescriptor*>(iter);
    if (available_end < desc->physical_start) {
      memory_manager->MarkAllocated(
          FrameID{ available_end / kBytesPerFrame },
          (desc->physical_start - available_end) / kBytesPerFrame);
    }

    const auto physical_end =
        desc->physical_start + desc->number_of_pages * kUEFIPageSize;

    if (IsAvailable(static_cast<MemoryType>(desc->type))) {
      available_end = physical_end;
    } else {
      memory_manager->MarkAllocated(
          FrameID{ desc->physical_start / kBytesPerFrame },
          desc->number_of_pages * kUEFIPageSize / kBytesPerFrame);
    }
  }
  memory_manager->SetMemoryRange(FrameID{ 1 }, FrameID{ available_end / kBytesPerFrame });

  if (auto err = InitilizeHeap(*memory_manager)) {
    Log(kError, "failed to allocate pages: %s at %s:%d\n", err.Name(), err.File(),
        err.Line());
    exit(1);
  }
  //メモリマネージャの初期化終了

  /*フレームバッファを初期化*/
  framebuffer_init(frame_buffer_config, { 0x00, 0x00, 0x00 });

  //console_init();
  console = new (console_buf) Console{ kDesktopFGColor, kDesktopBGColor };

  printk("Hello. SaintOS World.\n");

  Log(kInfo, "[INFO] Console Format:%d rows * %d columns.\n", console->ttyRows,
      console->ttyColumns);

  Log(kInfo, "[INFO] Kernel Stack begin: 0x%08x, length: %08x(%d)\n", &kernel_main_stack,
      size_of_stack, size_of_stack);

  //pci_scan();
  auto err = pci::ScanAllBus();
  Log(kInfo, "[INFO] ScanAllBus: %s\n", err.Name());

  for (int i = 0; i < pci::num_device; ++i) {
    const auto& dev = pci::devices[i];
    auto vendor_id  = pci::ReadVendorId(dev.bus, dev.device, dev.function);
    auto device_id  = pci::ReadDeviceId(dev.bus, dev.device, dev.function);
    auto class_code = pci::ReadClassCode(dev.bus, dev.device, dev.function);
    Log(kInfo,
        "[PCI_INFO] %02d:%02d:%d: vendor:%04x, device:%04x, class:%08x, head "
        "%02x\n",
        dev.bus, dev.device, dev.function, vendor_id, device_id, class_code,
        dev.header_type);
  }

  //xhc_init();
  std::array<Message, 32> main_queue_data;
  ArrayQueue<Message> main_queue{ main_queue_data };
  ::main_queue = &main_queue;

  pci::Device* xhc_dev = nullptr;
  for (int i = 0; i < pci::num_device; ++i) {
    /*------------------------------------------------------------ 
      base = 30, sub =03, interface=0c が xHCデバイス
    ------------------------------------------------------------*/
    if (pci::devices[i].class_code.Match(0x0Cu, 0x03u, 0x30u)) {
      xhc_dev = &pci::devices[i];

      /*----------------------------------------------------- 
        Intel製のコントローラを優先する。
        なおQEMUではNEC製のコントローラがエミュレートされているので、
        以下のif文では検出されない.
      -----------------------------------------------------*/
      if (PCI_VENDOR_INTEL == pci::ReadVendorId(*xhc_dev)) {
        break;
      }
    }
  }

  /*-----------------------------------------------------------
    検出したxHCの情報を表示.
  -----------------------------------------------------------*/
  if (xhc_dev) {
    Log(kInfo, "[XHCI_INFO] xHC has been found: %02d:%02d:%d\n", xhc_dev->bus,
        xhc_dev->device, xhc_dev->function);
    Log(kInfo, "[XHCI_INFO] xHC vendor:%04x, device:%04x\n", xhc_dev->vendor_id,
        xhc_dev->device_id);
  }

  /*-----------------------------------------------------------
    IDTをロード
  -----------------------------------------------------------*/
  const uint16_t cs = GetCS();
  Log(kDebug, "[DEBUG] Code Segment Address: 0x%08x\n", cs);
  Log(kDebug, "[DEBUG] IntHandlerXHCI: 0x%08x\n",
      reinterpret_cast<uint64_t>(IntHandlerXHCI));

  SetIDTEntry(idt[IntrVector::kXHCI], MakeIDTAttr(DescType::kInterruptGate, 0),
              reinterpret_cast<uint64_t>(IntHandlerXHCI), cs);
  Log(kDebug, "[DEBUG] Length of IDTEntry : sizeof(idt)/16 = %d\n", sizeof(idt) / 16);
  Log(kDebug, "[DEBUG] &idt[0]: 0x%08x\n", reinterpret_cast<uintptr_t>(&idt[0]));
  Log(kDebug, "[DEBUG] &idt[0x%02x]: 0x%08x\n", IntrVector::kXHCI,
      reinterpret_cast<uintptr_t>(&idt[IntrVector::kXHCI]));
  Log(kDebug, "[DEBUG] %016x %08x %08x\n", idt[IntrVector::kXHCI].offset_high,
      idt[IntrVector::kXHCI].offset_middle, idt[IntrVector::kXHCI].offset_low);
  LoadIDT(sizeof(idt) - 1, reinterpret_cast<uintptr_t>(&idt[0]));

  /*-----------------------------------------------------------
    MSIを設定
  -----------------------------------------------------------*/
  const uint8_t bsp_local_apic_id =
      *reinterpret_cast<const uint32_t*>(LAPIC_ID_REG) >> 24;

  Log(kInfo, "[INFO] BSP LocalAPIC ID: %d\n", bsp_local_apic_id);
  Log(kInfo, "[INFO] TriggerMode: %d, DeliverlyMode: %d ,Vector: 0x%02x\n",
      pci::MSITriggerMode::kLevel, pci::MSIDeliveryMode::kFixed, IntrVector::kXHCI);

  pci::ConfigureMSIFixedDestination(*xhc_dev, bsp_local_apic_id,
                                    pci::MSITriggerMode::kLevel,
                                    pci::MSIDeliveryMode::kFixed, IntrVector::kXHCI, 0);

  /*--------------------------------------------------------------
    xHCのベースアドレスレジスタの読み取り
  --------------------------------------------------------------*/
  const WithError<uint64_t> xhc_bar = pci::ReadBar(*xhc_dev, 0);
  Log(kInfo, "[XHCI INFO] ReadBar: %s\n", xhc_bar.error.Name());
  const uint64_t xhc_mmio_base = xhc_bar.value & ~static_cast<uint64_t>(0xf);
  Log(kInfo, "[XHCI INFO] xHC MMI/O base address: %08lx\n", xhc_mmio_base);

  /*-------------------------------------------------------------
    xHCIコントローラを制御するためのクラスのインスタンス生成
  -------------------------------------------------------------*/
  usb::xhci::Controller xhc{ xhc_mmio_base };

  /*-------------------------------------------------------------
    Intel Panther Point チップ対応
  -------------------------------------------------------------*/
  if (PCI_VENDOR_INTEL == pci::ReadVendorId(*xhc_dev)) {
    SwitchEhci2Xhci(*xhc_dev);
  }

  /*-------------------------------------------------------------
    xHCの初期化(xHCの動作に必要な設定を実施)
  -------------------------------------------------------------*/
  {
    auto err = xhc.Initialize();
    Log(kInfo, "[XHCI INFO] xhc.Intialize: %s\n", err.Name());
  }
  /*-------------------------------------------------------------
    xHCの起動
  -------------------------------------------------------------*/
  Log(kInfo, "[XHCI INFO] xHC starting.\n");
  xhc.Run();

  ::xhc = &xhc;

  usb::HIDMouseDriver::default_observer = MouseObserver;

  for (int i = 1; i <= xhc.MaxPorts(); ++i) {
    auto port = xhc.PortAt(i);
    Log(kInfo, "Port %d: IsConnected=%d\n", i, port.IsConnected());

    if (port.IsConnected()) {
      /*- 注意 -------------------------------------------------------
        C++のADL(実引数依存の名前検索)のおかげで ConfigurPort() と書けるが、
        以下では、名前空間 usb::xhci:: を明示しておくこととする 
      --------------------------------------------------------------*/
      if (auto err = usb::xhci::ConfigurePort(xhc, port)) {
        Log(kError, "failed to configure port: %s at %s:%d\n", err.Name(), err.File(),
            err.Line());
        continue;
      }
    }
  }

  const int kFrameWidth  = frame_buffer_config.horizontal_resolution;
  const int kFrameHeight = frame_buffer_config.vertical_resolution;

  auto bgwindow = std::make_shared<Window>(kFrameWidth, kFrameHeight);
  auto bgwriter = bgwindow->Writer();

  DrawDesktop(*bgwriter);
  console->SetWriter(bgwriter);

  auto mouse_window = std::make_shared<Window>(kMouseCursorWidth, kMouseCursorHeight);
  mouse_window->SetTransparentColor(kMouseTransparentColor);
  DrawMouseCursor(mouse_window->Writer(), { 0, 0 });

  layer_manager = new LayerManager;
  layer_manager->SetWriter(pixel_writer);

  auto bglayer_id = layer_manager->NewLayer().SetWindow(bgwindow).Move({ 0, 0 }).ID();
  mouse_layer_id =
      layer_manager->NewLayer().SetWindow(mouse_window).Move({ 200, 200 }).ID();

  layer_manager->UpDown(bglayer_id, 0);
  layer_manager->UpDown(mouse_layer_id, 1);
  layer_manager->Draw();

  while (true) {
    __asm__("cli");
    if (main_queue.Count() == 0) {
      __asm__("sti\n\thlt");
      continue;
    }

    Message msg = main_queue.Front();
    main_queue.Pop();
    __asm__("sti");

    switch (msg.type) {
      case Message::kInterruptXHCI:
        while (xhc.PrimaryEventRing()->HasFront()) {
          if (auto err = usb::xhci::ProcessEvent(xhc)) {
            Log(kError, "Error while ProcessEvent: %s at %s:%d\n", err.Name(), err.File(),
                err.Line());
          }
        }
        break;

      default:
        Log(kError, "Unknown message type: %d\n", msg.type);
    }
  }

  halt();
}
