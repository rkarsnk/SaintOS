/*
PCIバス制御プログラム
*/

#pragma once

#include <array>
#include <cstdint>

#include <cpufunc.hpp>
#include <error.hpp>
#include <logger.hpp>

#include <pcidevs.h>

namespace pci {
  struct ClassCode {
    uint8_t base, sub, interface;

    bool Match(uint8_t b) {
      return (b == base);
    }

    bool Match(uint8_t b, uint8_t s) {
      return Match(b) && (s == sub);
    }

    bool Match(uint8_t b, uint8_t s, uint8_t i) {
      return Match(b, s) && (i == interface);
    }
  };

  struct Device {
    uint8_t bus, device, function, header_type;
    uint16_t vendor_id, device_id;
    ClassCode class_code;
  };

  void pci_cfg_test();

  void WritePciConfigAddress(uint32_t address);
  void WritePciConfigData(uint32_t value);

  uint32_t ReadPciDataDWORD(uint32_t reg);
  uint16_t ReadPciDataWORD(uint32_t reg);
  uint8_t ReadPciDataBYTE(uint32_t reg);

  uint16_t ReadVendorId(uint8_t bus, uint8_t device, uint8_t function);
  uint16_t ReadDeviceId(uint8_t bus, uint8_t device, uint8_t function);
  uint8_t ReadHeaderType(uint8_t bus, uint8_t device, uint8_t function);
  ClassCode ReadClassCode(uint8_t bus, uint8_t device, uint8_t function);

  inline uint16_t ReadVendorId(const Device& dev) {
    return ReadVendorId(dev.bus, dev.device, dev.function);
  }

  uint32_t ReadConfReg(const Device& dev, uint8_t reg_addr);

  void WriteConfReg(const Device& dev, uint8_t reg_addr, uint32_t value);

  uint32_t ReadBusNumbers(uint8_t bus, uint8_t device, uint8_t function);

  bool IsSingleFunctionDevice(uint8_t header_type);

  inline std::array<Device, 32> devices;
  inline int num_device;

  Error ScanAllBus();

  constexpr uint8_t CalcBarAddress(uint bar_index) {
    return PCIR_BAR0 + (4 * bar_index);
  }

  WithError<uint64_t> ReadBar(Device& device, uint bar_index);

  /* PCI Capability Register の共通ヘッダ */
  union CapabilityHeader {
    uint32_t data;
    struct {
      uint32_t cap_id   : 8;
      uint32_t next_ptr : 8;
      uint32_t cap      : 16;
    } __packed bits;
  } __packed;

  const uint8_t kCapabilityMSI  = 0x05;
  const uint8_t kCapabilityMSIX = 0x11;

  CapabilityHeader ReadCapabilityHeader(const Device& dev, uint8_t addr);

  /* MSI Cabability 構造 */
  struct MSICapability {
    union {
      uint32_t data;
      struct {
        uint32_t cap_id                  : 8;  //08
        uint32_t next_ptr                : 8;  //16
        uint32_t msi_enable              : 1;  //17
        uint32_t multi_msg_capable       : 3;  //20
        uint32_t multi_msg_enable        : 3;  //23
        uint32_t addr_64_capable         : 1;  //24
        uint32_t per_vector_mask_capable : 1;  //25
        uint32_t                         : 7;  //32
      } __packed bits;
    } __packed header;

    uint32_t msg_addr;        //
    uint32_t msg_upper_addr;  //
    uint32_t msg_data;        //
    uint32_t mask_bits;       //
    uint32_t pending_bits;    //
  } __packed;

  Error ConfigureMSI(const Device& dev, uint32_t msg_addr, uint32_t msg_data,
                     uint num_vector_component);

  enum class MSITriggerMode {
    kEdge  = 0,  //0
    kLevel = 1   //1
  };

  enum class MSIDeliveryMode {
    kFixed          = 0b000,
    kLowestPriority = 0b001,
    kSMI            = 0b010,
    kNMI            = 0b100,
    kINIT           = 0b101,
    kExtINT         = 0b111,
  };

  Error ConfigureMSIFixedDestination(const Device& dev, uint8_t apic_id,
                                     MSITriggerMode trigger_mode,
                                     MSIDeliveryMode delivery_mode,
                                     uint8_t vector, uint num_vector_exponent);
}  // namespace pci

void pci_scan();
