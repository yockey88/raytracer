/**
 * \file texture.cpp
 **/
#include "texture.hpp"

#include "interval.hpp"

Color SolidColor::Value(double u , double v , const Point3& p) const {
  return albedo;
}

CheckerTexture::CheckerTexture(double scale , const Color& c1 , const Color& c2) 
    : inv_scale (1.0 / scale) {
  even = NewRef<SolidColor>(c1);
  odd = NewRef<SolidColor>(c2);
}

Color CheckerTexture::Value(double u , double v , const Point3& p) const {
  auto x_int = int32_t(glm::floor(inv_scale * p.x));
  auto y_int = int32_t(glm::floor(inv_scale * p.y));
  auto z_int = int32_t(glm::floor(inv_scale * p.z));

  bool is_even = (x_int + y_int + z_int) % 2 == 0;

  return is_even ? 
    even->Value(u , v , p) : odd->Value(u , v , p);
}

Color ImageTexture::Value(double u , double v , const Point3& p) const {
  if (image.Height() <= 0) {
    return Color(0 , 1 , 1);
  }

  u = Interval(0 , 1).Clamp(u);
  v = 1.0 - Interval(0 , 1).Clamp(v);

  auto i = int32_t(u * image.Width());
  auto j = int32_t(v * image.Height());
  auto pix = image.PixelData(i , j);

  auto color_scale = 1.0 / 255.0;
  return Color(color_scale * pix[0] , color_scale * pix[1] , color_scale * pix[2]);
}

Color NoiseTexture::Value(double u , double v , const Point3& p) const {
  return Color(1.0 , 1.0 , 1.0) * 0.5 * (1.0 + noise.Noise(scale * p));  
}
