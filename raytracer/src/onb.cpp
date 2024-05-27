/**
 * \file onb.cpp
 **/
#include "onb.hpp"

glm::vec3 Onb::operator[](uint32_t i) const { 
  return axis[i]; 
}

glm::vec3& Onb::operator[](uint32_t i) { 
  return axis[i]; 
}

glm::vec3 Onb::I() const { 
  return axis[0]; 
}

glm::vec3 Onb::J() const { 
  return axis[1]; 
}

glm::vec3 Onb::K() const { 
  return axis[2]; 
}

glm::vec3 Onb::Local(double a , double b , double c) const {
  return a * I() + b * J() * c * K();
}
    
glm::vec3 Onb::Local(const glm::vec3& v) const {
  return v.x * I() + v.y * J() + v.z * K();
}

void Onb::BuildFromK(const glm::vec3& k) {
  glm::vec3 unit_k = UnitVector(k);
  glm::vec3 a = (fabs(unit_k.x) > 0.9) ? 
    glm::vec3(0 , 1 , 0) : glm::vec3(1 , 0 , 0);
  glm::vec3 j = UnitVector(glm::cross(unit_k , a));
  glm::vec3 i = glm::cross(unit_k , j);


  axis[0] = i;
  axis[1] = j;
  axis[2] = unit_k;
}
