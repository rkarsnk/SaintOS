#pragma once

#include <array>
#include <cstdint>

#define EOI_REG_ADDR 0xfee000b0  //End-of-Interruptレジスタの番地

enum class DescType {
  kUpper8Bytes   = 0,
  kLDT           = 2,
  kTSSAvailable  = 9,
  kTSSBusy       = 11,
  kCallGate      = 12,
  kInterruptGate = 14,
  kTrapGate      = 15,
};

/* IDT定義 */
union IntrDescAttr {
  uint16_t data;
  struct {
    uint16_t interrupt_stack_table      : 3;  //03
    uint16_t                            : 5;  //08
    DescType type                       : 4;  //12
    uint16_t                            : 1;  //13
    uint16_t descriptor_privilege_level : 2;  //15
    uint16_t present                    : 1;  //16
  } __packed bits;
} __packed;

struct IntrDesc {
  uint16_t offset_low;
  uint16_t segment_selector;
  IntrDescAttr attr;
  uint16_t offset_middle;
  uint32_t offset_high;
  uint32_t reserved;
} __packed;

extern std::array<IntrDesc, 256> idt;

constexpr IntrDescAttr MakeIDTAttr(DescType type,
                                   uint8_t descriptor_privilege_level,
                                   uint8_t interrupt_stack_table = 0,
                                   bool present                  = true) {
  IntrDescAttr attr{};
  attr.bits.interrupt_stack_table      = interrupt_stack_table;
  attr.bits.type                       = type;
  attr.bits.descriptor_privilege_level = descriptor_privilege_level;
  attr.bits.present                    = present;
  return attr;
}

void SetIDTEntry(IntrDesc& desc, IntrDescAttr attr, uint64_t offset,
                 uint16_t segment_selector);

class IntrVector {
 public:
  enum Number {
    kXHCI = 0x40,
  };
};

struct IntrFrame {
  uint64_t rip;
  uint64_t cs;
  uint64_t rflags;
  uint64_t rsp;
  uint64_t ss;
};

void NotifyEndOfInterrupt();