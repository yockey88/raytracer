/**
 * \file ray.hpp
 **/
#ifndef RAY_HPP
#define RAY_HPP

#include <glm/glm.hpp>

#include "defines.hpp"

class Ray {
  public:
    Ray() {}
    Ray(const Point3& origin , const glm::vec3& direction)
      : origin(origin) , direction(direction) , tm(0) {}
    Ray(const Point3& origin , const glm::vec3& direction , double time)
      : origin(origin) , direction(direction) , tm(time) {}

    const Point3& Origin() const;
    const glm::vec3& Direction() const;

    Point3 At(double t) const;

    double Time() const;

  private:
    Point3 origin;
    glm::vec3 direction;
    double tm;
};

#endif // !RAY_HPP
