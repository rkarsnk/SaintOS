/**
 * @file main.cpp
 *
 * カーネル本体のプログラムを書いたソースファイル. 
 */

#include <cstdint>
#include <cstddef>

#include <FrameBufferConfig.hpp>

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
        uint8_t* PixelAt(int x, int y){
            return config_.frame_buffer + 4 * (config_.pixels_per_scan_line * y + x);
        }
};

class RGBResv8BitPerColorPixelWriter : public PixelWriter {
    public:
        using PixelWriter::PixelWriter;

    virtual void Write(int x , int y, const PixelColor& color){
        auto p = PixelAt(x, y);
        p[0] = color.r;
        p[1] = color.g;
        p[2] = color.b;
    }
};

class BGRResv8BitPerColorPixelWriter : public PixelWriter {
    public:
        using PixelWriter::PixelWriter;

    virtual void Write(int x , int y, const PixelColor& color){
        auto p = PixelAt(x, y);
        p[0] = color.b;
        p[1] = color.g;
        p[2] = color.r;
    }
};


/**
 * Placement new
 * インスタンスを特定のメモリアドレスに配置する.
 */
void* operator new(size_t size, void* buf){
    return buf;
}
void operator delete(void* obj) noexcept{
}

/**
 * グローバル変数 .bssセクションに配置される
 */
char pixel_writer_buf[sizeof(RGBResv8BitPerColorPixelWriter)];
PixelWriter* pixel_writer;

extern "C" void KernelMain(const FrameBufferConfig& frame_buffer_config){
    switch (frame_buffer_config.pixel_format)
    {
    case kPixelRGBResv8BitPerColor:
        pixel_writer = new(pixel_writer_buf) RGBResv8BitPerColorPixelWriter{frame_buffer_config};
        break;
    case kPixelBGRResv8BitPerColor:
        pixel_writer = new(pixel_writer_buf) BGRResv8BitPerColorPixelWriter{frame_buffer_config};
        break;
    }

    for(int x = 0; x < frame_buffer_config.horizontal_resolution; ++x) {
        for (int y = 0; y < frame_buffer_config.vertical_resolution; ++y) {
            pixel_writer->Write(x, y, {0, 0, 0});
        }
    }
    for(int x = 0; x < 200; ++x) {
        for(int y = 0; y < 100 ; ++y ){
            pixel_writer->Write(x, y, {200,200,200});
        }
    }
    while (1) __asm__("hlt");
}