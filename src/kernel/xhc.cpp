#include <xhc.hpp>

char mouse_cursor_buf[sizeof(MouseCursor)];
MouseCursor* mouse_cursor;

void xhc_init() {
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
    Log(kInfo, "xHC vendor:%04x, device:%04x\n", xhc_dev->vendor_id,
        xhc_dev->device_id);
  }

  const WithError<uint64_t> xhc_bar = pci::ReadBar(*xhc_dev, 0);
  Log(kDebug, "ReadBar: %s\n", xhc_bar.error.Name());
  const uint64_t xhc_mmio_base = xhc_bar.value & ~static_cast<uint64_t>(0xf);
  Log(kDebug, "xHC MMI/O base address: %08lx\n", xhc_mmio_base);

  usb::xhci::Controller xhc{xhc_mmio_base};

  if (PCI_VENDOR_INTEL == pci::ReadVendorId(*xhc_dev)) {
    SwitchEhci2Xhci(*xhc_dev);
  }

  auto err = xhc.Initialize();
  Log(kDebug, "xhc.Intialize: %s\n", err.Name());

  Log(kInfo, "xHC starting.\n");
  xhc.Run();

  usb::HIDMouseDriver::default_observer = MouseObserver;

  for (int i = 1; i <= xhc.MaxPorts(); ++i) {
    auto port = xhc.PortAt(i);
    Log(kDebug, "Port %d: IsConnected=%d\n", i, port.IsConnected());

    if (port.IsConnected()) {
      if (auto err = ConfigurePort(xhc, port)) {
        Log(kError, "failed to configure port: %s at %s:%d\n", err.Name(),
            err.File(), err.Line());
        continue;
      }
    }
  }

  while (1) {
    if (auto err = ProcessEvent(xhc)) {
      Log(kError, "Error while ProcessEvent: %s at %s:%d\n", err.Name(),
          err.File(), err.Line());
    }
  }
}

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
  Log(kDebug, "SwitchEhci2Xhci: SS = %02, xHCI = %02x\n", superspeed_ports,
      ehci2xhci_ports);
}

void MouseObserver(int8_t displacement_x, int8_t displacement_y) {
  mouse_cursor->MoveRelative({displacement_x, displacement_y});
}

extern "C" void __cxa_pure_virtual() {
  while (1)
    __asm__("hlt");
}