#include <mouse.hpp>

namespace cursor {
  const int kMouseCursorWidth = 15;
  const int kMouseCursorHeight = 24;
  const char mouse_cursor_shape[kMouseCursorHeight][kMouseCursorWidth + 1]{
      "@              ",  //
      "@@             ",  //
      "@.@            ",  //
      "@..@           ",  //
      "@...@          ",  //
      "@....@         ",  //
      "@.....@        ",  //
      "@......@       ",  //
      "@.......@      ",  //
      "@........@     ",  //
      "@.........@    ",  //
      "@..........@   ",  //
      "@...........@  ",  //
      "@............@ ",  //
      "@......@@@@@@@@",  //
      "@......@       ",  //
      "@....@@.@      ",  //
      "@...@ @.@      ",  //
      "@..@   @.@     ",  //
      "@.@    @.@     ",  //
      "@@      @.@    ",  //
      "@       @.@    ",  //
      "         @.@   ",  //
      "          @@   ",  //
  };

  void DrawMouseCursor(PixelWriter* pixel_writer, Vector2D<int> position) {
    for (int dy = 0; dy < kMouseCursorHeight; ++dy) {
      for (int dx = 0; dx < kMouseCursorWidth; ++dx) {
        if (mouse_cursor_shape[dy][dx] == '@') {
          pixel_writer->Write(position.x + dx, position.y + dy,
                              {0xFF, 0xFF, 0xFF});
        } else if (mouse_cursor_shape[dy][dx] == '.') {
          pixel_writer->Write(position.x + dx, position.y + dy,
                              {0x00, 0x00, 0x00});
        }
      }
    }
  }

  void EraseMouseCursor(PixelWriter* pixel_writer, Vector2D<int> position,
                        PixelColor erase_color) {
    for (int dy = 0; dy < kMouseCursorHeight; ++dy) {
      for (int dx = 0; dx < kMouseCursorWidth; ++dx) {
        if (mouse_cursor_shape[dy][dx] != ' ') {
          pixel_writer->Write(position.x + dx, position.y + dy, erase_color);
        }
      }
    }
  }
}  // namespace cursor

/* MouseCursor コンストラクタ Overload  */
MouseCursor::MouseCursor(PixelWriter* writer, PixelColor erase_color,
                         Vector2D<int> initial_position)
    : pixel_writer_{writer},
      erase_color_{erase_color},
      position_{initial_position} {
  cursor::DrawMouseCursor(pixel_writer_, position_);
}

void MouseCursor::MoveRelative(Vector2D<int> displacement) {
  cursor::EraseMouseCursor(pixel_writer_, position_, erase_color_);
  position_ += displacement;
  cursor::DrawMouseCursor(pixel_writer_, position_);
}

char mouse_cursor_buf[sizeof(MouseCursor)];
MouseCursor* mouse_cursor;

void mouse_cursor_init() {
  mouse_cursor = new (mouse_cursor_buf)
      MouseCursor{pixel_writer, {0xFF, 0xFF, 0xFF}, {500, 400}};
}