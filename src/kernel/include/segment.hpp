#pragma once

#include <array>
#include <cstdint>

#include <x64/descriptor.hpp>

union SegmentDescriptor {
  uint64_t data;
  struct {
    uint64_t limit_low                  : 16;
    uint64_t base_low                   : 16;
    uint64_t base_middle                : 8;
    DescriptorType type                 : 4;
    uint64_t system_segment             : 1;
    uint64_t descriptor_privilege_level : 2;
    uint64_t present                    : 1;
    uint64_t limit_high                 : 4;
    uint64_t available                  : 1;
    uint64_t long_mode                  : 1;
    uint64_t default_operation_size     : 1;
    uint64_t granularity                : 1;
    uint64_t base_high                  : 8;
  } __attribute__((packed)) bits;
} __attribute__((packed));

void SetCodeSegment(SegmentDescriptor& desc, DescriptorType type,
                    uint32_t descriptor_privilegde_level, uint32_t base,
                    uint32_t limit);

void SetDataSegment(SegmentDescriptor& desc, DescriptorType type,
                    uint32_t descriptor_privilegde_level, uint32_t base,
                    uint32_t limit);

void SetupSegments();