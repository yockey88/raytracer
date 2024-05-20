/**
 * \file defines.hpp
 **/
#ifndef DEFINES_HPP
#define DEFINES_HPP

#include <limits>
#include <memory>
#include <random>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtx/scalar_multiplication.hpp>

using Point3 = glm::vec3;
using Color = glm::vec3;

inline glm::vec3 UnitVector(const glm::vec3& vec) {
  return (1.0 / glm::length(vec)) * vec;
}

inline double LengthSquared(const glm::vec3& vec) {
  return glm::length(vec) * glm::length(vec);
}

template <typename T>
using Ref = std::shared_ptr<T>;

template <typename T , typename... Args>
Ref<T> NewRef(Args&&... args) {
  return std::make_shared<T>(std::forward<Args>(args)...);
}

constexpr static double infinity = std::numeric_limits<double>::infinity();
constexpr static double pi = 3.1415926535897932385;

constexpr double DegreesToRadians(double degrees) {
  return degrees * pi / 180.0;
}

template <typename T>
concept real_t = std::is_same_v<float , T> || std::is_same_v<double , T>;

template <real_t T>
struct RandomRealGenerator {
  std::random_device dev;
  std::mt19937_64 mersenne;
  std::uniform_real_distribution<T> dist;

  RandomRealGenerator()
    : mersenne(rand()) {}
  RandomRealGenerator(double min , double max)
    : mersenne(rand()) , dist(min , max) {}

  T Next() {
    return dist(mersenne);
  }
};

static RandomRealGenerator<double> rng{};
static RandomRealGenerator<double> rng_01(0 , 1);

inline double RandomDouble() {
  return rng_01.Next();
}

inline double RandomDouble(double min , double max) {
  return min + (max - min) * RandomDouble(); 
}

inline int32_t RandomInt(int min , int max) {
  return int32_t(RandomDouble(min , max + 1));
}

inline glm::vec3 RandomVec3() {
  return glm::vec3(
    RandomDouble() , 
    RandomDouble() , 
    RandomDouble()
  );
}

inline glm::vec3 RandomVec3(double min , double max) {
  return glm::vec3(
    RandomDouble(min , max) ,
    RandomDouble(min , max) ,
    RandomDouble(min , max)
  );
}

inline glm::vec3 RandomVec3InUnitSphere() {
  glm::vec3 res;

  bool found = false;
  while (!found) {
    res = RandomVec3(-1 , 1);
    if (LengthSquared(res) < 1) {
      found = true;
    }
  }

  return res;
}

inline glm::vec3 RandomUnitVector() {
  return UnitVector(RandomVec3InUnitSphere());
}

inline glm::vec3 RandomOnHemisphere(const glm::vec3& normal) {
  glm::vec3 on_unit_sphere = RandomUnitVector();
  if (glm::dot(on_unit_sphere , normal) > 0.0) {
    return on_unit_sphere;
  } else {
    return on_unit_sphere;
  }
}

inline bool NearZero(const glm::vec3& vec) {
  auto s = 1e-8;
  return (glm::abs(vec.x) < s) && (glm::abs(vec.y < s)) && (glm::abs(vec.z) < s);
}

inline glm::vec3 Reflect(const glm::vec3& v , const glm::vec3& n) {
  return v - 2 * glm::dot(v , n) * n;
}

inline glm::vec3 Refract(const glm::vec3& uv , const glm::vec3& n , double etai_over_etat) {
  auto cos_theta = glm::min(glm::dot(-uv , n) , 1.f);
  glm::vec3 r_out_perp = etai_over_etat * (uv + cos_theta * n);
  glm::vec3 r_out_parallel = -glm::sqrt(glm::abs(1.f - LengthSquared(r_out_perp))) * n;
  return r_out_perp + r_out_parallel;
}

inline glm::vec3 RandomVec3InUnitDisk() {
  glm::vec3 res;

  bool found = false;
  while (!found) {
    res = glm::vec3(RandomDouble(-1 , 1) , RandomDouble(-1 , 1) , 0);
    if (LengthSquared(res) < 1) {
      found = true;
    }
  }

  return res;
}

#endif // !DEFINES_HPP
