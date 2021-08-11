#pragma once

#include <frame_buffer_config.h>

// PixelColor 構造体
struct PixelColor {
  uint8_t r, g, b;
};

inline bool operator==(const PixelColor& lhs, const PixelColor& rhs) {
  return lhs.r == rhs.r && lhs.g == rhs.g && lhs.b == rhs.b;
}

inline bool operator!=(const PixelColor& lhs, const PixelColor& rhs) {
  return !(lhs == rhs);
}

/**
 * PixelWriterクラス
 */
class PixelWriter {
 public:
  virtual ~PixelWriter()                                    = default;
  virtual void Write(int x, int y, const PixelColor& color) = 0;
  virtual int Width() const                                 = 0;
  virtual int Height() const                                = 0;
};

class FrameBufferWriter : public PixelWriter {
 public:
  FrameBufferWriter(const FrameBufferConfig& config) : config_{ config } {}
  virtual ~FrameBufferWriter() = default;
  virtual int Width() const override {
    return config_.horizontal_resolution;
  }
  virtual int Height() const override {
    return config_.vertical_resolution;
  }

 protected:
  /**
   * PixelAt
   * 座標(x, y)から操作するフレームバッファのPixelのメモリアドレスを取得する
   */
  uint8_t* PixelAt(int x, int y) {
    return config_.frame_buffer + 4 * (config_.pixels_per_scan_line * y + x);
  }

 private:
  const FrameBufferConfig& config_;
};

class RGBResv8BitPerColorPixelWriter : public FrameBufferWriter {
 public:
  using FrameBufferWriter::FrameBufferWriter;
  virtual void Write(int x, int y, const PixelColor& color) override;
};

class BGRResv8BitPerColorPixelWriter : public FrameBufferWriter {
 public:
  using FrameBufferWriter::FrameBufferWriter;
  virtual void Write(int x, int y, const PixelColor& color) override;
};

template <typename T>
struct Vector2D {
  T x, y;

  template <typename U>
  Vector2D<T>& operator+=(const Vector2D<U>& rhs) {
    x += rhs.x;
    y += rhs.y;
    return *this;
  }
};

void DrawRectangle(PixelWriter& writer, const Vector2D<int>& pos,
                   const Vector2D<int>& size, const PixelColor& color);
void FillRectangle(PixelWriter& writer, const Vector2D<int>& pos,
                   const Vector2D<int>& size, const PixelColor& color);

void DrawCircle(PixelWriter& writer, const Vector2D<int>& center, int radius,
                const PixelColor& color);
void FillCircle(PixelWriter& writer, const Vector2D<int>& center, int radius,
                const PixelColor& color);

void DrawFillEllpse(PixelWriter& writer, const Vector2D<int>& center, int rad_x,
                    int rad_y, const PixelColor& color);

const PixelColor kDesktopBGColor{45, 118, 237};
const PixelColor kDesktopFGColor{255, 255, 255};

void DrawDesktop(PixelWriter& writer);