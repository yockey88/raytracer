/**
 * \file constant_medium.hpp
 **/
#include "constant_medium.hpp"

#include <iostream>

#include "defines.hpp"
#include "interval.hpp"

bool ConstantMedium::Hit(const Ray& r , Interval rayt , HitRecord& rec) const {
  const bool enable_debug = false;
  const bool debugging = enable_debug && RandomDouble() < 0.00001;

  HitRecord rec1 , rec2;

  if (!boundary->Hit(r , Interval::universe , rec1)) {
    return false;
  }

  if (!boundary->Hit(r , Interval(rec1.t + 0.0001 , infinity) , rec2)) {
    return false;
  }

  if (debugging) {
    std::cout << "\nt_min = " << rec1.t << " , t_max = " << rec2.t << "\n";
  }

  if (rec1.t < rayt.min) {
    rec1.t = rayt.min; 
  }

  if (rec2.t > rayt.max) {
    rec2.t = rayt.max;
  }

  if (rec1.t >= rec2.t) {
    return false;
  }

  if (rec1.t < 0) {
    rec1.t = 0;
  }

  auto ray_len = r.Direction().length();
  auto distance_inside_boundary = (rec2.t - rec1.t) * ray_len;
  auto hit_dist = neg_inv_density * glm::log(RandomDouble());

  if (hit_dist > distance_inside_boundary) {
    return false;
  }

  rec.t = rec1.t + hit_dist / ray_len;
  rec.point = r.At(rec.t);

  if (debugging) {
    std::cout << "Hit Distance = " << hit_dist << "\n" 
              << "rec.t = " << rec.t << "\n"
              << "rec.point = [" << rec.point.x << " , " << rec.point.y << " , " << rec.point.z << "]\n";
  }

  rec.normal = RandomUnitVector(); 
  rec.front_face = (RandomInt() % 2) == 0 ?
    true : false;
  rec.mat = phase_function;

  return true;
}

Aabb ConstantMedium::BoundingBox() const {
  return boundary->BoundingBox();
}

double ConstantMedium::PdfValue(const Point3& origin , const glm::vec3& direction) const {
  return boundary->PdfValue(origin , direction);
}

glm::vec3 ConstantMedium::Random(const Point3& origin) const {
  return boundary->Random(origin);
}
