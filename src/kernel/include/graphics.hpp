#pragma once

#include <frame_buffer_config.h>

// PixelColor 構造体
struct PixelColor {
  uint8_t r, g, b;
};

/**
 * PixelWriterクラス
 *
 *
 */
class PixelWriter {
 private:
  /**
   * config_
   * フレームバッファ構成情報を受け取るメンバ変数
   */
  const FrameBufferConfig& config_;

 public:
  /**
   * PixelWrite()
   * コンストラクタ（インスタンスをメモリ上に構築する）
   * 引数で受け取ったフレームバッファ構成情報をメンバ変数のconfig_にコピーする
   */
  PixelWriter(const FrameBufferConfig& config) : config_{config} {
  }
  /**
   * ~PixelWrite()
   * デストラクタ （インスタンスをメモリ上から破棄する）
   */
  virtual ~PixelWriter() = default;
  /**
   * Write
   * x, y で指定されたPixel座標の色をcolorで指定された色にする
   */
  virtual void Write(int x, int y, const PixelColor& color) = 0;

 protected:
  /**
   * PixelAt
   * 座標(x, y)から操作するフレームバッファのPixelのメモリアドレスを取得する
   */
  uint8_t* PixelAt(int x, int y) {
    if ((config_.pixels_per_scan_line * y + x) >= 0) {
      return config_.frame_buffer + 4 * (config_.pixels_per_scan_line * y + x);
    } else {
      return config_.frame_buffer;
    }
  }
};

class RGBResv8BitPerColorPixelWriter : public PixelWriter {
 public:
  using PixelWriter::PixelWriter;
  virtual void Write(int x, int y, const PixelColor& color) override;
};

class BGRResv8BitPerColorPixelWriter : public PixelWriter {
 public:
  using PixelWriter::PixelWriter;
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