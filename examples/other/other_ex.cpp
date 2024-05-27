#include <iomanip>
#include <iostream>

#include "defines.hpp"

static inline double f(const glm::vec3& dir) {
  auto cos_theta = dir.z;
  return glm::pow(cos_theta , 3);
}

static inline double pdf(const glm::vec3& d) {
  return d.z / pi;
}

int main() {
  auto n = 1000000;

  auto sum = 0.0;

  for (auto i = 0; i < n; ++i) {
    glm::vec3 dir = RandomCosineDirection();
    sum += f(dir) / pdf(dir);
  }

  std::cout << std::fixed << std::setprecision(12);
  std::cout << "pi / 2 = " << pi / 2.0 << "\n";
  std::cout << "estimate = " << sum / n << "\n";
}
