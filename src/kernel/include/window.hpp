#pragma once

#include <optional>
#include <vector>

//独自ヘッダ
#include <graphics.hpp>

class Window {
 public:
  /**
   * WindowWriterクラス 
   * Windowクラス専用の書き込み機能
   * 継承元：PixelWriterクラス
   * */
  class WindowWriter : public PixelWriter {
   public:
    WindowWriter(Window& window) : window_{ window } {}
    virtual void Write(int x, int y, const PixelColor& c) override {
      window_.At(x, y) = c;
    }
    virtual int Width() const override {
      return window_.Width();
    }
    virtual int Height() const override {
      return window_.Height();
    }

   private:
    Window& window_;
  };

  Window(int width, int height);
  ~Window()                 = default;
  Window(const Window& rhs) = delete;
  Window& operator=(const Window& rhs) = delete;

  void DrawTo(PixelWriter& writer, Vector2D<int> position);
  void SetTransparentColor(std::optional<PixelColor> c);
  WindowWriter* Writer();

  PixelColor& At(int x, int y);
  const PixelColor& At(int x, int y) const;

  int Width() const;
  int Height() const;

 private:
  int width_, height_;                           //幅、高さ
  std::vector<std::vector<PixelColor>> data_{};  //ピクセル配列
  WindowWriter writer_{ *this };                 //ピクセルへの書き込み機能
  /* transparent_color_ : 透過色
  (初期値は値をもたない = どの色も透過しない) */
  std::optional<PixelColor> transparent_color_{ std::nullopt };
};