/* ---------------------------------------------------------------------------
CONFIG_ADDRESSレジスタ:
| 31|30  -   24|23  -  16|15    -   11|10     -     8|7     -      2|  1| 0 |
|   | Reserved | Bus No. | Device No. | Fucntion No. | Register No. | 0 | 0 |
  ^
  | Enable bit (1 = enabled, 0 = disabled)
                                   ref) PCI Local Bus Specification Rev.3.0 P.50
  -----------------------------------------------------------------------------*/

/*---------------------------------------------------
  PC/ATでは，
   - CF8h : CONFIG_BASEADDRESS
   - CFCh : CONFIG_DATA
       ref) PCI Local Bus Specification Rev.3.0 P50
---------------------------------------------------*/

#define PCI_CONF_PORT 0x0cf8
#define PCI_DATA_PORT 0x0cfc

/* PCI Configuration spaces offset */
#define PCIR_VENDOR_AND_DEVICE_ID 0x00  //32bits
#define PCIR_VENDOR_ID            0x00  //16bits, Vendor ID
#define PCIR_DEVICE_ID            0x02  //16bits, Device ID
#define PCIR_COMMAND_AND_STATUS   0x04  //32bits
#define PCIR_COMMAND              0x04  //16bits, PCI Command
#define PCIR_STATUS               0x06  //16bits, PCI Status
#define PCIR_CLASSCODE            0x08  //32bits
#define PCIR_REVISION_ID          0x08  //8bits, Revision ID
#define PCIR_CLASS_INTERFACE      0x09  //8bits, Programming Interfaces
#define PCIR_SUB_CLASS            0x0A  //8bits, Device sub class
#define PCIR_BASE_CLASS           0x0B  //8bits, Device base class
#define PCIR_CACHE_LINE_SIZE      0x0C  //8bits, Cache Line Size
#define PCIR_LATENCY_TIMER        0x0D  //8bits, Latency Timer
#define PCIR_HEADER_TYPE          0x0E  //8bits
#define PCIR_BIST                 0x0F  //8bits
#define PCIR_BAR0                 0x10  //32bits, Base Address Register
#define PCIR_BAR1                 0x14  //32bits
#define PCIR_BAR2                 0x18  //32bits
#define PCIR_BAR3                 0x1C  //32bits
#define PCIR_BAR4                 0x20  //32bits
#define PCIR_BAR5                 0x24  //32bits
#define PCIR_CAP_PTR              0x34  //8bits

/* PCI Header Type */
#define PCI_HEADER_TYPE_MASK    0x7f  //PCI header type mask
#define PCI_HEADER_TYPE_NORMAL  0     //PCI header type 0
#define PCI_HEADER_TYPE_BRIDGE  1     //PCI header type 1 bridge
#define PCI_HEADER_TYPE_CARDBUS 2     //PCI header type 2 cardbus

/* Header type 1 (PCI-to-PCI bridges) */
#define PCI_BUS_NO           0x18  //Primary and Secondary bus number
#define _PCI_PRIMARY_BUS     0x18  // Primary bus number
#define _PCI_SECONDARY_BUS   0x19  // Secondary bus number
#define _PCI_SUBORDINATE_BUS 0x1a  // Highest bus number behind the bridge

/* Vendor ID Definition */
#define PCI_VENDOR_INTEL 0x8086
#define PCI_VENDOR_NEC   0x1033

/* Device ID Definition */
#define PCI_PRODUCT_NEC_PD720200 0x0194