#pragma once

#include <frame_buffer_config.h>
#include <graphics.hpp>
#include <operator.hpp>

extern char pixel_writer_buf[sizeof(RGBResv8BitPerColorPixelWriter)];
extern PixelWriter* pixel_writer;

void framebuffer_init(const FrameBufferConfig& config, const PixelColor& color);