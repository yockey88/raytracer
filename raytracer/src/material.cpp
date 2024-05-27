/**
 * \file material.cpp
 **/
#include "material.hpp"

#include "defines.hpp"
#include "hittable.hpp"
    
bool Material::Scatter(const Ray& r_in , const HitRecord& rec , ScatterRecord& srec) const {
  return false;
}
    
double Material::ScatteringPdf(const Ray& r_in , const HitRecord& rec , const Ray& scattered) const {
  return 0.0;
}
    
Color Material::Emitted(const Ray& r_in , const HitRecord& rec , double u , double v , const Point3& p) const {
  return Color(0 , 0 , 0);
}

bool Lambertian::Scatter(const Ray& r_in , const HitRecord& rec , ScatterRecord& srec) const {
  srec.attenuation = texture->Value(rec.u , rec.v , rec.point);
  srec.pdf = NewRef<CosinePdf>(rec.normal);
  srec.skip_pdf = false;
  return true;
}

double Lambertian::ScatteringPdf(const Ray& r_in , const HitRecord& rec , const Ray& scattered) const {
  auto cos_theta = glm::dot(rec.normal , UnitVector(scattered.Direction()));
  return cos_theta < 0 ? 
    0 : cos_theta / pi;
}

bool Metal::Scatter(const Ray& r_in , const HitRecord& rec , ScatterRecord& srec) const {
  glm::vec3 reflected = glm::reflect(r_in.Direction() , rec.normal);
  reflected = UnitVector(reflected) + (fuzz * RandomUnitVector());

  srec.attenuation = albedo;
  srec.pdf = nullptr;
  srec.skip_pdf = true;
  srec.skip_pdf_ray = Ray(rec.point , reflected , r_in.Time());

  return true;
}

bool Dielectric::Scatter(const Ray& r_in , const HitRecord& rec , ScatterRecord& srec) const {
  srec.attenuation = Color(1.0 , 1.0 , 1.0);
  srec.pdf = nullptr;
  srec.skip_pdf = true;

  double ri = rec.front_face ? 
    (1.0 / refraction_idx) : refraction_idx;

  glm::vec3 unit_dir = UnitVector(r_in.Direction());
  double cos_theta = std::fmin(glm::dot(-unit_dir , rec.normal) , 1.0);
  double sin_theta = glm::sqrt(1.0 - cos_theta * cos_theta);

  bool cannot_refract = ri * sin_theta > 1.0;

  glm::vec3 direction;
  if (cannot_refract || Reflectance(cos_theta , ri) > RandomDouble()) {
    direction = Reflect(unit_dir , rec.normal);
  } else {
    direction = Refract(unit_dir , rec.normal , ri);
  }

  srec.skip_pdf_ray = Ray(rec.point , direction , r_in.Time());
  return true;
}

double Dielectric::Reflectance(double cos , double refraction_idx) {
  auto r0 = (1 - refraction_idx) / (1 + refraction_idx);
  r0 = r0 * r0;
  return r0 + (1 - r0) * glm::pow((1 - cos) , 5);
}

Color DiffuseLight::Emitted(const Ray& r_in , const HitRecord& rec , double u , double v , const Point3& p) const {
  // if (!rec.front_face) {
  //   return Color(0 , 0 , 0);
  // }
  return texture->Value(u , v , p);
}

bool Isotropic::Scatter(const Ray& r_in , const HitRecord& rec , ScatterRecord& srec) const {
  srec.attenuation = texture->Value(rec.u , rec.v , rec.point);
  srec.pdf = NewRef<SpherePdf>();
  srec.skip_pdf = false;
  return true;
}
    
double Isotropic::ScatteringPdf(const Ray& r_in , const HitRecord& rec , const Ray& scattered) const {
  return 1.0 / (4 * pi);
}
