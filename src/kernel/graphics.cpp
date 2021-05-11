/**
 * @file graphics.cpp
 *
 * 画像描画関連のプログラムを集めたファイル．
 */
#include <graphics.hpp>

void RGBResv8BitPerColorPixelWriter::Write(int x , int y, const PixelColor& color){
    auto p = PixelAt(x, y);
    p[0] = color.r;
    p[1] = color.g;
    p[2] = color.b;
};

void BGRResv8BitPerColorPixelWriter::Write(int x , int y, const PixelColor& color){
    auto p = PixelAt(x, y);
    p[0] = color.b;
    p[1] = color.g;
    p[2] = color.r;
};
