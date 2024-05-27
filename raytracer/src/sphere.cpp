/**
 * \file sphere.cpp
 **/
#include "sphere.hpp"

Sphere::Sphere(const Point3& center , double radius , Ref<Material> material)
    : center1(center) , radius(fmax(0 , radius)) , material(material) , is_moving(false) {
  auto rvec = glm::vec3(radius , radius , radius);
  bbox = Aabb(center1 - rvec , center1 + rvec);
}

Sphere::Sphere(const Point3& center1 , const Point3& center2 , double radius , Ref<Material> material)
    : center1(center1) , radius(fmax(0 , radius)) , material(material) , is_moving(true) {
  auto rvec = glm::vec3(radius , radius , radius);
  Aabb box1(center1 - rvec , center1 + rvec);
  Aabb box2(center2 - rvec , center2 + rvec);
  bbox = Aabb(box1 , box2);

  center_vec = center2 - center1;
}


bool Sphere::Hit(const Ray& r , Interval rayt , HitRecord& rec) const {
  Point3 center = is_moving ?
    SphereCenter(r.Time()) : center1;

  glm::vec3 oc = center - r.Origin();
  auto a = LengthSquared(r.Direction());
  auto h = glm::dot(r.Direction() , oc);
  auto c = LengthSquared(oc) - radius * radius;
  auto discriminant = h * h - a * c;

  if (discriminant < 0) {
    return false;
  } 

  auto sqrtd = glm::sqrt(discriminant);

  auto root = (h - sqrtd) / a;
  if (!rayt.OpenContains(root)) {
    root = (h + sqrtd) / a;
    if (!rayt.OpenContains(root)) {
      return false;
    }
  }

  rec.t = root;
  rec.point = r.At(rec.t);

  glm::vec3 outward_normal = (rec.point - center1) / radius;
  rec.SetFaceNormal(r, outward_normal);

  GetSphereUV(outward_normal , rec.u , rec.v);

  rec.mat = material;

  return true;
}

Aabb Sphere::BoundingBox() const {
  return bbox; 
}

double Sphere::PdfValue(const Point3& origin , const glm::vec3& direction) const {
  return 0.0;
}

glm::vec3 Sphere::Random(const Point3& origin) const {
  return glm::vec3(1 , 0 , 0);
}

Point3 Sphere::SphereCenter(double time) const {
  /// linearly interpolate from center1 to center2 according to time,
  ///   t = 0 => center1 and t = 1 => center 2
  return center1 + time * center_vec;
}

void Sphere::GetSphereUV(const Point3& p , double& u , double& v) {
  auto theta = glm::acos(-p.y);
  auto phi = atan2(-p.z , p.x) + pi;

  u = phi / (2 * pi);
  v = theta / pi;
}
