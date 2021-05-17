#pragma once

#include <console.hpp>
#include <frame_buffer_config.h>
#include <graphics.hpp>

extern char pixel_writer_buf[sizeof(RGBResv8BitPerColorPixelWriter)];
extern PixelWriter* pixel_writer;

extern char console_buf[sizeof(Console)];
extern Console* console;