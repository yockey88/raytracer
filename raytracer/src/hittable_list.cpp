/**
 * \file hittable_list.cpp
 **/
#include "hittable_list.hpp"

HittableList::HittableList(Ref<Hittable> obj) {
  Add(obj);
}

void HittableList::Clear() {
  objects.clear();
}

void HittableList::Add(Ref<Hittable> obj) {
  objects.push_back(obj);
  bbox = Aabb(bbox , obj->BoundingBox());
}

bool HittableList::Hit(const Ray& r , Interval rayt , HitRecord& rec) const {
  HitRecord temp_rec;
  bool hit_anything = false;
  auto closest_so_far = rayt.max;

  for (const auto& obj : objects) {
    if (obj->Hit(r , Interval(rayt.min , closest_so_far) , temp_rec)) {
      hit_anything = true;
      closest_so_far = temp_rec.t;
      rec = temp_rec;
    }
  }

  return hit_anything;
}

Aabb HittableList::BoundingBox() const {
  return bbox;
}

double HittableList::PdfValue(const Point3& origin , const glm::vec3& direction) const {
  auto weight = 1.0 / objects.size();
  auto sum = 0.0;

  for (const auto& obj : objects) {
    sum += weight * obj->PdfValue(origin , direction);
  }

  return sum;
}

glm::vec3 HittableList::Random(const Point3& origin) const {
  auto int_size = int32_t(objects.size());
  return objects[RandomInt(0 , int_size - 1)]->Random(origin);
}
