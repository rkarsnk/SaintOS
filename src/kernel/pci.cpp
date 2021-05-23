#include <pci.hpp>
/* ---------------------------------------------------------------------------
CONFIG_ADDRESSレジスタ:
| 31|30  -   24|23  -  16|15    -   11|10     -     8|7     -      2|  1| 0 |
|   | Reserved | Bus No. | Device No. | Fucntion No. | Register No. | 0 | 0 |
  ^
  | Enable bit (1 = enabled, 0 = disabled)
                                   ref) PCI Local Bus Specification Rev.3.0 P.50
  -----------------------------------------------------------------------------*/
namespace {
  using namespace pci;

  uint32_t MakeAddress(uint8_t bus, uint8_t device, uint8_t function,
                       uint8_t reg) {
    /*
    xを左にbitsシフトした値を返すラムダ関数
    */
    auto shl = [](uint32_t x, unsigned int bits) {
      return x << bits;
    };

    return shl(1, 31)          // set enable bit
           | shl(bus, 16)      // set bus_no
           | shl(device, 11)   // set device_no
           | shl(function, 8)  // set function_no
           | (reg & ~0x03);    // set 1bit and 0bit field = 00 (1111100)
  }

  Error AddDevice(const Device& device) {
    if (num_device == devices.size()) {
      return Error::kFull;
    }

    devices[num_device] = device;
    ++num_device;
    return Error::kSuccess;
  }

  Error ScanBus(uint8_t bus);

  Error ScanFunction(uint8_t bus, uint8_t device, uint8_t function) {
    auto class_code = ReadClassCode(bus, device, function);
    auto header_type = ReadHeaderType(bus, device, function);
    auto vendor_id = ReadVendorId(bus, device, function);
    auto device_id = ReadDeviceId(bus, device, function);
    Device dev{bus,       device,    function,  header_type,
               vendor_id, device_id, class_code};
    if (auto err = AddDevice(dev)) {
      return err;
    }

    //PCI-PCIブリッジ決め打ち
    // ？？？header_type == 1　で条件文書いても同様のことができるのではないだろうか．
    if (class_code.Match(0x06u, 0x04u)) {
      auto bus_numbers = ReadBusNumbers(bus, device, function);
      uint8_t secondary_bus = (bus_numbers >> 8) & 0xffu;
      return ScanBus(secondary_bus);
    }

    return Error::kSuccess;
  }

  Error ScanDevice(uint8_t bus, uint8_t device) {
    if (auto err = ScanFunction(bus, device, 0)) {
      return err;
    }

    if (IsSingleFunctionDevice(ReadHeaderType(bus, device, 0))) {
      return Error::kSuccess;
    }

    for (uint8_t function = 1; function < 8; ++function) {
      if (ReadVendorId(bus, device, function) == 0xffffu) {
        continue;
      }
      if (auto err = ScanFunction(bus, device, function)) {
        return err;
      }
    }
    return Error::kSuccess;
  }

  Error ScanBus(uint8_t bus) {
    for (uint8_t device = 0; device < 32; ++device) {
      if (ReadVendorId(bus, device, 0) == 0xffffu) {
        continue;
      }
      if (auto err = ScanDevice(bus, device)) {
        return err;
      }
    }
    return Error::kSuccess;
  }

}  // namespace

/*
// enable configuration space accesses and return data port address
static int
pci_cfgenable(unsigned bus, unsigned slot, unsigned func, int reg, int bytes)
{
        int dataport = 0;

        if (bus <= PCI_BUSMAX && slot <= PCI_SLOTMAX && func <= PCI_FUNCMAX &&
            (unsigned)reg <= PCI_REGMAX && bytes != 3 &&
            (unsigned)bytes <= 4 && (reg & (bytes - 1)) == 0) {
                outl(CONF1_ADDR_PORT, (1U << 31) | (bus << 16) | (slot << 11) 
                    | (func << 8) | (reg & ~0x03));
                dataport = CONF1_DATA_PORT + (reg & 0x03);
        }
        return (dataport);
}

*/

namespace pci {

