#pragma once

#include <array>
#include <cstdint>

enum class DescriptorType {
  kUpper8Bytes   = 0,
  kLDT           = 2,
  kTSSAvailable  = 9,
  kTSSBusy       = 11,
  kCallGate      = 12,
  kInterruptGate = 14,
  kTrapGate      = 15,
};

/* IDT定義 */
union InterruptDescriptorAttribute {
  uint16_t data;
  struct {
    uint16_t interrupt_stack_table      : 3;  //03
    uint16_t                            : 5;  //08
    DescriptorType type                 : 4;  //12
    uint16_t                            : 1;  //13
    uint16_t descriptor_privilege_level : 2;  //15
    uint16_t present                    : 1;  //16
  } __packed bits;
} __packed;

struct InterruptDescriptor {
  uint16_t offset_low;
  uint16_t segment_selector;
  InterruptDescriptorAttribute attr;
  uint16_t offset_middle;
  uint32_t offset_high;
  uint32_t reserved;
} __packed;

extern std::array<InterruptDescriptor, 256> idt;

constexpr InterruptDescriptorAttribute MakeIDTAttr(
    DescriptorType type, uint8_t descriptor_privilege_level,
    bool present = true, uint8_t interrupt_stack_table = 0) {
  InterruptDescriptorAttribute attr{};
  attr.bits.interrupt_stack_table      = interrupt_stack_table;
  attr.bits.type                       = type;
  attr.bits.descriptor_privilege_level = descriptor_privilege_level;
  attr.bits.present                    = present;
  return attr;
}

void SetIDTEntry(InterruptDescriptor& desc, InterruptDescriptorAttribute attr,
                 uint64_t offset, uint16_t segment_selector);

class InterruptVector {
 public:
  enum Number {
    kXHCI = 0x40,
  };
};

struct InterruptFrame {
  uint64_t rip;
  uint64_t cs;
  uint64_t rflags;
  uint64_t rsp;
  uint64_t ss;
};

void NotifyEndOfInterrupt();