/**
 * \file perlin.hpp
 **/
#ifndef PERLIN_HPP
#define PERLIN_HPP

#include "defines.hpp"

class Perlin {
  public:
    Perlin();

    ~Perlin();

    double Noise(const Point3& p) const;

    double Turb(const Point3& p , int32_t depth) const;

  private:
    static const int32_t point_count = 256;

    glm::vec3* rand_vec;
    int32_t* perm_x;
    int32_t* perm_y;
    int32_t* perm_z;

    static int32_t* PerlinGeneratePerm();

    static void Permute(int32_t* p , int32_t n);

    static double PerlinInterp(glm::vec3 c[2][2][2] , double u , double v , double w);
};

#endif // !PERLIN_HPP
