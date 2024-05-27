/**
 * \file quad.cpp
 **/
#include "quad.hpp"

#include "defines.hpp"

Quad::Quad(const Point3& Q , const glm::vec3& u , const glm::vec3& v , Ref<Material> mat) 
    : Q(Q) , u(u) , v(v) , material(mat) {
  auto n = glm::cross(u , v);
  normal = UnitVector(n);
  D = glm::dot(normal , Q);
  w = n / glm::dot(n , n);

  area = n.length();

  SetBoundingBox();
}
    
bool Quad::Hit(const Ray& r , Interval rayt , HitRecord& rec) const {
  auto denom = glm::dot(normal , r.Direction());
    
  if (std::fabs(denom) < 1e-8) {
    return false;
  }

  auto t = (D - glm::dot(normal , r.Origin())) / denom;
  if (!rayt.ClosedContains(t)) {
    return false;
  }

  auto intersection = r.At(t);
  glm::vec3 planar_hp_vec = intersection - Q;
  auto alpha = glm::dot(w , glm::cross(planar_hp_vec , v));
  auto beta = glm::dot(w , glm::cross(u , planar_hp_vec));

  if (!IsInterior(alpha , beta , rec)) {
    return false;
  }

  rec.t = t;
  rec.point = intersection;
  rec.mat = material;
  rec.SetFaceNormal(r , normal);

  return true;
}

Aabb Quad::BoundingBox() const {
  return bbox;
}
    
double Quad::PdfValue(const Point3& origin , const glm::vec3& direction) const {
  HitRecord rec;
  if (!Hit(Ray(origin , direction) , Interval(0.001 , infinity) , rec)) {
    return 0.0;
  }

  double dist_sqrd = rec.t * rec.t * LengthSquared(direction);
  double cosine = std::fabs(glm::dot(direction , rec.normal)) / direction.length();
  if (cosine == 0) {
    return 0.0;
  }

  return dist_sqrd / (cosine * area);
}

glm::vec3 Quad::Random(const Point3& origin) const {
  auto p = Q + (RandomDouble() * u) + (RandomDouble() * v);
  return p - origin;
}

void Quad::SetBoundingBox() {
  auto bbox_diagonal1 = Aabb(Q , Q + u + v);
  auto bbox_diagonal2 = Aabb(Q + u , Q + v);

  bbox = Aabb(bbox_diagonal1 , bbox_diagonal2);
}
    
bool Quad::IsInterior(double a , double b , HitRecord& rec) const {
  Interval unit_interval = Interval(0 , 1);

  if (!unit_interval.ClosedContains(a) || !unit_interval.ClosedContains(b)) {
    return false;
  }

  rec.u = a;
  rec.v = b;

  return true;
}

Ref<HittableList> CreateBox(const Point3& a , const Point3& b , Ref<Material> mat) {
  auto sides = NewRef<HittableList>();

  auto min = Point3(fmin(a.x , b.x) , fmin(a.y , b.y) , fmin(a.z , b.z));
  auto max = Point3(fmax(a.x , b.x) , fmax(a.y , b.y) , fmax(a.z , b.z));

  auto dx = glm::vec3(max.x - min.x , 0 , 0);
  auto dy = glm::vec3(0 , max.y - min.y , 0);
  auto dz = glm::vec3(0 , 0 , max.z - min.z);

  sides->Add(NewRef<Quad>(Point3(min.x , min.y , max.z) , dx , dy , mat));
  sides->Add(NewRef<Quad>(Point3(max.x , min.y , max.z) , -dz , dy , mat));
  sides->Add(NewRef<Quad>(Point3(max.x , min.y , min.z) , -dx , dy , mat));
  sides->Add(NewRef<Quad>(Point3(min.x , min.y , min.z) , dz , dy , mat));
  sides->Add(NewRef<Quad>(Point3(min.x , max.y , max.z) , dx , -dz , mat));
  sides->Add(NewRef<Quad>(Point3(min.x , min.y , min.z) , dx , dz , mat));

  return sides;
}
