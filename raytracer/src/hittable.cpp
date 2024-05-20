/**
 * \file hittable.cpp
 **/
#include "hittable.hpp"

#include "defines.hpp"
#include "material.hpp"

void HitRecord::SetFaceNormal(const Ray& r , const glm::vec3& outward_normal) {
  front_face = glm::dot(r.Direction() , outward_normal) < 0;
  normal = front_face ? 
    outward_normal : -outward_normal;
}

Translate::Translate(Ref<Hittable> object , const glm::vec3& offset) 
    : object(object) , offset(offset) {
  bbox = object->BoundingBox() + offset;
}

bool Translate::Hit(const Ray& r , Interval rayt , HitRecord& rec) const {
  Ray offset_r(r.Origin() - offset , r.Direction() , r.Time());

  if (!object->Hit(offset_r , rayt , rec)) {
    return false;
  }

  rec.point += offset;

  return true;
}

Aabb Translate::BoundingBox() const {
  return bbox;
}

RotateY::RotateY(Ref<Hittable> object , double angle)
    : object(object) , angle(angle) {
  auto radians = DegreesToRadians(angle);

  sin_theta = glm::sin(radians);
  cos_theta = glm::cos(radians);

  bbox = object->BoundingBox();

  Point3 min(infinity , infinity , infinity);
  Point3 max(-infinity , -infinity , -infinity);

  for (int32_t i = 0; i < 2; ++i) {
    for (int32_t j = 0; j < 2; ++j) {
      for (int32_t k = 0; k < 2; ++k) {
        auto x = i * bbox.x.max + (1 - i) * bbox.x.min;
        auto y = j * bbox.y.max + (1 - j) * bbox.y.min;
        auto z = k * bbox.z.max + (1 - k) * bbox.z.min;

        auto newx = cos_theta * x + sin_theta * z;
        auto newz = -sin_theta * x + cos_theta * z;

        glm::vec3 tester(newx , y , newz);

        min.x = fmin(min.x , tester.x);
        max.x = fmax(max.x , tester.x);

        min.y = fmin(min.y , tester.y);
        max.y = fmax(max.y , tester.y);

        min.z = fmin(min.z , tester.z);
        max.z = fmax(max.z , tester.z);
      }
    }
  }

  bbox = Aabb(min , max);
}

bool RotateY::Hit(const Ray& r , Interval rayt , HitRecord& rec) const {
  auto origin = r.Origin();
  auto dir = r.Direction();

  origin.x = cos_theta * r.Origin().x - sin_theta * r.Origin().z;
  origin.z = sin_theta * r.Origin().x + cos_theta * r.Origin().z;

  dir.x = cos_theta * r.Direction().x - sin_theta * r.Direction().z;
  dir.z = sin_theta * r.Direction().x + cos_theta * r.Direction().z;

  Ray rotated_r(origin , dir , r.Time());

  if (!object->Hit(rotated_r , rayt , rec)) {
    return false;
  }

  auto p = rec.point;

  p.x = cos_theta * rec.point.x + sin_theta * rec.point.z;
  p.z = -sin_theta * rec.point.x + cos_theta * rec.point.z;

  auto normal = rec.normal;
  normal.x = cos_theta * rec.normal.x + sin_theta * rec.normal.z;
  normal.z = -sin_theta * rec.normal.x + cos_theta * rec.normal.z;

  rec.point = p;
  rec.normal = normal;

  return true;
}

Aabb RotateY::BoundingBox() const {
  return bbox; 
}
