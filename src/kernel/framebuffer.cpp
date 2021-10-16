#include <framebuffer.hpp>

/*
PixelWriter framebuffer_init(const FrameBufferConfig& config, const PixelColor& color) {
  switch (config.pixel_format) {
    case kPixelRGBResv8BitPerColor:
      pixel_writer = new (pixel_writer_buf) RGBResv8BitPerColorPixelWriter{ config };
      break;
    case kPixelBGRResv8BitPerColor:
      pixel_writer = new (pixel_writer_buf) BGRResv8BitPerColorPixelWriter{ config };
      break;
  }
  return pixel_writer;
}
*/