#pragma once

#include <array>  //arrayをインクルードすることで配置newの定義は不要に
#include <cstddef>
#include <cstdint>
#include <cstdio>

//void* operator new(size_t size, void* buf) noexcept;
void operator delete(void* obj) noexcept;