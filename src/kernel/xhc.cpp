#include <xhc.hpp>



/**
 * Intel Panther Point チップ対応
 */
void SwitchEhci2Xhci(const pci::Device& xhc_dev) {
  bool intel_ehc_exist = false;
  for (int i = 0; i < pci::num_device; ++i) {
    if (pci::devices[i].class_code.Match(0x0cu, 0x03u, 0x20u) /* EHCI */ &&
        PCI_VENDOR_INTEL == pci::ReadVendorId(pci::devices[i])) {
      intel_ehc_exist = true;
      break;
    }
  }
  if (!intel_ehc_exist) {
    return;
  }

  uint32_t superspeed_ports = pci::ReadConfReg(xhc_dev, 0xdc);  // USB3PRM
  pci::WriteConfReg(xhc_dev, 0xd8, superspeed_ports);           // USB3_PSSEN
  uint32_t ehci2xhci_ports = pci::ReadConfReg(xhc_dev, 0xd4);   // XUSB2PRM
  pci::WriteConfReg(xhc_dev, 0xd0, ehci2xhci_ports);            // XUSB2PR
  Log(kInfo, "SwitchEhci2Xhci: SS = %02x, xHCI = %02x\n", superspeed_ports,
      ehci2xhci_ports);
}

// void xhc_init() {
//   std::array<Message, 32> main_queue_data;
//   ArrayQueue<Message> main_queue{ main_queue_data };
//   ::main_queue = &main_queue;

//   pci::Device* xhc_dev = nullptr;
//   for (int i = 0; i < pci::num_device; ++i) {
//     /*------------------------------------------------------------ 
//       base = 30, sub =03, interface=0c が xHCデバイス
//     ------------------------------------------------------------*/
//     if (pci::devices[i].class_code.Match(0x0Cu, 0x03u, 0x30u)) {
//       xhc_dev = &pci::devices[i];

//       /*----------------------------------------------------- 
//         Intel製のコントローラを優先する。
//         なおQEMUではNEC製のコントローラがエミュレートされているので、
//         以下のif文では検出されない.
//       -----------------------------------------------------*/
//       if (PCI_VENDOR_INTEL == pci::ReadVendorId(*xhc_dev)) {
//         break;
//       }
//     }
//   }

//   /*-----------------------------------------------------------
//     検出したxHCの情報を表示.
//   -----------------------------------------------------------*/
//   if (xhc_dev) {
//     Log(kInfo, "[XHCI_INFO] xHC has been found: %02d:%02d:%d\n", xhc_dev->bus,
//         xhc_dev->device, xhc_dev->function);
//     Log(kInfo, "[XHCI_INFO] xHC vendor:%04x, device:%04x\n", xhc_dev->vendor_id,
//         xhc_dev->device_id);
//   }

//   /*-----------------------------------------------------------
//     IDTをロード
//   -----------------------------------------------------------*/
//   const uint16_t cs = GetCS();
//   Log(kDebug, "[DEBUG] Code Segment Address: 0x%08x\n", cs);
//   Log(kDebug, "[DEBUG] IntHandlerXHCI: 0x%08x\n",
//       reinterpret_cast<uint64_t>(IntHandlerXHCI));

//   SetIDTEntry(idt[IntrVector::kXHCI], MakeIDTAttr(DescType::kInterruptGate, 0),
//               reinterpret_cast<uint64_t>(IntHandlerXHCI), cs);
//   Log(kDebug, "[DEBUG] Length of IDTEntry : sizeof(idt)/16 = %d\n", sizeof(idt) / 16);
//   Log(kDebug, "[DEBUG] &idt[0]: 0x%08x\n", reinterpret_cast<uintptr_t>(&idt[0]));
//   Log(kDebug, "[DEBUG] &idt[0x%02x]: 0x%08x\n", IntrVector::kXHCI,
//       reinterpret_cast<uintptr_t>(&idt[IntrVector::kXHCI]));
//   Log(kDebug, "[DEBUG] %016x %08x %08x\n", idt[IntrVector::kXHCI].offset_high,
//       idt[IntrVector::kXHCI].offset_middle, idt[IntrVector::kXHCI].offset_low);
//   LoadIDT(sizeof(idt) - 1, reinterpret_cast<uintptr_t>(&idt[0]));

