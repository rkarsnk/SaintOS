#include <cstdint>

extern "C" void KernelMain(uint64_t frame_buffer_base, uint64_t frame_buffer_size){
    uint8_t* frame_buffer = reinterpret_cast<uint8_t*>(frame_buffer_base);
    // フレームバッファを白にする
    for(uint64_t i=0; i < frame_buffer_size; ++i){
        // frame_buffer[i] = i % 256;
        frame_buffer[i] = 255;
    }
    while (1)
        __asm__("hlt");
}