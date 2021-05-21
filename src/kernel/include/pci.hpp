/*
PCIバス制御プログラム
*/

#pragma once

#include <array>
#include <cstdint>

#include <error.hpp>
#include <printk.hpp>

#include <asmfunc.h>

/* ---------------------------------------------------------------------------
CONFIG_ADDRESSレジスタ:
| 31|30  -   24|23  -  16|15    -   11|10     -     8|7     -      2|  1| 0 |
|   | Reserved | Bus No. | Device No. | Fucntion No. | Register No. | 0 | 0 |
  ^
  | Enable bit (1 = enabled, 0 = disabled)
                                   ref) PCI Local Bus Specification Rev.3.0 P.50
  -----------------------------------------------------------------------------*/

/* PCI Configuration spaces offset */
#define PCI_VENDOR_ADN_DEVICE_ID 0x00  //32bits
#define _PCI_VENDOR_ID 0x00            //16bits, Vendor ID
#define _PCI_DEVICE_ID 0x02            //16bits, Device ID
#define PCI_COMMAND_AND_STATUS 0x04    //32bits
#define _PCI_COMMAND 0x04              //16bits, PCI Command
#define _PCI_STATUS 0x06               //16bits, PCI Status
#define PCI_CLASSCODE 0x08             //32bits
#define _PCI_REVISION_ID 0x08          //8bits, Revision ID
#define _PCI_CLASS_INTERFACE 0x09      //8bits, Programming Interfaces
#define _PCI_SUB_CLASS 0x0A            //8bits, Device sub class
#define _PCI_BASE_CLASS 0x0B           //8bits, Device base class
#define PCI_HEADER_TYPE 0x0C           //32bits
#define _PCI_CACHE_LINE_SIZE 0x0C      //8bits, Cache Line Size
#define _PCI_LATENCY_TIMER 0x0D        //8bits, Latency Timer
#define _PCI_HEADER_TYPE 0x0E          //8bits
#define _PCI_BIST 0x0F                 //8bits

/* PCI Header Type */
#define PCI_HEADER_TYPE_MASK 0x7f  //PCI header type mask
#define PCI_HEADER_TYPE_NORMAL 0   //PCI header type 0
#define PCI_HEADER_TYPE_BRIDGE 1   //PCI header type 1 bridge
#define PCI_HEADER_TYPE_CARDBUS 2  //PCI header type 2 cardbus

/* Header type 1 (PCI-to-PCI bridges) */
#define PCI_BUS_NO 0x18          //Primary and Secondary bus number
#define _PCI_PRIMARY_BUS 0x18    // Primary bus number
#define _PCI_SECONDARY_BUS 0x19  // Secondary bus number

namespace pci {
  /*------------------------------------------------
  PC/ATでは，
   - CF8h : CONFIG_BASEADDRESS
   - CFCh : CONFIG_DATA
   refer to PCI Local Bus Specification Rev.3.0 P50
  -------------------------------------------------*/
  const uint16_t kConfigAddress = 0x0CF8;
  const uint16_t kConfigData = 0x0CFC;

  void WritePciConfigAddress(uint32_t address);
  void WritePciConfigData(uint32_t value);
  uint32_t ReadPciConfigData();

  uint16_t ReadVendorId(uint8_t bus, uint8_t device, uint8_t function);
  uint16_t ReadDeviceId(uint8_t bus, uint8_t device, uint8_t function);
  uint8_t ReadHeaderType(uint8_t bus, uint8_t device, uint8_t function);
  uint32_t ReadClassCode(uint8_t bus, uint8_t device, uint8_t function);
  uint32_t ReadBusNumbers(uint8_t bus, uint8_t device, uint8_t function);

  bool IsSingleFunctionDevice(uint8_t header_type);

  struct Device {
    uint8_t bus, device, function, header_type;
  };

  inline std::array<Device, 32> devices;
  inline int num_device;

  Error ScanAllBus();

}  // namespace pci

void pci_init();