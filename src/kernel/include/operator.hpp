#pragma once

#include <cstddef>
#include <cstdint>
#include <cstdio>

void* operator new(size_t size, void* buf);
void operator delete(void* obj) noexcept;