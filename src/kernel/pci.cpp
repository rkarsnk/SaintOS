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
    auto class_code  = ReadClassCode(bus, device, function);
    auto header_type = ReadHeaderType(bus, device, function);
    auto vendor_id   = ReadVendorId(bus, device, function);
    auto device_id   = ReadDeviceId(bus, device, function);
    Device dev{bus,       device,    function,  header_type,
               vendor_id, device_id, class_code};
    if (auto err = AddDevice(dev)) {
      return err;
    }

    //PCI-PCIブリッジ決め打ち
    // ？？？header_type == 1　で条件文書いても同様のことができるのではないだろうか．
    if (class_code.Match(0x06u, 0x04u)) {
      auto bus_numbers      = ReadBusNumbers(bus, device, function);
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

  /* 指定されたMSIケーパビリティ構造を読み取る */
  MSICapability ReadMSICapability(const Device& dev, uint8_t cap_addr) {
    MSICapability msi_cap{};

    msi_cap.header.data = ReadConfReg(dev, cap_addr);
    msi_cap.msg_addr    = ReadConfReg(dev, cap_addr + 4);

    uint8_t msg_data_addr = cap_addr + 8;
    if (msi_cap.header.bits.addr_64_capable) {  //addr_64_capable が0以外
      msi_cap.msg_upper_addr = ReadConfReg(dev, cap_addr + 8);
      msg_data_addr          = cap_addr + 12;
    }

    msi_cap.msg_data = ReadConfReg(dev, msg_data_addr);

    if (msi_cap.header.bits.per_vector_mask_capable) {
      msi_cap.mask_bits    = ReadConfReg(dev, msg_data_addr + 4);
      msi_cap.pending_bits = ReadConfReg(dev, msg_data_addr + 8);
    }

    return msi_cap;
  }

  /* 指定されたMSIケーパビリティ構造に書き込む */
  void WriteMSICapability(const Device& dev, uint8_t cap_addr,
                          const MSICapability& msi_cap) {
    WriteConfReg(dev, cap_addr, msi_cap.header.data);
    WriteConfReg(dev, cap_addr + 4, msi_cap.msg_addr);

    uint8_t msg_data_addr = cap_addr + 8;
    if (msi_cap.header.bits.addr_64_capable) {
      WriteConfReg(dev, cap_addr + 8, msi_cap.msg_upper_addr);
      msg_data_addr = cap_addr + 12;
    }

    WriteConfReg(dev, msg_data_addr, msi_cap.msg_data);

    if (msi_cap.header.bits.per_vector_mask_capable) {
      WriteConfReg(dev, msg_data_addr + 4, msi_cap.mask_bits);
      WriteConfReg(dev, msg_data_addr + 8, msi_cap.pending_bits);
    }
  }

  /* 指定されたMSIレジスタを設定する */
  Error ConfigureMSIRegister(const Device& dev, uint8_t cap_addr,
                             uint32_t msg_addr, uint32_t msg_data,
                             uint num_vector_exponent) {
    auto msi_cap = ReadMSICapability(dev, cap_addr);

    if (msi_cap.header.bits.multi_msg_capable <= num_vector_exponent) {
      msi_cap.header.bits.multi_msg_enable =
          msi_cap.header.bits.multi_msg_capable;
    } else {
      msi_cap.header.bits.multi_msg_enable = num_vector_exponent;
    }

    msi_cap.header.bits.msi_enable = 1;

    msi_cap.msg_addr = msg_addr;
    msi_cap.msg_data = msg_data;
    Log(kInfo, "[DEBUG] Message Address = %08x, Message Data = %08x\n",
        msg_addr, msg_data);
    WriteMSICapability(dev, cap_addr, msi_cap);
    return MAKE_ERROR(Error::kSuccess);
  }

  /* 指定されたMSI-Xレジスタを設定する */
  Error ConfigureMSIXRegister(const Device& dev, uint8_t cap_addr,
                              uint32_t msg_addr, uint32_t mas_data,
                              uint num_vector_exponent) {
    return MAKE_ERROR(Error::kNotImplemented);
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

  uint8_t ReadCapabilityHeader(uint8_t bus, uint8_t device, uint8_t function) {
    WritePciConfigAddress(MakeAddress(bus, device, function, PCIR_CAP_PTR));
    return ReadPciDataBYTE(PCIR_CAP_PTR);
  }

  ClassCode ReadClassCode(uint8_t bus, uint8_t device, uint8_t function) {
    WritePciConfigAddress(MakeAddress(bus, device, function, PCIR_CLASSCODE));
    auto reg = ReadPciDataDWORD(PCIR_CLASSCODE);
    ClassCode cc;
    cc.base      = (reg >> 24) & 0xffu;
    cc.sub       = (reg >> 16) & 0xffu;
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

  WithError<uint64_t> ReadBar(Device& device, uint bar_index) {
    if (bar_index >= 6) {
      return {0, MAKE_ERROR(Error::kIndexOutOfRange)};
    }

    const auto addr = CalcBarAddress(bar_index);
    const auto bar  = ReadConfReg(device, addr);

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

  /* addrには PCIR_CAP_PTR(0x34h)に記録されているアドレスが指定される */
  CapabilityHeader ReadCapabilityHeader(const Device& dev, uint8_t addr) {
    CapabilityHeader header;
    header.data = pci::ReadConfReg(dev, addr);
    return header;
  }

  Error ConfigureMSI(const Device& dev, uint32_t msg_addr, uint32_t msg_data,
                     uint num_vector_exponent) {
    uint8_t cap_addr     = ReadConfReg(dev, 0x34) & 0xffu;
    uint8_t msi_cap_addr = 0, msix_cap_addr = 0;

    while (cap_addr != 0) {
      auto header = ReadCapabilityHeader(dev, cap_addr);
      if (header.bits.cap_id == kCapabilityMSI) {
        msi_cap_addr = cap_addr;
      } else if (header.bits.cap_id == kCapabilityMSIX) {
        msix_cap_addr = cap_addr;
      }
      cap_addr = header.bits.next_ptr;
    }

    if (msi_cap_addr) {  // msi_cap_addrが0以外
      return ConfigureMSIRegister(dev, msi_cap_addr, msg_addr, msg_data,
                                  num_vector_exponent);
    } else if (msix_cap_addr) {
      return ConfigureMSIXRegister(dev, msi_cap_addr, msg_addr, msg_data,
                                   num_vector_exponent);
    }
    return MAKE_ERROR(Error::kNoPCIMSI);
  }

  Error ConfigureMSIFixedDestination(const Device& dev, uint8_t apic_id,
                                     MSITriggerMode trigger_mode,
                                     MSIDeliveryMode delivery_mode,
                                     uint8_t vector, uint num_vector_exponent) {
    uint32_t msg_addr = 0xfee00000u | (apic_id << 12);  //set Destination ID
    uint32_t msg_data = (static_cast<uint32_t>(delivery_mode) << 8) | vector;
    if (trigger_mode == MSITriggerMode::kLevel) {
      msg_data |= 0xc000;
    }
    return ConfigureMSI(dev, msg_addr, msg_data, num_vector_exponent);
  }

}  //namespace pci

/*
void pci_scan() {
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
}
*/