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
    return (1U << 31)         // set enable bit
           | (bus << 16)      // set bus_no
           | (device << 11)   // set device_no
           | (function << 8)  // set function_no
           | (reg & ~0x03);   // set 1bit and 0bit field = 00
  }

  Error AddDevice(const Device& device) {
    if (num_device == devices.size()) {
      return MAKE_ERROR(Error::kFull);
    }

    devices[num_device] = device;
    ++num_device;
    return MAKE_ERROR(Error::kSuccess);
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

    return MAKE_ERROR(Error::kSuccess);
  }

  Error ScanDevice(uint8_t bus, uint8_t device) {
    if (auto err = ScanFunction(bus, device, 0)) {
      return err;
    }

    if (IsSingleFunctionDevice(ReadHeaderType(bus, device, 0))) {
      return MAKE_ERROR(Error::kSuccess);
    }

    for (uint8_t function = 1; function < 8; ++function) {
      if (ReadVendorId(bus, device, function) == 0xffffu) {
        continue;
      }
      if (auto err = ScanFunction(bus, device, function)) {
        return err;
      }
    }
    return MAKE_ERROR(Error::kSuccess);
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
    return MAKE_ERROR(Error::kSuccess);
  }
}  //namespace

namespace pci {
  void WritePciConfigAddress(uint32_t address) {
    outl(PCI_CONF_PORT, address);
  }

  void WritePciConfigData(uint32_t value) {
    outl(PCI_DATA_PORT, value);
  }

  uint32_t ReadPciDataDWORD(uint32_t reg) {
    return inl(PCI_DATA_PORT + (reg & 0x03));
  }

  uint16_t ReadPciDataWORD(uint32_t reg) {
    return inw(PCI_DATA_PORT + (reg & 0x03));
  }

  uint8_t ReadPciDataBYTE(uint32_t reg) {
    return inb(PCI_DATA_PORT + (reg & 0x03));
  }

  uint16_t ReadVendorId(uint8_t bus, uint8_t device, uint8_t function) {
    WritePciConfigAddress(MakeAddress(bus, device, function, PCIR_VENDOR_ID));
    //上位16bitをマスクして，下位16bitのVENDOR IDを取り出す．
    return ReadPciDataWORD(PCIR_VENDOR_ID);
  }

  uint16_t ReadDeviceId(uint8_t bus, uint8_t device, uint8_t function) {
    WritePciConfigAddress(MakeAddress(bus, device, function, PCIR_DEVICE_ID));
    return ReadPciDataWORD(PCIR_DEVICE_ID);
  }

  uint8_t ReadHeaderType(uint8_t bus, uint8_t device, uint8_t function) {
    WritePciConfigAddress(MakeAddress(bus, device, function, PCIR_HEADER_TYPE));
    //16bit右シフト後，上位8bitをマスクして，下位8bitのHeader Typeを取り出す
    return ReadPciDataBYTE(PCIR_HEADER_TYPE);
  }

  ClassCode ReadClassCode(uint8_t bus, uint8_t device, uint8_t function) {
    WritePciConfigAddress(MakeAddress(bus, device, function, PCIR_CLASSCODE));
    auto reg = ReadPciDataDWORD(PCIR_CLASSCODE);
    ClassCode cc;
    cc.base = (reg >> 24) & 0xffu;
    cc.sub = (reg >> 16) & 0xffu;
    cc.interface = (reg >> 8) & 0xffu;
    return cc;
  }

  uint32_t ReadBusNumbers(uint8_t bus, uint8_t device, uint8_t function) {
    WritePciConfigAddress(MakeAddress(bus, device, function, PCI_BUS_NO));
    return ReadPciDataDWORD(PCI_BUS_NO);
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
    return MAKE_ERROR(Error::kSuccess);
  }
  uint32_t ReadConfReg(const Device& dev, uint8_t reg_addr) {
    WritePciConfigAddress(
        MakeAddress(dev.bus, dev.device, dev.function, reg_addr));
    return ReadPciDataDWORD(reg_addr);
  }

  void WriteConfReg(const Device& dev, uint8_t reg_addr, uint32_t value) {
    WritePciConfigAddress(
        MakeAddress(dev.bus, dev.device, dev.function, reg_addr));
    WritePciConfigData(value);
  }

  WithError<uint64_t> ReadBar(Device& device, unsigned int bar_index) {
    if (bar_index >= 6) {
      return {0, MAKE_ERROR(Error::kIndexOutOfRange)};
    }

    const auto addr = CalcBarAddress(bar_index);
    const auto bar = ReadConfReg(device, addr);

    // 32 bit address
    if ((bar & 4u) == 0) {
      return {bar, MAKE_ERROR(Error::kSuccess)};
    }

    // 64 bit address
    if (bar_index >= 5) {
      return {0, MAKE_ERROR(Error::kIndexOutOfRange)};
    }

    const auto bar_upper = ReadConfReg(device, addr + 4);
    return {bar | (static_cast<uint64_t>(bar_upper) << 32),
            MAKE_ERROR(Error::kSuccess)};
  }

}  //namespace pci

void pci_init() {
  auto err = pci::ScanAllBus();
  Log(kInfo, "ScanAllBus: %s\n", err.Name());

  for (int i = 0; i < pci::num_device; ++i) {
    const auto& dev = pci::devices[i];
    auto vendor_id = pci::ReadVendorId(dev.bus, dev.device, dev.function);
    auto device_id = pci::ReadDeviceId(dev.bus, dev.device, dev.function);
    auto class_code = pci::ReadClassCode(dev.bus, dev.device, dev.function);
    Log(kInfo,
        "%02d:%02d:%d: vendor:%04x, device:%04x, class:%08x, head %02x\n",
        dev.bus, dev.device, dev.function, vendor_id, device_id, class_code,
        dev.header_type);
  }
}