/**
 * \file image.hpp
 **/
#ifndef IMAGE_HPP
#define IMAGE_HPP

#include <string>

class RtImage {
  public:
    RtImage() {}

    RtImage(const std::string& file_name);

    ~RtImage();

    bool Load(const std::string& file_name);

    int32_t Width() const;
    int32_t Height() const;

    const uint8_t* PixelData(int32_t x , int32_t y) const;

  private:
    const int32_t bytes_per_pixel = 3;
    float* fdata = nullptr;
    uint8_t* bdata = nullptr;
    int32_t img_width = 0;
    int32_t img_height = 0;
    int32_t bytes_per_scanline = 0;

    static int32_t Clamp(int32_t x , int32_t low , int32_t high);
    static uint8_t FloatToByte(float value);

    void ConvertToBytes();
};

#endif // !IMAGE_HPP
