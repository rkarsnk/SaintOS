/**
 * SPDX-License-Identifier: BSD-3-Clause
 * 
 * FreeBSD のsys/amd64/include/cpufunc.hから抜粋
 */
#pragma once

static __inline uint8_t inb(uint16_t port) {
  uint8_t data;
  __asm __volatile("inb %w1, %0" : "=a"(data) : "Nd"(port));
  return data;
}

static __inline uint16_t inw(uint16_t port) {
  uint16_t data;
  __asm __volatile("inw %w1, %0" : "=a"(data) : "Nd"(port));
  return data;
}

static __inline uint32_t inl(uint16_t port) {
  uint32_t data;
  __asm __volatile("in %w1, %0" : "=a"(data) : "Nd"(port));
  return data;
}

static __inline void outb(uint16_t port, uint8_t data) {
  __asm __volatile("outb %0, %w1" : : "a"(data), "Nd"(port));
}

static __inline void outw(uint16_t port, uint16_t data) {
  __asm __volatile("outw %0, %w1" : : "a"(data), "Nd"(port));
}

static __inline void outl(uint16_t port, uint32_t data) {
  __asm __volatile("outl %0, %w1" : : "a"(data), "Nd"(port));
}

static __inline void halt() {
  while (1)
    __asm__("hlt");
}