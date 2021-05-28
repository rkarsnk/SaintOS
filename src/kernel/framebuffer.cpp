#include <framebuffer.hpp>

char pixel_writer_buf[sizeof(RGBResv8BitPerColorPixelWriter)];
PixelWriter* pixel_writer;

void framebuffer_init(const FrameBufferConfig& config,
                      const PixelColor& color) {
  switch (config.pixel_format) {
    case kPixelRGBResv8BitPerColor:
      pixel_writer =
          new (pixel_writer_buf) RGBResv8BitPerColorPixelWriter{config};
      break;
    case kPixelBGRResv8BitPerColor:
      pixel_writer =
          new (pixel_writer_buf) BGRResv8BitPerColorPixelWriter{config};
      break;
  }

  for (int x = 0; x < config.horizontal_resolution; ++x) {
    for (int y = 0; y < config.vertical_resolution; ++y) {
      pixel_writer->Write(x, y, color);
    }
  }
}