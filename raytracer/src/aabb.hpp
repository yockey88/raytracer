/**
 * \file aabb.hpp
 **/
#ifndef AABB_HPP
#define AABB_HPP

#include "defines.hpp"
#include "interval.hpp"
#include "ray.hpp"

class Aabb {
  public:
    Interval x , y , z;

    Aabb() {}
    Aabb(const Interval& x , const Interval& y , const Interval& z);
    Aabb(const Point3& a , const Point3& b);
    Aabb(const Aabb& box0 , const Aabb& box1);

    const Interval& AxisInterval(uint32_t n) const;

    bool Hit(const Ray& r , Interval ray_t) const;

    uint32_t LongestAxis() const;

    static const Aabb empty , universe;

  private:
    void PadToMinimums();
};

Aabb operator+(const Aabb& bbox , const glm::vec3& offset);
Aabb operator+(const glm::vec3& offset , const Aabb& bbox);

#endif // !AABB_HPP
