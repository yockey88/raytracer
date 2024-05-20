/**
 * \file perlin.cpp
 **/
#include "perlin.hpp"

Perlin::Perlin() {
  rand_vec = new glm::vec3[point_count];
  for (int32_t i = 0; i < point_count; ++i) {
    rand_vec[i] = UnitVector(RandomVec3(-1 , 1));
  }

  perm_x = PerlinGeneratePerm();
  perm_y = PerlinGeneratePerm();
  perm_z = PerlinGeneratePerm();
}

Perlin::~Perlin() {
  delete[] rand_vec;
  delete[] perm_x;
  delete[] perm_y;
  delete[] perm_z;
}

double Perlin::Noise(const glm::vec3& p) const {
  auto u = p.x - glm::floor(p.x);
  auto v = p.y - glm::floor(p.y);
  auto w = p.z - glm::floor(p.z);

  auto i = int32_t(glm::floor(p.x));
  auto j = int32_t(glm::floor(p.y));
  auto k = int32_t(glm::floor(p.z));
  glm::vec3 c[2][2][2];

  for (int32_t di = 0; di < 2; ++di) {
    for (int32_t dj = 0; dj < 2; ++dj) {
      for (int32_t dk = 0; dk < 2; ++dk) {
        c[di][dj][dk] = rand_vec[
          perm_x[(i + di) & 255] ^
          perm_y[(j + dj) & 255] ^ 
          perm_z[(k + dk) & 255]
        ];
      }
    }
  }

  return PerlinInterp(c , u , v , w);
}

double Perlin::Turb(const Point3& p , int32_t depth) const {
  auto accum = 0.0;
  auto temp_p = p;
  auto weight = 1.0;

  for (int32_t i = 0; i < depth; ++i) {
    accum += weight * Noise(temp_p);
    weight *= 0.5;
    temp_p *= 2;
  }

  return fabs(accum);
}

int32_t* Perlin::PerlinGeneratePerm() {
  auto p = new int32_t[point_count];

  for (int32_t i = 0; i < point_count; ++i) {
    p[i] = i;
  }

  Permute(p , point_count);

  return p;
}

void Perlin::Permute(int32_t* p , int32_t n) {
  for (int32_t i = n - 1; i > 0; --i) {
    int32_t target = RandomInt(0 , i);
    int32_t tmp = p[i];
    p[i] = p[target];
    p[target] = tmp;
  }
}

double Perlin::PerlinInterp(glm::vec3 c[2][2][2] , double u , double v , double w) {
  auto uu = u * u * (3 - 2 * u);
  auto vv = v * v * (3 - 2 * v);
  auto ww = w * w * (3 - 2 * w);

  auto accum = 0.0;
  for (int32_t i = 0; i < 2; ++i) {
    for (int32_t j = 0; j < 2; ++j) {
      for (int32_t k = 0; k < 2; ++k) {
        glm::vec3 weight_v(u - i , v - j , w - k);

        accum += (i * uu + (1 - i) * (1 - uu)) *
                 (j * vv + (1 - j) * (1 - vv)) *
                 (k * ww + (1 - k) * (1 - ww)) *
                 glm::dot(c[i][j][k] , weight_v);
      }
    }
  }

  return accum;
}
