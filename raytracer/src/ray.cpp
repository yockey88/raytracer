/**
 * \file ray.cpp
 **/
#include "ray.hpp"

const Point3& Ray::Origin() const { 
  return origin; 
}

const glm::vec3& Ray::Direction() const { 
  return direction; 
}

Point3 Ray::At(double t) const {
  return origin + t * direction;
}

double Ray::Time() const {
  return tm;
}
