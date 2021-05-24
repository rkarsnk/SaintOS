#include <xhc.hpp>

void scan_xhc() {
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
}