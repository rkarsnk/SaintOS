#include <interrupt.hpp>

std::array<IntrDesc, 256> idt;

void SetIDTEntry(IntrDesc& desc, IntrDescAttr attr, uint64_t offset,
                 uint16_t segment_selector) {
  desc.attr             = attr;
  desc.offset_low       = offset & 0xffffu;
  desc.offset_middle    = (offset >> 16) & 0xffffu;
  desc.offset_high      = offset >> 32;
  desc.segment_selector = segment_selector;
}

void NotifyEndOfInterrupt() {
  volatile auto end_of_interrupt = reinterpret_cast<uint32_t*>(EOI_REG_ADDR);
  //EOIレジスタに0を書き込み、割込み処理の完了をCPUに通知する
  *end_of_interrupt = 0;
}