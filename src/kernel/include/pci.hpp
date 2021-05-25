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

/* ---------------------------------------------------------------------------
CONFIG_ADDRESSレジスタ:
| 31|30  -   24|23  -  16|15    -   11|10     -     8|7     -      2|  1| 0 |
|   | Reserved | Bus No. | Device No. | Fucntion No. | Register No. | 0 | 0 |
  ^
  | Enable bit (1 = enabled, 0 = disabled)
                                   ref) PCI Local Bus Specification Rev.3.0 P.50
  -----------------------------------------------------------------------------*/

/* PCI Configuration spaces offset */
#define PCIR_VENDOR_AND_DEVICE_ID 0x00  //32bits
#define PCIR_VENDOR_ID 0x00             //16bits, Vendor ID
#define PCIR_DEVICE_ID 0x02             //16bits, Device ID
#define PCIR_COMMAND_AND_STATUS 0x04    //32bits
#define PCIR_COMMAND 0x04               //16bits, PCI Command
#define PCIR_STATUS 0x06                //16bits, PCI Status
#define PCIR_CLASSCODE 0x08             //32bits
#define PCIR_REVISION_ID 0x08           //8bits, Revision ID
#define PCIR_CLASS_INTERFACE 0x09       //8bits, Programming Interfaces
#define PCIR_SUB_CLASS 0x0A             //8bits, Device sub class
#define PCIR_BASE_CLASS 0x0B            //8bits, Device base class
#define PCIR_CACHE_LINE_SIZE 0x0C       //8bits, Cache Line Size
#define PCIR_LATENCY_TIMER 0x0D         //8bits, Latency Timer
#define PCIR_HEADER_TYPE 0x0E           //8bits
#define PCIR_BIST 0x0F                  //8bits
#define PCIR_BAR0 0x10                  //32bits, Base Address Register
#define PCIR_BAR1 0x14                  //32bits
#define PCIR_BAR2 0x18                  //32bits
#define PCIR_BAR3 0x1C                  //32bits
#define PCIR_BAR4 0x20                  //32bits
#define PCIR_BAR5 0x24                  //32bits

/* PCI Header Type */
#define PCI_HEADER_TYPE_MASK 0x7f  //PCI header type mask
#define PCI_HEADER_TYPE_NORMAL 0   //PCI header type 0
#define PCI_HEADER_TYPE_BRIDGE 1   //PCI header type 1 bridge
#define PCI_HEADER_TYPE_CARDBUS 2  //PCI header type 2 cardbus

/* Header type 1 (PCI-to-PCI bridges) */
#define PCI_BUS_NO 0x18            //Primary and Secondary bus number
#define _PCI_PRIMARY_BUS 0x18      // Primary bus number
#define _PCI_SECONDARY_BUS 0x19    // Secondary bus number
#define _PCI_SUBORDINATE_BUS 0x1a  // Highest bus number behind the bridge

#define PCI_CONF_PORT 0x0cf8
#define PCI_DATA_PORT 0x0cfc

namespace pci {
  /*------------------------------------------------
  PC/ATでは，
   - CF8h : CONFIG_BASEADDRESS
   - CFCh : CONFIG_DATA
   refer to PCI Local Bus Specification Rev.3.0 P50
  -------------------------------------------------*/
  const uint16_t kConfigAddress = 0x0cf8;
  const uint16_t kConfigData = 0x0cfc;

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

  constexpr uint8_t CalcBarAddress(unsigned int bar_index) {
    return PCIR_BAR0 + (4 * bar_index);
  }

  WithError<uint64_t> ReadBar(Device& device, unsigned int bar_index);
}  // namespace pci

void pci_init();