  void pci_cfg_test() {
    outl(PCI_CONF_PORT, (1U << 31) | (0 << 16) | (4 << 11) | (0 << 8) |
                            (PCI_CLASSCODE & ~0x03));
    printk("[DEBUG] ClassCode %08x:%08x\n",
           (PCI_DATA_PORT + (PCI_CLASSCODE & 0x03)),
           inl(PCI_DATA_PORT + (PCI_CLASSCODE & 0x03)));

    outl(PCI_CONF_PORT, (1U << 31) | (0 << 16) | (4 << 11) | (0 << 8) |
                            (_PCI_BASE_CLASS & ~0x03));
    printk("[DEBUG] 8-7 %08x:%02x\n",
           (PCI_DATA_PORT + (_PCI_BASE_CLASS & 0x03)),
           inb(PCI_DATA_PORT + (_PCI_BASE_CLASS & 0x03)));

    outl(PCI_CONF_PORT, (1U << 31) | (0 << 16) | (4 << 11) | (0 << 8) |
                            (_PCI_SUB_CLASS & ~0x03));
    printk("[DEBUG] 6-5 %08x:%02x\n",
           (PCI_DATA_PORT + (_PCI_SUB_CLASS & 0x03)),
           inb(PCI_DATA_PORT + (_PCI_SUB_CLASS & 0x03)));

    outl(PCI_CONF_PORT, (1U << 31) | (0 << 16) | (4 << 11) | (0 << 8) |
                            (_PCI_CLASS_INTERFACE & ~0x03));
    printk("[DEBUG] 4-3 %08x:%02x\n",
           (PCI_DATA_PORT + (_PCI_CLASS_INTERFACE & 0x03)),
           inb(PCI_DATA_PORT + (_PCI_CLASS_INTERFACE & 0x03)));

    outl(PCI_CONF_PORT, (1U << 31) | (0 << 16) | (4 << 11) | (0 << 8) |
                            (_PCI_CLASS_INTERFACE & ~0x03));
    printk("[DEBUG] 2-1 %08x:%02x\n",
           (PCI_DATA_PORT + (_PCI_REVISION_ID & 0x03)),
           inb(PCI_DATA_PORT + (_PCI_REVISION_ID & 0x03)));
  }

  void WritePciConfigAddress(uint32_t address) {
    outl(kConfigAddress, address);
  }

  void WritePciConfigData(uint32_t value) {
    outl(kConfigData, value);
  }

  uint32_t ReadPciConfigData() {
    return inl(kConfigData);
  }

  uint16_t ReadVendorId(uint8_t bus, uint8_t device, uint8_t function) {
    WritePciConfigAddress(
        MakeAddress(bus, device, function, PCI_VENDOR_AND_DEVICE_ID));
    //上位16bitをマスクして，下位16bitのVENDOR IDを取り出す．
    return ReadPciConfigData() & 0xffffu;
  }

  uint16_t ReadDeviceId(uint8_t bus, uint8_t device, uint8_t function) {
    WritePciConfigAddress(
        MakeAddress(bus, device, function, PCI_VENDOR_AND_DEVICE_ID));
    //16bit 右にシフトして，上位16bitのDevice IDを取り出す．
    return ReadPciConfigData() >> 16;
  }

  uint8_t ReadHeaderType(uint8_t bus, uint8_t device, uint8_t function) {
    WritePciConfigAddress(
        MakeAddress(bus, device, function, PCI_HEADER_TYPE));
    //16bit右シフト後，上位8bitをマスクして，下位8bitのHeader Typeを取り出す
    return (ReadPciConfigData() >> 16) & 0xffu;
  }

  ClassCode ReadClassCode(uint8_t bus, uint8_t device, uint8_t function) {
    WritePciConfigAddress(
        MakeAddress(bus, device, function, PCI_CLASSCODE));
    auto reg = ReadPciConfigData();
    ClassCode cc;
    cc.base = (reg >> 24) & 0xffu;
    cc.sub = (reg >> 16) & 0xffu;
    cc.interface = (reg >> 8) & 0xffu;
    return cc;
  }

  uint32_t ReadBusNumbers(uint8_t bus, uint8_t device, uint8_t function) {
    WritePciConfigAddress(MakeAddress(bus, device, function, PCI_BUS_NO));
    return ReadPciConfigData();
  }

  bool IsSingleFunctionDevice(uint8_t header_type) {
    return (header_type & 0x80u) == 0;
  }

  Error ScanAllBus() {
    num_device = 0;

    auto header_type = ReadHeaderType(0, 0, 0);
    if (IsSingleFunctionDevice(header_type)) {
      return ScanBus(0);
    }

    for (uint8_t function = 0; function < 8; ++function) {
      if (ReadVendorId(0, 0, function) == 0xffffu) {
        continue;
      }
      if (auto err = ScanBus(function)) {
        return err;
      }
    }
    return Error::kSuccess;
  }

}  // namespace pci

void pci_init() {
  auto err = pci::ScanAllBus();
  printk("ScanAllBus: %s\n", err.Name());

  for (int i = 0; i < pci::num_device; ++i) {
    const auto& dev = pci::devices[i];
    auto vendor_id = pci::ReadVendorId(dev.bus, dev.device, dev.function);
    auto device_id = pci::ReadDeviceId(dev.bus, dev.device, dev.function);
    auto class_code =
        pci::ReadClassCode(dev.bus, dev.device, dev.function);
    printk("%02d:%02d:%d: ", dev.bus, dev.device, dev.function);
    printk("vendor:%04x, device:%04x, class:%08x, head %02x\n", vendor_id,
           device_id, class_code, dev.header_type);
  }
}