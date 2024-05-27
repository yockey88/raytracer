/**
 * \file bvh.cpp
 **/
#include "bvh.hpp"

#include <algorithm>
    
BvhNode::BvhNode(std::vector<Ref<Hittable>>& objects , size_t start , size_t end) {
  bbox = Aabb::empty;
  for (size_t obj_idx = start; obj_idx < end; ++obj_idx) {
    bbox = Aabb(bbox , objects[obj_idx]->BoundingBox());
  }

  uint32_t axis = bbox.LongestAxis();

  auto comparator = (axis == 0) ? 
    BoxXCompare : (axis == 1) ?
      BoxYCompare : BoxZCompare;

  size_t object_span = end - start;
  if (object_span == 1) {
    left = right = objects[start];
  } else if (object_span == 2) {
    left = objects[start];
    right = objects[start + 1];
  } else {
    std::sort(objects.begin() + start , objects.begin() + end , comparator);

    auto mid = start + object_span / 2;
    left = NewRef<BvhNode>(objects , start , mid);
    right = NewRef<BvhNode>(objects , mid , end);
  }

  bbox = Aabb(left->BoundingBox() , right->BoundingBox());
}

bool BvhNode::Hit(const Ray& r , Interval rayt , HitRecord& rec) const {
  if (!bbox.Hit(r , rayt)) {
    return false;
  } 

  bool hit_left = left != nullptr && left->Hit(r , rayt , rec);
  bool hit_right = right != nullptr && right->Hit(r , Interval(rayt.min , hit_left ? rec.t : rayt.max) , rec);

  return hit_left || hit_right;
}

Aabb BvhNode::BoundingBox() const {
  return bbox;
}
    
double BvhNode::PdfValue(const Point3& origin , const glm::vec3& direction) const {
  auto weight = 1.0 / 2.0;
  auto sum = 0.0;
  
  sum += weight * left->PdfValue(origin , direction);
  sum += weight * right->PdfValue(origin , direction);

  return sum;
} 

glm::vec3 BvhNode::Random(const Point3& origin) const {
  auto random = RandomInt();
  return random % 2 == 0 ?
    left->Random(origin) : right->Random(origin);
}

bool BvhNode::BoxCompare(const Ref<Hittable> a , const Ref<Hittable> b , int axis_index) {
  auto a_axis_interval = a->BoundingBox().AxisInterval(axis_index);
  auto b_axis_interval = b->BoundingBox().AxisInterval(axis_index);
  return a_axis_interval.min < b_axis_interval.min;
}

bool BvhNode::BoxXCompare(const Ref<Hittable> a , const Ref<Hittable> b) {
  return BoxCompare(a , b , 0);
}

bool BvhNode::BoxYCompare(const Ref<Hittable> a , const Ref<Hittable> b) {
  return BoxCompare(a , b , 1);
}

bool BvhNode::BoxZCompare(const Ref<Hittable> a , const Ref<Hittable> b) {
  return BoxCompare(a , b , 2);
}

