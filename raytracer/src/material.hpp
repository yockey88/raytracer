/**
 * \file material.hpp
 **/
#ifndef MATERIAL_HPP
#define MATERIAL_HPP

#include "defines.hpp"
#include "ray.hpp"
#include "texture.hpp"

class HitRecord;

class Material {
  public:
    virtual ~Material() = default;

    virtual bool Scatter(const Ray& r_in , const HitRecord& rec , Color& attentuation , Ray& scattered) const;
    virtual Color Emitted(double u , double v , const Point3& p) const;
};

class Lambertian : public Material {
  public:
    Lambertian(const Color& albedo) 
      : texture(NewRef<SolidColor>(albedo)) {}
    Lambertian(Ref<Texture> tex) 
      : texture(tex) {}

    virtual bool Scatter(const Ray& r_in , const HitRecord& rec , Color& attentuation , Ray& scattered) const override;

  private:
    Ref<Texture> texture;
};

class Metal : public Material {
  public:
    Metal(const Color& albedo , double fuzz) 
      : albedo(albedo) , fuzz(fuzz < 1 ? fuzz : 1) {}

    virtual bool Scatter(const Ray& r_in , const HitRecord& rec , Color& attentuation , Ray& scattered) const override;

  private:
    Color albedo;
    double fuzz;
};

class Dielectric : public Material {
  public:
    Dielectric(double refraction_idx)
      : refraction_idx(refraction_idx) {}

    virtual bool Scatter(const Ray& r_in , const HitRecord& rec , Color& attentuation , Ray& scattered) const override;

  private:
    double refraction_idx;

    static double Reflectance(double cos , double refraction_idx) {
      auto r0 = (1 - refraction_idx) / (1 + refraction_idx);
      r0 = r0 * r0;
      return r0 + (1 - r0) * glm::pow((1 - cos) , 5);
    }
};

class DiffuseLight : public Material {
  public:
    DiffuseLight(Ref<Texture> tex)
      : texture(tex) {}
    DiffuseLight(const Color& emit)
      : texture(NewRef<SolidColor>(emit)) {}

    virtual Color Emitted(double u , double v , const Point3& p) const override;

  private:
    Ref<Texture> texture;
};

#endif // !MATERIAL_HPP
