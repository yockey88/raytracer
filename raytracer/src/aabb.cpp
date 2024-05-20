/**
 * \file aabb.cpp
 **/
#include "aabb.hpp"

const Aabb Aabb::empty = Aabb(Interval::empty , Interval::empty , Interval::empty);
const Aabb Aabb::universe = Aabb(Interval::universe , Interval::universe , Interval::universe);
    
Aabb::Aabb(const Interval& x , const Interval& y , const Interval& z) 
    : x(x) , y(y) , z(z) {
  PadToMinimums();
}

Aabb::Aabb(const Point3& a , const Point3& b) {
  x = (a.x <= b.x) ?
    Interval(a.x , b.x) : Interval(b.x , a.x);
  y = (a.y <= b.y) ?
    Interval(a.y , b.y) : Interval(b.y , a.y);
  z = (a.z <= b.z) ?
    Interval(a.z , b.z) : Interval(b.z , a.z);

  PadToMinimums();
}
    
Aabb::Aabb(const Aabb& box0 , const Aabb& box1) {
  x = Interval(box0.x , box1.x);
  y = Interval(box0.y , box1.y);
  z = Interval(box0.z , box1.z);
}

const Interval& Aabb::AxisInterval(uint32_t n) const {
  if (n == 1) return y;
  if (n == 2) return z;
  return x;
}

bool Aabb::Hit(const Ray& r , Interval ray_t) const {
  const Point3& ray_origin = r.Origin();
  const glm::vec3& ray_dir = r.Direction();

  for (uint32_t axis = 0; axis < 3; ++axis) {
    const Interval& ax = AxisInterval(axis);
    double axis_val;
    double orig_val;
    switch (axis) {
      case 0: 
        axis_val = ray_dir.x; 
        orig_val = ray_origin.x;
        break;
      case 1: 
        axis_val = ray_dir.y; 
        orig_val = ray_origin.y;
        break;
      case 2: 
        axis_val = ray_dir.z; 
        orig_val = ray_origin.z;
        break;
      default:
        throw std::runtime_error("UNREACHABLE CODE | INVALID AXIS INDEX IN AABB::HIT");
    }
    const double adinv = 1.0 / axis_val;

    auto t0 = (ax.min - orig_val) * adinv;
    auto t1 = (ax.max - orig_val) * adinv;

    if (t0 < t1) {
      if (t0 > ray_t.min) ray_t.min = t0;
      if (t1 < ray_t.max) ray_t.max = t1;
    } else {
      if (t1 > ray_t.min) ray_t.min = t1;
      if (t0 < ray_t.max) ray_t.max = t0;
    }

    if (ray_t.max <= ray_t.min)
      return false;
  }

  return true;
}

uint32_t Aabb::LongestAxis() const {
  if (x.Size() > y.Size()) {
    return x.Size() > z.Size() ? 
      0 : 2;
  } else {
    return y.Size() > z.Size() ?
      1 : 2;
  }
}

void Aabb::PadToMinimums() {
  double delta = 0.0001;
  if (x.Size() < delta) x = x.Expand(delta);
  if (y.Size() < delta) y = y.Expand(delta);
  if (z.Size() < delta) z = z.Expand(delta);
}

Aabb operator+(const Aabb& bbox , const glm::vec3& offset) {
  return Aabb(bbox.x + offset.x , bbox.y + offset.y , bbox.z + offset.z);
}

Aabb operator+(const glm::vec3& offset , const Aabb& bbox) {
  return bbox + offset;
}
