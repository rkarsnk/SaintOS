/**
 * @file graphics.cpp
 *
 * 画像描画関連のプログラムを集めたファイル．
 */
#include <graphics.hpp>

void RGBResv8BitPerColorPixelWriter::Write(int x, int y,
                                           const PixelColor& color) {
  auto p = PixelAt(x, y);
  p[0] = color.r;
  p[1] = color.g;
  p[2] = color.b;
};

void BGRResv8BitPerColorPixelWriter::Write(int x, int y,
                                           const PixelColor& color) {
  auto p = PixelAt(x, y);
  p[0] = color.b;
  p[1] = color.g;
  p[2] = color.r;
};

void DrawRectangle(PixelWriter& writer, const Vector2D<int>& pos,
                   const Vector2D<int>& size, const PixelColor& color) {
  for (int dx = 0; dx < size.x; ++dx) {
    writer.Write(pos.x + dx, pos.y, color);               //長方形上横線
    writer.Write(pos.x + dx, pos.y + size.y - 1, color);  //長方形下横線
  }
  for (int dy = 0; dy < size.y; ++dy) {
    writer.Write(pos.x, pos.y + dy, color);               //長方形左横線
    writer.Write(pos.x + size.x - 1, pos.y + dy, color);  //長方形右横線
  }
}

void FillRectangle(PixelWriter& writer, const Vector2D<int>& pos,
                   const Vector2D<int>& size, const PixelColor& color) {
  for (int dy = 0; dy < size.y; ++dy) {
    for (int dx = 0; dx < size.x; ++dx) {
      writer.Write(pos.x + dx, pos.y + dy, color);
    }
  }
}

void FillCircle(PixelWriter& writer, const Vector2D<int>& center, int radius,
                const PixelColor& color) {
  int dx;
  int dy;

  for (int cy = 0; cy < center.y + (radius * 2); ++cy) {
    for (int cx = 0; cx < center.x + (radius * 2); ++cx) {
      dx = cx - center.x;
      dy = cy - center.y;

      if ((dx * dx) + (dy * dy) < radius * radius) {
        writer.Write(center.x + dx, center.y + dy, color);
      }
    }
  }
}

void DrawCircle(PixelWriter& writer, const Vector2D<int>& center, int radius,
                const PixelColor& color) {
  int dx;
  int dy;

  for (int cy = 0; cy < center.y + (radius * 2); ++cy) {
    for (int cx = 0; cx < center.x + (radius * 2); ++cx) {
      dx = cx - center.x;
      dy = cy - center.y;

      if ((radius - 2) * (radius - 2) < (dx * dx) + (dy * dy) &&
          (dx * dx) + (dy * dy) < radius * radius) {
        writer.Write(center.x + dx, center.y + dy, color);
      }
    }
  }
}

void DrawFillEllpse(PixelWriter& writer, const Vector2D<int>& pos, int rad_x,
                    int rad_y, const PixelColor& color) {
}

/*----------------------------------------------------------------------
  const int kFrameWidth = frame_buffer_config.horizontal_resolution;
  const int kFrameHeight = frame_buffer_config.vertical_resolution;

  //四角形を出力
  DrawRectangle(*pixel_writer, {0, 21}, {kFrameWidth, 1}, {0x00, 0x00,
  0x00});
  //塗りつぶし四角形を出力
  FillRectangle(*pixel_writer, {0, 0}, {kFrameWidth, 20}, {0xBB, 0xBB,
  0xBB});

  //円を出力
  DrawCircle(*pixel_writer, {200, 300}, 80, {0xFF, 0x00, 0x00});
  //塗りつぶし円を出力
  FillCircle(*pixel_writer, {600, 300}, 80, {0x00, 0x00, 0xFF});
  ----------------------------------------------------------------------*/