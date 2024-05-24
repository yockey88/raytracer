/**
 * \file material.cpp
 **/
#include "material.hpp"

#include "defines.hpp"
#include "hittable.hpp"
    
bool Material::Scatter(const Ray& r_in , const HitRecord& rec , Color& attentuation , Ray& scattered) const {
  return false;
}
    
Color Material::Emitted(double u , double v , const Point3& p) const {
  return Color(0 , 0 , 0);
}

bool Lambertian::Scatter(const Ray& r_in , const HitRecord& rec , Color& attenuation , Ray& scattered) const {
  auto scatter_dir = rec.normal + RandomUnitVector();
  if (NearZero(scatter_dir)) {
    scatter_dir = rec.normal;
  }

  scattered = Ray(rec.point , scatter_dir , r_in.Time());
  attenuation = texture->Value(rec.u , rec.v , rec.point);
  return true;
}

bool Metal::Scatter(const Ray& r_in , const HitRecord& rec , Color& attenuation , Ray& scattered) const {
  glm::vec3 reflected = Reflect(r_in.Direction() , rec.normal);
  reflected = UnitVector(reflected) + (fuzz * RandomUnitVector());
  scattered = Ray(rec.point , reflected , r_in.Time());
  attenuation = albedo;
  return glm::dot(scattered.Direction() , rec.normal) > 0;
}

bool Dielectric::Scatter(const Ray& r_in , const HitRecord& rec , Color& attentuation , Ray& scattered) const {
  attentuation = Color(1.0 , 1.0 , 1.0);
  double ri = rec.front_face ? 
    (1.0 / refraction_idx) : refraction_idx;

  glm::vec3 unit_dir = UnitVector(r_in.Direction());
  double cos_theta = glm::min(glm::dot(-unit_dir , rec.normal) , 1.f);
  double sin_theta = glm::sqrt(1.0 - cos_theta * cos_theta);

  bool cannot_refract = ri * sin_theta > 1.0;

  glm::vec3 direction;
  if (cannot_refract || Reflectance(cos_theta , ri) > RandomDouble()) {
    direction = Reflect(unit_dir , rec.normal);
  } else {
    direction = Refract(unit_dir , rec.normal , ri);
  }

  scattered = Ray(rec.point , direction , r_in.Time());
  return true;
}

Color DiffuseLight::Emitted(double u , double v , const Point3& p) const {
  return texture->Value(u , v , p);
}

bool Isotropic::Scatter(const Ray& r_in , const HitRecord& rec , Color& attenuation , Ray& scattered) const {
  scattered = Ray(rec.point , RandomUnitVector() , r_in.Time());
  attenuation = texture->Value(rec.u , rec.v , rec.point);
  return true;
}
