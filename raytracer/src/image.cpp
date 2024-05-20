/**
 * \file image.cpp
 **/
#include "image.hpp"

#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

RtImage::RtImage(const std::string& file_name) {
  if (!Load(file_name)) {
    std::cerr << "ERROR : Failed to load image file : " << file_name << "\n";
  }
}

RtImage::~RtImage() {
  delete[] bdata;
  STBI_FREE(fdata);
}

bool RtImage::Load(const std::string& file_name) {
  auto n = bytes_per_pixel;
  fdata = stbi_loadf(file_name.c_str() , &img_width , &img_height , &n , bytes_per_pixel);
  if (fdata == nullptr) {
    return false;
  }

  bytes_per_scanline = img_width * bytes_per_pixel;
  ConvertToBytes();
  return true;
}

int32_t RtImage::Width() const {
  return (fdata == nullptr) ?
    0 : img_width;
}

int32_t RtImage::Height() const {
  return (fdata == nullptr) ?
    0 : img_height;
}

const uint8_t* RtImage::PixelData(int32_t x , int32_t y) const {
  static uint8_t magenta[] = { 255 , 0 , 255 };

  if (bdata == nullptr) {
    return magenta;
  }

  x = Clamp(x , 0 , img_width);
  y = Clamp(y , 0 , img_height);

  return bdata + y * bytes_per_scanline + x * bytes_per_pixel;
}

int32_t RtImage::Clamp(int32_t x , int32_t low , int32_t high) {
  if (x < low) return low;
  if (x < high) return x;
  return high - 1;
}

uint8_t RtImage::FloatToByte(float value) {
  if (value <= 0.0) {
    return 0;
  }

  if (1.0 <= value) {
    return 255;
  }

  return static_cast<uint8_t>(256.0 * value);
}

void RtImage::ConvertToBytes() {
  int32_t total_bytes = img_width * img_height * bytes_per_pixel;
  bdata = new uint8_t[total_bytes];

  auto* bptr = bdata;
  auto* fptr = fdata;

  for (auto i = 0; i < total_bytes; ++i , ++fptr , ++bptr) {
    *bptr = FloatToByte(*fptr);
  }
}
