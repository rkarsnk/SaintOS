#pragma once

//
#include <frame_buffer_config.h>
#include <global.hpp>
#include <graphics.hpp>
#include <operator.hpp>

void framebuffer_init(const FrameBufferConfig& config,
                      const PixelColor& color);