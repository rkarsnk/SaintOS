#include <pci.hpp>

namespace {
  using namespace pci;
  uint32_t MakeAddress(uint8_t bus, uint8_t device, uint8_t function,
                       uint8_t reg_addr) {
    /*
    xを左にbitsシフトした値を返すラムダ関数
    */
    auto shl = [](uint32_t x, unsigned int bits) {
      return x << bits;
    };

    return shl(1, 31)             // set enable bit
           | shl(bus, 16)         // set bus_no
           | shl(device, 11)      // set device_no
           | shl(function, 8)     // set function_no
           | (reg_addr & 0xfcu);  // set 1bit and 0bit field = 00 (1111100)
  }

  Error AddDevice(uint8_t bus, uint8_t device, uint8_t function,
                  uint8_t header_type) {
    if (num_device == devices.size()) {
      return Error::kFull;
    }

    devices[num_device] = Device{bus, device, function, header_type};
    ++num_device;
    return Error::kSuccess;
  }

  Error ScanBus(uint8_t bus);

  Error ScanFunction(uint8_t bus, uint8_t device, uint8_t function) {
    auto header_type = ReadHeaderType(bus, device, function);
    if (auto err = AddDevice(bus, device, function, header_type)) {
      return err;
    }

    auto class_code = ReadClassCode(bus, device, function);
    uint8_t base = (class_code >> 24) & 0xffu;
    uint8_t sub = (class_code >> 16) & 0xffu;

    //PCI-PCIブリッジ決め打ち
    // ？？？header_type == 1　で条件文書いても同様のことができるのではないだろうか．
    if (base == 0x06u && sub == 0x04u) {
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

namespace pci {
  void WritePciConfigAddress(uint32_t address) {
    IoOut32(kConfigAddress, address);
  }

  void WritePciConfigData(uint32_t value) {
    IoOut32(kConfigData, value);
  }

  uint32_t ReadPciConfigData() {
    return IoIn32(kConfigData);
  }

  uint16_t ReadVendorId(uint8_t bus, uint8_t device, uint8_t function) {
    WritePciConfigAddress(
        MakeAddress(bus, device, function, PCI_VENDOR_ADN_DEVICE_ID));
    //上位16bitをマスクして，下位16bitのVENDOR IDを取り出す．
    return ReadPciConfigData() & 0xffffu;
  }

  uint16_t ReadDeviceId(uint8_t bus, uint8_t device, uint8_t function) {
    WritePciConfigAddress(
        MakeAddress(bus, device, function, PCI_VENDOR_ADN_DEVICE_ID));
    //16bit 右にシフトして，上位16bitのDevice IDを取り出す．
    return ReadPciConfigData() >> 16;
  }

  uint8_t ReadHeaderType(uint8_t bus, uint8_t device, uint8_t function) {
    WritePciConfigAddress(
        MakeAddress(bus, device, function, PCI_HEADER_TYPE));
    //16bit右シフト後，上位8bitをマスクして，下位8bitのHeader Typeを取り出す
    return (ReadPciConfigData() >> 16) & 0xffu;
  }

  uint32_t ReadClassCode(uint8_t bus, uint8_t device, uint8_t function) {
    WritePciConfigAddress(
        MakeAddress(bus, device, function, PCI_CLASSCODE));
    return ReadPciConfigData();
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