//   /*-----------------------------------------------------------
//     MSIを設定
//   -----------------------------------------------------------*/
//   const uint8_t bsp_local_apic_id =
//       *reinterpret_cast<const uint32_t*>(LAPIC_ID_REG) >> 24;

//   Log(kInfo, "[INFO] BSP LocalAPIC ID: %d\n", bsp_local_apic_id);
//   Log(kInfo, "[INFO] TriggerMode: %d, DeliverlyMode: %d ,Vector: 0x%02x\n",
//       pci::MSITriggerMode::kLevel, pci::MSIDeliveryMode::kFixed, IntrVector::kXHCI);

//   pci::ConfigureMSIFixedDestination(*xhc_dev, bsp_local_apic_id,
//                                     pci::MSITriggerMode::kLevel,
//                                     pci::MSIDeliveryMode::kFixed, IntrVector::kXHCI, 0);

//   /*--------------------------------------------------------------
//     xHCのベースアドレスレジスタの読み取り
//   --------------------------------------------------------------*/
//   const WithError<uint64_t> xhc_bar = pci::ReadBar(*xhc_dev, 0);
//   Log(kInfo, "[XHCI INFO] ReadBar: %s\n", xhc_bar.error.Name());
//   const uint64_t xhc_mmio_base = xhc_bar.value & ~static_cast<uint64_t>(0xf);
//   Log(kInfo, "[XHCI INFO] xHC MMI/O base address: %08lx\n", xhc_mmio_base);

//   /*-------------------------------------------------------------
//     xHCIコントローラを制御するためのクラスのインスタンス生成
//   -------------------------------------------------------------*/
//   usb::xhci::Controller xhc{ xhc_mmio_base };

//   /*-------------------------------------------------------------
//     Intel Panther Point チップ対応
//   -------------------------------------------------------------*/
//   if (PCI_VENDOR_INTEL == pci::ReadVendorId(*xhc_dev)) {
//     SwitchEhci2Xhci(*xhc_dev);
//   }

//   /*-------------------------------------------------------------
//     xHCの初期化(xHCの動作に必要な設定を実施)
//   -------------------------------------------------------------*/
//   {
//     auto err = xhc.Initialize();
//     Log(kInfo, "[XHCI INFO] xhc.Intialize: %s\n", err.Name());
//   }
//   /*-------------------------------------------------------------
//     xHCの起動
//   -------------------------------------------------------------*/
//   Log(kInfo, "[XHCI INFO] xHC starting.\n");
//   xhc.Run();

//   ::xhc = &xhc;

//   usb::HIDMouseDriver::default_observer = MouseObserver;

//   for (int i = 1; i <= xhc.MaxPorts(); ++i) {
//     auto port = xhc.PortAt(i);
//     Log(kInfo, "Port %d: IsConnected=%d\n", i, port.IsConnected());

//     if (port.IsConnected()) {
//       /*- 注意 -------------------------------------------------------
//         C++のADL(実引数依存の名前検索)のおかげで ConfigurPort() と書けるが、
//         以下では、名前空間 usb::xhci:: を明示しておくこととする 
//       --------------------------------------------------------------*/
//       if (auto err = usb::xhci::ConfigurePort(xhc, port)) {
//         Log(kError, "failed to configure port: %s at %s:%d\n", err.Name(), err.File(),
//             err.Line());
//         continue;
//       }
//     }
//   }

//   while (true) {
//     __asm__("cli");
//     if (main_queue.Count() == 0) {
//       __asm__("sti\n\thlt");
//       continue;
//     }

//     Message msg = main_queue.Front();
//     main_queue.Pop();
//     __asm__("sti");

//     switch (msg.type) {
//       case Message::kInterruptXHCI:
//         while (xhc.PrimaryEventRing()->HasFront()) {
//           if (auto err = usb::xhci::ProcessEvent(xhc)) {
//             Log(kError, "Error while ProcessEvent: %s at %s:%d\n", err.Name(), err.File(),
//                 err.Line());
//           }
//         }
//         break;

//       default:
//         Log(kError, "Unknown message type: %d\n", msg.type);
//     }
//   }
// }

extern "C" void __cxa_pure_virtual() {
  halt();
}