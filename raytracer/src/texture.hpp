/**
 * \file texture.hpp
 **/
#ifndef TEXTURE_HPP
#define TEXTURE_HPP

#include "defines.hpp"
#include "image.hpp"
#include "perlin.hpp"

class Texture {
  public:
    virtual ~Texture() {}

    virtual Color Value(double u , double v , const Point3& p) const = 0;
};

class SolidColor : public Texture {
  public:
    SolidColor(const Color& albedo)
      : albedo(albedo) {}
  
    SolidColor(double red , double green , double blue)
      : SolidColor(Color(red , green , blue)) {}

    virtual ~SolidColor() override {}

    virtual Color Value(double u , double v , const Point3& p) const override;

  private:
    Color albedo;
};

class CheckerTexture : public Texture {
  public:
    CheckerTexture(double scale , Ref<Texture> even , Ref<Texture> odd)
      : inv_scale(1.0 / scale) , even(even) , odd(odd) {}

    CheckerTexture(double scale , const Color& c1 , const Color& c2);

    virtual ~CheckerTexture() override {}

    virtual Color Value(double u , double v , const Point3& p) const override;

  private:
    double inv_scale;
    Ref<Texture> even;
    Ref<Texture> odd;
};

class ImageTexture : public Texture {
  public:
    ImageTexture(const std::string& file)
      : image(file) {}

    virtual ~ImageTexture() override {}

    virtual Color Value(double u , double v , const Point3& p) const override;

  private:
    RtImage image;
};

class NoiseTexture : public Texture {
  public:
    NoiseTexture() {}

    NoiseTexture(double scale)
      : scale(scale) {}

    virtual ~NoiseTexture() override {}

    virtual Color Value(double u , double v , const Point3& p) const override;

  private:
    Perlin noise;

    double scale;
};

#endif // !TEXTURE_HPP
