#pragma once

#include <error.hpp>
#include <interrupt.hpp>
#include <logger.hpp>
#include <mouse.hpp>
#include <pci.hpp>
#include <layer.hpp>

#include <asmfunc.h>
#include <cpufunc.hpp>
#include <queue.hpp>

#include <usb/device.hpp>
#include <usb/memory.hpp>
#include <usb/xhci/trb.hpp>
#include <usb/xhci/xhci.hpp>

#include <usb/classdriver/mouse.hpp>

void SwitchEhci2Xhci(const pci::Device& xhc_dev);
void xhc_init();

void MouseObserver(int8_t displacement_x, int8_t displacement_y);

extern "C" void __cxa_pure_virtual();