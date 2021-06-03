#pragma once

#include <stdint.h>

extern "C" {
  uint16_t GetCS(void);
  void LoadIDT(uint16_t limit, uint64_t offset);
}