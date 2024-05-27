/**
 * \file pdf.cpp
 **/
#include "pdf.hpp"

#include "defines.hpp"

double SpherePdf::Value(const glm::vec3& direction) const {
  return 1.0 / (4 * pi);
}

glm::vec3 SpherePdf::Generate() const {
  return RandomUnitVector();
}

CosinePdf::CosinePdf(const glm::vec3& k) {
  ijk.BuildFromK(k);
}

double CosinePdf::Value(const glm::vec3& direction) const {
  double cos_theta = glm::dot(UnitVector(direction) , ijk.K()); 
  return std::fmax(0 , cos_theta / pi);
}

glm::vec3 CosinePdf::Generate() const {
  return ijk.Local(RandomCosineDirection());
}

HittablePdf::HittablePdf(const Ref<Hittable>& objects , const Point3& origin)
  : objects(objects) , origin(origin) {}

double HittablePdf::Value(const glm::vec3& direction) const {
  return objects->PdfValue(origin , direction);
}

glm::vec3 HittablePdf::Generate() const {
  return objects->Random(origin);
}

MixturePdf::MixturePdf(Ref<Pdf> p0 , Ref<Pdf> p1) {
  dists[0] = p0;
  dists[1] = p1;
}

double MixturePdf::Value(const glm::vec3& direction) const {
  return 0.5 * dists[0]->Value(direction) + 0.5 * dists[1]->Value(direction);
}

glm::vec3 MixturePdf::Generate() const {
  if (RandomDouble() < 0.5) {
    return dists[0]->Generate();
  } else {
    return dists[1]->Generate();
  }
}
