/**
 * \file onb.hpp
 **/
#ifndef ONB_HPP
#define ONB_HPP

#include "defines.hpp"

class Onb {
  public:
    Onb() {}

    glm::vec3 operator[](uint32_t i) const;
    glm::vec3& operator[](uint32_t i);

    glm::vec3 I() const; 
    glm::vec3 J() const; 
    glm::vec3 K() const; 

    glm::vec3 Local(double a , double b , double c) const;

    glm::vec3 Local(const glm::vec3& v) const;

    void BuildFromK(const glm::vec3& w);

  private:
    glm::vec3 axis[3];
};

#endif // !ONB_HPP
